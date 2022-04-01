/*
  ==============================================================================

    CustomButton.h
    Created: 22 Jul 2021 11:42:17am
    Author:  csmit

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomButton  : public juce::Component,
    public juce::Button::Listener
{
public:
    CustomButton(juce::AudioProcessorValueTreeState& parameters, juce::String buttonStateId);
    ~CustomButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;

    void changeState(bool buttonState);

private:
    juce::TextButton decayLimButton;

    bool buttonState = false;
    
    juce::Label buttonTitle{ {}, "DECAY\nLIMITER" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomButton)
};
