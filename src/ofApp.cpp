#include "ofApp.h"

void ofApp::setup(){
	ofBackground(0);

	serial.listDevices();
	
	ofXml settings;
	bool settingsSuccess = settings.load("settings.xml");

	int baud = 9600;
	bool serialSuccess;
	std::string imageDir;
	if(settingsSuccess == true){
		std::string serialPort = settings.findFirst("settings/serialPort").getValue();
		ofLog() << "serial port: " << serialPort;
		serialSuccess = serial.setup(serialPort, baud);
		imageDir = settings.findFirst("settings/imageDir").getValue();
		if(imageDir != "" && imageDir.substr(imageDir.length() - 1, 1) != "/"){
			imageDir += "/";
		}
		ofLog() << "image dir: " << imageDir;

	}else{
		ofLog() << "Failed to load settings";
		serialSuccess = serial.setup(0, baud);
	}

	if(serialSuccess != true){
		ofLogError() << "Could not open serial connection";
		ofExit();
	}
	
	vector<std::string> sources;
	sources.push_back(imageDir + "movin_00.png");
	sources.push_back(imageDir + "movin_01.png");
	sources.push_back(imageDir + "movin_02.png");
	sources.push_back(imageDir + "movin_03.png");
	
	anima.setup(sources);
	anima.frameRate = 15;
	anima.stop();
	
	playDuration = 1.0f;
	lastPlayTime = 0.0f;
	
	#ifdef TARGET_RASPBERRY_PI
		ofSetFullscreen(true);
		ofHideCursor();
	#endif
}

void ofApp::update(){
	anima.update();
	
	bool nl = false;
	
	if(serial.available()){
		while(serial.available()){
			int c = serial.readByte();
			if(c == 10){
				nl = true;
				break;
			}
			serialMessage += c;
		}
	}
	
	if(nl && ofIsStringInString(serialMessage, "Fingers move!")){
		anima.play();
		lastPlayTime = ofGetElapsedTimef();
		ofLog() << "---";
		ofLog() << "Fingers move!";
		ofLog() << "---";
	}
	
	if(nl){
		ofLog() << "serial: " << serialMessage;
		serialMessage.clear();
	}
	
	float delta = ofGetElapsedTimef() - lastPlayTime;
	if(delta > playDuration){
		anima.stop();
	}
}

void ofApp::draw(){
	anima.draw();
}

void ofApp::exit(){
	serial.close();
}

void ofApp::mousePressed(int x, int y, int button){
	if(anima.isPlaying){
		anima.stop();
	}else{
		anima.play();
	}
}

void ofApp::keyPressed(int key){
	if(key == 'f'){
		ofToggleFullscreen();
	}
}
