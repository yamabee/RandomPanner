/*
  ==============================================================================

    TransientAnalysis.cpp
    Created: 17 May 2021 10:00:05am
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#include "TransientAnalysis.h"


TransientAnalysis::TransientAnalysis() {}
TransientAnalysis::~TransientAnalysis() {}

void TransientAnalysis::processSignal(const juce::AudioBuffer<float> &buffer) {
    const float* leftChannel = buffer.getReadPointer(0);
    const float* rightChannel = buffer.getReadPointer(1);
    const int N = buffer.getNumSamples();
    
    processSignal(leftChannel, rightChannel, N);
    
}

void TransientAnalysis::processSignal(const float* leftChannel, const float* rightChannel, const int N) {
    for (int n = 0; n < N; n++) {
        float xL = leftChannel[n];
        float xR = rightChannel[n];
        
        transientSample = xL + xR;
        
    }
}

void TransientAnalysis::analyzeSample(float x, int chan) {
    float envFast = (1-gainSmoothFast) * 2 * abs(x) + gainSmoothFast * fbFast;
    fbFast = envFast;
    
    float envSlow = (1-gainSmoothSlow) * 3 * abs(x) + gainSmoothSlow * fbSlow;
    fbSlow = envSlow;
    
    transientDiff = envFast - envSlow;
    
    if (chan == 0) {
        if (flag == false) {
            if (transientDiff >= threshold) {
                //needs to set the random panning change
                flag = true;
                
            }
        }
        if (transientDiff < 0)
            flag = false;
    }
}

void TransientAnalysis::setThreshold(float newThreshold) {
    threshold = newThreshold;
    
}

bool TransientAnalysis::getFlag() {
    return flag;
    
}
