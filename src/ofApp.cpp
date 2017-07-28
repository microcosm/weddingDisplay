#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    textureFrameFadeAmount = 0.00035;
    framesBeforeSwitch = 120;
    framesForFade = 60;
    framesAfterTransitionBeforeLoad = 60;
    framesAfterTransitionBeforeStart = 180;
    numSnapchatImages = 3;

    ofToggleFullscreen();
    ofSetFrameRate(60);
    ofHideCursor();

    masker.setup(2);
    incrementFrameNum = -1;
    isLayered = false;
    isTransitioning = false;
    justReset = false;
    firstIncrement = true;
    underTextureID = overTextureID = 0;

    setupImages();
    setupImageFaders();
}

void ofApp::setupImages(){
    photosDirectory = "photos";
    snapChatImageDirectory = "snapchat";
    ofDirectory dir(photosDirectory);
    dir.listDir();
    vector<ofFile> contents = dir.getFiles();

    for(int i = 0; i < contents.size(); i++){
        if(contents.at(i).isDirectory() && contents.at(i).getFileName() != snapChatImageDirectory){
            otherImageDirectories.push_back(contents.at(i).getFileName());
        }
    }
    cout << "Found image directories:" << endl;
    for(int i = 0; i < otherImageDirectories.size(); i++){
        cout << otherImageDirectories.at(i) << endl;
    }
    cout << "------";
    currentDirectoryID = 0;
    mode = OTHER_PHOTOS;
    loadImages();
}

void ofApp::setupImageFaders(){
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
            underTextureID = overTextureID = 0;
        }
        if(framesSinceTransition == framesAfterTransitionBeforeStart){
            firstIncrement = true;
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

        if(justReset){
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

void ofApp::incrementDirectoryID(){
    currentDirectoryID++;
    if(currentDirectoryID >= otherImageDirectories.size()){
        currentDirectoryID = 0;
    }
}

void ofApp::incrementDisplayMode(){
    if(mode == SNAPCHAT_PHOTOS){
        mode = OTHER_PHOTOS;
        incrementDirectoryID();
    }else if(mode == OTHER_PHOTOS){
        mode = SNAPCHAT_PHOTOS;
    }
}

void ofApp::loadImages(){
    string currentImagesDirectory = mode == SNAPCHAT_PHOTOS ? snapChatImageDirectory : otherImageDirectories.at(currentDirectoryID);
    ofDirectory dir(photosDirectory + "/" + currentImagesDirectory);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    textures.clear();

    if(mode == SNAPCHAT_PHOTOS){
        loadSnapchatImages(dir);
    }else{
        loadOtherImages(dir);
    }
}

void ofApp::loadSnapchatImages(ofDirectory &dir){
    snapchatImagesPaths.clear();
    numImages = numSnapchatImages;

    //cout << "Loaded from snapchat folder:" << endl;
    for(int i = 0; i < dir.size(); i++){
        snapchatImagesPaths.push_back(dir.getPath(i));
        //cout << dir.getPath(i) << endl;
    }
    //cout << "-----" << endl;
    random_shuffle(snapchatImagesPaths.begin(), snapchatImagesPaths.end());
    snapchatImagesPaths.resize(numSnapchatImages);
    /*cout << "Shuffled and trimmed:" << endl;
    for(int i = 0; i < snapchatImagesPaths.size(); i++){
        cout << snapchatImagesPaths.at(i) << endl;
    }
    cout << "-----" << endl;*/
    for(int i = 0; i < numSnapchatImages; i++){
        textures.push_back(texture);
        textures.back().setup(snapchatImagesPaths.at(i), 1, TEXTURE_OFFSET_MIDDLE_CENTER);
    }
}

void ofApp::loadOtherImages(ofDirectory &dir){
    numImages = dir.size();
    for(int i = 0; i < numImages; i++){
        textures.push_back(texture);
        textures.back().setup(dir.getPath(i), 1, TEXTURE_OFFSET_MIDDLE_CENTER);
    }
}
