/*
  ==============================================================================

    ToggleButtonSync.h
    Created: 13 Apr 2021 2:08:32pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ToggleButtonSync : public LookAndFeel_V4 {
    
public:
    ToggleButtonSync() {};
    virtual ~ToggleButtonSync() {};
    
    virtual void drawToggleButton(Graphics &g, ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        
        Image buttonImage;
        
        if (button.getToggleState() == 0) {
            buttonImage = ImageCache::getFromMemory(BinaryData::SyncOff_png, BinaryData::SyncOff_pngSize);
            
        }
        else {
            buttonImage = ImageCache::getFromMemory(BinaryData::SyncOnLime_png, BinaryData::SyncOnLime_pngSize);
            
        }
        
        g.drawImage(buttonImage, // image
                    (int)0, (int)0, button.getWidth(), button.getHeight(), //desination
                    0, 0, buttonImage.getWidth(), buttonImage.getHeight()); //source
        
    }
};
