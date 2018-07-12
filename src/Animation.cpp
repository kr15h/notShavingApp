#include "Animation.h"

void Animation::setup(vector<std::string> & sources){
	currentFrame = 0;
	isPlaying = false;
	
	for(auto i = 0; i < sources.size(); i++){
		ofImage img;
		if(img.load(sources[i])){
			frameImages.push_back(img);
		}else{
			ofLogError() << "Failed to load image: " << sources[i];
		}
	}
	
	frameRate = 10.0f;
	lastFrameTime = ofGetElapsedTimef();
}

void Animation::update(){
	frameTime = 1.0f / frameRate;
	float now = ofGetElapsedTimef();
	float delta = now - lastFrameTime;

	if(isPlaying){
		if(delta > frameTime){
			lastFrameTime = now;
			currentFrame++;
			if(currentFrame >= frameImages.size()){
				currentFrame = 0;
			}
		}
	}
}

void Animation::draw(){
	frameImages[currentFrame].draw(0, 0, ofGetWidth(), ofGetHeight());
}

void Animation::play(){
	isPlaying = true;
}

void Animation::stop(){
	isPlaying = false;
	currentFrame = 0;
}

void Animation::pause(){
	isPlaying = false;
}
