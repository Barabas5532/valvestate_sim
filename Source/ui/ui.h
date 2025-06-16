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

#include "resources.h"

#include "PluginEditor.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include <memory>

using namespace juce;

class ValvestateLookAndFeel : public LookAndFeel_V4 {
public:
  ValvestateLookAndFeel() {}

// Do not draw JUCE UI elements in release mode
#if !_DEBUG
  void drawRotarySlider(Graphics &, int, int, int, int, float, const float,
                        const float, Slider &) override {}
  void drawToggleButton(Graphics &, ToggleButton &, bool, bool) override {}
#endif
};

/**
 * Draws a knob using a set of image files showing different rotations of the
 * knob
 */
class ImageKnob final : public juce::Slider {
public:
  explicit ImageKnob(const std::function<const char*(int&)>& get_image_resource)
      : Slider(), get_image_resource{get_image_resource} {
    setInterceptsMouseClicks(false, false);

    int size = 0;
    auto memory = get_image_resource(size);
    image = ImageCache::getFromMemory(memory, size);

    setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  }

  void paint(juce::Graphics &g) override {
    auto range = getNormalisableRange();
    double value = range.convertTo0to1(getValue());
    jassert(value >= 0);
    jassert(value <= 1);
    
    // The denoising is not applied to the first image, so we don't use it
    int index = roundToInt((value * (IMAGE_COUNT - 2))) + 1;
    jassert(index >= 1);
    jassert(index < IMAGE_COUNT);

    auto single_image_height = image.getHeight() / IMAGE_COUNT;
    
    // clang-format off
    g.drawImage(image,
                // destination position
                0, 0,
                // destination size
                image.getWidth(), single_image_height,
                // source position
                0, index * single_image_height,
                // source size
                image.getWidth(), single_image_height);
    // clang-format on
  }

private:
  static constexpr int IMAGE_COUNT = 257;

  Image image;
  std::function<const char *(int &dataSizeInBytes)> get_image_resource;
};

/**
 * Draws an LED using an image file when the parameter is enabled
 */
class ImageLed final : public juce::Button {
public:
  explicit ImageLed(
      const String &buttonName,
      const std::function<const char *(int &)> &get_image_resource)
      : Button(buttonName), get_image_resource{get_image_resource} {
    setInterceptsMouseClicks(false, false);

    int size = 0;
    auto memory = get_image_resource(size);
    image = ImageCache::getFromMemory(memory, size);
  }

  void paintButton(Graphics &g, bool, bool) override {
    if (getToggleState())
      g.drawImageAt(image, 0, 0, false);
  };

private:
  Image image;
  std::function<const char *(int &dataSizeInBytes)> get_image_resource;
};

/** UI page containing the controls for the plugin.
 *
 * It is shown after loading assets completes.
 */
class ValvestateUi final  : public Component
{
public:
  explicit ValvestateUi (AudioProcessorValueTreeState &p) : knobGain(get_gain_image_resource), knobBass(get_bass_image_resource), knobMiddle(get_middle_image_resource), knobTreble(get_treble_image_resource), knobContour(get_contour_image_resource), knobVolume(get_volume_image_resource), odLed("od_led", get_led_image_resource) {
    setLookAndFeel(&vsLookAndFeel);

    applySliderStyle(gain);
    applySliderStyle(bass);
    applySliderStyle(middle);
    applySliderStyle(treble);
    applySliderStyle(contour);
    applySliderStyle(volume);

    gainAttachment = std::make_unique<SliderAttachment>(p, "gain", gain);
    bassAttachment = std::make_unique<SliderAttachment>(p, "bass", bass);
    middleAttachment = std::make_unique<SliderAttachment>(p, "middle", middle);
    trebleAttachment = std::make_unique<SliderAttachment>(p, "treble", treble);
    contourAttachment = std::make_unique<SliderAttachment>(p, "contour", contour);
    volumeAttachment = std::make_unique<SliderAttachment>(p, "volume", volume);

    odAttachment = std::make_unique<ButtonAttachment>(p, "od", odButton);
    odLedAttachment = std::make_unique<ButtonAttachment>(p, "od", odLed);

    knobGainAttachment = std::make_unique<SliderAttachment>(p, "gain", knobGain);
    knobBassAttachment = std::make_unique<SliderAttachment>(p, "bass", knobBass);
    knobMiddleAttachment = std::make_unique<SliderAttachment>(p, "middle", knobMiddle);
    knobTrebleAttachment = std::make_unique<SliderAttachment>(p, "treble", knobTreble);
    knobContourAttachment = std::make_unique<SliderAttachment>(p, "contour", knobContour);
    knobVolumeAttachment = std::make_unique<SliderAttachment>(p, "volume", knobVolume);
    
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
  };
  
  ~ValvestateUi() override {
    setLookAndFeel(nullptr);
  };

  //==============================================================================
  void paint (Graphics& g) override 
  {
    g.drawImageAt(backgroundImage, 0, 0, false);

    auto ledImageHalfWidth = ledImage.getWidth() / 2;
    auto ledImageHalfHeight = ledImage.getHeight() / 2;
    g.drawImageAt(ledImage, 917 - ledImageHalfWidth, 250 - ledImageHalfHeight, false);
  }

  
  void resized() override {
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
  };

    Slider gain, bass, middle, treble, contour, volume;

    ImageKnob knobGain, knobBass, knobMiddle, knobTreble, knobContour,
        knobVolume;

  private:
  static Rectangle<int> getKnobBounds(Point<int> p) {
    return Rectangle{p, p + Point{60, 60}};
  }

  void applySliderStyle(Slider &s) {
    s.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
#if _DEBUG
    s.setPopupDisplayEnabled(true, true, nullptr);
#endif
    s.setLookAndFeel(&vsLookAndFeel);
  }

  ValvestateLookAndFeel vsLookAndFeel;

  ToggleButton odButton;

  Image backgroundImage;
  Image ledImage;

  ImageLed odLed;

  typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
  typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

  std::unique_ptr<SliderAttachment> gainAttachment;
  std::unique_ptr<SliderAttachment> bassAttachment;
  std::unique_ptr<SliderAttachment> middleAttachment;
  std::unique_ptr<SliderAttachment> trebleAttachment;
  std::unique_ptr<SliderAttachment> contourAttachment;
  std::unique_ptr<SliderAttachment> volumeAttachment;
  std::unique_ptr<ButtonAttachment> odAttachment;
  std::unique_ptr<ButtonAttachment> odLedAttachment;

  std::unique_ptr<SliderAttachment> knobGainAttachment;
  std::unique_ptr<SliderAttachment> knobBassAttachment;
  std::unique_ptr<SliderAttachment> knobMiddleAttachment;
  std::unique_ptr<SliderAttachment> knobTrebleAttachment;
  std::unique_ptr<SliderAttachment> knobContourAttachment;
  std::unique_ptr<SliderAttachment> knobVolumeAttachment;

  static constexpr int UI_WIDTH = 1267;
  static constexpr int UI_HEIGHT = 712;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValvestateUi)
};
