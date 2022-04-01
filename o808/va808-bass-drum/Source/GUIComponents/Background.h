/*
  ==============================================================================

    "Background.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 15th July 2021
    Author:  Cameron Smith, UoE s1338237

    This component class contains all the static visual background elements of
    the application. This includes labels as well as shapes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Background  : public juce::Component
{
public:
    Background();
    ~Background() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // two font sizes, large and small
    juce::Font bigFont{ 24.0f };
    juce::Font smallFont{ 12.0f };

    // title labels giving the name and authorship
    juce::Label labelOne{ {}, "TR-808 BD" };
    juce::Label labelTwo{ {}, "VIRTUAL ANALOGUE MODEL" };
    juce::Label labelAuthor{ {}, "BY CAMERON SMITH" };
    
    // labels for the "bass drum" label, seperated out with capitilizations for accruate resizing
    juce::Label B{ {}, "B" };
    juce::Label ass{ {}, "ASS" };
    juce::Label D{ {}, "D" };
    juce::Label rum{ {}, "RUM" };

    juce::Label C{ {}, "C" };
    juce::Label ircuit{ {}, "IRCUIT" };
    juce::Label M{ {}, "M" };
    juce::Label ods{ {}, "ODS" };

    juce::Label O{ {}, "O" };
    juce::Label verdrive{ {}, "VERDRIVE" };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Background)
};
