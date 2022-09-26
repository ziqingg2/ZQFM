/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "fm_synth.h"


enum RadioButtonID {
    carButtons = 101
};

enum RadioButtonIDs {
    mod1Buttons = 10001
};

enum RadioButtonIds {
    mod2Buttons = 1001
};


//==============================================================================
FmSynthAudioProcessorEditor::FmSynthAudioProcessorEditor (FmSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1300, 700);
    
    //volume slider
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0,1.0,0.1);
    volumeSlider.addListener(this);
    volumeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,80,volumeSlider.getTextBoxHeight());
    volumeSlider.setValue(0.0);
    volumeSlider.setColour(volumeSlider.thumbColourId, juce::Colours::white);
    volumeSlider.setColour(volumeSlider.rotarySliderFillColourId,juce::Colours::whitesmoke);
    volumeSlider.setColour(volumeSlider.backgroundColourId,juce::Colours::black);
    
    //volume slider label
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Ouput Level",juce::dontSendNotification);
    volumeLabel.attachToComponent(&volumeSlider,false);
    
    //attack slider
    addAndMakeVisible(attackSlider);
    attackSlider.setRange(0.0,1.0,0.1);
    attackSlider.addListener(this);
    attackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,80,attackSlider.getTextBoxHeight());
    attackSlider.setValue(0.0);
    attackSlider.setColour(volumeSlider.thumbColourId, juce::Colours::white);
    attackSlider.setColour(volumeSlider.thumbColourId, juce::Colours::white);
    attackSlider.setColour(volumeSlider.rotarySliderFillColourId,juce::Colours::whitesmoke);
    attackSlider.setColour(volumeSlider.backgroundColourId,juce::Colours::black);
    
    //sustain slider label
    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack Level",juce::dontSendNotification);
    attackLabel.attachToComponent(&attackSlider,false);
    
    
    //sustain slider
    addAndMakeVisible(sustainSlider);
    sustainSlider.setRange(0.0,1.0,0.1);
    sustainSlider.addListener(this);
    sustainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,80,sustainSlider.getTextBoxHeight());
    sustainSlider.setValue(0.0);
    sustainSlider.setColour(volumeSlider.thumbColourId, juce::Colours::white);
    sustainSlider.setColour(volumeSlider.rotarySliderFillColourId,juce::Colours::whitesmoke);
    sustainSlider.setColour(volumeSlider.backgroundColourId,juce::Colours::black);
    
    
    //sustain slider label
    addAndMakeVisible(sustainLabel);
    sustainLabel.setText("Sustain Level",juce::dontSendNotification);
    sustainLabel.attachToComponent(&sustainSlider,false);
    
    //fm1 dial
    addAndMakeVisible(fm1Dial);
    fm1Dial.setRange(0.0,2000.0,1.0);
    fm1Dial.addListener(this);
    fm1Dial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    fm1Dial.setTextBoxStyle(juce::Slider::TextBoxBelow,false,80,volumeSlider.getTextBoxHeight());
    fm1Dial.setTextValueSuffix("Hz");
    fm1Dial.setValue(0.0);
    fm1Dial.setColour(volumeSlider.thumbColourId, juce::Colours::white);
    fm1Dial.setColour(volumeSlider.rotarySliderFillColourId,juce::Colours::whitesmoke);
    fm1Dial.setColour(volumeSlider.backgroundColourId,juce::Colours::black);
    
    //fm1 dial label
    addAndMakeVisible(fm1Label);
    fm1Label.setText("modulator 1 frequency",juce::dontSendNotification);
    fm1Label.attachToComponent(&fm1Dial,false);
    
    //fm2 dial
    addAndMakeVisible(fm2Dial);
    fm2Dial.setRange(0.0,2000.0,1.0);
    fm2Dial.addListener(this);
    fm2Dial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    fm2Dial.setTextBoxStyle(juce::Slider::TextBoxBelow,false,80,volumeSlider.getTextBoxHeight());
    fm2Dial.setTextValueSuffix("Hz");
    fm2Dial.setValue(0.0);
    fm2Dial.setColour(volumeSlider.thumbColourId, juce::Colours::white);
    fm2Dial.setColour(volumeSlider.rotarySliderFillColourId,juce::Colours::whitesmoke);
    fm2Dial.setColour(volumeSlider.backgroundColourId,juce::Colours::black);
    
    //fm2 dial label
    addAndMakeVisible(fm2Label);
    fm2Label.setText("modulator 2 frequency",juce::dontSendNotification);
    fm2Label.attachToComponent(&fm2Dial,false);
    
    //beta1 dial
    addAndMakeVisible(beta1Dial);
    beta1Dial.setRange(0,100,1);
    beta1Dial.addListener(this);
    beta1Dial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    beta1Dial.setTextBoxStyle(juce::Slider::TextBoxBelow,false,80,volumeSlider.getTextBoxHeight());
    beta1Dial.setValue(0);
    beta1Dial.setColour(volumeSlider.thumbColourId, juce::Colours::white);
    beta1Dial.setColour(volumeSlider.rotarySliderFillColourId,juce::Colours::whitesmoke);
    beta1Dial.setColour(volumeSlider.backgroundColourId,juce::Colours::black);
    
    //beta1 dial label
    addAndMakeVisible(beta1Label);
    beta1Label.setText("modulator 1 amplitude",juce::dontSendNotification);
    beta1Label.attachToComponent(&beta1Dial,false);
    
    //beta2 dial
    addAndMakeVisible(beta2Dial);
    beta2Dial.setRange(0,100,1);
    beta2Dial.addListener(this);
    beta2Dial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    beta2Dial.setTextBoxStyle(juce::Slider::TextBoxBelow,false,80,volumeSlider.getTextBoxHeight());
    beta2Dial.setValue(0);
    beta2Dial.setColour(volumeSlider.thumbColourId, juce::Colours::white);
    beta2Dial.setColour(volumeSlider.rotarySliderFillColourId,juce::Colours::whitesmoke);
    beta2Dial.setColour(volumeSlider.backgroundColourId,juce::Colours::black);
    
    //beta2 dial label
    addAndMakeVisible(beta2Label);
    beta2Label.setText("modulator 2 amplitude",juce::dontSendNotification);
    beta2Label.attachToComponent(&beta2Dial,false);
    
    //carrier waveform label
    addAndMakeVisible(carLabel);
    carLabel.setBounds(150, 15, 300, 30);
    
    
    //carrier buttons
    //sine
    addAndMakeVisible(sineCar);
    sineCar.setRadioGroupId(carButtons);
    sineCar.onClick = [this] {audioProcessor.fm_synth.set_car_sin(); };
    sineCar.setClickingTogglesState(true);
    
    //square
    addAndMakeVisible(squareCar);
    squareCar.setRadioGroupId(carButtons);
    squareCar.onClick = [this] {audioProcessor.fm_synth.set_car_sq(); };
    squareCar.setClickingTogglesState(true);
    
    //saw
    addAndMakeVisible(sawCar);
    sawCar.setRadioGroupId(carButtons);
    sawCar.onClick = [this] {audioProcessor.fm_synth.set_car_saw(); };
    sawCar.setClickingTogglesState(true);
    
    //modulator 1 buttons
    //sine
    addAndMakeVisible(sineMod1);
    sineMod1.setRadioGroupId(mod1Buttons);
    sineMod1.onClick = [this] {audioProcessor.fm_synth.set_mod1_sin(); };
    sineMod1.setClickingTogglesState(true);
    
    //square
    addAndMakeVisible(squareMod1);
    squareMod1.setRadioGroupId(mod1Buttons);
    squareMod1.onClick = [this] {audioProcessor.fm_synth.set_mod1_sq(); };
    squareMod1.setClickingTogglesState(true);
    
    //saw
    addAndMakeVisible(sawMod1);
    sawMod1.setRadioGroupId(mod1Buttons);
    sawMod1.onClick = [this] {audioProcessor.fm_synth.set_mod1_saw(); };
    sawMod1.setClickingTogglesState(true);
    
    //modulator 2 buttons
    addAndMakeVisible(sineMod2);
    sineMod2.setRadioGroupId(mod2Buttons);
    sineMod2.onClick = [this] {audioProcessor.fm_synth.set_mod2_sin(); };
    sineMod2.setClickingTogglesState(true);
    
    //square
    addAndMakeVisible(squareMod2);
    squareMod2.setRadioGroupId(mod2Buttons);
    squareMod2.onClick = [this] {audioProcessor.fm_synth.set_mod2_sq(); };
    squareMod2.setClickingTogglesState(true);
    
    //saw
    addAndMakeVisible(sawMod2);
    sawMod2.setRadioGroupId(mod2Buttons);
    sawMod2.onClick = [this] {audioProcessor.fm_synth.set_mod2_saw(); };
    sawMod2.setClickingTogglesState(true);
    
    //reset buttons
    addAndMakeVisible(resetMod1);
    resetMod1.onClick = [this] {
        audioProcessor.fm_synth.set_fm1(0);
        audioProcessor.fm_synth.set_beta1(0);
        fm1Dial.setValue(0);
        beta1Dial.setValue(0);
    };
    
    addAndMakeVisible(resetMod2);
    resetMod2.onClick = [this] {
        audioProcessor.fm_synth.set_fm2(0);
        audioProcessor.fm_synth.set_beta2(0);
        fm2Dial.setValue(0);
        beta2Dial.setValue(0);
    };
}

