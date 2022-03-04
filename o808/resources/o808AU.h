
#include <TargetConditionals.h>
#if TARGET_OS_IOS == 1
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif

#define IPLUG_AUVIEWCONTROLLER IPlugAUViewController_vo808
#define IPLUG_AUAUDIOUNIT IPlugAUAudioUnit_vo808
#import <o808AU/IPlugAUViewController.h>
#import <o808AU/IPlugAUAudioUnit.h>

//! Project version number for o808AU.
FOUNDATION_EXPORT double o808AUVersionNumber;

//! Project version string for o808AU.
FOUNDATION_EXPORT const unsigned char o808AUVersionString[];

@class IPlugAUViewController_vo808;
