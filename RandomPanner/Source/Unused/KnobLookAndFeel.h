/*
  ==============================================================================

    KnobLookAndFeel.h
    Created: 3 Mar 2021 10:31:22pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class KnobLookAndFeel : public LookAndFeel_V4

{
public:
    KnobLookAndFeel();
    
    void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider &slider) override;
    
private:
    
};

class KnobLookAndFeelV2 : public LookAndFeel_V3

{
public:
    KnobLookAndFeelV2();
    
    Image sprite = ImageCache::getFromMemory(BinaryData::personalizedKnob_png, BinaryData::personalizedKnob_pngSize);
    void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider &slider) override;
    
private:
    Image myKnob;
};
