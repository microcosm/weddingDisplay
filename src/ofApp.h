#pragma once

#include "ofMain.h"
#include "ofxTexturePlane.h"
#include "ofxLayerMask.h"
#include "ofxAnimatableFloat.h"

enum displayMode {
    SNAPCHAT_PHOTOS,
    OTHER_PHOTOS
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void setupImages();
    void setupImageFaders();
    void update();
    void draw();
    void keyPressed(int key);
    void incrementTextureID(int& target);
    void incrementDisplayMode();
    void incrementDirectoryID();
    void loadImages();
    void loadSnapchatImages(ofDirectory &dir);
    void loadOtherImages(ofDirectory &dir);

    vector<ofxTexturePlane> textures;
    ofxTexturePlane texture;
    ofxLayerMask masker;
    bool isLayered, firstIncrement, justReset, isTransitioning;
    int numImages, numSnapchatImages;
    int framesBeforeSwitch, framesForFade, framesAfterTransitionBeforeLoad, framesSinceTransition, framesAfterTransitionBeforeStart;
    float textureFrameFadeAmount;
    int incrementFrameNum;
    int underTextureID, overTextureID;
    ofxAnimatableFloat maskOpacity, imageOpacity;
    uint64_t frameNum;
    displayMode mode;
    int currentDirectoryID;
    string photosDirectory, snapChatImageDirectory;
    vector<string> otherImageDirectories, snapchatImagesPaths;
};
