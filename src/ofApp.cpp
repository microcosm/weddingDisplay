#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    textureFrameFadeAmount = 0.00035;
    framesBeforeSwitch = 240;
    framesForFade = 30;

    ofSetFrameRate(60);
    masker.setup(2);
    incrementFrameNum = -1;
    isLayered = false;
    firstIncrement = true;

    underTextureID = overTextureID = 0;

    maskOpacity.reset(0);
    maskOpacity.setDuration(framesForFade / 60.f);
    maskOpacity.setRepeatType(PLAY_ONCE);
    maskOpacity.setCurve(EASE_IN_EASE_OUT);

    ofDirectory dir("photos");
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    numImages = dir.size();

    for(int i = 0; i < numImages; i++){
        texture.setup(dir.getPath(i), 1, TEXTURE_OFFSET_MIDDLE_CENTER);
        textures.push_back(texture);
    }
    masker.toggleOverlay();
}

//--------------------------------------------------------------
void ofApp::update(){
    frameNum = ofGetFrameNum();
    if(frameNum > 0 && frameNum % framesBeforeSwitch == 0){
        isLayered = !isLayered;

        if(isLayered){
            increment(overTextureID);
            textures.at(overTextureID).setTextureScale(1);
        }else{
            increment(underTextureID);
            textures.at(underTextureID).setTextureScale(1);
        }

        maskOpacity.animateTo(isLayered ? 255 : 0);
    }

    maskOpacity.update(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
    masker.beginLayer(0);
    {
        textures.at(underTextureID).draw();
        textures.at(underTextureID).incrementTextureScale(textureFrameFadeAmount);
    }
    masker.endLayer(0);

    masker.beginLayer(1);
    {
        textures.at(overTextureID).draw();
        textures.at(overTextureID).incrementTextureScale(textureFrameFadeAmount);
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
    target += firstIncrement ? 1 : 2;
    if(target >= numImages){
        target = 0 + (target - numImages);
    }
    firstIncrement = false;
}
