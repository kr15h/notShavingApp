#pragma once

#include "ofMain.h"
#include "Animation.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void mousePressed(int x, int y, int button);
	
	ofSerial serial;
	Animation anima;
	std::string serialMessage;
	
	float playDuration;
	float lastPlayTime;
};
