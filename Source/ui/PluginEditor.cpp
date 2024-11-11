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

static const char *get_gain_image_resource(size_t i, int &dataSizeInBytes) {
  jassert(i < gain_binary_data::namedResourceListSize);
  return gain_binary_data::getNamedResource(gain_binary_data::namedResourceList[i],
                                          dataSizeInBytes);
}

static const char *get_bass_image_resource(size_t i, int &dataSizeInBytes) {
  jassert(i < bass_binary_data::namedResourceListSize);
  return bass_binary_data::getNamedResource(bass_binary_data::namedResourceList[i],
                                          dataSizeInBytes);
}

static const char *get_middle_image_resource(size_t i, int &dataSizeInBytes) {
  jassert(i < middle_binary_data::namedResourceListSize);
  return middle_binary_data::getNamedResource(middle_binary_data::namedResourceList[i],
                                          dataSizeInBytes);
}

static const char *get_treble_image_resource(size_t i, int &dataSizeInBytes) {
  jassert(i < treble_binary_data::namedResourceListSize);
  return treble_binary_data::getNamedResource(treble_binary_data::namedResourceList[i],
                                          dataSizeInBytes);
}

static const char *get_contour_image_resource(size_t i, int &dataSizeInBytes) {
  jassert(i < contour_binary_data::namedResourceListSize);
  return contour_binary_data::getNamedResource(contour_binary_data::namedResourceList[i],
                                          dataSizeInBytes);
}

static const char *get_volume_image_resource(size_t i, int &dataSizeInBytes) {
  jassert(i < volume_binary_data::namedResourceListSize);
  return volume_binary_data::getNamedResource(volume_binary_data::namedResourceList[i],
                                          dataSizeInBytes);
}

//==============================================================================
ValvestateAudioProcessorEditor::ValvestateAudioProcessorEditor (ValvestateAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), knobGain(get_gain_image_resource), knobBass(get_bass_image_resource), knobMiddle(get_middle_image_resource), knobTreble(get_treble_image_resource), knobContour(get_contour_image_resource), knobVolume(get_volume_image_resource)
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

    buttonAttachment.reset(new ButtonAttachment(processor.parameters, "od", button));

    knobGainAttachment.reset(new KnobAttachment(processor.parameters, "gain", knobGain));
    knobBassAttachment.reset(new KnobAttachment(processor.parameters, "bass", knobBass));
    knobMiddleAttachment.reset(new KnobAttachment(processor.parameters, "middle", knobMiddle));
    knobTrebleAttachment.reset(new KnobAttachment(processor.parameters, "treble", knobTreble));
    knobContourAttachment.reset(new KnobAttachment(processor.parameters, "contour", knobContour));
    knobVolumeAttachment.reset(new KnobAttachment(processor.parameters, "volume", knobVolume));

    addAndMakeVisible(knobGain);
    addAndMakeVisible(knobBass);
    addAndMakeVisible(knobMiddle);
    addAndMakeVisible(knobTreble);
    addAndMakeVisible(knobContour);
    addAndMakeVisible(knobVolume);

    addAndMakeVisible(button);
    addAndMakeVisible(gain);
    addAndMakeVisible(bass);
    addAndMakeVisible(middle);
    addAndMakeVisible(treble);
    addAndMakeVisible(contour);
    addAndMakeVisible(volume);

    backgroundImage = ImageCache::getFromMemory (BackgroundBinaryData::background_png,
                                                (size_t)BackgroundBinaryData::background_pngSize);

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
}

void ValvestateAudioProcessorEditor::resized()
{
    button.setBounds(100, 100, 100, 100);
    gain.setBounds(Rectangle(Point{409, 204}, Point{474, 266}));
    bass.setBounds(Rectangle(Point{486, 204}, Point{546, 266}));
    middle.setBounds(Rectangle(Point{558, 204}, Point{616, 266}));
    treble.setBounds(Rectangle(Point{629, 204}, Point{689, 266}));
    contour.setBounds(Rectangle(Point{702, 204}, Point{760, 266}));
    volume.setBounds(Rectangle(Point{774, 204}, Point{843, 266}));

    knobGain.setBounds(getLocalBounds());
    knobBass.setBounds(getLocalBounds());
    knobMiddle.setBounds(getLocalBounds());
    knobTreble.setBounds(getLocalBounds());
    knobContour.setBounds(getLocalBounds());
    knobVolume.setBounds(getLocalBounds());
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
