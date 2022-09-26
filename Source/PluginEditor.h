/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FmSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    FmSynthAudioProcessorEditor (FmSynthAudioProcessor&);
    ~FmSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FmSynthAudioProcessor& audioProcessor;
    void sliderValueChanged(juce::Slider* slider) override;
    
    //volume slider
    juce::Slider volumeSlider;
    juce::Label volumeLabel;
    
    //sustain slider
    juce::Slider sustainSlider;
    juce::Label sustainLabel;
    
    //attack slider
    juce::Slider attackSlider;
    juce::Label attackLabel;
    
    //modulator frequency dial
    juce::Slider fm1Dial;
    juce::Label fm1Label;
    
    juce::Slider fm2Dial;
    juce::Label fm2Label;
    
    //modulatr amplitude dials
    juce::Slider beta1Dial;
    juce::Label beta1Label;
    
    juce::Slider beta2Dial;
    juce::Label beta2Label;

    //carrier type buttons
    juce::TextButton sineCar{"Sine"};
    juce::TextButton squareCar{"Square"};
    juce::TextButton sawCar{"Sawtooth"};
    
    //carrier waveform label
    juce::Label carLabel{{}, " carrier waveform"};
    
    //modulator type buttons
    juce::TextButton sineMod1{"Sine"};
    juce::TextButton sineMod2{"Sine"};
    
    juce::TextButton squareMod1 {"Square"};
    juce::TextButton sawMod1 {"Sawtooth"};
    
    juce::TextButton squareMod2 {"Sqaure"};
    juce::TextButton sawMod2 {"Sawtooth"};
    
    //reset modulator 1 and 2 beta and frequencies to zero
    juce::TextButton resetMod1{"reset"};
    juce::TextButton resetMod2{"reset"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmSynthAudioProcessorEditor)
};
