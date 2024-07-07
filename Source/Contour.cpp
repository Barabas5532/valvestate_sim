/*
    Copyright 2020 Barabas Raffai

    This file is part of Valvestate Sim.

    Valvestate Sim is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    Valvestate Sim is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with Valvestate Sim.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Contour.h"
#include "TableInterpolate.h"

ContourFilter::ContourFilter()
{
    //we must set the filter order before the first prepare call, so that
    //the correct amount of memory gets allocated
    setParameter(0.5);
}

ContourFilter::~ContourFilter(){}

void ContourFilter::prepare(dsp::ProcessSpec spec)
{
    filter.prepare(spec);
    sampleRate = spec.sampleRate;
}

static float linearise_parameter(float p) {
  static constexpr float table[] = {
    0.0, 0.000399703253, 0.0007903886671, 0.001171540937, 0.001542644759, 
    0.001903184826, 0.002252645834, 0.002590512478, 0.002916269452, 0.003229401453, 
    0.003529318414, 0.00380802645, 0.004064715103, 0.004305528396, 0.004536610351, 
    0.00476410499, 0.004994156335, 0.005232908407, 0.00548650523, 0.005761090825, 
    0.006061839569, 0.006364010237, 0.006662375576, 0.006967717353, 0.007290817335, 
    0.007642457291, 0.008033418988, 0.008474484193, 0.008976434676, 0.009550052203, 
    0.01021078394, 0.01104679121, 0.01210654749, 0.01341421882, 0.01499397123, 
    0.01686997075, 0.01906638341, 0.02160737525, 0.02451711229, 0.02781976057, 
    0.03214355966, 0.04275863988, 0.06006502759, 0.08304081952, 0.1106641124, 
    0.1419130029, 0.1757655879, 0.2111999639, 0.2471942278, 0.2827264763, 
    0.3182061903, 0.3607463924, 0.4095979266, 0.4625298775, 0.5173113293, 
    0.5717113665, 0.6234990736, 0.6704435348, 0.7103138347, 0.7408790575, 
    0.7618800328, 0.780206636, 0.7968191307, 0.8118648697, 0.825491206, 
    0.8378454926, 0.8490750823, 0.8593273282, 0.8687495831, 0.8774892001, 
    0.8856583044, 0.8932546516, 0.9003219688, 0.9069089735, 0.9130643835, 
    0.9188369165, 0.9242752902, 0.9294282225, 0.9343444308, 0.9390726331, 
    0.9436407161, 0.9480285933, 0.9522361467, 0.9562642411, 0.9601137414, 
    0.9637855124, 0.9672804191, 0.9705993262, 0.9737430986, 0.9767126013, 
    0.9795467067, 0.982295052, 0.9849480246, 0.9874955246, 0.9899274519, 
    0.9922337067, 0.9944041888, 0.9964287984, 0.9982974355,         1.0, 
  };
  
  return linear_interpolate_table(table, p);
}

void ContourFilter::setParameter(float p)
{
    p = linearise_parameter(p);

    float K = 2*sampleRate;

    float B0 = 9.05251759610179e-11*std::pow(K, 3)*std::pow(p, 2) -
        1.08075708355892e-10*std::pow(K, 3)*p + 1.13156469951272e-6*std::pow(K,
                2)*std::pow(p, 2) - 1.15683089695001e-6*std::pow(K, 2)*p -
        1.89606569211334e-8*std::pow(K, 2) - 0.000315827467966071*K*p -
        0.000205739036275041*K - 0.0902364194188775;

    float B1 = -1.81050351922036e-10*std::pow(K, 3)*std::pow(p, 2) +
        2.16151416711785e-10*std::pow(K, 3)*p - 0.000631654935932142*K*p -
        0.000411478072550081*K - 0.36094567767551;

    float B2 = -2.26312939902545e-6*std::pow(K, 2)*std::pow(p, 2) +
        2.31366179390002e-6*std::pow(K, 2)*p + 3.79213138422667e-8*std::pow(K,
                2) - 0.541418516513265;

    float B3 = 1.81050351922036e-10*std::pow(K, 3)*std::pow(p, 2) -
        2.16151416711785e-10*std::pow(K, 3)*p + 0.000631654935932142*K*p +
        0.000411478072550081*K - 0.36094567767551;

    float B4 = -9.05251759610179e-11*std::pow(K, 3)*std::pow(p, 2) +
        1.08075708355892e-10*std::pow(K, 3)*p + 1.13156469951272e-6*std::pow(K,
                2)*std::pow(p, 2) - 1.15683089695001e-6*std::pow(K, 2)*p -
        1.89606569211334e-8*std::pow(K, 2) + 0.000315827467966071*K*p +
        0.000205739036275041*K - 0.0902364194188775;

    float A0 = 3.50824760873489e-15*std::pow(K, 4)*std::pow(p, 2) -
        4.18840913192565e-15*std::pow(K, 4)*p +
        1.73715033387475e-10*std::pow(K, 3)*std::pow(p, 2) -
        2.15704710341094e-10*std::pow(K, 3)*p -
        4.18840913192564e-12*std::pow(K, 3) + 2.09000180472839e-6*std::pow(K,
                2)*std::pow(p, 2) - 2.70050081212777e-6*std::pow(K, 2)*p -
        1.22402003248511e-7*std::pow(K, 2) - 0.000583333333333333*K*p -
        0.00052012272153041*K - 0.166666666666667;

    float A1 = -1.40329904349395e-14*std::pow(K, 4)*std::pow(p, 2) +
        1.67536365277026e-14*std::pow(K, 4)*p - 3.4743006677495e-10*std::pow(K,
                3)*std::pow(p, 2) + 4.31409420682187e-10*std::pow(K, 3)*p +
        8.37681826385129e-12*std::pow(K, 3) - 0.00116666666666667*K*p -
        0.00104024544306082*K - 0.666666666666667;

    float A2 = 2.10494856524093e-14*std::pow(K, 4)*std::pow(p, 2) -
        2.51304547915539e-14*std::pow(K, 4)*p - 4.18000360945678e-6*std::pow(K,
                2)*std::pow(p, 2) + 5.40100162425555e-6*std::pow(K, 2)*p +
        2.44804006497022e-7*std::pow(K, 2) - 1.0;

    float A3 = -1.40329904349395e-14*std::pow(K, 4)*std::pow(p, 2) +
        1.67536365277026e-14*std::pow(K, 4)*p + 3.4743006677495e-10*std::pow(K,
                3)*std::pow(p, 2) - 4.31409420682187e-10*std::pow(K, 3)*p -
        8.37681826385129e-12*std::pow(K, 3) + 0.00116666666666667*K*p +
        0.00104024544306082*K - 0.666666666666667;

    float A4 = 3.50824760873489e-15*std::pow(K, 4)*std::pow(p, 2) -
        4.18840913192565e-15*std::pow(K, 4)*p -
        1.73715033387475e-10*std::pow(K, 3)*std::pow(p, 2) +
        2.15704710341094e-10*std::pow(K, 3)*p +
        4.18840913192564e-12*std::pow(K, 3) + 2.09000180472839e-6*std::pow(K,
                2)*std::pow(p, 2) - 2.70050081212777e-6*std::pow(K, 2)*p -
        1.22402003248511e-7*std::pow(K, 2) + 0.000583333333333333*K*p +
        0.00052012272153041*K - 0.166666666666667;

    jassert (A0 != 0);

    filter.coefficients->coefficients.clear();

    auto A0inv = static_cast<float> (1) / A0;

    filter.coefficients->coefficients.add (B0 * A0inv,
                                           B1 * A0inv,
                                           B2 * A0inv,
                                           B3 * A0inv,
                                           B4 * A0inv,
                                           A1 * A0inv,
                                           A2 * A0inv,
                                           A3 * A0inv,
                                           A4 * A0inv);
}

void ContourFilter::process(dsp::ProcessContextReplacing<float> context)
{
    filter.process(context);
}

void ContourFilter::reset()
{
    filter.reset();
}
