#pragma once

#include "ofMain.h"
#include "ofxTexturePlane.h"
#include "ofxLayerMask.h"
#include "ofxAnimatableFloat.h"

enum displayMode {
    PHOTOS_OF_US,
    KID_PHOTOS,
    INSTAGRAM_PHOTOS
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void incrementTextureID(int& target);
    void incrementDisplayMode();
    void loadImages();

    vector<ofxTexturePlane> textures;
    ofxTexturePlane texture;
    ofxLayerMask masker;
    bool isLayered, firstIncrement, justReset, isTransitioning;
    int numImages;
    int framesBeforeSwitch, framesForFade, framesAfterTransitionBeforeLoad, framesSinceTransition, framesAfterTransitionBeforeStart;
    float textureFrameFadeAmount;
    int incrementFrameNum;
    int underTextureID, overTextureID;
    ofxAnimatableFloat maskOpacity, imageOpacity;
    uint64_t frameNum;
    displayMode mode;
    map<displayMode, string> imageLocations;
};
