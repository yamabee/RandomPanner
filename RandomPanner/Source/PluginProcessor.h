/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RandomPanning.h"
#include "Saturation.h"
#include "Biquad.h"

//==============================================================================
/**
*/
class RandomPannerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    RandomPannerAudioProcessor();
    ~RandomPannerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    

    
    float timeMS = 100.f;
    RandomPanning::NoteSelection noteSelect = RandomPanning::NoteSelection::QUARTER;
    
    bool tempoSyncd = true;
    
//    float width = 0.f;
    float lpFrequency = 20000.f;
    float hpFrequency = 50.f;
    int satAlpha = 1;
    float smoothing = 0.15;
    
    bool saturationOn = true;
    
private:
    
    RandomPanning randPan;
    Saturation saturation;
    Biquad lowPass;
    Biquad highPass;
    
    AudioPlayHead* playHead; // pointer to the object
    AudioPlayHead::CurrentPositionInfo currentPositionInfo; // need to check playHead each time through the buffer
    
    float bpm = 120.f;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomPannerAudioProcessor)

};
