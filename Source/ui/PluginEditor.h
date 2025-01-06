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
#include "ui.h"

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

#if _DEBUG
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
#endif

private:
    void applySliderStyle(Slider &s);

    ValvestateAudioProcessor& processor;

    ValvestateLookAndFeel vsLookAndFeel;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    Slider gain, bass, middle, treble, contour, volume;
    ToggleButton odButton;

    std::unique_ptr<SliderAttachment> gainAttachment;
    std::unique_ptr<SliderAttachment> bassAttachment;
    std::unique_ptr<SliderAttachment> middleAttachment;
    std::unique_ptr<SliderAttachment> trebleAttachment;
    std::unique_ptr<SliderAttachment> contourAttachment;
    std::unique_ptr<SliderAttachment> volumeAttachment;
    std::unique_ptr<ButtonAttachment> odAttachment;
    std::unique_ptr<ButtonAttachment> odLedAttachment;

    Image backgroundImage;
    Image ledImage;
    
    ImageKnob knobGain;
    ImageKnob knobBass;
    ImageKnob knobMiddle;
    ImageKnob knobTreble;
    ImageKnob knobContour;
    ImageKnob knobVolume;
    
    ImageLed odLed;

    std::unique_ptr<SliderAttachment> knobGainAttachment;
    std::unique_ptr<SliderAttachment> knobBassAttachment;
    std::unique_ptr<SliderAttachment> knobMiddleAttachment;
    std::unique_ptr<SliderAttachment> knobTrebleAttachment;
    std::unique_ptr<SliderAttachment> knobContourAttachment;
    std::unique_ptr<SliderAttachment> knobVolumeAttachment;

    static constexpr int UI_WIDTH = 1267;
    static constexpr int UI_HEIGHT = 712;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValvestateAudioProcessorEditor)
};
