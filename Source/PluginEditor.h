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
#include "PluginProcessor.h"
#include "UI.h"

//==============================================================================
/**
*/
class ValvestateAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ValvestateAudioProcessorEditor (ValvestateAudioProcessor&);
    ~ValvestateAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void applyLabelStyle(Label &l);
    void applySliderStyle(Slider &s);

    ValvestateAudioProcessor& processor;

    ValvestateLookAndFeel vsLookAndFeel;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    Slider gain, bass, middle, treble, contour, volume;
    ToggleButton button;

    std::unique_ptr<SliderAttachment> gainAttachment, bassAttachment, middleAttachment,
                     trebleAttachment, contourAttachment, volumeAttachment;
    std::unique_ptr<ButtonAttachment> buttonAttachment;
    
    Label title;

    Label buttonLabel, gainLabel, bassLabel, middleLabel, trebleLabel, contourLabel,
          volumeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValvestateAudioProcessorEditor)
};
