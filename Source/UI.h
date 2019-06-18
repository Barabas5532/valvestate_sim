#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

const Colour GOLD(0xffa1781b);

class ValvestateLookAndFeel : public LookAndFeel_V4
{
public:
    ValvestateLookAndFeel()
    {
        setColour (Slider::thumbColourId, Colours::red);
        setColour (ResizableWindow::backgroundColourId, GOLD);
    }

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
            float sliderPos, const float rotaryStartAngle, const float
            rotaryEndAngle, Slider& slider) override
    {
        auto radius = jmin (width / 2, height / 2) - 20.0f;
        auto centreX = x + width  * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour (GOLD);
        g.fillEllipse (rx, ry, rw, rw);

        // outline
        g.setColour (Colours::black);
        g.drawEllipse (rx, ry, rw, rw, 5.0f);

        Path p;
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 3.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius*0.8f, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));

        // pointer
        g.setColour (Colours::black);
        g.fillPath (p);
    }
};