FmSynthAudioProcessorEditor::~FmSynthAudioProcessorEditor()
{
}

//==============================================================================
void FmSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::dimgrey);
    
    juce::Line<float> line1 (juce::Point<float> (433,0),
                            juce::Point<float> (433,700));
    
    juce::Line<float> line2 (juce::Point<float> (866,0),
                            juce::Point<float> (866,700));
    
    juce::Line<float> line3 (juce::Point<float> (0,0),
                            juce::Point<float> (0,700));
    
    juce::Line<float> line4 (juce::Point<float> (1300,0),
                            juce::Point<float> (1300,700));
    
    juce::Line<float> line5 (juce::Point<float> (0,0),
                            juce::Point<float> (0,1300));
    
    juce::Line<float> line6 (juce::Point<float> (0,700),
                            juce::Point<float> (1300,700));
    
    juce::Line<float> line7 (juce::Point<float> (0,350),
                            juce::Point<float> (433,350));
    
    g.setColour (juce::Colours::white);
    g.drawLine(line1,2.0f);
    g.drawLine(line2,2.0f);
    g.drawLine(line3,2.0f);
    g.drawLine(line4,2.0f);
    g.drawLine(line5,2.0f);
    g.drawLine(line6,2.0f);
    g.drawLine(line7,2.0f);

    
    g.setFont (15.0f);
    g.drawFittedText ("ZQ-FM001", getLocalBounds(), juce::Justification::topLeft, 1);
}

void FmSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    volumeSlider.setBounds(160,380,90,120);
    sustainSlider.setBounds(260,560,90,120);
    attackSlider.setBounds(80,560,90,120);
    
    
    fm1Dial.setBounds(580,40,160,120);
    beta1Dial.setBounds(580,210,160,120);
    
    fm2Dial.setBounds(1000,40,160,120);
    beta2Dial.setBounds(1000,210,160, 120);
    
    
    sineCar.setBounds(135,60,150,45);
    squareCar.setBounds(135,150,150,45);
    sawCar.setBounds(135,240,150,45);
    
    sineMod1.setBounds(600,390,150,45);
    squareMod1.setBounds(600,480,150,45);
    sawMod1.setBounds(600,570,150,45);

    sineMod2.setBounds(1020,390,150,45);
    squareMod2.setBounds(1020,480,150,45);
    sawMod2.setBounds(1020,570,150,45);
    
    resetMod1.setBounds(780,650,80,45);
    resetMod2.setBounds(1214,650,80,45);
    
}

void FmSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioProcessor.fm_synth.set_ampl(volumeSlider.getValue());
    }
    
    if (slider == &sustainSlider)
    {
        audioProcessor.fm_synth.set_decay(sustainSlider.getValue());
    }
    
    if (slider == &attackSlider)
    {
        audioProcessor.fm_synth.set_attack(attackSlider.getValue());
    }
    
    if (slider == &fm1Dial)
    {
        audioProcessor.fm_synth.set_fm1(fm1Dial.getValue());
    }
    
    if (slider == &fm2Dial)
    {
        audioProcessor.fm_synth.set_fm2(fm2Dial.getValue());
    }
    
    if (slider == &beta1Dial)
    {
        audioProcessor.fm_synth.set_beta1(beta1Dial.getValue());
    }
    
    if (slider == &beta2Dial)
    {
        audioProcessor.fm_synth.set_beta2(beta2Dial.getValue());
    }
    
    
}
