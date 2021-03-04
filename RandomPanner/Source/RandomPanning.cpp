/*
  ==============================================================================

    RandomPanning.cpp
    Created: 3 Mar 2021 3:12:08pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#include "RandomPanning.h"

// Constructor
RandomPanning::RandomPanning(){}

void RandomPanning::processSignal(float *signal, const int numSamples, const int chan) {
    
    for (int n = 0; n < numSamples; n++) {
        float x = signal[n]; // get value at memory location
        x = processSample(x, chan);
        signal[n] = x;
        
    }
}

float RandomPanning::processSample(float x, int chan) {
    
    if (chan == 1) {
        leftAmpSmooth = alphaSmooth * leftAmpSmooth + (1-alphaSmooth) * leftAmp;
        y = leftAmp * x;
    }
    else if (chan == 2) {
        rightAmpSmooth = alphaSmooth * rightAmpSmooth + (1-alphaSmooth) * rightAmp;
        y = rightAmp * x;
    }
    
    count++;
    
    if (count == noteSamples) {
        setPan();
        count = 1;
        
    }
    
    return y;
    
}

void RandomPanning::prepare(float newFs) {
    Fs = newFs;
    setBeatSamples();
    noteSamples = beatSamples * noteDuration;
    
}

void RandomPanning::setBPM(float newBPM) {
    bpm = newBPM;
    setBeatSamples();
    noteSamples = beatSamples * noteDuration;
    
}

void RandomPanning::setNoteDuration(float newNoteDuration) {
    noteDuration = newNoteDuration;
    noteSamples = beatSamples * noteDuration;
    
}

void RandomPanning::setBeatSamples(){
    float bps = bpm/60;
    float secondsPerBeat = 1/bps;
    beatSamples = secondsPerBeat * Fs;
    
}

void RandomPanning::setPan() {
    panValue = rand() % 1;
    leftAmp = sqrt(1-panValue);
    rightAmp = sqrt(panValue);
    
}



