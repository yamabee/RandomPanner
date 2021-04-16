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
                       ), treeState(*this, nullptr, "RandPanSliderParams", createParameterLayout())
#endif
{
}

RandomPannerAudioProcessor::~RandomPannerAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout RandomPannerAudioProcessor::createParameterLayout(){
    std::vector<std::unique_ptr<RangedAudioParameter>>(params);
    
    params.push_back(std::make_unique<AudioParameterInt>("timeValue", "Time", 0, 1000, 300));
    params.push_back(std::make_unique<AudioParameterInt>("widthValue", "Width", 2, 100, 100));
    params.push_back(std::make_unique<AudioParameterFloat>("smoothValue", "Smoothing", 0.f, 1.f, 0.01f));
    params.push_back(std::make_unique<AudioParameterInt>("lpCutOffValue", "Low Pass Cut-Off", 50, 18000, 1000));
    params.push_back(std::make_unique<AudioParameterInt>("hpCutOffValue", "High Pass Cut-Off", 50, 18000, 1000));
    params.push_back(std::make_unique<AudioParameterInt>("noteSelection", "Note Selection", 1, 9, 3));
    params.push_back(std::make_unique<AudioParameterFloat>("saturationValue", "Saturation", 1, 10, 1));
    params.push_back(std::make_unique<AudioParameterBool>("syncButton", "Sync Button", true));
    params.push_back(std::make_unique<AudioParameterBool>("lpButton", "Low Pass Button", false));
    params.push_back(std::make_unique<AudioParameterBool>("hpButton", "High Pass Button", false));
    params.push_back(std::make_unique<AudioParameterBool>("saturationButton", "Saturation Button", false));
    
    return { params.begin(), params.end() };
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
    
    lowPass.setFs(sampleRate);
    lowPass.setFilterType(Biquad::LPF);
    lowPass.setFreq(lpFrequency);
    
    highPass.setFs(sampleRate);
    highPass.setFilterType(Biquad::HPF);
    highPass.setFreq(hpFrequency);
    
}

void RandomPannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RandomPannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    
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
    
    randPan.setSmoothing(smoothing);
    saturation.setAlpha(satAlpha);
    lowPass.setFreq((double)lpFrequency);
    highPass.setFreq((double)hpFrequency);
    randPan.setWidth(width);
    
    if (tempoSyncd) {
        playHead = this->getPlayHead(); // playhead pointer comes from the DAW we are assigning to the internal pointer in our plugin
        playHead->getCurrentPosition(currentPositionInfo); // passed by reference so it can be overwritten
        
        float newBPM = currentPositionInfo.bpm;
        
        if (bpm != newBPM) {
            // update randPan
            randPan.setBPM(newBPM);
            bpm = newBPM;
        }
        randPan.setNoteDuration(noteSelect);
        
    }
    
    else { // not tempo sync'd
        randPan.setTimeMS(timeMS);
        
    }
    
    randPan.processSignal(buffer);
    saturation.processSignal(buffer);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            for (int n = 0; n < buffer.getNumSamples() ; ++n){
                float x = buffer.getReadPointer(channel)[n];
                x = randPan.processSample(x,channel);
                
                if (saturationEnabled) {
                    x = saturation.processSample(x, channel);
                }
                
                if (lpEnabled) {
                    x = lowPass.processSample(x, channel);
                }
                
                if (hpEnabled) {
                    x = highPass.processSample(x, channel);
                }
                
                buffer.getWritePointer(channel)[n] = x;
            }
        }
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
    auto currentState = treeState.copyState();
    std::unique_ptr<XmlElement> xml (currentState.createXml());
    copyXmlToBinary(*xml, destData);
    
}

void RandomPannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    if ( xml && xml->hasTagName(treeState.state.getType())) {
        treeState.replaceState(ValueTree::fromXml(*xml));
    }
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RandomPannerAudioProcessor();
}

void RandomPannerAudioProcessor::setButtonState(bool& buttonState) {
    if (buttonState == true) {
        buttonState = false;
    }
    else if (buttonState == false) {
        buttonState = true;
    }
}
