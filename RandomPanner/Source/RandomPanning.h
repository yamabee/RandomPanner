/*
  ==============================================================================

    RandomPanning.h
    Created: 3 Mar 2021 3:12:08pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <random>

class RandomPanning {
    
public:
    // Constructor
    RandomPanning();
    
    void processSignal(float* signal, const int numSamples, const int chan);
    float processSample(float x, int chan);
    
    void prepare(float newFs);
    
    void setBPM(float newBPM);
    void setBeatSamples();
    void setNoteDuration(float newNoteDuration);
    void setPan();
    
private:
    float Fs = 48000.f;
    float bpm = 120.f;
    float noteDuration = 1.f; // 1 - quarter, 2 - half, 0.5 - 8th, 0.25 - 16th
    
    float beatSamples = 24000.f;
    float noteSamples = 24000.f;
    
    float panValue = rand() % 1;
    float leftAmp = sqrt(0.5f);
    float rightAmp = sqrt(0.5f);
    
    float alphaSmooth = 0.1155f; // smoothing filter
    float leftAmpSmooth = sqrt(0.5f);
    float rightAmpSmooth = sqrt(0.5f);
    
    int count = 1;
    float y = 0.f;
    
//    static const int BUFFERSIZE = 960000;
//    float buffer[2][BUFFERSIZE] = {0.f};
//    int writeIndex[2] = {47999,47999}; // write pointer to buffer array
};
