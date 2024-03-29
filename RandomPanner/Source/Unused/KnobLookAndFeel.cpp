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

KnobLookAndFeelV2::KnobLookAndFeelV2() {
    auto myKnob = ImageCache::getFromMemory(BinaryData::personalizedKnob_png, BinaryData::personalizedKnob_pngSize);
}
    
void KnobLookAndFeelV2::drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider &slider) {
    
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    AffineTransform rotator;
//    g.drawImageTransformed(sprite, rotator.rotated(angle), (float)(sprite.getWidth()/2), (float)(sprite.getHeight()/2));
        
//    if (myKnob.isValid()) {
//        const double rotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
//        const int frames = myKnob.getHeight() / myKnob.getWidth();
//        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
//        const float radius = jmin(width/2.0f, height/2.0f);
//        const float centreX = x + width * 0.5f;
//        const float centreY = y + height * 0.5f;
//        const float rx = centreX - radius - 1.0f;
//        const float ry = centreY - radius;
//
//        g.drawImage(myKnob, (int)rx, (int)ry, 2 * (int)radius, 2 * (int)radius, 0, frameId*myKnob.getWidth(), myKnob.getWidth(), myKnob.getWidth());
//    }
//
//    else {
//        static const float textPercent = 0.35f;
//        Rectangle<float> text_bounds(1.f + width * (1.f - textPercent) / 2.f, 0.5f * height, width * textPercent, 0.5f * height);
//
//        g.setColour(Colours::white);
//
//        g.drawFittedText(String("No Image"), text_bounds.getSmallestIntegerContainer(), Justification::horizontallyCentred, Justification::centred, 1);
//    }
}

