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
    
//==============================================================================
// SET BACKGROUND
//==============================================================================
    auto backgroundImage = ImageCache::getFromMemory(BinaryData::MainBackground_png, BinaryData::MainBackground_pngSize);
    
    if (! backgroundImage.isNull()) {
        backgroundImageComponent.setImage(backgroundImage,RectanglePlacement::stretchToFit);
    }
    else {
        jassert (! backgroundImage.isNull());
    }
    
    addAndMakeVisible(backgroundImageComponent);
    
//==============================================================================
// SET DEFAULT FONT
//==============================================================================
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Avenir Next");
    setFontParameters(&font);
    
//==============================================================================
// SLIDERS
//==============================================================================
    timeSlider.addListener(this);
    timeSlider.setRange(0, 1000, 1);
    timeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    timeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    timeSlider.setColour(Slider::trackColourId, Colour(180, 254, 39));
    timeSlider.setColour(Slider::thumbColourId, Colours::transparentBlack);
    addAndMakeVisible(timeSlider);
    
    timeLabel.setText("time (ms)", dontSendNotification);
    timeLabel.attachToComponent(&timeSlider, false);
    timeLabel.setFont(font);
    timeLabel.setColour(Label::textColourId, Colours::silver);
    timeLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(timeLabel);
    
    widthSlider.addListener(this);
    widthSlider.setRange(2,100,1);
    widthSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    widthSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(widthSlider);

    widthSlider.setLookAndFeel(&largeKnob);

    widthLabel.setText("width",dontSendNotification);
    widthLabel.attachToComponent(&widthSlider, false);
    widthLabel.setFont(font);
    widthLabel.setColour(Label::textColourId, Colours::silver);
    widthLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(widthLabel);
    
    lpCutOffSlider.addListener(this);
    lpCutOffSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lpCutOffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    lpCutOffSlider.setTextValueSuffix(" Hz");
    lpCutOffSlider.setRange(50, 18000, 1);
    lpCutOffSlider.setSkewFactorFromMidPoint(5000);
    addAndMakeVisible(lpCutOffSlider);
    
    lpCutOffSlider.setLookAndFeel(&smallKnob);
    
    lpFrequencyLabel.setText("low cut",dontSendNotification);
    lpFrequencyLabel.attachToComponent(&lpCutOffSlider, false);
    lpFrequencyLabel.setFont(font);
    lpFrequencyLabel.setColour(Label::textColourId, Colours::silver);
    lpFrequencyLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(lpFrequencyLabel);
    
    hpCutOffSlider.addListener(this);
    hpCutOffSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    hpCutOffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    hpCutOffSlider.setTextValueSuffix(" Hz");
    hpCutOffSlider.setRange(50, 18000, 1);
    hpCutOffSlider.setSkewFactorFromMidPoint(5000);
    addAndMakeVisible(hpCutOffSlider);

    hpCutOffSlider.setLookAndFeel(&smallKnob);

    hpFrequencyLabel.setText("high cut",dontSendNotification);
    hpFrequencyLabel.attachToComponent(&hpCutOffSlider, false);
    hpFrequencyLabel.setFont(font);
    hpFrequencyLabel.setColour(Label::textColourId, Colours::silver);
    hpFrequencyLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(hpFrequencyLabel);
    
    saturationSlider.addListener(this);
    saturationSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    saturationSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    saturationSlider.setRange(1, 10, 1);
    addAndMakeVisible(saturationSlider);

    saturationSlider.setLookAndFeel(&smallKnob);

    saturationLabel.setText("saturation", dontSendNotification);
    saturationLabel.attachToComponent(&saturationSlider, false);
    saturationLabel.setFont(font);
    saturationLabel.setColour(Label::textColourId, Colours::silver);
    saturationLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(saturationLabel);
    
    smoothingSlider.addListener(this);
    smoothingSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    smoothingSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    smoothingSlider.setRange(0, 1, 0.01);
    addAndMakeVisible(smoothingSlider);
    
    smoothingSlider.setLookAndFeel(&smallKnob);
    
    smoothingLabel.setText("smoothing", dontSendNotification);
    smoothingLabel.attachToComponent(&smoothingSlider, false);
    smoothingLabel.setFont(font);
    smoothingLabel.setColour(Label::textColourId, Colours::silver);
    smoothingLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(smoothingSlider);

