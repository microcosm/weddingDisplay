#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    masker.setup(2);
    maskOn = true;

    for(int i = 1; i <= 5; i++){
        texture.setup("wedding" + ofToString(i) + ".jpg");
        textures.push_back(texture);
    }
    masker.toggleOverlay();
}

//--------------------------------------------------------------
void ofApp::update(){
    if(ofGetFrameNum() % 30 == 0){
        maskOn = !maskOn;
    }
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
        maskOn ? ofBackground(ofColor::white) : ofBackground(ofColor::black);
    }
    masker.endMask(1);

    masker.draw();
    masker.drawOverlay();
}
