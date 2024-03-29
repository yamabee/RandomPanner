/*
  ==============================================================================

    RandomPanning.cpp
    Created: 3 Mar 2021 3:12:08pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#include "RandomPanning.h"

//============================================================================================
// CONSTRUCTOR
//============================================================================================

RandomPanning::RandomPanning(){}

//============================================================================================
// PROCESSING BY CHANNEL
//============================================================================================

void RandomPanning::processSignal(float* leftChannel, float* rightChannel, const int N) {
    for (int n = 0; n < N; n++) {
        float xL = leftChannel[n];
        float xR = rightChannel[n];
        
        leftChannel[n] = processSample(xL,0);
        rightChannel[n] = processSample(xR,1);
        
        
    }
}

float RandomPanning::processSample(float x, int chan) {
    if (chan == 0) {
        leftAmpSmooth = alphaSmooth * leftAmpSmooth + (1-alphaSmooth) * leftAmp;
        y = leftAmpSmooth * x;
        
        count++;
        
        if (count >= timeSamples) {
            setPan();
            count = 1;
            
        }
    }
    else if (chan == 1) {
        rightAmpSmooth = alphaSmooth * rightAmpSmooth + (1-alphaSmooth) * rightAmp;
        y = rightAmpSmooth * x;
        
    }
    return y;
}

float RandomPanning::processSampleTransient(float x, int chan) {
    if (chan == 0) {
        leftAmpSmooth = alphaSmooth * leftAmpSmooth + (1-alphaSmooth) * leftAmp;
        y = leftAmpSmooth * x;
    }
    
    else if (chan == 1) {
        rightAmpSmooth = alphaSmooth * rightAmpSmooth + (1-alphaSmooth) * rightAmp;
        y = rightAmpSmooth * x;
    }
    
    return y;
}

//============================================================================================
// PROCESSING BY BLOCK
//============================================================================================

void RandomPanning::processSignal(juce::AudioBuffer<float> &buffer) {
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    int N = buffer.getNumSamples();
    
    processSignal(leftChannel, rightChannel, N);
    
}

void RandomPanning::processSample(float x, float &leftSample, float &rightSample) {
    leftAmpSmooth = alphaSmooth * leftAmpSmooth + (1-alphaSmooth) * leftAmp;
    leftSample = leftAmpSmooth * x;
    
    rightAmpSmooth = alphaSmooth * rightAmpSmooth + (1-alphaSmooth) * rightAmp;
    rightSample = rightAmpSmooth * x;
    
    count++;
    
    if (count >= timeSamples) {
        setPan();
        count = 1;
        
    }
}

//============================================================================================
// GETTERS AND SETTERS
//============================================================================================

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
    float secondsPerNote = noteDuration * secondsPerBeat;
    float msPerNote = secondsPerNote * 1000.f;
    setTimeMS(msPerNote);
    
}

void RandomPanning::setSmoothing(float newAlphaSmooth) {
    alphaSmooth = newAlphaSmooth;
}

void RandomPanning::setPan() {
    setRandomNumber();
    panValue = (float)randomNumber/100.f;
    leftAmp = sqrt(1-panValue);
    rightAmp = sqrt(panValue);
    
}

void RandomPanning::setRandomNumber() {
    if (randomNumberPrev <= 50) {
        randomNumber = rand() % (maxWidth - minWidth + 1) + minWidth;
        while (randomNumber <= 50) {
            randomNumber = rand() % (maxWidth - minWidth + 1) + minWidth;
        }
    }
    else if (randomNumberPrev > 50) {
        randomNumber = rand() % (maxWidth - minWidth + 1) + minWidth;
        while (randomNumber > 50) {
            randomNumber = rand() % (maxWidth - minWidth + 1) + minWidth;
        }
    }
    
    randomNumberPrev = randomNumber;
    
}

void RandomPanning::setWidth(float newWidth) {
    width = newWidth;
    
    maxWidth = 50 + width/2;
    minWidth = 50 - width/2;
    
}
