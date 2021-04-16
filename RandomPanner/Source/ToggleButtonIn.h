/*
  ==============================================================================

    ToggleButtonIn.h
    Created: 13 Apr 2021 2:08:14pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ToggleButtonIn : public LookAndFeel_V4 {
    
public:
    ToggleButtonIn() {};
    virtual ~ToggleButtonIn() {};
    
    virtual void drawToggleButton(Graphics &g, ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        
        Image buttonImage;
        
        if (button.getToggleState() == 0) {
            buttonImage = ImageCache::getFromMemory(BinaryData::InOffButton_png, BinaryData::InOffButton_pngSize);
            
        }
        else {
            buttonImage = ImageCache::getFromMemory(BinaryData::InOnButtonLime_png, BinaryData::InOnButtonLime_pngSize);
            
        }
        
        g.drawImage(buttonImage, // image
                    (int)0, (int)0, button.getWidth(), button.getHeight(), //desination
                    0, 0, buttonImage.getWidth(), buttonImage.getHeight()); //source
        
    }
};
