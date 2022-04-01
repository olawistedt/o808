#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"
#include "va808-bass-drum/Source/VA808BassDrum.h"
#include "va808-bass-drum/Source/Overdrive.h"

const int kNumPresets = 1;

enum EParams
{
  kParamGain = 0,
  kParamNoteGlideTime,
  kParamAttack,
  kParamDecay,
  kParamSustain,
  kParamRelease,
  kParamLFOShape,
  kParamLFORateHz,
  kParamLFORateTempo,
  kParamLFORateMode,
  kParamLFODepth,



  k808Level,
  k808Tone,
  k808Decay,
  k808Tuning,
  k808Mix,
  k808Gain,




  kNumParams
};

#if IPLUG_DSP
// will use EParams in o808_DSP.h
#include "o808_DSP.h"
#endif

enum EControlTags
{
  kCtrlTagMeter = 0,
  kCtrlTagLFOVis,
  kCtrlTagScope,
  kCtrlTagRTText,
  kCtrlTagKeyboard,
  kCtrlTagBender,
  kNumCtrlTags
};

using namespace iplug;
using namespace igraphics;

class o808 final : public Plugin
{
public:
  o808(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
public:
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;
  bool OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData) override;
  double smoothing(double v);

private:
  unsigned int mSamples = 0;
  o808DSP<sample> mDSP {16};
  IPeakSender<2> mMeterSender;
  ISender<1> mLFOVisSender;

  /// Instance of the TR-808 bass drum class
  VA808BassDrum bassDrum;

  /// Adjustable gain value for the output level
  float outputGain = 0.7f;

  /// Instance of the overdive class
  Overdrive overdrive;

  ///// Smoothed value of the overdrive gain
  //juce::SmoothedValue<float> smoothedGain;

  ///// Smoothed value of the overdrive mix
  //juce::SmoothedValue<float> smoothedMix;

#endif
};
