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
    
    timeSlider.addListener(this);
    timeSlider.setRange(0, 300, 1);
    timeSlider.setValue(audioProcessor.timeMS);
    timeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    timeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    addAndMakeVisible(timeSlider);
    
    timeLabel.setText("Time (MS)", dontSendNotification);
    timeLabel.attachToComponent(&timeSlider, false);
    timeLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(timeLabel);
    
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
    
    lpCutOffSlider.addListener(this);
    lpCutOffSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lpCutOffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    lpCutOffSlider.setTextValueSuffix(" Hz");
    lpCutOffSlider.setRange(50, 20000, 1);
    lpCutOffSlider.setValue(audioProcessor.lpFrequency);
    lpCutOffSlider.setSkewFactorFromMidPoint(1000.0);
    addAndMakeVisible(lpCutOffSlider);

    lpCutOffSlider.setLookAndFeel(&knobLookAndFeel);
    
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::lightblue);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::rebeccapurple);
    
    lpFrequencyLabel.setText("LPF Cut-Off",dontSendNotification);
    lpFrequencyLabel.attachToComponent(&lpCutOffSlider, false);
    lpFrequencyLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(lpFrequencyLabel);
    
    hpCutOffSlider.addListener(this);
    hpCutOffSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    hpCutOffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    hpCutOffSlider.setTextValueSuffix(" Hz");
    hpCutOffSlider.setRange(50, 20000, 1);
    hpCutOffSlider.setValue(audioProcessor.hpFrequency);
    hpCutOffSlider.setSkewFactorFromMidPoint(5000);
    addAndMakeVisible(hpCutOffSlider);

    hpCutOffSlider.setLookAndFeel(&knobLookAndFeel);

    hpFrequencyLabel.setText("HPF Cut-Off",dontSendNotification);
    hpFrequencyLabel.attachToComponent(&hpCutOffSlider, false);
    hpFrequencyLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(hpFrequencyLabel);
    
    saturationSlider.addListener(this);
    saturationSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    saturationSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    saturationSlider.setRange(1, 10, 1);
    saturationSlider.setValue(audioProcessor.satAlpha);
    addAndMakeVisible(saturationSlider);

    saturationSlider.setLookAndFeel(&knobLookAndFeel);

    saturationLabel.setText("Saturation", dontSendNotification);
    saturationLabel.attachToComponent(&saturationSlider, false);
    saturationLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(saturationLabel);
    
    smoothingSlider.addListener(this);
    smoothingSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    smoothingSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    smoothingSlider.setRange(0, 1, 0.01);
    smoothingSlider.setValue(audioProcessor.smoothing);
    addAndMakeVisible(smoothingSlider);
    
    smoothingSlider.setLookAndFeel(&knobLookAndFeel);
    
    smoothingLabel.setText("Smoothing", dontSendNotification);
    smoothingLabel.attachToComponent(&smoothingSlider, false);
    smoothingLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(smoothingSlider);
    
    tempoSyncButton.addListener(this);
    tempoSyncButton.setButtonText("Tempo Sync'd");
    tempoSyncButton.setToggleState(audioProcessor.tempoSyncd, dontSendNotification);
    tempoSyncButton.setRadioGroupId(1);
    addAndMakeVisible(tempoSyncButton);

    notTempoSyncButton.addListener(this);
    notTempoSyncButton.setButtonText("Sync Off");
    notTempoSyncButton.setToggleState(!audioProcessor.tempoSyncd, dontSendNotification);
    notTempoSyncButton.setRadioGroupId(1);
    addAndMakeVisible(notTempoSyncButton);
    
    noteSelector.setEnabled(audioProcessor.tempoSyncd);
    timeSlider.setEnabled(!audioProcessor.tempoSyncd);
    
    saturationEnabledButton.addListener(this);
    saturationEnabledButton.setToggleState(audioProcessor.saturationEnabled, dontSendNotification);
    saturationEnabledButton.setButtonText("Disabled");
    saturationSlider.setEnabled(audioProcessor.saturationEnabled);
    saturationEnabledButton.setColour(TextButton::buttonOnColourId, Colours::lightsteelblue);
    saturationEnabledButton.setColour(TextButton::textColourOnId, Colours::black);
    addAndMakeVisible(saturationEnabledButton);
    
    lpEnabledButton.addListener(this);
    lpEnabledButton.setToggleState(audioProcessor.lpEnabled, dontSendNotification);
    lpEnabledButton.setButtonText("Disabled");
    lpCutOffSlider.setEnabled(audioProcessor.lpEnabled);
    lpEnabledButton.setColour(TextButton::buttonOnColourId, Colours::lightsteelblue);
    lpEnabledButton.setColour(TextButton::textColourOnId, Colours::black);
    addAndMakeVisible(lpEnabledButton);
    
    hpEnabledButton.addListener(this);
    hpEnabledButton.setToggleState(audioProcessor.hpEnabled, dontSendNotification);
    hpEnabledButton.setButtonText("Disabled");
    hpCutOffSlider.setEnabled(audioProcessor.hpEnabled);
    hpEnabledButton.setColour(TextButton::buttonOnColourId, Colours::lightsteelblue);
    hpEnabledButton.setColour(TextButton::textColourOnId, Colours::black);
    addAndMakeVisible(hpEnabledButton);
    
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
    
    Rectangle<int> timeArea = panningSection.withTrimmedRight(325).withTrimmedTop(20).withTrimmedBottom(20).reduced(border);
    
