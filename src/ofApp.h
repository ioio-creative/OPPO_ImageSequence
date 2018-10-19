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

#define DEBUG
//#define RELEASE
#define IS_RECONNECT_TO_MOBILE 0

#ifdef DEBUG
#define DISPLAY_WIDTH ofGetWidth()
#define DISPLAY_HEIGHT ofGetHeight()
#endif

#ifdef RELEASE
#define DISPLAY_WIDTH ofGetWidth()
#define DISPLAY_HEIGHT ofGetHeight()
#endif


class ofApp : public ofBaseApp
{
private:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

	
	/* settings */
	
	const int frameRate = 24;
	const bool isToggleFullScreen = false;
	const bool isLogToConsole = false;
	//const string adbPortForwardCmd = "/usr/local/Cellar/android-platform-tools/25.0.3/bin/adb forward tcp:12580 tcp:10086";
	const string adbPortForwardCmd = "adb forward tcp:12580 tcp:10086";
	const string mobileServerIp = "127.0.0.1";
	const int mobileServerPort = 12580;
	const string imgExtenstion = "png";
	const bool isThreadedLoadImage = false;
	const int reconnectTimeMillis = 60000;
	const bool isClearPixelsAfterLoadingTexture = true;

	/* end of settings */
	

	ofxTCPClient mobileController;
	bool connected;
	bool connectedFlag = false;
	string ctrSpeed = "peace";
	float move = 0.0;
	ofxImageSequence sequenceA;
	ofxImageSequence sequenceB;
	ofImage background;
	bool playing = true;  //controls if playing automatically, or controlled by the mouse
    int dSmooth;
	float speed;
	float percent;
	bool seqA = false;
    const float dragSpeed = 2000;
    int couDefault = 0;
    const float speedDefault = -0.000223;  // 1 / number of images in one image sequence folder
	//const float speedDefault = -0.1;
    float acc[11];
    const float accFactor = 0.006;
    bool flagDefault = false;
    int numOfFramesToStopAfterDrag = 10;

	int connectTime = 0;
	int deltaTime = 0;

	void connectToMobile();
	void connectToMobileIfTimeoutInUpdate();	
};

