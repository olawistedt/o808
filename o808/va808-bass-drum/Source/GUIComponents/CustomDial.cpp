/*
  ==============================================================================

    "CustomDial.cpp"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 15th July 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomDial.h"

//==============================================================================
CustomDial::CustomDial(juce::String dialName, juce::Colour dialColour, juce::AudioProcessorValueTreeState& parameters, juce::String paramID)
{
    // setting size of the total dial component
    setSize(100, 105);

    // adding the dial itself and setting range from 0 to 1
    addAndMakeVisible(dial);
    dial.setRange(0.0f, 1.0f);

    //setting the style as a rotary dial (with horizontal or vertical drag) and removing text box
    dial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    dial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    // calling function in the look and feel class to set the colour that was used in initialization
    customLook.setDialColour(dialColour);

    // applying the custom look to the dial
    dial.setLookAndFeel(&customLook);

    // adding attachment to the dial
    attachment = std::make_unique<SliderAttachment>(parameters, paramID, dial);
   
    // using the string that given to the class instance for the label text 
    label.setText(dialName, juce::NotificationType::dontSendNotification);

    // adding to the screen, centring and changing colour
    addAndMakeVisible(label);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colour(242, 226, 139));

}

CustomDial::~CustomDial()
{
}

void CustomDial::paint (juce::Graphics& g)
{

}

void CustomDial::resized()
{
    // set location of dial
    dial.setBounds(10, 25, 80, 80);

    // set location of label
    label.setBounds(10, 10, 80, 15);

}


