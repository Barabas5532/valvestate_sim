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
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>

class ValvestateLookAndFeel : public LookAndFeel_V4 {
public:
  ValvestateLookAndFeel() {}

// Do not draw JUCE UI elements sliders in release mode
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
  explicit ImageKnob(const std::function<const char*(size_t, int&)>& get_image_resource)
      : Slider(), get_image_resource{get_image_resource} {
    setInterceptsMouseClicks(false, false);

    for (size_t i = 0; i < IMAGE_COUNT; ++i)
    {
      int size = 0;
      auto memory = get_image_resource(i, size);
      images[i] = ImageCache::getFromMemory(memory, size);
    }
  }

  void paint(juce::Graphics &g) override {
    auto range = getNormalisableRange();
    double value = range.convertTo0to1(getValue());;
    jassert(value >= 0);
    jassert(value <= 1);
    
    int index = roundToInt((value * (IMAGE_COUNT - 1)));
    jassert(index >= 0);
    jassert(index < IMAGE_COUNT);
 
    g.drawImageAt(images[index], 0, 0, false);
  }

private:
  static constexpr int IMAGE_COUNT = 256;

  std::array<Image, IMAGE_COUNT> images;
  std::function<const char*(size_t i, int& dataSizeInBytes)> get_image_resource;
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
