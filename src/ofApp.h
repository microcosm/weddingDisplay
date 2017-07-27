#pragma once

#include "ofMain.h"
#include "ofxTexturePlane.h"
#include "ofxLayerMask.h"
#include "ofxAnimatableFloat.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void increment(int& target);

    vector<ofxTexturePlane> textures;
    ofxTexturePlane texture;
    ofxLayerMask masker;
    bool isLayered, firstIncrement;
    int numImages;
    int framesBeforeSwitch, framesForFade;
    float textureFrameFadeAmount;
    int incrementFrameNum;
    int underTextureID, overTextureID;
    ofxAnimatableFloat maskOpacity;
    uint64_t frameNum;
};
