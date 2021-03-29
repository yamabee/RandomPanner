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
    
    bpmSlider.addListener(this);
    bpmSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    bpmSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    bpmSlider.setValue(audioProcessor.bpmValue);
    bpmSlider.setRange(40, 300, 1);
    addAndMakeVisible(bpmSlider);
    
    bpmLabel.setText("BPM", dontSendNotification);
    bpmLabel.attachToComponent(&bpmSlider, false);
    bpmLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(bpmLabel);
    
//    widthSlider.addListener(this);
//    widthSlider.setValue(audioProcessor.width);
//    widthSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    widthSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
//    addAndMakeVisible(widthSlider);
//
//    widthSlider.setLookAndFeel(&knobLookAndFeel);
//
//    widthLabel.setText("Width",dontSendNotification);
//    widthLabel.attachToComponent(&widthSlider, false);
//    widthLabel.setJustificationType(Justification::centred);
//    addAndMakeVisible(widthLabel);
    
    noteSelector.addListener(this);
    noteSelector.addItem("Whole", 1);
    noteSelector.addItem("Half", 2);
    noteSelector.addItem("Quarter", 3);
    noteSelector.addItem("8th", 4);
    noteSelector.addItem("16th", 5);
    noteSelector.addItem("Dotted Half", 6);
    noteSelector.addItem("Dotted Quarter", 7);
    noteSelector.addItem("Dotted 8th", 8);
    noteSelector.addItem("Dotted 16th", 9);
    noteSelector.setSelectedId(3);
    addAndMakeVisible(noteSelector);
    
//    lpFrequencySlider.addListener(this);
//    lpFrequencySlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    lpFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
//    lpFrequencySlider.setTextValueSuffix(" Hz");
//    lpFrequencySlider.setRange(50, 2000, 1);
//    lpFrequencySlider.setSkewFactorFromMidPoint(1000.0);
//    addAndMakeVisible(lpFrequencySlider);
//
//    lpFrequencySlider.setLookAndFeel(&knobLookAndFeel);
    
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::lightblue);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::rebeccapurple);
    
//    lpFrequencyLabel.setText("LPF Frequency",dontSendNotification);
//    lpFrequencyLabel.attachToComponent(&lpFrequencySlider, false);
//    lpFrequencyLabel.setJustificationType(Justification::centred);
//    addAndMakeVisible(lpFrequencyLabel);
    
//    hpFrequencySlider.addListener(this);
//    hpFrequencySlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    hpFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
//    hpFrequencySlider.setTextValueSuffix(" Hz");
//    hpFrequencySlider.setRange(1000, 20000, 1);
//    hpFrequencySlider.setSkewFactorFromMidPoint(5000);
//    addAndMakeVisible(hpFrequencySlider);
//
//    hpFrequencySlider.setLookAndFeel(&knobLookAndFeel);
//
//    hpFrequencyLabel.setText("HPF Frequency",dontSendNotification);
//    hpFrequencyLabel.attachToComponent(&hpFrequencySlider, false);
//    hpFrequencyLabel.setJustificationType(Justification::centred);
//    addAndMakeVisible(hpFrequencyLabel);
    
//    saturationSlider.addListener(this);
//    saturationSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    saturationSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
//    saturationSlider.setRange(0, 10, 1);
//    addAndMakeVisible(saturationSlider);
//
//    saturationSlider.setLookAndFeel(&knobLookAndFeel);
//
//    saturationLabel.setText("Saturation", dontSendNotification);
//    saturationLabel.attachToComponent(&saturationSlider, false);
//    saturationLabel.setJustificationType(Justification::centred);
//    addAndMakeVisible(saturationLabel);
    
    smoothingSlider.addListener(this);
    smoothingSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    smoothingSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    smoothingSlider.setRange(0, 1, 0.01);
    addAndMakeVisible(smoothingSlider);
    
    smoothingSlider.setLookAndFeel(&knobLookAndFeel);
    
    smoothingLabel.setText("Smoothing", dontSendNotification);
    smoothingLabel.attachToComponent(&smoothingSlider, false);
    smoothingLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(smoothingSlider);
    
//    tempoSyncButton.addListener(this);
//    tempoSyncButton.setButtonText("Tempo Sync'd");
//    tempoSyncButton.setToggleState(audioProcessor.tempoSyncd, dontSendNotification);
//    tempoSyncButton.setRadioGroupId(1);
//    addAndMakeVisible(tempoSyncButton);
//
//    notTempoSyncButton.addListener(this);
//    notTempoSyncButton.setButtonText("Sync Off");
//    notTempoSyncButton.setToggleState(!audioProcessor.tempoSyncd, dontSendNotification);
//    notTempoSyncButton.setRadioGroupId(1);
//    addAndMakeVisible(notTempoSyncButton);
//
//    noteSelector.setEnabled(audioProcessor.tempoSyncd);
    
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
    
