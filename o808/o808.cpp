#include "o808.h"
#include "IPlug_include_in_plug_src.h"
#include "LFO.h"

o808::o808(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(k808Level)->InitDouble("Level", 0.5, 0.0, 1.0, 0.2);
  GetParam(k808Tone)->InitDouble("Tone", 0.8, 0.0, 1.0, 0.2);
  GetParam(k808Decay)->InitDouble("Decay", 0.5, 0.0, 1.0, 0.2);
  GetParam(k808Tuning)->InitDouble("Tuning", 0.5, 0.0, 1.0, 0.2);
  GetParam(k808Mix)->InitDouble("Mix", 0.0, 0.0, 1.0, 0.002);
  GetParam(k808Gain)->InitDouble("Gain", 0.0, 0.0, 1.0, 0.002);

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->EnableMouseOver(true);
    pGraphics->EnableMultiTouch(true);

#ifdef OS_WEB
    pGraphics->AttachPopupMenuControl();
#endif

    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds().GetPadded(-20.f);
    const IRECT controls = b.GetGridCell(1, 2, 2);
    pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(0, 2, 6).GetCentredInside(90), k808Level, "Level"));
    pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(1, 2, 6).GetCentredInside(90), k808Tone, "Tone"));
    pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(2, 2, 6).GetCentredInside(90), k808Tuning, "Tuning"));
    pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(3, 2, 6).GetCentredInside(90), k808Decay, "Decay"));
  };
#endif
}

#if IPLUG_DSP

double o808::smoothing(double v)
{
  double fs = GetSampleRate();
  double t = 0.1; //t = <time>; // in seconds
  double c = 1.0 / (t * fs); // increment coeff
  double p = 0.0; // memory. initialise/retrigger to 0.0
  double y = p; // output sample
  p += c;
  if (p >= 1.0) { p = 1.0; }
  return y;
}

void o808::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  // Channel declaration.
  PLUG_SAMPLE_DST* out01 = outputs[0];  PLUG_SAMPLE_DST* out02 = outputs[1];
  for (int offset = 0; offset < nFrames; ++offset)
  {
    float v_out = bassDrum.process();

    ////===========================================
    ////APPLYING OVERDRIVE

    // updating the mix and gain variables from the smoothed values
  //  float overdriveMix = smoothedMix.getNextValue();
  //  float overdriveGain = smoothedGain.getNextValue();

    //// store the dry sample before overdive, reduce volume
    float drySample = v_out * outputGain /*ola laggt till*/ * 0.05f;

    // get the overdriven sample
  //  float overdrivenSample = overdrive.process(v_out * outputGain * (1.f + overdriveGain * 4.0f));

    //// mix between the two to get the current output sample
    //float currentSample = (1.0f - overdriveMix) * drySample + overdriveMix * overdrivenSample;

    ////===========================================
    ////WRITING OUTPUT TO BUFFER

    //// for each channel, write the currentSample float to the output
    //for (int chan = 0; chan < outputBuffer.getNumChannels(); chan++)
    //{
    //  // The output sample is scaled by 0.2 so that it is not too loud by default
    //  outputBuffer.addSample(chan, sampleIndex, currentSample);
    //}


    while (!mMidiQueue.Empty())
    {
      IMidiMsg msg = mMidiQueue.Peek();
      if (msg.StatusMsg() == IMidiMsg::kNoteOn)
      {
        int velocity = msg.Velocity();
        int noteNr = msg.NoteNumber();
        bassDrum.activate(velocity);
      }
      //else if (msg.StatusMsg() == IMidiMsg::kNoteOff)
      //{
      //  mSynth.NoteOff(msg.NoteNumber());
      //}
      mMidiQueue.Remove();
    }
    *out01++ = *out02++ = drySample;
  }
}

void o808::OnIdle()
{
  mMeterSender.TransmitData(*this);
  mLFOVisSender.TransmitData(*this);
}

void o808::OnReset()
{
  bassDrum.setSampleRate(static_cast<float>(GetSampleRate()));
  overdrive.setSampleRate(static_cast<float>(GetSampleRate()));

  //// setting up smoothed values for the gain and mix of the overdrive
  //smoothedGain.reset(sampleRate, 0.01);
  //smoothedGain.setCurrentAndTargetValue(0.0);
  //smoothedMix.reset(sampleRate, 0.01);
  //smoothedMix.setCurrentAndTargetValue(0.0);
}

void o808::ProcessMidiMsg(const IMidiMsg& msg)
{
  TRACE;
  mMidiQueue.Add(msg); // Take care of MIDI events in ProcessBlock()
}

#if IPLUG_DSP
void o808::OnParamChange(int paramIdx)
{
  double value = GetParam(paramIdx)->Value();

  //  mDSP.SetParam(paramIdx, GetParam(paramIdx)->Value());
  if (paramIdx >= k808Level && paramIdx <= k808Gain)
  {
    float level = static_cast<float>(GetParam(k808Level)->Value());
    float tone = static_cast<float>(GetParam(k808Tone)->Value());
    float decay = static_cast<float>(GetParam(k808Decay)->Value());
    float tuning = static_cast<float>(GetParam(k808Tuning)->Value());
    //GetParam(k808Mix)->Value();
    //GetParam(k808Gain)->Value();

    bassDrum.updateParams(level, tone, decay, tuning, 1.0);
  }
}
#endif

bool o808::OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData)
{
  if (ctrlTag == kCtrlTagBender && msgTag == IWheelControl::kMessageTagSetPitchBendRange)
  {
    const int bendRange = *static_cast<const int*>(pData);
    mDSP.mSynth.SetPitchBendRange(bendRange);
  }

  return false;
}
#endif
