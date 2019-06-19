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

        // shadow
        Point<int> point(0, 0);
        Colour shadowColour(0x55000000);
        DropShadow s(shadowColour, 10, point);
        Path p;

        p.addEllipse (rx-10, ry-10, rw+20, rw+20);
        s.drawForPath(g, p);

        // fill
        g.setColour (GOLD);
        g.fillEllipse (rx, ry, rw, rw);

        // outline
        g.setColour (Colours::black);
        g.drawEllipse (rx, ry, rw, rw, 5.0f);


        // ticks
        g.setColour (Colours::black);
        auto tickLength = 15.0f;
        auto tickThickness = 3.0f;
        int nTicks = 21;
        for (int i = 0; i < nTicks; ++i)
        {
            p.clear();
            if(i%2)
            {
                float thickness = tickThickness/2;
                p.addRectangle ( -thickness * 0.5f, -radius-5,
                        thickness, -tickLength/2);
            }
            else
            {
                p.addRectangle ( -tickThickness * 0.5f, -radius-5,
                        tickThickness, -tickLength);
            }

            auto tmpAngle = rotaryStartAngle + i/(float)(nTicks-1) * (rotaryEndAngle - rotaryStartAngle);

            if(tmpAngle*1.02 >= 3.14*3/2.0f && tmpAngle*0.98 <= 3.14*3/2.0f)
                tmpAngle = 3.1415*3/2.0f;

            if(tmpAngle*1.02 >= 3.14*5/2.0f && tmpAngle*0.98 <= 3.14*5/2.0f)
                tmpAngle = 3.1415*5/2.0f;

            p.applyTransform (AffineTransform::rotation (tmpAngle).translated (centreX, centreY));
            g.fillPath (p);
        }

        // pointer
        p.clear();
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 3.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius*0.8f, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));

        g.setColour (Colours::black);
        g.fillPath (p);
    }

    void drawToggleButton(Graphics &g, ToggleButton &b, 
            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        const int buttonSize = 40;
        const int outlineThickness = 3;
        auto centreX = b.getWidth()/2;
        auto centreY = b.getHeight()/2;

        g.setColour (Colours::black);

        g.drawRect(centreX-buttonSize, centreY-buttonSize/2, buttonSize,
                buttonSize, outlineThickness);
        g.drawRect(centreX, centreY-buttonSize/2, buttonSize,
                buttonSize, outlineThickness);
        if(b.getToggleState())
        {
            g.fillRect(centreX, centreY-buttonSize/2, buttonSize, buttonSize);
        }
        else
        {
            g.fillRect(centreX-buttonSize, centreY-buttonSize/2, buttonSize,
                buttonSize);
        }
    }
};
