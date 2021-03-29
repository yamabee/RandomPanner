/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RandomPannerAudioProcessor::RandomPannerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

RandomPannerAudioProcessor::~RandomPannerAudioProcessor()
{
}

//==============================================================================
const juce::String RandomPannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RandomPannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RandomPannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RandomPannerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RandomPannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RandomPannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RandomPannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RandomPannerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RandomPannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void RandomPannerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RandomPannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    randPan.prepare(sampleRate);
    
}

void RandomPannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RandomPannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
//    const AudioChannelSet& mainInput = layouts.getMainInputChannelSet();
//    const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();
//
//    return mainInput.size() == 1 && mainOutput.size() == 2;
    
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RandomPannerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
//        if (tempoSyncd) {
//            playHead = this->getPlayHead(); // playhead pointer comes from the DAW we are assigning to the internal pointer in our plugin
//            playHead->getCurrentPosition(currentPositionInfo); // passed by reference so it can be overwritten
        
//            float newBPM = currentPositionInfo.bpm;
        
            if (bpm != bpmValue) {
                // update randPan
                randPan.setBPM(bpmValue);
                bpm = bpmValue;
            }
            
            randPan.setNoteDuration(noteSelect);
//        }
    
//        else{ // not tempo sync'd
//            randPan.setBPM(timeMS);
//
//        }
    
    randPan.processSignal(buffer);
    
}

//==============================================================================
bool RandomPannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RandomPannerAudioProcessor::createEditor()
{
    return new RandomPannerAudioProcessorEditor (*this);
}

//==============================================================================
void RandomPannerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RandomPannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RandomPannerAudioProcessor();
}
