/**
*
*	ofxImageSequence example project
*
*  Created by James George ( http://www.jamesgeorge.org ) on 6/15/10.
*  In collaboration with FlightPhase ( http://www.flightphase.com )
*		- Updated for 0.8.4 by James George on 12/10/2014 for Specular (http://specular.cc) (how time flies!)
*
*  Copyright (c) 2010-2015
*/

#pragma once

#include "ofMain.h"
#include "ofxImageSequence.h"
#include "ofxNetwork.h"
#include "ofxGui.h"

//#define DEBUG
#define RELEASE
#define IS_RECONNECT_TO_MOBILE 1

#define TEXTURE_WIDTH 1080
#define TEXTURE_HEIGHT 1920
#define SEQUENCE_FPS 100
#define APP_FPS 25

#define FOLDER_A "frameA"
//#define FOLDER_B "frameB"
#define IMAGE_FMT "png"

#ifdef DEBUG
#define DISPLAY_WIDTH ofGetWidth()
#define DISPLAY_HEIGHT ofGetHeight()
#endif

#ifdef RELEASE
#define DISPLAY_WIDTH ofGetWindowWidth()
#define DISPLAY_HEIGHT ofGetWindowHeight()
#endif

#define FBO_SCALE ((float)DISPLAY_HEIGHT/TEXTURE_HEIGHT)

class ofApp : public ofBaseApp
{
private:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);

	bool drawGui = true;

	/* ofParameters */
	ofxPanel gui;
	ofParameterGroup speedParameters;
	ofParameter<int> targetFps;
	ofParameter<string> imgFmt;
	ofParameter<float> idleSpeed;
	ofParameter<float> dragSpeedMultiplier;
	ofParameter<float> slideMultiplier;
	ofParameter<int> dragFrameThreshold;
	ofParameter<float> timeoutSeconds;
	ofParameter<bool> isToggleFullScreen = false;
	ofParameter<bool> guiStartup = true;

	void guiSetup();

	/* settings */
	
	//const int frameRate = APP_FPS;
	//const string imgExtenstion = IMAGE_FMT;
	const bool isLogToConsole = false;
	//const string adbPortForwardCmd = "/usr/local/Cellar/android-platform-tools/25.0.3/bin/adb forward tcp:12580 tcp:10086";
	const string adbPortForwardCmd = "adb forward tcp:12580 tcp:10086";
	const string mobileServerIp = "127.0.0.1";
	const int mobileServerPort = 12580;
	const bool isThreadedLoadImage = false;
	const uint64_t reconnectTimeMillis = 5000;
	const bool isClearPixelsAfterLoadingTexture = true;

	/* end of settings */

	ofxTCPClient* mobileController;
	bool connectedFlag = false;
	string ctrSpeed = "peace";
	//move := number of frames to proceed during dragging
	float move = 0.0;
	ofxImageSequence sequenceA;
	//ofxImageSequence sequenceB;
	//ofImage background;
	bool playing = true;  //controls if playing automatically, or controlled by the mouse
    int dSmooth;
	float speed;
	float percent;
	//bool seqA = false;
    const float DragSpeedMultiplier = 1;
	bool forwardUpward = true;
    float idleTimeout = 0;
	bool isIdle = true;
	//Speed := frames per second
    const float MinSpeed = 25;
	const float SlideMultiplier = 0.25;
	//const float speedDefault = -0.1;
    
	bool isDragging = false;
	uint64_t connectTime = 0;

	ofTexture *drawTex;

	ofFbo sequenceFbo;

	void connectToMobile();
	void connectToMobileIfTimeoutInUpdate();	
};

