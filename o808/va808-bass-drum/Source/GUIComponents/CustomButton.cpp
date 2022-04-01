/*
  ==============================================================================

    CustomButton.cpp
    Created: 22 Jul 2021 11:42:17am
    Author:  csmit

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomButton.h"

//==============================================================================
CustomButton::CustomButton(juce::AudioProcessorValueTreeState& parameters, juce::String buttonStateId)
{
    setSize(100, 105);

    
    decayLimButton.setColour(juce::TextButton::buttonColourId, juce::Colour(43, 42, 48));
    decayLimButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(194, 105, 35));
    
    decayLimButton.setButtonText("ON");
    decayLimButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    decayLimButton.setClickingTogglesState(true);
    
    decayLimButton.addListener(this);

    

    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, "BUTTON STATE", decayLimButton);
    
    addAndMakeVisible(decayLimButton);

    // adding to the screen, centring and changing colour
    addAndMakeVisible(buttonTitle);
    buttonTitle.setJustificationType(juce::Justification::centred);
    buttonTitle.setColour(juce::Label::textColourId, juce::Colour(242, 226, 139));
}

CustomButton::~CustomButton()
{
}

void CustomButton::paint (juce::Graphics& g)
{
}

void CustomButton::resized()
{
    
    // set location of label
    buttonTitle.setBounds(10, 10, 80, 30);

    // set location of button
    decayLimButton.setBounds(20, 52, 60, 41);


}

void CustomButton::buttonClicked(juce::Button* button)
{
    if (button == &decayLimButton)
    {
        decayLimButton.onClick = [this]() {changeState(decayLimButton.getToggleState()); };
    }
}


void CustomButton::changeState(bool buttonState)
{

    if (buttonState == false)
    {
        decayLimButton.setButtonText("OFF");
    }
    else if (buttonState == true)
    {
        decayLimButton.setButtonText("ON");
    }


}
