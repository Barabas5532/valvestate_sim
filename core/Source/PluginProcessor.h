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

    using AudioProcessor::processBlock;
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
    NormalisableRange<float> logRange;

public:
    AudioProcessorValueTreeState parameters;

private:
    std::atomic<float> *od = nullptr;
    std::atomic<float> *gain = nullptr;
    std::atomic<float> *bass = nullptr;
    std::atomic<float> *middle = nullptr;
    std::atomic<float> *treble = nullptr;
    std::atomic<float> *contourP = nullptr;
    std::atomic<float> *volume = nullptr;

    InputFilter input;
    GainControl gaincontrol;
    Clipping clipping;
    FMVFilter fmv;
    ContourFilter contour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValvestateAudioProcessor)
};