//==============================================================================
// COMBO BOX
//==============================================================================
    
    noteSelector.addListener(this);
    noteSelector.addItem("whole", 1);
    noteSelector.addItem("half", 2);
    noteSelector.addItem("quarter", 3);
    noteSelector.addItem("8th", 4);
    noteSelector.addItem("16th", 5);
    noteSelector.addItem("dotted half", 6);
    noteSelector.addItem("dotted quarter", 7);
    noteSelector.addItem("dotted 8th", 8);
    noteSelector.addItem("dotted 16th", 9);
    noteSelector.setSelectedId(3);
    addAndMakeVisible(noteSelector);
    
//==============================================================================
// BUTTONS
//==============================================================================
    
    tempoSyncEnabledButton.addListener(this);
    tempoSyncEnabledButton.setToggleState(audioProcessor.tempoSyncd, dontSendNotification);
    noteSelector.setEnabled(audioProcessor.tempoSyncd);
    timeSlider.setEnabled(!audioProcessor.tempoSyncd);
    addAndMakeVisible(tempoSyncEnabledButton);
    
    tempoSyncEnabledButton.setLookAndFeel(&syncButton);
    
    saturationEnabledButton.addListener(this);
    saturationEnabledButton.setToggleState(audioProcessor.saturationEnabled, dontSendNotification);
    saturationSlider.setEnabled(audioProcessor.saturationEnabled);
    addAndMakeVisible(saturationEnabledButton);
    
    saturationEnabledButton.setLookAndFeel(&inButton);
    
    lpEnabledButton.addListener(this);
    lpEnabledButton.setToggleState(audioProcessor.lpEnabled, dontSendNotification);
    lpCutOffSlider.setEnabled(audioProcessor.lpEnabled);
    addAndMakeVisible(lpEnabledButton);
    
    lpEnabledButton.setLookAndFeel(&inButton);
    
    hpEnabledButton.addListener(this);
    hpEnabledButton.setToggleState(audioProcessor.hpEnabled, dontSendNotification);
    hpCutOffSlider.setEnabled(audioProcessor.hpEnabled);
    addAndMakeVisible(hpEnabledButton);
    
    hpEnabledButton.setLookAndFeel(&inButton);
    
//==============================================================================
// ATTACHMENTS
//==============================================================================
    
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "timeValue", timeSlider));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "widthValue", widthSlider));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "smoothValue", smoothingSlider));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "lpCutOffValue", lpCutOffSlider));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "hpCutOffValue", hpCutOffSlider));
    sliderAttachments.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, "saturationValue", saturationSlider));
    
    comboBoxAttachment.emplace_back(new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.treeState, "noteSelection", noteSelector));
    
    buttonAttachments.emplace_back(new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.treeState, "syncButton", tempoSyncEnabledButton));
    buttonAttachments.emplace_back(new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.treeState, "lpButton", lpEnabledButton));
    buttonAttachments.emplace_back(new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.treeState, "hpButton", hpEnabledButton));
    buttonAttachments.emplace_back(new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.treeState, "saturationButton", saturationEnabledButton));
    
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
    
    Rectangle<int> timeArea = panningSection.withTrimmedRight(325).withTrimmedLeft(0).withTrimmedTop(20).withTrimmedBottom(20).reduced(border);
    
    Rectangle<int> widthArea = panningSection.withTrimmedLeft(180).withTrimmedRight(180).withTrimmedBottom(110).withTrimmedTop(40).reduced(border);
    
    Rectangle<int> noteSelectorArea = panningSection.withTrimmedLeft(150).withTrimmedRight(150).withTrimmedTop(170).withTrimmedBottom(70).reduced(border);
    
    Rectangle<int> smoothingArea = panningSection.removeFromRight(170).withTrimmedTop(90).withTrimmedBottom(90).reduced(border);
    
    Rectangle<int> hpfArea = filterSection.withTrimmedTop(30).withTrimmedBottom(150).reduced(border);
    Rectangle<int> lpfArea = filterSection.withTrimmedTop(165).withTrimmedBottom(15).reduced(border);
    
    Rectangle<int> saturationArea = saturationSection.withTrimmedRight(10).withTrimmedTop(90).withTrimmedBottom(90).reduced(border);
    
    timeSlider.setBounds(timeArea);
    widthSlider.setBounds(widthArea);
    noteSelector.setBounds(noteSelectorArea);
    lpCutOffSlider.setBounds(lpfArea);
    hpCutOffSlider.setBounds(hpfArea);
    saturationSlider.setBounds(saturationArea);
    smoothingSlider.setBounds(smoothingArea);
    
    tempoSyncEnabledButton.setBounds(210, 230, 74, 20);
    
    saturationEnabledButton.setBounds(660, 125, 25, 25);
    lpEnabledButton.setBounds(513, 200, 25, 25);
    hpEnabledButton.setBounds(513,65,25,25);
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void RandomPannerAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    if (slider == &widthSlider) {
        audioProcessor.width = widthSlider.getValue();
    }
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
    if (button == &tempoSyncEnabledButton) {
        tempoSyncButtonState = ButtonState::Disabled;
        audioProcessor.setButtonState(audioProcessor.tempoSyncd);
        tempoSyncEnabledButton.setToggleState(audioProcessor.tempoSyncd, dontSendNotification);
        audioProcessor.tempoSyncd = false;
        timeSlider.setEnabled(true);
        noteSelector.setEnabled(false);
    }
    
    if (button == &saturationEnabledButton) {
        saturationButtonState = ButtonState::Disabled;
        audioProcessor.setButtonState(audioProcessor.saturationEnabled);
        saturationEnabledButton.setToggleState(audioProcessor.saturationEnabled, dontSendNotification);
        saturationSlider.setEnabled(audioProcessor.saturationEnabled);
    }
    
    if (button == &lpEnabledButton) {
        lpButtonState = ButtonState::Disabled;
        audioProcessor.setButtonState(audioProcessor.lpEnabled);
        lpEnabledButton.setToggleState(audioProcessor.lpEnabled, dontSendNotification);
        lpCutOffSlider.setEnabled(audioProcessor.lpEnabled);
    }
    
    if (button == &hpEnabledButton) {
        hpButtonState = ButtonState::Disabled;
        audioProcessor.setButtonState(audioProcessor.hpEnabled);
        hpEnabledButton.setToggleState(audioProcessor.hpEnabled, dontSendNotification);
        hpCutOffSlider.setEnabled(audioProcessor.hpEnabled);
    }
}

