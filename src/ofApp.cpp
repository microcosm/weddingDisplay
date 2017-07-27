#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    masker.setup(2);
    isLayered = false;
    msBeforeSwitch = 30;
    msForFade = 10;

    maskOpacity.reset(0);
    maskOpacity.setDuration(msForFade / 60.f);
    maskOpacity.setRepeatType(PLAY_ONCE);
    maskOpacity.setCurve(EASE_IN_EASE_OUT);

    for(int i = 1; i <= 5; i++){
        texture.setup("wedding" + ofToString(i) + ".jpg");
        textures.push_back(texture);
    }
    masker.toggleOverlay();
}

//--------------------------------------------------------------
void ofApp::update(){
    if(ofGetFrameNum() % msBeforeSwitch == 0){
        isLayered = !isLayered;
        maskOpacity.animateTo(isLayered ? 255 : 0);
    }

    maskOpacity.update(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
    masker.beginLayer(0);
    {
        textures.at(0).draw();
    }
    masker.endLayer(0);

    masker.beginLayer(1);
    {
        textures.at(1).draw();
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
