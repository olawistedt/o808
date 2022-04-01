/*
  ==============================================================================

    "Background.cpp"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 15th July 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include <JuceHeader.h>
#include "Background.h"

//==============================================================================
Background::Background()
{
    // adding some titles with name and authorship to the window
    addAndMakeVisible(labelOne);
    labelOne.setFont(juce::Font(35.0f));
    labelOne.setColour(juce::Label::textColourId, juce::Colour(156, 53, 34));
    labelOne.setJustificationType(juce::Justification::left);
    addAndMakeVisible(labelTwo);
    labelTwo.setFont(juce::Font(20.0f));
    labelTwo.setColour(juce::Label::textColourId, juce::Colour(194, 105, 35));
    labelTwo.setJustificationType(juce::Justification::right);
    addAndMakeVisible(labelAuthor);
    labelAuthor.setFont(juce::Font(10.0f));
    labelAuthor.setColour(juce::Label::textColourId, juce::Colour(226, 195, 79));
    labelAuthor.setJustificationType(juce::Justification::right);

    // adding text for the "Bass Drum" label in a way that mimics the styling on the original unit
    addAndMakeVisible(B);
    B.setFont(bigFont);
    B.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));
    addAndMakeVisible(ass);
    ass.setFont(smallFont);
    ass.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));
    addAndMakeVisible(D);
    D.setFont(bigFont);
    D.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));
    addAndMakeVisible(rum);
    rum.setFont(smallFont);
    rum.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));

    addAndMakeVisible(C);
    C.setFont(bigFont);
    C.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));
    addAndMakeVisible(ircuit);
    ircuit.setFont(smallFont);
    ircuit.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));
    addAndMakeVisible(M);
    M.setFont(bigFont);
    M.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));
    addAndMakeVisible(ods);
    ods.setFont(smallFont);
    ods.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));

    addAndMakeVisible(O);
    O.setFont(bigFont);
    O.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));
    addAndMakeVisible(verdrive);
    verdrive.setFont(smallFont);
    verdrive.setColour(juce::Label::textColourId, juce::Colour(32, 22, 2));

}

Background::~Background()
{
}

void Background::paint (juce::Graphics& g)
{

    // Filling background of window with a dark grey colour and a gradient in the top right corner
    juce::ColourGradient blackGrad{ juce::Colour(23, 22, 28), 200.0f, 70.0f, juce::Colour(43, 42, 48), 210.0f, 0.0f, false };
    juce::Rectangle<int> backgroundRect{ 0, 0, 327, 400 };
    g.setGradientFill(blackGrad);
    g.fillRect(backgroundRect);

    // adding two vertical lines surrounding the bass drum dials
    juce::Rectangle<int> leftLine{ 7, 7, 3, 350 };
    juce::Rectangle<int> rightLine{ 110, 7, 3, 350 };
    g.setColour(juce::Colour(126, 131, 127));
    g.drawRect(leftLine);
    g.fillRect(leftLine);
    g.drawRect(rightLine);
    g.fillRect(rightLine);

    // adding the background box to the "bass drum" label
    float cornerSize = 5.5f;
    g.setColour(juce::Colour(229, 216, 181));
    juce::Rectangle<float> labelBox{ 15, 325, 90, 32 };
    g.fillRoundedRectangle(labelBox, cornerSize);
    juce::Rectangle<float> modLabelBox{ 120, 325, 96, 32 };
    g.fillRoundedRectangle(modLabelBox, cornerSize);
    juce::Rectangle<float> overdriveLabelBox{ 225, 325, 96, 32 };
    g.fillRoundedRectangle(overdriveLabelBox, cornerSize);


    //===================================================================
    // this next section is adding the stylized boxes at the bottom of the window
    // purely cosmetic, to capture the feel of the unit

    cornerSize = 1.5f;

    juce::Rectangle<float> redOne{ 7, 364, 13, 29 };
    juce::Rectangle<float> redTwo{ 27, 364, 13, 29 };
    juce::Rectangle<float> redThree{ 47, 364, 13, 29 };
    juce::Rectangle<float> redFour{ 67, 364, 13, 29 };
    g.setColour(juce::Colour(156, 53, 34));

    g.fillRoundedRectangle(redOne, cornerSize);
    g.fillRoundedRectangle(redTwo, cornerSize);
    g.fillRoundedRectangle(redThree, cornerSize);
    g.fillRoundedRectangle(redFour, cornerSize);

    juce::Rectangle<float> orangeOne{ 87, 364, 13, 29 };
    juce::Rectangle<float> orangeTwo{ 107, 364, 13, 29 };
    juce::Rectangle<float> orangeThree{ 127, 364, 13, 29 };
    juce::Rectangle<float> orangeFour{ 147, 364, 13, 29 };
    g.setColour(juce::Colour(194, 105, 35));

    g.fillRoundedRectangle(orangeOne, cornerSize);
    g.fillRoundedRectangle(orangeTwo, cornerSize);
    g.fillRoundedRectangle(orangeThree, cornerSize);
    g.fillRoundedRectangle(orangeFour, cornerSize);

    juce::Rectangle<float> yellowOne{ 167, 364, 13, 29 };
    juce::Rectangle<float> yellowTwo{ 187, 364, 13, 29 };
    juce::Rectangle<float> yellowThree{ 207, 364, 13, 29 };
    juce::Rectangle<float> yellowFour{ 227, 364, 13, 29 };
    g.setColour(juce::Colour(226, 195, 79));

    g.fillRoundedRectangle(yellowOne, cornerSize);
    g.fillRoundedRectangle(yellowTwo, cornerSize);
    g.fillRoundedRectangle(yellowThree, cornerSize);
    g.fillRoundedRectangle(yellowFour, cornerSize);

    juce::Rectangle<float> whiteOne{ 247, 364, 13, 29 };
    juce::Rectangle<float> whiteTwo{ 267, 364, 13, 29 };
    juce::Rectangle<float> whiteThree{ 287, 364, 13, 29 };
    juce::Rectangle<float> whiteFour{ 307, 364, 13, 29 };
    g.setColour(juce::Colour(232, 222, 221));

    g.fillRoundedRectangle(whiteOne, cornerSize);
    g.fillRoundedRectangle(whiteTwo, cornerSize);
    g.fillRoundedRectangle(whiteThree, cornerSize);
    g.fillRoundedRectangle(whiteFour, cornerSize);

    //===================================================================

}

void Background::resized()
{
    // setting locations of the title labels
    labelOne.setBounds(124, 7, 196, 60);
    labelTwo.setBounds(124, 40, 196, 50);
    labelAuthor.setBounds(124, 78, 196, 20);

    // setting lcoations of bass drum label elements
    B.setBounds(17, 325, 90, 32);
    ass.setBounds(30, 328, 90, 32);
    D.setBounds(54, 325, 90, 32);
    rum.setBounds(69, 328, 90, 32);

    C.setBounds(116, 325, 90, 32);
    ircuit.setBounds(130, 328, 90, 32);
    M.setBounds(170, 325, 90, 32);
    ods.setBounds(186, 328, 90, 32);

    O.setBounds(233, 325, 90, 32);
    verdrive.setBounds(249, 328, 90, 32);

}
