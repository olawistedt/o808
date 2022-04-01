# IPLUG2_ROOT should point to the top level IPLUG2 folder from the project folder
# By default, that is three directories up from /Examples/o808/config
IPLUG2_ROOT = ../../iPlug2

include ../../common-web.mk

SRC += $(PROJECT_ROOT)/o808.cpp
SRC += $(PROJECT_ROOT)/../iPlug2/IPlug/Extras/Synth/MidiSynth.cpp
SRC += $(PROJECT_ROOT)/../iPlug2/IPlug/Extras/Synth/VoiceAllocator.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/Overdrive.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808BassDrum.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808Components/BridgedTNetwork.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808Components/FeedbackBuffer.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808Components/LevelStage.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808Components/OutputStage.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808Components/PulseShaper.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808Components/RetriggeringPulse.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808Components/ToneStage.cpp
SRC += $(PROJECT_ROOT)/va808-bass-drum/Source/VA808Components/TriggerLogic.cpp

# WAM_SRC += 

# WAM_CFLAGS +=

WEB_CFLAGS += -DIGRAPHICS_NANOVG -DIGRAPHICS_GLES2

WAM_LDFLAGS += -O0 -s EXPORT_NAME="'AudioWorkletGlobalScope.WAM.o808'" -s ASSERTIONS=0

WEB_LDFLAGS += -O0 -s ASSERTIONS=0

WEB_LDFLAGS += $(NANOVG_LDFLAGS)
