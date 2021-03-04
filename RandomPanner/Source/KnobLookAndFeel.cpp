/*
  ==============================================================================

    KnobLookAndFeel.cpp
    Created: 3 Mar 2021 10:31:22pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#include "KnobLookAndFeel.h"

// Constructor
KnobLookAndFeel::KnobLookAndFeel(){}

void KnobLookAndFeel::drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) {
    
    float diameter = jmin(width-20,height-20);
    float radius = diameter/2;
    float centreX = x + (width/2);
    float centreY = y + (height/2);
    float rx = centreX - (radius);
    float ry = centreY - (radius);
    float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    Rectangle<float> dialArea(rx,ry, diameter,diameter);
    
    g.setColour(Colours::black);
    g.fillEllipse(dialArea);
    
    Path dialTick;
    g.setColour(Colours::lightsteelblue);
    g.fillPath(dialTick);
    dialTick.addRectangle(0,-radius,3.f, radius*0.33f);
    g.fillPath(dialTick,AffineTransform::rotation(angle).translated(centreX, centreY));
    
    
    g.setColour(Colours::lightsteelblue);
    g.drawEllipse(rx, ry, diameter, diameter, 6.0);
    
}
