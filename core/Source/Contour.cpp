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
//#include <cmath>

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

void ContourFilter::setParameter(float p)
{
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
