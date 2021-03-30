/*
  ==============================================================================

    Saturation.cpp
    Created: 29 Mar 2021 5:42:40pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#include "Saturation.h"

// Constructor
Saturation::Saturation(){}

void Saturation::prepare(float newFs) {
    Fs = newFs;
    
}

void Saturation::setAlpha(float newAlpha) {
    alpha = newAlpha;
}

void Saturation::processSignal(juce::AudioBuffer<float> &buffer) {
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    int N = buffer.getNumSamples();
    
    processSignal(leftChannel, rightChannel, N);
}

void Saturation::processSignal(float* leftChannel, float* rightChannel, const int N) {
    for (int n = 0; n < N; ++n) {
        float xL = leftChannel[n];
        float xR = rightChannel[n];
        
        leftChannel[n] = processSample(xL,0);
        rightChannel[n] = processSample(xR,1);
    }
}

float Saturation::processSample(float x, int channel) {
    
    float y = (2/M_PI)*atan(x * alpha);
    
    return y;
}
