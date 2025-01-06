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
  ValvestateLookAndFeel() {
#ifndef _DEBUG
    setColour(Slider::thumbColourId, Colours::transparentBlack);
    setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    setColour(Slider::rotarySliderOutlineColourId, Colours::transparentBlack);
#endif

    setColour(Label::textColourId, Colours::black);
  }

#if 0
  void drawRotarySlider(Graphics &g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle, Slider &slider) override {
    float margin = 15;
    auto radius = jmin(width / 2, height / 2) - margin;
    auto centreX = x + width * 0.5f;
    auto centreY = y + height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle =
        rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // shadow
    Point<int> point(0, 0);
    Colour shadowColour(0x55000000);
    auto shadowSize = 10;

    DropShadow s(shadowColour, shadowSize, point);
    Path p;

    p.addEllipse(rx - shadowSize / 2.0f, ry - shadowSize / 2.0f,
                 rw + shadowSize, rw + shadowSize);
    s.drawForPath(g, p);

    // ticks
    g.setColour(Colours::black);
    auto tickLength = 15.0f;
    auto tickThickness = 3.0f;
    int nTicks = 21;
    for (int i = 0; i < nTicks; ++i) {
      p.clear();
      if (i % 2) {
        float thickness = tickThickness / 2;
        p.addRectangle(-thickness * 0.5f, -radius, thickness, -tickLength / 2);
      } else {
        p.addRectangle(-tickThickness * 0.5f, -radius, tickThickness,
                       -tickLength);
      }

      auto tmpAngle =
          rotaryStartAngle +
          i / (float)(nTicks - 1) * (rotaryEndAngle - rotaryStartAngle);

      p.applyTransform(
          AffineTransform::rotation(tmpAngle).translated(centreX, centreY));
      g.fillPath(p);
    }

    // knob body
    Image knobImage[5];
    knobImage[0] = ImageCache::getFromMemory(BinaryData::knob0_png,
                                             (size_t)BinaryData::knob0_pngSize);
    knobImage[1] = ImageCache::getFromMemory(BinaryData::knob1_png,
                                             (size_t)BinaryData::knob1_pngSize);
    knobImage[2] = ImageCache::getFromMemory(BinaryData::knob2_png,
                                             (size_t)BinaryData::knob2_pngSize);
    knobImage[3] = ImageCache::getFromMemory(BinaryData::knob3_png,
                                             (size_t)BinaryData::knob3_pngSize);
    knobImage[4] = ImageCache::getFromMemory(BinaryData::knob4_png,
                                             (size_t)BinaryData::knob4_pngSize);
    // g.drawImage(knobImage, margin, centreY-(100-2*margin)/2, 100-2*margin,
    // 100-2*margin, 0, 0, 200, 200);
    AffineTransform transform;
    transform = transform.scaled(2 * radius / knobImage[0].getWidth(),
                                 2 * radius / knobImage[0].getHeight());
    transform = transform.translated(margin, (height / 2 - radius));
    int index = (int)(angle / 2.0f / 3.1415f * 24 * 5) % 5;
    g.drawImageTransformed(knobImage[index], transform);
  }
#endif

// Do not draw toggle button in release mode
#if !_DEBUG
  void drawToggleButton(Graphics &, ToggleButton &, bool, bool) override {}
#endif
};

/**
 * Draws a knob using a set of image files showing different rotations of the
 * knob
 */
class ImageKnob final : public juce::Component {
public:
  explicit ImageKnob(const std::function<const char*(size_t, int&)>& get_image_resource)
      : param{0.5}, get_image_resource{get_image_resource} {
    setInterceptsMouseClicks(false, false);

    for (size_t i = 0; i < IMAGE_COUNT; ++i)
    {
      int size = 0;
      auto memory = get_image_resource(i, size);
      images[i] = ImageCache::getFromMemory(memory, size);
    }
  }

  void paint(juce::Graphics &g) override {
    double value = normRange.convertTo0to1(param);
    jassert(value >= 0);
    jassert(value <= 1);
    
    int index = roundToInt((value * (IMAGE_COUNT - 1)));
    jassert(index >= 0);
    jassert(index < IMAGE_COUNT);
 
    g.drawImageAt(images[index], 0, 0, false);
  }
  
  void setValue(float value) {
    param = value;
    repaint();
  }

  void setNormalisableRange (NormalisableRange<double> newRange) {
    normRange = newRange;
  }

private:
  static constexpr int IMAGE_COUNT = 256;

  float param;
  std::array<Image, IMAGE_COUNT> images;
  NormalisableRange<double> normRange { 0.0, 10.0 };
  std::function<const char*(size_t i, int& dataSizeInBytes)> get_image_resource;
};

/* Knob attachement system is based on the juce::SliderAttachement, with the
 * write path from Slider to the ValueTree removed, since our knob components
 * are for display only. The changing of parameters is handled by invisible
 * juce::Sliders drawn over the top of the knob image.
 */

class KnobAttachment;
class KnobParameterAttachment;

inline std::unique_ptr<KnobParameterAttachment> makeKnobAttachment (const AudioProcessorValueTreeState& stateToUse,
                                                        const String& parameterID,
                                                        ImageKnob& control)
{
  if (auto* parameter = stateToUse.getParameter (parameterID))
    return std::make_unique<KnobParameterAttachment> (*parameter, control);

  jassertfalse;
  return nullptr;
}

class JUCE_API  KnobAttachment
{
public:
  KnobAttachment (AudioProcessorValueTreeState& stateToUse,
                    const String& parameterID,
                    ImageKnob& slider) : attachment (makeKnobAttachment(stateToUse, parameterID, slider))
  {
  }

private:
  std::unique_ptr<KnobParameterAttachment> attachment;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobAttachment)
};

class KnobParameterAttachment
{
public:
  KnobParameterAttachment (RangedAudioParameter& param, ImageKnob& slider) 
  : slider{slider}, attachment {param, [this] (float f) { setValue (f); }, nullptr}
  {
    auto range = param.getNormalisableRange();

    auto convertFrom0To1Function = [range] (double currentRangeStart,
                                            double currentRangeEnd,
                                            double normalisedValue) mutable
    {
      range.start = (float) currentRangeStart;
      range.end = (float) currentRangeEnd;
      return (double) range.convertFrom0to1 ((float) normalisedValue);
    };

    auto convertTo0To1Function = [range] (double currentRangeStart,
                                          double currentRangeEnd,
                                          double mappedValue) mutable
    {
      range.start = (float) currentRangeStart;
      range.end = (float) currentRangeEnd;
      return (double) range.convertTo0to1 ((float) mappedValue);
    };

    auto snapToLegalValueFunction = [range] (double currentRangeStart,
                                             double currentRangeEnd,
                                             double mappedValue) mutable
    {
      range.start = (float) currentRangeStart;
      range.end = (float) currentRangeEnd;
      return (double) range.snapToLegalValue ((float) mappedValue);
    };

    NormalisableRange<double> newRange { (double) range.start,
                                         (double) range.end,
                                         std::move (convertFrom0To1Function),
                                         std::move (convertTo0To1Function),
                                         std::move (snapToLegalValueFunction) };
    newRange.interval = range.interval;
    newRange.skew = range.skew;
    newRange.symmetricSkew = range.symmetricSkew;

    slider.setNormalisableRange (newRange);

    sendInitialUpdate();
  }

private:
  void setValue (float newValue)
  {
    slider.setValue (newValue);
  }
  
  void sendInitialUpdate() { attachment.sendInitialUpdate(); }

  ImageKnob& slider;
  ParameterAttachment attachment;
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
