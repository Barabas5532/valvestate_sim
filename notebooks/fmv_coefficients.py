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
#     display_name: Python 3 (ipykernel)
#     language: python
#     name: python3
# ---

# %%
import matplotlib.pyplot as plt
import numpy as np
import scipy.signal as sp

# %%
# 0.5 0.5 0.5
b = [0.623173, -1.83112, 1.79378, -0.585832]
a = [1, -2.89192, 2.78504, -0.893114]

# %%
w, h = sp.freqz(b, a, fs=48e3*2)
plt.semilogx(w, 20 * np.log10(np.abs(h)))

# %%
# 0 1 0
b = [0.431653, -0.395056, -0.431653, 0.395056]
a = [1, -0.90483, -0.99961, 0.905219]

# %%
w, h = sp.freqz(b, a,  fs=48e3*2)
plt.semilogx(w, 20 * np.log10(np.abs(h)))

# %%
