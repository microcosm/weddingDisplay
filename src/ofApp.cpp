#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    frameScaleAmount = 0.00035;
    framesBeforeSwitchToNextPhoto = 120;
    framesForFades = 60;
    framesAfterModeTransitionBeforeLoad = 60;
    framesAfterModeTransitionBeforeStart = 200;
    numSnapchatImages = 8;
    showOverlay = false;
    rootDirectory = "/Users/amcw/Desktop/Wedding Slideshow";
    snapChatImageDirectory = "/Users/amcw/Pictures/Photos Library.photoslibrary/Masters/2017/08";

    ofToggleFullscreen();
    ofSetFrameRate(30);
    ofHideCursor();

    masker.setup(2);
    isLayered = false;
    isTransitioning = false;
    justReset = false;
    firstIncrement = true;
    underTextureID = overTextureID = 0;
    font.load(rootDirectory + "/fonts/coneria_script/Demo_ConeriaScript.ttf", 100, true);

    setupImages();
    setupImageFaders();
}

void ofApp::setupImages(){
    photosDirectory = "photos";
    ofDirectory dir(rootDirectory + "/" + photosDirectory);
    dir.listDir();
    vector<ofFile> contents = dir.getFiles();

    for(int i = 0; i < contents.size(); i++){
        if(contents.at(i).isDirectory()){
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
    maskOpacity.setDuration(framesForFades / 60.f);
    maskOpacity.setRepeatType(PLAY_ONCE);
    maskOpacity.setCurve(EASE_IN_EASE_OUT);
    imageOpacity.reset(0);
    imageOpacity.setDuration(framesForFades / 5 / 60.f);
    imageOpacity.setRepeatType(PLAY_ONCE);
    imageOpacity.setCurve(EASE_IN_EASE_OUT);
    directoryNameOpacity.reset(0);
    directoryNameOpacity.setDuration(framesForFades / 3 / 60.f);
    directoryNameOpacity.setRepeatType(PLAY_ONCE);
    directoryNameOpacity.setCurve(EASE_IN_EASE_OUT);

    imageOpacity.animateTo(255);
}

//--------------------------------------------------------------
void ofApp::update(){
    frameNum = ofGetFrameNum();

    if(isTransitioning){
        framesSinceTransition++;
        if(framesSinceTransition == framesAfterModeTransitionBeforeLoad){
            incrementDisplayMode();
            loadImages();
            directoryNameOpacity.animateTo(255);
            underTextureID = overTextureID = 0;
        }
        if(framesSinceTransition == framesAfterModeTransitionBeforeStart - 20 - (directoryNameOpacity.getDuration() * 60)){
            directoryNameOpacity.animateTo(0);
        }
        if(framesSinceTransition == framesAfterModeTransitionBeforeStart){
            firstIncrement = true;
            isTransitioning = false;
            imageOpacity.reset(0);
            imageOpacity.animateTo(255);
        }
    }else if(frameNum > 0 && frameNum % framesBeforeSwitchToNextPhoto == 0){
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
            directoryNameOpacity.reset(0);
        }

        maskOpacity.animateTo(isLayered ? 255 : 0);
    }

    maskOpacity.update(1.0f/60.0f);
    imageOpacity.update(1.0f/60.0f);
    directoryNameOpacity.update(1.0f/60.f);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::black);
    masker.beginLayer(0);
    {
        ofClear(ofColor::black);
        if(isTransitioning && !isLayered){
            drawDirectoryName();
        }else{
            ofSetColor(ofColor(ofColor::white, imageOpacity.val()));
            textures.at(underTextureID).incrementTextureScale(frameScaleAmount);
            textures.at(underTextureID).draw();
        }
    }
    masker.endLayer(0);

    masker.beginLayer(1);
    {
        ofClear(ofColor::black);
        if(isTransitioning && isLayered) {
            drawDirectoryName();
        }else{
            ofSetColor(ofColor(ofColor::white, imageOpacity.val()));
            textures.at(overTextureID).incrementTextureScale(frameScaleAmount);
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

    if(showOverlay){
        ofSetColor(ofColor::white);
        ofDrawBitmapString(ofGetFrameRate(), 20, 20);
        masker.drawOverlay();
    }
}

void ofApp::drawDirectoryName(){
    ofSetColor(ofColor(ofColor::white, directoryNameOpacity.val()));
    if(mode == SNAPCHAT_PHOTOS){
        font.drawStringCentered("Selfie Booth", ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    }else{
        string dirName = getActualName(otherImageDirectories.at(currentDirectoryID));
        font.drawStringCentered(dirName, ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    }
}

void ofApp::keyPressed(int key){
    if(key == 'o'){
        masker.toggleOverlay();
        showOverlay = !showOverlay;
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
    if(mode == OTHER_PHOTOS){
        if(currentDirectoryID == 1 || currentDirectoryID == 3 || currentDirectoryID == 6){
            mode = SNAPCHAT_PHOTOS;
        }else{
            incrementDirectoryID();
        }
    }else if(mode == SNAPCHAT_PHOTOS){
        mode = OTHER_PHOTOS;
        incrementDirectoryID();
    }
}

void ofApp::loadImages(){
    textures.clear();

    if(mode == SNAPCHAT_PHOTOS){
        ofDirectory dir(snapChatImageDirectory);
        files.clear();
        scan_dir(dir);
        loadSnapchatImages(dir);
    }else{
        ofDirectory dir(rootDirectory + "/" + photosDirectory + "/" + otherImageDirectories.at(currentDirectoryID));
        dir.allowExt("png");
        dir.allowExt("jpg");
        dir.allowExt("PNG");
        dir.allowExt("JPG");
        dir.listDir();
        loadOtherImages(dir);
    }
}

void ofApp::loadSnapchatImages(ofDirectory &dir){
    snapchatImagesPaths.clear();
    numImages = numSnapchatImages;

    //cout << "Loaded from snapchat folder:" << endl;
    for(int i = 0; i < files.size(); i++){
        snapchatImagesPaths.push_back(files.at(i).getAbsolutePath());
        //cout << files.at(i).getAbsolutePath() << endl;
    }
    //cout << "-----" << endl;
    random_shuffle(snapchatImagesPaths.begin(), snapchatImagesPaths.end());
    snapchatImagesPaths.resize(numSnapchatImages);
    /*cout << "Shuffled and trimmed:" << endl;
    for(int i = 0; i < snapchatImagesPaths.size(); i++){
        cout << snapchatImagesPaths.at(i) << endl;
    }
    cout << "-----" << endl;*/
    for(int i = 0; i < numImages; i++){
        loadImage(snapchatImagesPaths.at(i));
    }
}

void ofApp::loadOtherImages(ofDirectory &dir){
    numImages = dir.size();
    for(int i = 0; i < numImages; i++){
        loadImage(dir.getPath(i));
    }
}

void ofApp::loadImage(string path){
    textures.push_back(texture);

    image.load(path);
    if(image.getWidth() > image.getHeight()){
        textures.back().setup(path, ofGetWidth(), ofGetHeight(), 1, TEXTURE_OFFSET_MIDDLE_CENTER);
    }else{
        textures.back().setup(path, ofGetWidth(), ofGetHeight(), 1, TEXTURE_OFFSET_MIDDLE_CENTER, TEXTURE_MODE_FIT, TEXTURE_WRAP_CLAMP_TO_BORDER);
    }
}

string ofApp::getActualName(string s) {
    char delim = ')';
    vector<string> elems;
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems.at(1);
}

//https://gist.github.com/aman-tiwari/daa815b4c6e938875017
const string allowed_ext[] = {"jpg", "png", "JPG", "PNG"};

void ofApp::scan_dir(ofDirectory dir){
    ofDirectory new_dir;
    int size = dir.listDir();
    for (int i = 0; i < size; i++){
        if (dir.getFile(i).isDirectory()){
            cout << "Found directory: " << dir.getFile(i).getAbsolutePath() << endl;
            new_dir = ofDirectory(dir.getFile(i).getAbsolutePath());
            new_dir.listDir();
            new_dir.sort();
            scan_dir(new_dir);
        } else if (std::find(std::begin(allowed_ext),
                             std::end(allowed_ext),
                             dir.getFile(i).getExtension()) != std::end(allowed_ext)) {
            cout << "Found file: " << dir.getFile(i).getAbsolutePath() << endl;
            files.push_back(dir.getFile(i));
        }
    }
}
