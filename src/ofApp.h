#pragma once

#include "ofMain.h"
#include "ofxTexturePlane.h"
#include "ofxLayerMask.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    vector<ofxTexturePlane> textures;
    ofxTexturePlane texture;
    ofxLayerMask masker;
    bool maskOn;
};
