# ---
# jupyter:
#   jupytext:
#     formats: ipynb,py:percent
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#       jupytext_version: 1.16.2
#   kernelspec:
#     display_name: valvestate
#     language: python
#     name: valvestate
# ---

# %% [markdown]
# # Valvestate DSP
#
# This document describes the design process of the DSP algorithms for the plugin.
# [Schematics](https://drtube.com/schematics/marshall/pc0689.pdf)

# %%
import numpy as np
import scipy.signal as signal
from scipy.interpolate import PchipInterpolator, CubicSpline, Akima1DInterpolator
import matplotlib.pyplot as plt
import csv
import sympy
import re

# %%
# Sample rate to use when plotting digital filter responses
rate = 48000
# Bilinear transform parameter, optimal at DC
k = 2*rate

# Default plot settings
plt.rcParams["figure.figsize"] = (10, 5)
plt.rcParams['axes.grid'] = True

from sympy.abc import s, z, K


# %% [markdown]
# # Filters
#
# Analog filters are discretectised using the method described in [DISCRETIZATION OF THE ’59 FENDER BASSMAN TONE STACK by David T. Yeh, Julius O. Smith](https://ccrma.stanford.edu/~dtyeh/papers/yeh06_dafx.pdf).
#
# In summary, symbolic analysis is performed on the schematic in the s-domain, then the bilinear transform is used to genereate a z-domain transfer function, which can be implemented as an IIR filter.
#
# They gloss over how exactly to perform the symbolic circuit analysis. I have used QSapecNG for this. This software performs symbolic analysis on a graphical schematic, so it is quite easy to use. Unfortunately it has many bugs, and is not maintained anymore. Schematics are available for each circuit under the file mentioned in the relevant code snippet.
#
# [Lcapy](https://github.com/mph-/lcapy), [ahkab](https://github.com/ahkab/ahkab), [SCAM](https://github.com/echeever/scam), Wolfram SystemModeler could possibly work here. I have only tried lcapy, and altough it would be nice to do all the work inside Python, it was too slow to be usable.

# %%
def print_as_cpp_coefficients(coeffs, name):
    for i in range(len(coeffs)):    
        tmp = f"float {name}{i} = {coeffs[i]};"
        # substitute python power syntax a**2 to std::pow(a, 2)
        print(re.sub(r"([a-zA-Z])\*\*(\d)", r"std::pow(\1, \2)", tmp))
    
print_as_cpp_coefficients(range(0, 3), "test")
print_as_cpp_coefficients([s**5], "power")


# %% [markdown]
# Helper function to perform bilinear transform by substituting $s$ for $K\frac{1-z^{-1}}{1+z^{-1}}$.

# %%
def bilinear_transform(tf_s):
    s_sub = K*(1-z**-1)/(1+z**-1)
    tf_z = tf_s.subs(s, s_sub).normal().simplify().factor()
    n, d = sympy.fraction(tf_z)
    n_frac = sympy.poly(n, z)
    d_frac = sympy.poly(d, z)
    B = n_frac.all_coeffs()
    A = d_frac.all_coeffs()
    return B, A

b0, b1, b2, a0, a1, a2 = sympy.symbols("b0, b1, b2, a0, a1, a2")

# this should print the general bilinear transform for a biquad
bilinear_transform((b0 + b1 * s**-1 + b2 * s**-2)/(a0 + a1 * s**-1 + a2 * s**-2))


# %%
def substitute_parameter(A, parameters):
    return [float(sympy.sympify(AA).subs(parameters)) for AA in A]


# %% [markdown] jp-MarkdownHeadingCollapsed=true
# ## Input filter
#
# This is the first stage of the amplifier, it's suspiciously similar to a tubescreamer pedal.

# %%
# input_filter.info
tf_s = (s**2 + 2.23219e+06*s + 4.52694e+08)/(1 * s**2 + 104527*s + 4.52694e+08)

n, d = sympy.fraction(tf_s)
n_poly = sympy.Poly(n, s)
d_poly = sympy.Poly(d, s)
B = n_poly.all_coeffs()
A = d_poly.all_coeffs()

tf = signal.lti([float(BB) for BB in B], [float(AA) for AA in A])
w, H = signal.freqresp(tf)

plt.figure()
plt.semilogx(w/2/np.pi, 20*np.log10(np.abs(H)), label="analogue")

B, A = bilinear_transform(tf_s)

print_as_cpp_coefficients(B, "B")
print_as_cpp_coefficients(A, "A")

b = substitute_parameter(B, {K: k})
a = substitute_parameter(A, {K: k})
f = signal.dlti(b, a)
w, H = signal.dfreqresp(f)

plt.semilogx(w/2/np.pi*rate, 20*np.log10(np.abs(H)), label="digital")
plt.title("Frequency response of input filter")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.xlim(20, 20e3)
plt.ylim(0, 30)
plt.legend()
plt.show()

