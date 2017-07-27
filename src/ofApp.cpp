#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    masker.setup(2);
    incrementFrameNum = -1;
    isLayered = false;
    framesBeforeSwitch = 30;
    framesForFade = 10;
    underTextureID = 0;
    overTextureID = 1;
    numImages = 5;

    maskOpacity.reset(0);
    maskOpacity.setDuration(framesForFade / 60.f);
    maskOpacity.setRepeatType(PLAY_ONCE);
    maskOpacity.setCurve(EASE_IN_EASE_OUT);

    for(int i = 1; i <= numImages; i++){
        texture.setup("wedding" + ofToString(i) + ".jpg");
        textures.push_back(texture);
    }
    masker.toggleOverlay();
}

//--------------------------------------------------------------
void ofApp::update(){
    frameNum = ofGetFrameNum();
    if(frameNum % framesBeforeSwitch == 0){
        isLayered = !isLayered;
        maskOpacity.animateTo(isLayered ? 255 : 0);
        incrementFrameNum = frameNum + framesForFade + 1;
    }

    if(frameNum == incrementFrameNum){
        increment(isLayered ? underTextureID : overTextureID);
    }

    maskOpacity.update(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
    masker.beginLayer(0);
    {
        textures.at(underTextureID).draw();
    }
    masker.endLayer(0);

    masker.beginLayer(1);
    {
        textures.at(overTextureID).draw();
    }
    masker.endLayer(1);

    masker.beginMask(1);
    {
        ofBackground(ofColor::black);
        ofSetColor(ofColor(255, 255, 255, maskOpacity.val()));
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    masker.endMask(1);

    masker.draw();
    masker.drawOverlay();
}

void ofApp::increment(int& target){
    target += 2;
    if(target >= numImages){
        target = 0 + (target - numImages);
    }
}
