#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    textureFrameFadeAmount = 0.00035;
    framesBeforeSwitch = 120;
    framesForFade = 60;
    framesAfterTransitionBeforeLoad = 60;
    framesAfterTransitionBeforeStart = 180;

    ofSetFrameRate(60);
    masker.setup(2);
    incrementFrameNum = -1;
    isLayered = false;
    isTransitioning = false;
    justReset = false;
    firstIncrement = true;
    underTextureID = overTextureID = 0;

    imageLocations[PHOTOS_OF_US] = "photos";
    imageLocations[INSTAGRAM_PHOTOS] = "photos";
    imageLocations[KID_PHOTOS] = "photos";
    mode = PHOTOS_OF_US;
    loadImages();

    maskOpacity.reset(0);
    maskOpacity.setDuration(framesForFade / 60.f);
    maskOpacity.setRepeatType(PLAY_ONCE);
    maskOpacity.setCurve(EASE_IN_EASE_OUT);
    imageOpacity.reset(0);
    imageOpacity.setDuration(framesForFade / 5 / 60.f);
    imageOpacity.setRepeatType(PLAY_ONCE);
    imageOpacity.setCurve(EASE_IN_EASE_OUT);
    imageOpacity.animateTo(255);
}

//--------------------------------------------------------------
void ofApp::update(){
    frameNum = ofGetFrameNum();

    if(isTransitioning){
        framesSinceTransition++;
        if(framesSinceTransition == framesAfterTransitionBeforeLoad){
            incrementDisplayMode();
            loadImages();
        }
        if(framesSinceTransition == framesAfterTransitionBeforeStart){
            firstIncrement = true;
            underTextureID = overTextureID = 0;
            isTransitioning = false;
            imageOpacity.reset(0);
            imageOpacity.animateTo(255);
        }
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
            isTransitioning = true;
            framesSinceTransition = 0;
        }

        maskOpacity.animateTo(isLayered ? 255 : 0);
    }

    maskOpacity.update(1.0f/60.0f);
    imageOpacity.update(1.0f/60.0f);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::black);
    masker.beginLayer(0);
    {
        ofClear(ofColor::black);
        if(isTransitioning && !isLayered){
            ofDrawBitmapString("TRANSITIONING...", 20, 20);
        }else{
            ofSetColor(ofColor(ofColor::white, imageOpacity.val()));
            textures.at(underTextureID).incrementTextureScale(textureFrameFadeAmount);
            textures.at(underTextureID).draw();
        }
    }
    masker.endLayer(0);

    masker.beginLayer(1);
    {
        ofClear(ofColor::black);
        if(isTransitioning && isLayered) {
            ofDrawBitmapString("TRANSITIONING...", 20, 20);
        }else{
            ofSetColor(ofColor(ofColor::white, imageOpacity.val()));
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

    textures.clear();
    for(int i = 0; i < numImages; i++){
        texture.setup(dir.getPath(i), 1, TEXTURE_OFFSET_MIDDLE_CENTER);
        textures.push_back(texture);
    }
}