# %% [markdown] jp-MarkdownHeadingCollapsed=true
# ## Gain control
#
# There are two sets of functions, as this circuit has two switchable modes (OD1/OD2).

# %%
# OD1.info / OD2.info
C1 = 220e-9
C2 = 47e-12
R1 = 22e3
R2 = 100e3
Rg = sympy.symbols("Rg")
Rg_value = 1e6

B1 = [R1*Rg*C1, 0]
A1 = [R1*R2*Rg*C1*C2, R1*R2*C1 + R1*Rg*C2 + R2*Rg*C2, R1+R2]

B2 = [Rg*C1, 0]
R1 = 9.09e3
A2 = [R1*Rg*C1*C2, R1*C1 + Rg*C2, 1]

g = sympy.symbols("g")

tf1_s = ((B1[0]*s)/(A1[0]*s**2 + A1[1]*s + A1[2])).subs(Rg, Rg_value*g)
tf2_s = ((B2[0]*s)/(A2[0]*s**2 + A2[1]*s + A2[2])).subs(Rg, Rg_value*g)

B1z, A1z = bilinear_transform(tf1_s)
B2z, A2z = bilinear_transform(tf2_s)

print_as_cpp_coefficients(B1z, "B1")
print_as_cpp_coefficients(A1z, "A1")
print_as_cpp_coefficients(B2z, "B2")
print_as_cpp_coefficients(A2z, "A2")

G = [0.01, 0.1, 1]

plt.figure()
for B, A, label, linestyle in [[B1z, A1z, "OD1", "-"], [B2z, A2z, "OD2", "--"]]:
    for gg in G:
        subs = {K:k, g:gg}
        b = substitute_parameter(B, subs)
        a = substitute_parameter(A, subs)
            
        f = signal.dlti(b, a)
        w, H = signal.dfreqresp(f)
        
        # ignore divide by 0 errors
        with np.errstate(divide='ignore'):
            plt.semilogx(w/2/np.pi*rate, 20*np.log10(np.abs(H)), label=f"{label}, g = {gg}", linestyle=linestyle)

plt.legend()
plt.title("Frequency response of gain control")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.xlim(20, 20e3)
plt.ylim(-45, 45)
plt.show()

# %% [markdown]
# ## Contour filter
#
# Sound demos: 
# * https://youtu.be/d70jrnlN62k?t=395
# * https://youtu.be/5Ju7Zzl4D1g?t=191

# %%
# contour.info
Rp1, Rp2, p = sympy.symbols("Rp1 Rp2 p")
Rp = 100e3
V1 = 1

exp = ((( 1.0032e-14 * Rp1 * Rp2 * V1 + 1.94495e-10 * Rp1 * V1 ) * s**3 +
        ( 1.254e-10 * Rp1 * Rp2 * V1 + 4.56e-07 * Rp1 * V1 + 1.76e-07 * Rp2 * V1 + 0.0034122 * V1 ) * s**2 +
        ( 0.0057 * Rp1 * V1 + 0.0022 * Rp2 * V1 + 8 * V1 ) * s + 100000 * V1) / 
       (( 3.88784e-19 * Rp1 * Rp2 + 7.53755e-15 * Rp1 ) * s**4 +
        ( 1.92511e-14 * Rp1 * Rp2 + 5.04208e-10 * Rp1 + 3.88784e-11 * Rp2 + 7.53755e-07 ) * s**3 + 
        ( 2.31614e-10 * Rp1 * Rp2 + 7.82459e-06 * Rp1 + 1.05904e-06 * Rp2 + 0.0297419 ) * s**2 +
        ( 0.0105279 * Rp1 + 0.0040634 * Rp2 + 170.06 ) * s + 184700))

exp_final = exp.subs({Rp1: Rp*p, Rp2: Rp*(1-p)})
n, d = sympy.fraction(exp_final)
n_poly = sympy.Poly(n, s)
d_poly = sympy.Poly(d, s)
B = n_poly.all_coeffs()
A = d_poly.all_coeffs()

tf = (B[0]*s**3 + B[1]*s**2 + B[2]*s + B[3])/(A[0]*s**4 + A[1]*s**3 + A[2]*s**2 + A[3]*s + A[4])
Bz, Az = bilinear_transform(tf)

print_as_cpp_coefficients(Bz, "B")
print_as_cpp_coefficients(Az, "A")

# %%
P = [0, 0.0035, 0.006, 0.01, 0.03, 0.3]
for pp in P:
    b = substitute_parameter(B, {p: pp})
    a = substitute_parameter(A, {p: pp})

    tf = signal.lti(b, a)
    w, H = signal.freqresp(tf)
    plt.semilogx(w/2/np.pi, 20*np.log10(np.abs(H)), label=f"analogue {pp: .2f}")

b = substitute_parameter(Bz, {p: P[0], K: k})
a = substitute_parameter(Az, {p: P[0], K: k})

