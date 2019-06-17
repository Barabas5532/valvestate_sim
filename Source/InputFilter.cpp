#include "InputFilter.h"

InputFilter::InputFilter(){}

InputFilter::~InputFilter(){}

void InputFilter::prepare(dsp::ProcessSpec spec)
{
    float K = 2 * spec.sampleRate;
    float B0 = 1.10449884469421e-9*std::pow(K, 2) + 0.00246545127613797*K + 0.5;
    float B1 = 1.0 - 2.20899768938842e-9*std::pow(K, 2);
    float B2 = 1.10449884469421e-9*std::pow(K, 2) - 0.00246545127613797*K + 0.5;

    float A0 = 1.10449884469421e-9*std::pow(K, 2) + 0.000115449950739352*K + 0.5;
    float A1 = 1.0 - 2.20899768938842e-9*std::pow(K, 2);
    float A2 = 1.10449884469421e-9*std::pow(K, 2) - 0.000115449950739352*K + 0.5;

    filter.coefficients = 
        new dsp::IIR::Coefficients<float>(B0, B1, B2, A0, A1, A2);

    filter.prepare(spec);
}

void InputFilter::process(dsp::ProcessContextReplacing<float> context)
{
    filter.process(context);
}

void InputFilter::reset()
{
    filter.reset();
}
