#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    textureFrameFadeAmount = 0.00035;
    framesBeforeSwitch = 60;
    framesForFade = 30;

    ofSetFrameRate(60);
    masker.setup(2);
    incrementFrameNum = -1;
    isLayered = false;
    isTransitioning = false;
    justReset = false;
    firstIncrement = true;
    underTextureID = overTextureID = 0;

    mode = PHOTOS_OF_US;
    imageLocations[PHOTOS_OF_US] = "photos";
    imageLocations[INSTAGRAM_PHOTOS] = "photos";
    imageLocations[KID_PHOTOS] = "photos";
    loadImages();

    maskOpacity.reset(0);
    maskOpacity.setDuration(framesForFade / 60.f);
    maskOpacity.setRepeatType(PLAY_ONCE);
    maskOpacity.setCurve(EASE_IN_EASE_OUT);
}

//--------------------------------------------------------------
void ofApp::update(){
    frameNum = ofGetFrameNum();
    if(isTransitioning){

    }else if(frameNum > 0 && frameNum % framesBeforeSwitch == 0){
        isLayered = !isLayered;

        if(isLayered){
            incrementTextureID(overTextureID);
            textures.at(overTextureID).setTextureScale(1);
        }else{
            incrementTextureID(underTextureID);
            textures.at(underTextureID).setTextureScale(1);
        }

        if((mode == PHOTOS_OF_US || mode == KID_PHOTOS) && justReset){
            incrementDisplayMode();
            //loadImages();
            isTransitioning = true;
        }

        maskOpacity.animateTo(isLayered ? 255 : 0);
    }

    maskOpacity.update(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
    masker.beginLayer(0);
    {
        if(isTransitioning && !isLayered){
            ofClear(ofColor::black);
            ofDrawBitmapString("TRANSITIONING...", 20, 20);
        }else{
            textures.at(underTextureID).incrementTextureScale(textureFrameFadeAmount);
            textures.at(underTextureID).draw();
        }
    }
    masker.endLayer(0);

    masker.beginLayer(1);
    {
        if(isTransitioning && isLayered) {
            ofClear(ofColor::black);
            ofDrawBitmapString("TRANSITIONING...", 20, 20);
        }else{
            textures.at(overTextureID).incrementTextureScale(textureFrameFadeAmount);
            textures.at(overTextureID).draw();
        }
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

void ofApp::keyPressed(int key){
    if(key == 'o'){
        masker.toggleOverlay();
    }
}

void ofApp::incrementTextureID(int& target){
    target += firstIncrement ? 1 : 2;
    justReset = false;

    if(target >= numImages){
        target = 0 + (target - numImages);
        justReset = true;
    }
    firstIncrement = false;
}

void ofApp::incrementDisplayMode(){
    if(mode == PHOTOS_OF_US){
        mode = KID_PHOTOS;
    }else if(mode == KID_PHOTOS){
        mode = INSTAGRAM_PHOTOS;
    }else if(mode == INSTAGRAM_PHOTOS){
        mode = PHOTOS_OF_US;
    }
}

void ofApp::loadImages(){
    ofDirectory dir(imageLocations[mode]);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    numImages = dir.size();

    for(int i = 0; i < numImages; i++){
        texture.setup(dir.getPath(i), 1, TEXTURE_OFFSET_MIDDLE_CENTER);
        textures.push_back(texture);
    }
}
