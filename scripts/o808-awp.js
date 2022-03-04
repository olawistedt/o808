/* Declares the o808 Audio Worklet Processor */

class o808_AWP extends AudioWorkletGlobalScope.WAMProcessor
{
  constructor(options) {
    options = options || {}
    options.mod = AudioWorkletGlobalScope.WAM.o808;
    super(options);
  }
}

registerProcessor("o808", o808_AWP);
