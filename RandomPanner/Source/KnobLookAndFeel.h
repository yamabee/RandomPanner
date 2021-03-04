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
    
    void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotartStart, const float rotaryEndAngle, Slider &slider) override;
    
private:
    
};
