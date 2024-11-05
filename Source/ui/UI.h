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

const Colour GOLD(0xffa1781b);
const Colour DARKER(0xff111111);
const Colour DARK(0xff222222);
const Colour LIGHT(0xff555555);

class ValvestateLookAndFeel : public LookAndFeel_V4
{
public:
    ValvestateLookAndFeel()
    {
        setColour (Slider::thumbColourId, GOLD);
        setColour (Slider::rotarySliderOutlineColourId, DARK);
        setColour (Slider::rotarySliderFillColourId, DARKER);
        setColour (ResizableWindow::backgroundColourId, Colours::black);
        setColour (Label::textColourId, LIGHT);
    }

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
            float sliderPos, const float rotaryStartAngle, const float
            rotaryEndAngle, Slider& slider) override
    {
        float margin = 15;
        auto radius = jmin (width / 2, height / 2) - margin;
        auto centreX = x + width  * 0.5f;
        auto centreY = y + height * 0.5f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // ticks
        g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
        auto tickLength = 15.0f;
        auto tickThickness = 3.0f;
        int nTicks = 21;
        Path p;
        for (int i = 0; i < nTicks; ++i)
        {
            p.clear();
            if(i%2)
            {
                float thickness = tickThickness/2;
                p.addRectangle ( -thickness * 0.5f, -radius,
                        thickness, -tickLength/2);
            }
            else
            {
                p.addRectangle ( -tickThickness * 0.5f, -radius,
                        tickThickness, -tickLength);
            }

            auto tmpAngle = rotaryStartAngle + i/(float)(nTicks-1) * (rotaryEndAngle - rotaryStartAngle);

            p.applyTransform (AffineTransform::rotation (tmpAngle).translated (centreX, centreY));
            g.fillPath (p);
        }

        //knob body
        g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
        g.fillEllipse(centreX-radius, centreY-radius, 2*radius, 2*radius);

        g.setColour(slider.findColour(Slider::rotarySliderFillColourId));
        auto insideRadius = radius - 5;
        g.fillEllipse(centreX-insideRadius, centreY-insideRadius,
                2*insideRadius, 2*insideRadius);

        // pointer
        p.clear();
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 5.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius*0.7f, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));

        g.setColour (slider.findColour(Slider::thumbColourId));
        g.fillPath (p);
    }

    void drawToggleButton(Graphics &g, ToggleButton &b, 
            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        (void) shouldDrawButtonAsHighlighted;
        (void) shouldDrawButtonAsDown;

        //width and height of actual buttons
        auto w = 80;
        auto h = 40;

        auto centreX = b.getWidth()/2;
        auto centreY = b.getHeight()/2;

        Rectangle<int> topHalf(centreX - w/2, centreY - h, w, h);
        Rectangle<int> bottomHalf(centreX - w/2, centreY, w, h);

        g.setFont(Font(20, Font::FontStyleFlags::bold));

        //set slider thumb colours
        setColour (Slider::thumbColourId, b.getToggleState() ? Colours::red : GOLD); 

        //draw top half (OD1)
        g.setColour (b.getToggleState() ? findColour(Slider::rotarySliderFillColourId) : GOLD);
        g.fillRect (topHalf);

        g.setColour (findColour (Slider::rotarySliderOutlineColourId));
        g.drawText("OD1", topHalf, Justification::centred);

        //draw bottom half (OD2)
        g.setColour (b.getToggleState() ? Colours::red : findColour(Slider::rotarySliderFillColourId));
        g.fillRect (bottomHalf);

        g.setColour (findColour (Slider::rotarySliderOutlineColourId));
        g.drawText("OD2", bottomHalf, Justification::centred);

        //draw outline
        Path outline;
        outline.addRectangle(centreX - w/2, centreY - h, w, 2*h);
        outline.addRectangle(centreX - w/2, centreY, w, 0);

        g.setColour (findColour (Slider::rotarySliderOutlineColourId));
        g.strokePath(outline, PathStrokeType(3));
    }
};
