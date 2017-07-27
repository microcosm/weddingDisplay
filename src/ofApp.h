#pragma once

#include "ofMain.h"
#include "ofxTexturePlane.h"
#include "ofxLayerMask.h"
#include "ofxAnimatableFloat.h"

enum displayMode {
    PHOTOS_OF_US,
    INSTAGRAM_PHOTOS,
    KID_PHOTOS
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void increment(int& target);
    void loadImages();

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
    displayMode mode;
    map<displayMode, string> imageLocations;
};
