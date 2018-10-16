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

#pragma onc

#include "ofMain.h"
#include "ofxImageSequence.h"
#include "ofxNetwork.h"

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	

	ofxTCPClient mobileController;
	bool connected;
	bool connectedFlag = false;
	string ctrSpeed = "peace";
	float move = 0.0;
	ofxImageSequence sequenceA;
	ofxImageSequence sequenceB;
	ofImage background;
	bool playing;
    int dSmooth;
	float speed = 1.0;
	float percent;
	bool seqA = false;
    float dragSpeed;
    int couDefault;
    float speedDefault;
    float acc[11];
    float accFactor;
    bool flagDefault;
    int numOfFramesToStopAfterDrag;
    string adbPortForwardCmd;
};

