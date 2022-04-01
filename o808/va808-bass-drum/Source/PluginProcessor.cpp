/*
  ==============================================================================

    "PluginProcessor.cpp"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237
    
    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VA808BassDrumAudioProcessor::VA808BassDrumAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters(*this, nullptr, "Parameters", createParams())
#endif
{
    //==========================================================================
    // add wavetable synth voices to the synthesiser class
    for (int i = 0; i < voiceCount; i++)
    {
        drumSynth.addVoice(new DrumSynthVoice());
    }

    // add wavetable synth sound to the synthesiser class
    drumSynth.addSound(new DrumSynthSound());
}

VA808BassDrumAudioProcessor::~VA808BassDrumAudioProcessor()
{
}

//==============================================================================
const juce::String VA808BassDrumAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VA808BassDrumAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VA808BassDrumAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VA808BassDrumAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VA808BassDrumAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VA808BassDrumAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VA808BassDrumAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VA808BassDrumAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VA808BassDrumAudioProcessor::getProgramName (int index)
{
    return {};
}

void VA808BassDrumAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VA808BassDrumAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // passing the current sample rate to the synthesizer class
    drumSynth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < voiceCount; i++)
    {
        DrumSynthVoice* v = dynamic_cast<DrumSynthVoice*>(drumSynth.getVoice(i));

        // providing the voices with essential information (sample rate)
        v->prepareToPlay(sampleRate);
    }

    
}

void VA808BassDrumAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VA808BassDrumAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VA808BassDrumAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // removing any input audio, ensuring the buffer is clear
    buffer.clear();

    // update parameters
    setParams();

    // handing the synthesizer class the audio buffer to be filled
    drumSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool VA808BassDrumAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VA808BassDrumAudioProcessor::createEditor()
{
    return new VA808BassDrumAudioProcessorEditor (*this);
}

//==============================================================================
void VA808BassDrumAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VA808BassDrumAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VA808BassDrumAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout VA808BassDrumAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // adding parameters to the value tree state
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LEVEL", "Level", juce::NormalisableRange<float> {0.0f, 1.0f, 0.02f}, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TONE", "Tone", juce::NormalisableRange<float> {0.0f, 1.0f, 0.02f}, 0.8f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.0f, 1.0f, 0.02f}, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TUNING", "Tuning", juce::NormalisableRange<float> {0.0f, 1.0f, 0.02f}, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", juce::NormalisableRange<float> {0.0f, 1.0f, 0.002f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float> {0.0f, 1.0f, 0.002f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("BUTTON STATE", "Button State", true));

    return { params.begin(), params.end() };
}

void VA808BassDrumAudioProcessor::setParams()
{
    // getting parameter value from the paramter value tree state
    auto& level = *parameters.getRawParameterValue("LEVEL");
    auto& tone = *parameters.getRawParameterValue("TONE");
    auto& decay = *parameters.getRawParameterValue("DECAY");
    auto& tuning = *parameters.getRawParameterValue("TUNING");
    auto& mix = *parameters.getRawParameterValue("MIX");
    auto& gain = *parameters.getRawParameterValue("GAIN");
    auto& buttonState = *parameters.getRawParameterValue("BUTTON STATE");

    for (int i = 0; i < drumSynth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<DrumSynthVoice*>(drumSynth.getVoice(i)))
        {
            // passing all the up-to-date parameters to the synthesizer voice class
            voice->updateDrumParams(level, tone, decay, tuning, mix, gain, buttonState);
        }

    }
}