//    Rectangle<int> totalArea = getLocalBounds();
    Rectangle<int> panningSection(0, 0, 500, 300);
    
    Rectangle<int> filterSection(500, 0, 150, 300);
    Rectangle<int> saturationSection(650, 0, 150, 300);
    
    Rectangle<int> bpmArea = panningSection.withTrimmedRight(325).withTrimmedTop(20).withTrimmedBottom(20).reduced(border);
    
//    Rectangle<int> widthArea = panningSection.withTrimmedLeft(180).withTrimmedRight(180).withTrimmedBottom(130).withTrimmedTop(20).reduced(border);
    
    Rectangle<int> noteSelectorArea = panningSection.withTrimmedLeft(150).withTrimmedRight(150).withTrimmedTop(200).withTrimmedBottom(40).reduced(border);
    
    Rectangle<int> smoothingArea = panningSection.removeFromRight(170).withTrimmedTop(75).withTrimmedBottom(75).reduced(border);
    
//    Rectangle<int> lpfArea = filterSection.removeFromTop(getHeight()/2).withTrimmedTop(20).reduced(border);
//    Rectangle<int> hpfArea = filterSection.removeFromBottom(getHeight()/2).withTrimmedTop(20).reduced(border);
    
//    Rectangle<int> saturationArea = saturationSection.withTrimmedTop(75).withTrimmedBottom(75).reduced(border);
    
    bpmSlider.setBounds(bpmArea);
//    widthSlider.setBounds(widthArea);
    noteSelector.setBounds(noteSelectorArea);
//    lpFrequencySlider.setBounds(lpfArea);
//    hpFrequencySlider.setBounds(hpfArea);
//    saturationSlider.setBounds(saturationArea);
    smoothingSlider.setBounds(smoothingArea);
    
//    tempoSyncButton.setBounds(275,175,100,40);
//    notTempoSyncButton.setBounds(100, 175, 100, 40);
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void RandomPannerAudioProcessorEditor::sliderValueChanged(Slider* slider) {
//    if (slider == &widthSlider) {
//        audioProcessor.width = widthSlider.getValue();
//    }
//    if (slider == &lpFrequencySlider) {
//        audioProcessor.lpFrequency = lpFrequencySlider.getValue();
//    }
//    if (slider == &hpFrequencySlider) {
//        audioProcessor.hpFrequency = hpFrequencySlider.getValue();
//    }
//    if (slider == &saturationSlider) {
//        audioProcessor.saturation = saturationSlider.getValue();
//    }
    if (slider == &smoothingSlider) {
        audioProcessor.smoothing = smoothingSlider.getValue();
    }
}

void RandomPannerAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox) {
    if (comboBox == &noteSelector) {
        if (noteSelector.getSelectedId() == 1) {
            // whole
            audioProcessor.noteSelect = RandomPanning::NoteSelection::WHOLE;
        }
        
        if (noteSelector.getSelectedId() == 2) {
            // half
            audioProcessor.noteSelect = RandomPanning::NoteSelection::HALF;
        }
        
        if (noteSelector.getSelectedId() == 3) {
            // quarter
            audioProcessor.noteSelect = RandomPanning::NoteSelection::QUARTER;
        }
        
        if (noteSelector.getSelectedId() == 4) {
            // 8th
            audioProcessor.noteSelect = RandomPanning::NoteSelection::EIGHTH;
        }
        
        if (noteSelector.getSelectedId() == 5) {
            // 16th
            audioProcessor.noteSelect = RandomPanning::NoteSelection::SIXTEENTH;
        }
        
        if (noteSelector.getSelectedId() == 6) {
            // dotted half
            audioProcessor.noteSelect = RandomPanning::NoteSelection::DOTTEDHALF;
        }
        
        if (noteSelector.getSelectedId() == 7) {
            // dotted quarter
            audioProcessor.noteSelect = RandomPanning::NoteSelection::DOTTEDQUARTER;
        }
        
        if (noteSelector.getSelectedId() == 8) {
            // dotted 8th
            audioProcessor.noteSelect = RandomPanning::NoteSelection::DOTTEDEIGHTH;
        }
        
        if (noteSelector.getSelectedId() == 9) {
            // dotted 16th
            audioProcessor.noteSelect = RandomPanning::NoteSelection::DOTTEDSIXTEENTH;
        }
    }
}

//void RandomPannerAudioProcessorEditor::buttonClicked(Button* button) {
//    if (button == &tempoSyncButton) {
//        audioProcessor.tempoSyncd = true;
//        noteSelector.setEnabled(true);
//    }
//
//    if (button == &notTempoSyncButton) {
//        audioProcessor.tempoSyncd = false;
//        noteSelector.setEnabled(false);
//    }
//}

