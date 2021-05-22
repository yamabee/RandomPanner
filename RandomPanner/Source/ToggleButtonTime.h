/*
  ==============================================================================

    ToggleButtonTime.h
    Created: 19 May 2021 1:15:25pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ToggleButtonTime : public LookAndFeel_V4 {
    
public:
    ToggleButtonTime() {};
    virtual ~ToggleButtonTime() {};
    
    virtual void drawToggleButton(Graphics &g, ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        
        Image buttonImage;
        
        if (button.getToggleState() == 0) {
            buttonImage = ImageCache::getFromMemory(BinaryData::TimeOff_png, BinaryData::TimeOff_pngSize);
            
        }
        else {
            buttonImage = ImageCache::getFromMemory(BinaryData::TimeOnLime_png, BinaryData::TimeOnLime_pngSize);
            
        }
        
        g.drawImage(buttonImage, // image
                    (int)0, (int)0, button.getWidth(), button.getHeight(), //desination
                    0, 0, buttonImage.getWidth(), buttonImage.getHeight()); //source
        
    }
};
