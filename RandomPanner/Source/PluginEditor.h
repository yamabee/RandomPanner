/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Saturation.h"
#include "SmallKnob.h"
#include "LargeKnob.h"
#include "ToggleButtonIn.h"
#include "ToggleButtonSync.h"

//==============================================================================
/**
*/class RandomPannerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            public juce::Slider::Listener,
                                            public juce::ComboBox::Listener,
                                            public juce::Button::Listener

{
public:
    RandomPannerAudioProcessorEditor (RandomPannerAudioProcessor&);
    ~RandomPannerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBox) override;
    void buttonClicked(Button* button) override;
    
    void setButtonEnabled(Button* button);
    void setButtonDisabled(Button* button);
    void setFontParameters(Font* font);

private:
    ImageComponent backgroundImageComponent;
    
    RandomPannerAudioProcessor& audioProcessor;
    
    ComboBox noteSelector;
    Label noteSelectorLabel;
    
    Slider widthSlider;
    Label widthLabel;
    
    Slider timeSlider;
    Label timeLabel;
    
    Slider lpCutOffSlider;
    Label lpFrequencyLabel;
    
    Slider hpCutOffSlider;
    Label hpFrequencyLabel;
    
    Slider saturationSlider;
    Label saturationLabel;
    
    Slider smoothingSlider;
    Label smoothingLabel;
    
    SmallKnob smallKnob;
    LargeKnob largeKnob;
//    SliderLookAndFeel slideLAF;
    
    ToggleButtonIn inButton;
    ToggleButtonSync syncButton;
    
    enum class ButtonState {
        Enabled,
        Disabled
    };
    
    ButtonState saturationButtonState { ButtonState::Disabled };
    ButtonState lpButtonState { ButtonState::Disabled };
    ButtonState hpButtonState { ButtonState::Disabled };
    ButtonState tempoSyncButtonState { ButtonState::Enabled };

    ToggleButton saturationEnabledButton;
    ToggleButton lpEnabledButton;
    ToggleButton hpEnabledButton;
    ToggleButton tempoSyncEnabledButton;
    
    Font font;
    
public:
    
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>> comboBoxAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomPannerAudioProcessorEditor)
};
