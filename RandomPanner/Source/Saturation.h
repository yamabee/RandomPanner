/*
  ==============================================================================

    Saturation.h
    Created: 29 Mar 2021 5:42:40pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <math.h>
#include <cmath>

class Saturation {
    
public:
    
    Saturation();
    
    void prepare(float newFs);
    
    void setAlpha(float newAlpha);
    
    void processSignal(juce::AudioBuffer<float> &buffer);
    void processSignal(float* leftChannel, float* rightChannel, const int N);
    float processSample(float x, int channel);
    
private:
    float Fs = 48000;
    float alpha = 0.9;
    
};
