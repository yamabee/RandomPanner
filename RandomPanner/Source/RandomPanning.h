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
    
//    void processSignal(float* signal, const int numSamples, const int chan);
    void processSignal(float* leftChannel, float* rightChannel, const int N);
    void processSignal(juce::AudioBuffer<float> &buffer);
    void processSample(float x, float &leftSample, float &rightSample);
    float processSample(float x, int chan);
    
    float processSampleL(float x);
    float processSampleR(float x);
    
    void prepare(float newFs);
    
    void setTimeMS(float newTimeMS);
    
    void setBPM(float newBPM);
    
    enum NoteSelection {WHOLE, HALF, QUARTER, EIGHTH, SIXTEENTH, DOTTEDHALF, DOTTEDQUARTER, DOTTEDEIGHTH, DOTTEDSIXTEENTH};
    void setNoteDuration(NoteSelection newNoteSelection);
    
    void setBeatSamples();
    
    void setPan();
    void setRandomNumber();
    
    void setWidth(float newWidth);
    
    void setSmoothing(float newAlphaSmooth);
    
private:
    float Fs = 48000.f;
    float bpm = 120.f;
    
    NoteSelection noteSelect = QUARTER;
    
    float noteDuration = 1.f; // 1 - quarter, 2 - half, 0.5 - 8th, 0.25 - 16th
    
    float timeMS = 500.f;
    float timeSamples = round(timeMS*Fs/1000.f); // number of samples before random value changes
    
    int width = 100;
    int minWidth = 0;
    int maxWidth = 100;
    
    float panValue = rand() % 1;
    float leftAmp = sqrt(0.5f);
    float rightAmp = sqrt(0.5f);
    
    float alphaSmooth = 0.1155f; // smoothing filter
    float leftAmpSmooth = sqrt(0.5f);
    float rightAmpSmooth = sqrt(0.5f);
    
    int count = 1;
    float y = 0.f;
    
    int randomNumber = 0;
    int randomNumberPrev = 0;
    
};