//    Rectangle<int> widthArea = panningSection.withTrimmedLeft(180).withTrimmedRight(180).withTrimmedBottom(130).withTrimmedTop(20).reduced(border);
    
    Rectangle<int> noteSelectorArea = panningSection.withTrimmedLeft(150).withTrimmedRight(150).withTrimmedTop(200).withTrimmedBottom(40).reduced(border);
    
    Rectangle<int> smoothingArea = panningSection.removeFromRight(170).withTrimmedTop(75).withTrimmedBottom(75).reduced(border);
    
    Rectangle<int> lpfArea = filterSection.removeFromTop(getHeight()/2).withTrimmedTop(20).reduced(border);
    Rectangle<int> hpfArea = filterSection.removeFromBottom(getHeight()/2).withTrimmedTop(20).reduced(border);
    
    Rectangle<int> saturationArea = saturationSection.withTrimmedTop(75).withTrimmedBottom(75).reduced(border);
    
    timeSlider.setBounds(timeArea);
//    widthSlider.setBounds(widthArea);
    noteSelector.setBounds(noteSelectorArea);
    lpCutOffSlider.setBounds(lpfArea);
    hpCutOffSlider.setBounds(hpfArea);
    saturationSlider.setBounds(saturationArea);
    smoothingSlider.setBounds(smoothingArea);
    
    tempoSyncButton.setBounds(200, 260, 100, 40);
    notTempoSyncButton.setBounds(47.5, 260, 100, 40);
    
    saturationEnabledButton.setBounds(650, 0, 75, 25);
    lpEnabledButton.setBounds(500,0,75,25);
    hpEnabledButton.setBounds(500, 200, 75, 25);
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void RandomPannerAudioProcessorEditor::sliderValueChanged(Slider* slider) {
//    if (slider == &widthSlider) {
//        audioProcessor.width = widthSlider.getValue();
//    }
    if (slider == &lpCutOffSlider) {
        audioProcessor.lpFrequency = lpCutOffSlider.getValue();
    }
    if (slider == &hpCutOffSlider) {
        audioProcessor.hpFrequency = hpCutOffSlider.getValue();
    }
    if (slider == &saturationSlider) {
        audioProcessor.satAlpha = saturationSlider.getValue();
    }
    if (slider == &smoothingSlider) {
        audioProcessor.smoothing = smoothingSlider.getValue();
    }
    
    if (slider == &timeSlider) {
        audioProcessor.timeMS = timeSlider.getValue();
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

void RandomPannerAudioProcessorEditor::setButtonDisabled(Button* button) {
    if (button == &saturationEnabledButton) {
        saturationButtonState = ButtonState::Disabled;
        audioProcessor.setButtonState(audioProcessor.saturationEnabled);
        saturationEnabledButton.setToggleState(audioProcessor.saturationEnabled, dontSendNotification);
        saturationEnabledButton.setButtonText("Disabled");
        saturationSlider.setEnabled(audioProcessor.saturationEnabled);
    }
    
    if (button == &lpEnabledButton) {
        lpButtonState = ButtonState::Disabled;
        audioProcessor.setButtonState(audioProcessor.lpEnabled);
        lpEnabledButton.setToggleState(audioProcessor.lpEnabled, dontSendNotification);
        lpEnabledButton.setButtonText("Disabled");
        lpCutOffSlider.setEnabled(audioProcessor.lpEnabled);
    }
    
    if (button == &hpEnabledButton) {
        hpButtonState = ButtonState::Disabled;
        audioProcessor.setButtonState(audioProcessor.hpEnabled);
        hpEnabledButton.setToggleState(audioProcessor.hpEnabled, dontSendNotification);
        hpEnabledButton.setButtonText("Disabled");
        hpCutOffSlider.setEnabled(audioProcessor.hpEnabled);
    }
    // want to set the linked element to be enabled
}

void RandomPannerAudioProcessorEditor::setButtonEnabled(Button* button) {
    if (button == &saturationEnabledButton) {
        saturationButtonState = ButtonState::Enabled;
        audioProcessor.setButtonState(audioProcessor.saturationEnabled);
        saturationEnabledButton.setToggleState(audioProcessor.saturationEnabled, dontSendNotification);
        saturationEnabledButton.setButtonText("Enabled");
        saturationSlider.setEnabled(audioProcessor.saturationEnabled);
    }
    
    if (button == &lpEnabledButton) {
        lpButtonState = ButtonState::Enabled;
        audioProcessor.setButtonState(audioProcessor.lpEnabled);
        lpEnabledButton.setToggleState(audioProcessor.lpEnabled, dontSendNotification);
        lpEnabledButton.setButtonText("Enabled");
        lpCutOffSlider.setEnabled(audioProcessor.lpEnabled);
    }
    
    if (button == &hpEnabledButton) {
        hpButtonState = ButtonState::Enabled;
        audioProcessor.setButtonState(audioProcessor.hpEnabled);
        hpEnabledButton.setToggleState(audioProcessor.hpEnabled, dontSendNotification);
        hpEnabledButton.setButtonText("Enabled");
        hpCutOffSlider.setEnabled(audioProcessor.hpEnabled);
    }
    // want to set the linked element to be enabled
}

void RandomPannerAudioProcessorEditor::buttonClicked(Button* button) {
    if (button == &tempoSyncButton) {
        audioProcessor.tempoSyncd = true;
        timeSlider.setEnabled(false);
        noteSelector.setEnabled(true);
    }

    if (button == &notTempoSyncButton) {
        audioProcessor.tempoSyncd = false;
        timeSlider.setEnabled(true);
        noteSelector.setEnabled(false);
    }
    
    if (button == &saturationEnabledButton) {
        if (saturationButtonState == ButtonState::Disabled) {
            saturationEnabledButton.onClick = [this]() {
                setButtonEnabled(&saturationEnabledButton);
                
            };
        }
        else if (saturationButtonState == ButtonState::Enabled) {
            saturationEnabledButton.onClick = [this]() {
                setButtonDisabled(&saturationEnabledButton);
                
            };
        }
    }
    
    if (button == &lpEnabledButton) {
        if (lpButtonState == ButtonState::Disabled) {
            lpEnabledButton.onClick = [this]() {
                setButtonEnabled(&lpEnabledButton);
                
            };
        }
        else if (lpButtonState == ButtonState::Enabled) {
            lpEnabledButton.onClick = [this]() {
                setButtonDisabled(&lpEnabledButton);
                
            };
        }
    }
    
    if (button == &hpEnabledButton) {
        if (hpButtonState == ButtonState::Disabled) {
            hpEnabledButton.onClick = [this]() {
                setButtonEnabled(&hpEnabledButton);
                
            };
        }
        else if (hpButtonState == ButtonState::Enabled) {
            hpEnabledButton.onClick = [this]() {
                setButtonDisabled(&hpEnabledButton);
                
            };
        }
    }
}

