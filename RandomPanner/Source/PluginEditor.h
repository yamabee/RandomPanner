/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KnobLookAndFeel.h"

//==============================================================================
/**
*/class RandomPannerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            public juce::Slider::Listener,
                                            public juce::ComboBox::Listener
//                                            public juce::Button::Listener

{
public:
    RandomPannerAudioProcessorEditor (RandomPannerAudioProcessor&);
    ~RandomPannerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBox) override;
//    void buttonClicked(Button* button) override;

private:
    ImageComponent backgroundImageComponent;
    
    ComboBox noteSelector;
    Label noteSelectorLabel;
    
    Slider widthSlider;
    Label widthLabel;
    
    Slider bpmSlider;
    Label bpmLabel;
    
    Slider lpFrequencySlider;
    Label lpFrequencyLabel;
    ToggleButton enableLPFrequency;
    
    Slider hpFrequencySlider;
    Label hpFrequencyLabel;
    ToggleButton enableHPFrequency;
    
    Slider saturationSlider;
    Label saturationLabel;
    
    Slider smoothingSlider;
    Label smoothingLabel;
    
    KnobLookAndFeel knobLookAndFeel;
    
//    ToggleButton tempoSyncButton;
//    ToggleButton notTempoSyncButton;
    
    RandomPannerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomPannerAudioProcessorEditor)
};
