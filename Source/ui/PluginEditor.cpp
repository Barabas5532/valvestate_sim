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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BackgroundBinaryData.h"
#include "bass_binary_data.h"
#include "contour_binary_data.h"
#include "gain_binary_data.h"
#include "middle_binary_data.h"
#include "treble_binary_data.h"
#include "volume_binary_data.h"

static const char *get_gain_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = gain_binary_data::gain_pngSize;
  return gain_binary_data::gain_png;
}

static const char *get_bass_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = bass_binary_data::bass_pngSize;
  return bass_binary_data::bass_png;
}

static const char *get_middle_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = middle_binary_data::middle_pngSize;
  return middle_binary_data::middle_png;
}

static const char *get_treble_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = treble_binary_data::treble_pngSize;
  return treble_binary_data::treble_png;
}

static const char *get_contour_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = contour_binary_data::contour_pngSize;
  return contour_binary_data::contour_png;
}

static const char *get_volume_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = volume_binary_data::volume_pngSize;
  return volume_binary_data::volume_png;
}

static const char *get_led_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = BackgroundBinaryData::led_pngSize;
  return BackgroundBinaryData::led_png;
}

//==============================================================================
ValvestateAudioProcessorEditor::ValvestateAudioProcessorEditor (ValvestateAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), knobGain(get_gain_image_resource), knobBass(get_bass_image_resource), knobMiddle(get_middle_image_resource), knobTreble(get_treble_image_resource), knobContour(get_contour_image_resource), knobVolume(get_volume_image_resource), odLed("od_led", get_led_image_resource)
{
    setResizable(false, false);
    
    setLookAndFeel(&vsLookAndFeel);

    applySliderStyle(gain);
    applySliderStyle(bass);
    applySliderStyle(middle);
    applySliderStyle(treble);
    applySliderStyle(contour);
    applySliderStyle(volume);

    gainAttachment.reset(new SliderAttachment(processor.parameters, "gain", gain));
    bassAttachment.reset(new SliderAttachment(processor.parameters, "bass", bass));
    middleAttachment.reset(new SliderAttachment(processor.parameters, "middle", middle));
    trebleAttachment.reset(new SliderAttachment(processor.parameters, "treble", treble));
    contourAttachment.reset(new SliderAttachment(processor.parameters, "contour", contour));
    volumeAttachment.reset(new SliderAttachment(processor.parameters, "volume", volume));

    odAttachment.reset(new ButtonAttachment(processor.parameters, "od", odButton));
    odLedAttachment.reset(new ButtonAttachment(processor.parameters, "od", odLed));

    knobGainAttachment.reset(new SliderAttachment(processor.parameters, "gain", knobGain));
    knobBassAttachment.reset(new SliderAttachment(processor.parameters, "bass", knobBass));
    knobMiddleAttachment.reset(new SliderAttachment(processor.parameters, "middle", knobMiddle));
    knobTrebleAttachment.reset(new SliderAttachment(processor.parameters, "treble", knobTreble));
    knobContourAttachment.reset(new SliderAttachment(processor.parameters, "contour", knobContour));
    knobVolumeAttachment.reset(new SliderAttachment(processor.parameters, "volume", knobVolume));

    addAndMakeVisible(knobGain);
    addAndMakeVisible(knobBass);
    addAndMakeVisible(knobMiddle);
    addAndMakeVisible(knobTreble);
    addAndMakeVisible(knobContour);
    addAndMakeVisible(knobVolume);

    addAndMakeVisible(odButton);
    addAndMakeVisible(gain);
    addAndMakeVisible(bass);
    addAndMakeVisible(middle);
    addAndMakeVisible(treble);
    addAndMakeVisible(contour);
    addAndMakeVisible(volume);

    addAndMakeVisible(odLed);

    backgroundImage = ImageCache::getFromMemory (BackgroundBinaryData::background_png,
                                                (size_t)BackgroundBinaryData::background_pngSize);
                                                
    ledImage = ImageCache::getFromMemory (BackgroundBinaryData::led_png,
                                                 (size_t)BackgroundBinaryData::led_pngSize);

  setSize(UI_WIDTH, UI_HEIGHT);
}

void ValvestateAudioProcessorEditor::applySliderStyle(Slider &s)
{
    s.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
#if _DEBUG
    s.setPopupDisplayEnabled(true, true, nullptr);
#endif
    s.setLookAndFeel(&vsLookAndFeel);
}

ValvestateAudioProcessorEditor::~ValvestateAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void ValvestateAudioProcessorEditor::paint (Graphics& g)
{
  g.drawImageAt(backgroundImage, 0, 0, false);
  
  auto ledImageHalfWidth = ledImage.getWidth() / 2;
  auto ledImageHalfHeight = ledImage.getHeight() / 2;
  g.drawImageAt(ledImage, 917 - ledImageHalfWidth, 250 - ledImageHalfHeight, false);
}

static Rectangle<int> getKnobBounds(Point<int> p) {
  return Rectangle{p, p + Point{60, 60}};
}

void ValvestateAudioProcessorEditor::resized()
{
  odButton.setBounds(Rectangle{Point{464, 266}, Point{478, 282}});

    gain.setBounds(getKnobBounds(Point{398, 222}));
    bass.setBounds(getKnobBounds(Point{486, 220}));
    middle.setBounds(getKnobBounds(Point{561, 220}));
    treble.setBounds(getKnobBounds(Point{632, 220}));
    contour.setBounds(getKnobBounds(Point{704, 219}));
    volume.setBounds(getKnobBounds(Point{781, 221}));

    knobGain.setBounds(getLocalBounds());
    knobBass.setBounds(getLocalBounds());
    knobMiddle.setBounds(getLocalBounds());
    knobTreble.setBounds(getLocalBounds());
    knobContour.setBounds(getLocalBounds());
    knobVolume.setBounds(getLocalBounds());

    auto ledImageHalfWidth = ledImage.getWidth() / 2;
    auto ledImageHalfHeight = ledImage.getHeight() / 2;
    odLed.setBounds(ledImage.getBounds());
    odLed.setTopLeftPosition(
        {471 - ledImageHalfWidth, 250 - ledImageHalfHeight});
}

#if _DEBUG
void ValvestateAudioProcessorEditor::mouseDown(const MouseEvent &event) {
  Component::mouseDown(event);
  
  Logger::outputDebugString(juce::String::formatted("mouse down %d %d", event.x, event.y));
}

void ValvestateAudioProcessorEditor::mouseUp(const MouseEvent &event) {
  Component::mouseUp(event);

  Logger::outputDebugString(juce::String::formatted("mouse up %d %d", event.x, event.y));
}
#endif