void RandomPannerAudioProcessorEditor::setButtonEnabled(Button* button) {
    if (button == &tempoSyncEnabledButton) {
        tempoSyncButtonState = ButtonState::Enabled;
        audioProcessor.setButtonState(audioProcessor.tempoSyncd);
        tempoSyncEnabledButton.setToggleState(audioProcessor.tempoSyncd, dontSendNotification);
        audioProcessor.tempoSyncd = true;
        timeSlider.setEnabled(false);
        noteSelector.setEnabled(true);
    }
    
    if (button == &saturationEnabledButton) {
        saturationButtonState = ButtonState::Enabled;
        audioProcessor.setButtonState(audioProcessor.saturationEnabled);
        saturationEnabledButton.setToggleState(audioProcessor.saturationEnabled, dontSendNotification);
        saturationSlider.setEnabled(audioProcessor.saturationEnabled);
    }
    
    if (button == &lpEnabledButton) {
        lpButtonState = ButtonState::Enabled;
        audioProcessor.setButtonState(audioProcessor.lpEnabled);
        lpEnabledButton.setToggleState(audioProcessor.lpEnabled, dontSendNotification);
        lpCutOffSlider.setEnabled(audioProcessor.lpEnabled);
    }
    
    if (button == &hpEnabledButton) {
        hpButtonState = ButtonState::Enabled;
        audioProcessor.setButtonState(audioProcessor.hpEnabled);
        hpEnabledButton.setToggleState(audioProcessor.hpEnabled, dontSendNotification);
        hpCutOffSlider.setEnabled(audioProcessor.hpEnabled);
    }
}

void RandomPannerAudioProcessorEditor::buttonClicked(Button* button) {
    if (button == &tempoSyncEnabledButton) {
        if (tempoSyncButtonState == ButtonState::Enabled) {
            tempoSyncEnabledButton.onClick = [this]() {
                setButtonDisabled(&tempoSyncEnabledButton);
            };
        }
        else if (tempoSyncButtonState == ButtonState::Disabled) {
            tempoSyncEnabledButton.onClick = [this]() {
                setButtonEnabled(&tempoSyncEnabledButton);
            };
        }
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

void RandomPannerAudioProcessorEditor::setFontParameters(Font* font) {
    font->setTypefaceName("Avenir Next");
    font->setTypefaceStyle("Demi Bold");
    font->setHeight(20);
}

