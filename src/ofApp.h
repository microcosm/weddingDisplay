#pragma once

#include "ofMain.h"
#include "ofxTexturePlane.h"
#include "ofxLayerMask.h"
#include "ofxAnimatableFloat.h"
#include "ofxCenteredTrueTypeFont.h"

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
    void drawDirectoryName();
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
    bool isLayered, firstIncrement, justReset, isTransitioning, drawOverlay;
    int numImages, numSnapchatImages;
    int framesBeforeSwitchToNextPhoto, framesForFades, framesAfterModeTransitionBeforeLoad, framesAfterModeTransitionBeforeStart;
    int framesSinceTransition;
    float frameScaleAmount;
    int underTextureID, overTextureID;
    ofxAnimatableFloat maskOpacity, imageOpacity, directoryNameOpacity;
    uint64_t frameNum;
    displayMode mode;
    int currentDirectoryID;
    string rootDirectory, photosDirectory, snapChatImageDirectory;
    vector<string> otherImageDirectories, snapchatImagesPaths;
    ofxCenteredTrueTypeFont font;
};
