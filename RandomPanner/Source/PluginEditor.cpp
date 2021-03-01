/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RandomPannerAudioProcessorEditor::RandomPannerAudioProcessorEditor (RandomPannerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    auto backgroundImage = ImageCache::getFromMemory(BinaryData::MainBackground_png, BinaryData::MainBackground_pngSize);
    
    if (! backgroundImage.isNull()) {
        backgroundImageComponent.setImage(backgroundImage,RectanglePlacement::stretchToFit);
    }
    else {
        jassert (! backgroundImage.isNull());
    }
    
    addAndMakeVisible(backgroundImageComponent);
    
    widthSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    widthSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(widthSlider);
    
    widthLabel.setText("Width",dontSendNotification);
    widthLabel.attachToComponent(&widthSlider, false);
    widthLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(widthLabel);
    
    noteSelector.addItem("Half", 1);
    noteSelector.addItem("Quarter", 2);
    noteSelector.addItem("8th", 3);
    noteSelector.addItem("16th", 4);
    noteSelector.addItem("Dotted Quarter", 5);
    noteSelector.addItem("Dotted 8th", 6);
    noteSelector.addItem("Dotted 16th", 7);
    noteSelector.setSelectedId(2);
    addAndMakeVisible(noteSelector);
    
    lpFrequencySlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lpFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    lpFrequencySlider.setTextValueSuffix(" Hz");
    lpFrequencySlider.setRange(50, 2000, 1);
    lpFrequencySlider.setSkewFactorFromMidPoint(1000.0);
    addAndMakeVisible(lpFrequencySlider);
    
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::lightblue);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::rebeccapurple);
    
    lpFrequencyLabel.setText("LPF Frequency",dontSendNotification);
    lpFrequencyLabel.attachToComponent(&lpFrequencySlider, false);
    lpFrequencyLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(lpFrequencyLabel);
    
    hpFrequencySlider   .setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    hpFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    hpFrequencySlider.setTextValueSuffix(" Hz");
    hpFrequencySlider.setRange(1000, 20000, 1);
    hpFrequencySlider.setSkewFactorFromMidPoint(5000);
    addAndMakeVisible(hpFrequencySlider);
    
    hpFrequencyLabel.setText("HPF Frequency",dontSendNotification);
    hpFrequencyLabel.attachToComponent(&hpFrequencySlider, false);
    hpFrequencyLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(hpFrequencyLabel);
    
    saturationSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    saturationSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    saturationSlider.setRange(0, 10, 1);
    addAndMakeVisible(saturationSlider);
    
    saturationLabel.setText("Saturation", dontSendNotification);
    saturationLabel.attachToComponent(&saturationSlider, false);
    saturationLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(saturationLabel);
    
    smoothingSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    smoothingSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    smoothingSlider.setRange(0, 1, 0.01);
    addAndMakeVisible(smoothingSlider);
    
    smoothingLabel.setText("Smoothing", dontSendNotification);
    smoothingLabel.attachToComponent(&smoothingSlider, false);
    smoothingLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(smoothingSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (800, 300);
    setResizable(false, false);
    
}

RandomPannerAudioProcessorEditor::~RandomPannerAudioProcessorEditor()
{
}

//==============================================================================
void RandomPannerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colours::black);

}

void RandomPannerAudioProcessorEditor::resized()
{
    backgroundImageComponent.setBounds(0.f, 0.f, 800.f, 300.f);
    
    const int border = 20;
    
    Rectangle<int> totalArea = getLocalBounds();
    Rectangle<int> panningSection(0, 0, 500, 300);
    
    Rectangle<int> filterSection(500, 0, 150, 300);
    Rectangle<int> saturationSection(650, 0, 150, 300);
    
    Rectangle<int> widthArea = panningSection.withTrimmedLeft(180).withTrimmedRight(180).withTrimmedBottom(130).withTrimmedTop(20).reduced(border);
    
    Rectangle<int> noteSelectorArea = panningSection.withTrimmedLeft(150).withTrimmedRight(150).withTrimmedTop(200).withTrimmedBottom(40).reduced(border);
    
    Rectangle<int> smoothingArea = panningSection.removeFromRight(170).withTrimmedTop(75).withTrimmedBottom(75).reduced(border);
    
    Rectangle<int> lpfArea = filterSection.removeFromTop(getHeight()/2).reduced(border);
    Rectangle<int> hpfArea = filterSection.removeFromBottom(getHeight()/2).reduced(border);
    
    Rectangle<int> saturationArea = saturationSection.withTrimmedTop(75).withTrimmedBottom(75).reduced(border);
    
    widthSlider.setBounds(widthArea);
    noteSelector.setBounds(noteSelectorArea);
    lpFrequencySlider.setBounds(lpfArea);
    hpFrequencySlider.setBounds(hpfArea);
    saturationSlider.setBounds(saturationArea);
    smoothingSlider.setBounds(smoothingArea);
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
