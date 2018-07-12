#include "ofApp.h"

void ofApp::setup(){
	ofBackground(0);

	serial.listDevices();
	
	ofXml settings;
	bool settingsSuccess = settings.load("settings.xml");

	int baud = 9600;
	bool serialSuccess;
	if(settingsSuccess == true){
		std::string serialPort = settings.findFirst("settings/serialPort").getValue();
		ofLog() << "serial port: " << serialPort;
		serialSuccess = serial.setup(serialPort, baud);
	}else{
		ofLog() << "Failed to load settings";
		serialSuccess = serial.setup(0, baud);
	}

	if(serialSuccess != true){
		ofLogError() << "Could not open serial connection";
		ofExit();
	}
	
	vector<std::string> sources;
	sources.push_back("movin_00.png");
	sources.push_back("movin_01.png");
	sources.push_back("movin_02.png");
	sources.push_back("movin_03.png");
	
	anima.setup(sources);
	anima.frameRate = 15;
	anima.stop();
	
	playDuration = 1.0f;
	lastPlayTime = 0.0f;
	
	#ifdef TARGET_RASPBERRY_PI
		ofSetFullscreen(true);
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