tf = signal.dlti(b, a)
w, H = signal.dfreqresp(tf)
plt.semilogx(w/2/np.pi*rate, 20*np.log10(np.abs(H)), label=f"digital {P[0]: 0.2f}", linestyle='--')

plt.title("Frequency response of contour filter (dark mode)")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.xlim(20, 20e3)
plt.ylim(-30, -5)
plt.legend()
plt.show()

# %%
P = [0.3, 0.75, 0.88, 0.94, 0.977, 1]
for pp in P:
    b = substitute_parameter(B, {p: pp})
    a = substitute_parameter(A, {p: pp})

    tf = signal.lti(b, a)
    w, H = signal.freqresp(tf)
    plt.semilogx(w/2/np.pi, 20*np.log10(np.abs(H)), label=f"analogue {pp: .2f}")

b = substitute_parameter(Bz, {p: pp, K: k})
a = substitute_parameter(Az, {p: pp, K: k})

tf = signal.dlti(b, a)
w, H = signal.dfreqresp(tf)
plt.semilogx(w/2/np.pi*rate, 20*np.log10(np.abs(H)), label=f"digital {pp: 0.2f}", linestyle='--')

plt.title("Frequency response of contour filter (scoop mode)")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.xlim(20, 20e3)
plt.ylim(-30, -5)
plt.legend()
plt.show()

# %% [markdown]
# Notice how the response is non-linear here. Between 0 - 0.05, there is substantial high-cut. From 0.05 to 0.5 the response barely changes, then above 0.5 there is a mid-cut. It might be a nice improvement to add a function that transforms the parameter in order to linearise the effect.
#
# Further improvement is to run the filters at an oversampled rate, or use another method to reduce the effect of frequency warping. This can be seen on the graph as a difference between the analog and digital response at high frequencies.

# %%
y = [0, 0.0035, 0.006, 0.01, 0.03, 0.3, 0.75, 0.88, 0.94, 0.977, 1]
x = np.linspace(0, 1, num=len(y))

interp = {}
interp["akima1d"] = Akima1DInterpolator(x, y)
interp["pchip"] = PchipInterpolator(x, y)

plt.scatter(x, y)

for i in interp:
    x_interp = np.linspace(0, 1, num=1000)
    plt.plot(x_interp, [interp[i](x) for x in x_interp], label=f'{i}')
plt.legend()
plt.show()

best_interp = interp["pchip"]


# %%
# 9 decimal digits should be enough to exactly represent a 32-bit IEEE 754 float
def print_cpp_array(a):
    print("{", end="")
    for i in range(len(a)):
        if not i % 5:
            print("")
            
        print(f"{a[i]:11.10}, ", end="")
        
    print("\n}", end="")

        
print_cpp_array([0.000012345678901234567890, 1.000000001, 2., 3., 4., 5., 6.])

# %%
# evaluate interpolator by plotting frequency response against parameter changes

Pleft = np.linspace(0, 0.5, num=21)
Pright = np.linspace(0.5, 1, num=21)

def plot_parameter_sweep(P):
    for pp in P:
        b = substitute_parameter(B, {p: pp})
        a = substitute_parameter(A, {p: pp})
    
        tf = signal.lti(b, a)
        w, H = signal.freqresp(tf)
        plt.semilogx(w/2/np.pi, 20*np.log10(np.abs(H)), label=f"analogue {pp: .2f}")

    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Magnitude (dB)")
    plt.xlim(20, 20e3)
    plt.ylim(-30, -5)
    plt.show()

plt.subplot(2, 2, 1)
plt.title('no interp')
plot_parameter_sweep(Pleft)

plt.subplot(2, 2, 2)
plt.title('no interp')
plot_parameter_sweep(Pright)

plt.subplot(2, 2, 3)
plt.title('interp')
plot_parameter_sweep([best_interp(pp) for pp in Pleft])

plt.subplot(2, 2, 4)
plt.title('interp')
plot_parameter_sweep([best_interp(pp) for pp in Pright])

plt.show()

# %%
P = np.linspace(0, 1, num=100)
print_cpp_array([best_interp(pp) for pp in P])

# %% [markdown]
# # Distortion
#
# The distortion comes from a combination of diode clipping and a 12AX7 vacuum tube. A static waveshaper is used to simulate these circuits. This was created by entering the schematic into LTSpice, then running a DC operating point sweep.
#
# To output the data from LTSpice, click on the node that should be exported (V(follower) in this case). This should make a plot appear. Now we can right click on the plot then navigate to File -> Export data as text.

# %%
input_voltage = []
output_voltage = []

with open('clipping_waveshape.txt') as csvDataFile:
    csvReader = csv.reader(csvDataFile, delimiter='\t')
    next(csvReader) # skip the header line
    for row in csvReader:
        input_voltage.append(float(row[0]))
        output_voltage.append(float(row[1]))

# %%
plt.plot(input_voltage, output_voltage)
plt.xlabel("Input signal (V)")
plt.ylabel("Output signal (V)")
plt.show()
print_cpp_array(output_voltage)
