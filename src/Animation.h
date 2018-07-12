#pragma once

#include "ofMain.h"

class Animation{
public:
	void setup(vector<std::string> & sources);
	void update();
	void draw();
	
	void play();
	void stop();
	void pause();
	
	vector<ofImage> frameImages;
	unsigned int currentFrame;
	bool isPlaying;
	float frameRate;
	float frameTime;
	float lastFrameTime;
};
