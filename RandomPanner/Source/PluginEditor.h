/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Saturation.h"
#include "KnobLookAndFeel.h"

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

private:
    ImageComponent backgroundImageComponent;
    
    RandomPannerAudioProcessor& audioProcessor;
    
    ComboBox noteSelector;
    Label noteSelectorLabel;
    
//    Slider widthSlider;
//    Label widthLabel;
    
    Slider timeSlider;
    Label timeLabel;
    
    Slider lpCutOffSlider;
    Label lpFrequencyLabel;
    
//    ToggleButton enableLPFrequency;
    
    Slider hpFrequencySlider;
    Label hpFrequencyLabel;
//    ToggleButton enableHPFrequency;
    
    Slider saturationSlider;
    Label saturationLabel;
    
    Slider smoothingSlider;
    Label smoothingLabel;
    
    KnobLookAndFeel knobLookAndFeel;
//    KnobLookAndFeelV2 knobLookAndFeelV2;
    
    ToggleButton tempoSyncButton;
    ToggleButton notTempoSyncButton;
    
//    ToggleButton satEnabledButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomPannerAudioProcessorEditor)
};
