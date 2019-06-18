#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "InputFilter.h"
#include "GainControl.h"
#include "Clipping.h"
#include "FMV.h"
#include "Contour.h"

class ValvestateAudioProcessor  : public AudioProcessor
{
public:
    ValvestateAudioProcessor();
    ~ValvestateAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    AudioProcessorValueTreeState parameters;

    float *od;
    float *gain;
    float *bass;
    float *middle;
    float *treble;
    float *contourP;
    float *volume;

    InputFilter input;
    GainControl gaincontrol;
    Clipping clipping;
    FMVFilter fmv;
    ContourFilter contour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValvestateAudioProcessor)
};
