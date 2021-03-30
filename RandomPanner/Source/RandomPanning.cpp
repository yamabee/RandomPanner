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

void RandomPanning::processSignal(juce::AudioBuffer<float> &buffer) {
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    int N = buffer.getNumSamples();
    
    processSignal(leftChannel, rightChannel, N);
    
}
void RandomPanning::processSignal(float* leftChannel, float* rightChannel, const int N) {
    for (int n = 0; n < N; n++) {
        float xL = leftChannel[n];
        float xR = rightChannel[n];
        
        leftChannel[n] = processSample(xL,0);
        rightChannel[n] = processSample(xR,1);
        
        
    }
}

//void RandomPanning::processSignal(float *signal, const int numSamples, const int chan) {
//
//    for (int n = 0; n < numSamples; n++) {
//        float x = signal[n]; // get value at memory location
//        x = processSample(x, chan);
//        signal[n] = x;
//
//    }
//}

float RandomPanning::processSample(float x, int chan) {
    
    if (chan == 0) {
        leftAmpSmooth = alphaSmooth * leftAmpSmooth + (1-alphaSmooth) * leftAmp;
        y = leftAmpSmooth * x;
    }
    else if (chan == 1) {
        rightAmpSmooth = alphaSmooth * rightAmpSmooth + (1-alphaSmooth) * rightAmp;
        y = rightAmpSmooth * x;
        
        count++;
        
        if (count >= timeSamples) {
            setPan();
            count = 1;
            
        }
    }
    
    return y;
    
}

//float RandomPanning::processSampleL(float x) {
//
//
//    leftAmpSmooth = alphaSmooth * leftAmpSmooth + (1-alphaSmooth) * leftAmp;
//    y = leftAmpSmooth * x;
//
//    return y;
//}
//
//float RandomPanning::processSampleR(float x) {
//    rightAmpSmooth = alphaSmooth * rightAmpSmooth + (1-alphaSmooth) * rightAmp;
//    y = rightAmpSmooth * x;
//    
//    count++;
//
//    if (count >= timeSamples) {
//        setPan();
//        count = 1;
//    }
//
//    return y;
//}


void RandomPanning::prepare(float newFs) {
    Fs = newFs;
    timeSamples = round(timeMS*Fs/1000.f);
    
}

void RandomPanning::setBPM(float newBPM) {
    bpm = newBPM;
    setBeatSamples();
}

void RandomPanning::setTimeMS(float newTimeMS) {
    timeMS = newTimeMS;
    timeSamples = round(timeMS*Fs/1000.f);
//    count = 1;
    
}

void RandomPanning::setNoteDuration(NoteSelection newNoteSelection) {
    noteSelect = newNoteSelection;
    
    switch (noteSelect) {
        case WHOLE:
            noteDuration = 4;
            break;
            
        case HALF:
            noteDuration = 2;
            break;
            
        case QUARTER:
            noteDuration = 1;
            
            break;
            
        case EIGHTH:
            noteDuration = 0.5;
            break;
            
        case SIXTEENTH:
            noteDuration = 0.25;
            break;
            
        case DOTTEDHALF:
            noteDuration = 3;
            break;
            
        case DOTTEDQUARTER:
            noteDuration = 1.5;
            break;
            
        case DOTTEDEIGHTH:
            noteDuration = 0.75;
            break;
            
        case DOTTEDSIXTEENTH:
            noteDuration = 0.375;
            break;
            
        default:
            noteDuration = 1;
            break;
    }
    
    setBeatSamples();
    
}

void RandomPanning::setBeatSamples(){
    float bps = bpm/60.f;
    float secondsPerBeat = 1.f/bps;
//    beatSamples = secondsPerBeat * Fs;
//    noteSamples = beatSamples * noteDuration;
    float secondsPerNote = noteDuration * secondsPerBeat;
    float msPerNote = secondsPerNote * 1000.f;
    setTimeMS(msPerNote);
    
}

void RandomPanning::setSmoothing(float newAlphaSmooth) {
    alphaSmooth = newAlphaSmooth;
}

void RandomPanning::setPan() {
    setRandomNumber();
    panValue = randomNumber;
    leftAmp = sqrt(1-panValue);
    rightAmp = sqrt(panValue);
    
}

void RandomPanning::setRandomNumber() {
    
    if (randomNumberPrev <= 0.5) {
        randomNumber = (float)rand()/RAND_MAX;
        while (randomNumber <= 0.5) {
            randomNumber = (float)rand()/RAND_MAX;
        }
    }
    
    else if (randomNumberPrev > 0.5) {
        randomNumber = (float)rand()/RAND_MAX;
        while (randomNumber > 0.5) {
            randomNumber = (float)rand()/RAND_MAX;
        }
    }
    
    randomNumberPrev = randomNumber;
    
}
