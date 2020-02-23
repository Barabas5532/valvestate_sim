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
        setColour (Label::textColourId, Colours::black);
    }

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
            float sliderPos, const float rotaryStartAngle, const float
            rotaryEndAngle, Slider& slider) override
    {
        float margin = 15;
        auto radius = jmin (width / 2, height / 2) - margin;
        auto centreX = x + width  * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // ticks
        g.setColour (Colours::black);
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

            //if(tmpAngle*1.02 >= 3.14*3/2.0f && tmpAngle*0.98 <= 3.14*3/2.0f)
            //    tmpAngle = 3.1415*3/2.0f;

            //if(tmpAngle*1.02 >= 3.14*5/2.0f && tmpAngle*0.98 <= 3.14*5/2.0f)
            //    tmpAngle = 3.1415*5/2.0f;

            p.applyTransform (AffineTransform::rotation (tmpAngle).translated (centreX, centreY));
            g.fillPath (p);
        }

        //knob body
        g.setColour(Colours::black);
        g.fillEllipse(centreX-radius, centreY-radius, 2*radius, 2*radius);

        g.setColour(GOLD);
        auto insideRadius = radius - 5;
        g.fillEllipse(centreX-insideRadius, centreY-insideRadius,
                2*insideRadius, 2*insideRadius);

        // pointer
        p.clear();
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 5.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius*0.7f, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));

        g.setColour (Colours::black);
        g.fillPath (p);
    }

    void drawToggleButton(Graphics &g, ToggleButton &b, 
            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        auto width = b.getWidth();
        auto height = b.getHeight();
        float margin = 15;
        auto radius = jmin (width / 2, height / 2) - margin;

        Image switchImage[5];
        switchImage[0] = ImageCache::getFromMemory (BinaryData::switchup_png,
                (size_t)BinaryData::switchup_png);
        switchImage[1] = ImageCache::getFromMemory (BinaryData::switchcentre_png,
                (size_t)BinaryData::switchcentre_pngSize);
        switchImage[2] = ImageCache::getFromMemory (BinaryData::switchdown_png,
                (size_t)BinaryData::switchdown_pngSize);
        int index;

        auto centreX = b.getWidth()/2;
        auto centreY = b.getHeight()/2;

        if(b.getToggleState())
        {
            //draw up switch
            index = 0;
        }
        else
        {
            //draw down switch
            index = 2;
        }

        AffineTransform transform;
        transform = transform.scaled(2*radius/switchImage[0].getWidth(), 2*radius/switchImage[0].getHeight());
        transform = transform.translated(margin, (height/2-radius));
        g.drawImageTransformed(switchImage[index], transform);
    }
};
