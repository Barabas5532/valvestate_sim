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

#include "ValvestateProcessorBase.h"
#include "juce_audio_processors/juce_audio_processors.h"

class ValvestateAudioProcessor : public juce::AudioProcessor {
public:
  ValvestateAudioProcessor(
      const char *name,
      std::unique_ptr<ValvestateProcessorBase> dsp);
  ~ValvestateAudioProcessor();

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  using AudioProcessor::processBlock;
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

private:
  juce::NormalisableRange<float> logRange;

public:
  juce::AudioProcessorValueTreeState parameters;

private:
  std::atomic<float> *od = nullptr;
  std::atomic<float> *gain = nullptr;
  std::atomic<float> *bass = nullptr;
  std::atomic<float> *middle = nullptr;
  std::atomic<float> *treble = nullptr;
  std::atomic<float> *contour = nullptr;
  std::atomic<float> *volume = nullptr;

  const std::string_view name;
  std::unique_ptr<ValvestateProcessorBase> dsp;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValvestateAudioProcessor)
};