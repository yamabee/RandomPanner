/*
  ==============================================================================

    TransientAnalysis.h
    Created: 17 May 2021 10:00:05am
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "RandomPanning.h"

class TransientAnalysis {
    
public:
    // Constructor
    TransientAnalysis();
    // Deconstructor
    ~TransientAnalysis();
    
    void processSignal(const float* leftChannel,const float* rightChannel, const int N);
    void processSignal(const juce::AudioBuffer<float> &buffer);
    
    void processSample(float x, float const &leftSample, float const &rightSample);
    void analyzeSample(float x, int chan);
    
    void prepare(float newFs);
    
    void setThreshold(float newThreshold);
    
    bool getFlag();
    
private:
    
    float Fs = 48000;
    
    float gainSmoothFast = 0.9991f;
    float fbFast = 0; // feedback for the "fast" envelope
    float gainSmoothSlow = 0.9999f; // gain smoothing for "slow" envelope
    float fbSlow = 0;
    
    float envFast = 0;
    float envSlow = 0;
    
    float transientSample = 0;
    float transientDiff = 0;
    
    float threshold = 0.001f;
    bool flag = false;
    
};
