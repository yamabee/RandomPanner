/*
  ==============================================================================

    ComboBoxLookAndFeel.h
    Created: 13 Apr 2021 5:36:03pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ComboBoxLookAndFeel : public LookAndFeel_V4 {
    
public:
    
    ComboBoxLookAndFeel() {};
    virtual ~ComboBoxLookAndFeel() {};

    void drawComboBox (Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox&) override;
    
    Font getComboBoxFont (ComboBox& box) override {
        Font font;
        font.setSizeAndStyle(box.getHeight(), "Avenir Next", box.getWidth(), 10);
        return font;
    };
    
    void positionComboBoxText (ComboBox&, Label&) override;
