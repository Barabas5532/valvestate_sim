/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
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
