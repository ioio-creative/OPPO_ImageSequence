

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//File format for the example frames is
	//frame01.png 
	//this creates a method call where the parameters
	//prefix is frame, file type is png, from frame 1 to 11, 2 digits in the number

	//sequence.loadSequence("frame", "png", 1, 11, 2);
	//sequence.preloadAllFrames();	//this way there is no stutter when loading frames
    adbPortForwardCmd = "/usr/local/Cellar/android-platform-tools/25.0.3/bin/adb forward tcp:12580 tcp:10086";
	system(adbPortForwardCmd.c_str());
	
	connected = mobileController.setup("127.0.0.1",12580);

    bool isThreadedLoadImage = true;
    
	//ofSetVerticalSync(true);
	//ofSetWindowPosition(-1080, 0);
	sequenceA.enableThreadedLoad(isThreadedLoadImage);
	sequenceA.setExtension("png");
	sequenceA.loadSequence("frameM");
	
	sequenceB.enableThreadedLoad(isThreadedLoadImage);
	sequenceB.setExtension("png");
	sequenceB.loadSequence("frameN");


	playing = true ; //controls if playing automatically, or controlled by the mouse
    flagDefault = false;
	ofSetFrameRate(24);
	speed = -0.000223;
    couDefault = 0;
    speedDefault = -0.000223;  // 1 / number of images in one image sequence folder
    accFactor = 0.006;
    dragSpeed = 2000;
    numOfFramesToStopAfterDrag = 10;
    for(int i = 0; i<11; i++){
        acc[i] = (0-(accFactor/2))+(i*accFactor/11);
    }
    ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::update() {
	
	if (mobileController.isConnected()) {
		if (connectedFlag == false) {
			ofLog(OF_LOG_NOTICE, "CONNECTED!");
			connectedFlag = true;
		}
		ctrSpeed = mobileController.receive();
		if (ctrSpeed == "" && speed == 0) {
            if(couDefault <= numOfFramesToStopAfterDrag){
                couDefault ++;
            }
            if(couDefault > numOfFramesToStopAfterDrag && flagDefault == true){
                speed = speedDefault;
                flagDefault = false;
            }
		}
		if (ctrSpeed != "") {
			vector<string> splitedCtrSpeed = ofSplitString(ctrSpeed," ");
			ofLog(OF_LOG_NOTICE, "raw:" + ctrSpeed);
			
			if (splitedCtrSpeed[0] == "d") {
                dSmooth ++;
                //ofLog(OF_LOG_NOTICE, "ddd:" + ofToString(dSmooth));
                if(dSmooth > 5){
                    if(speed>0.0001){
                        speed *= 0.5;
                    }else{
                    //this following part for determing drag movement
                    speed = 0;
                    couDefault = 0;
                    flagDefault = true;
                    move = ofToFloat(splitedCtrSpeed[1])/dragSpeed;
                    percent += move;
                    ofLog(OF_LOG_NOTICE, "f:" + ofToString(move));
                    
                    }
                }
				

			}
			else if (splitedCtrSpeed[0] == "s") {
				// this following part for determining speed
                dSmooth = 0;
				ctrSpeed = splitedCtrSpeed[1];
               
                if(ctrSpeed == "10"){
                    if(speed < acc[10]){
                        speed = acc[10];
                    }
                }else if(ctrSpeed == "9"){
                    if(speed < acc[9]){
                        speed = acc[9];
                    }
                }else if(ctrSpeed == "8"){
                    if(speed < acc[8]){
                        speed = acc[8];
                    }
                }else if(ctrSpeed == "7"){
                    if(speed < acc[7]){
                        speed = acc[7];
                    }
                }else if(ctrSpeed == "6"){
                    if(speed < acc[6]){
                        speed = acc[6];
                    }
                }else if(ctrSpeed == "4"){
                    if(speed > acc[4]){
                        speed = acc[4];
                    }
                }else if(ctrSpeed == "3"){
                    if(speed > acc[3]){
                        speed = acc[3];
                    }
                }else if(ctrSpeed == "2"){
                    if(speed > acc[2]){
                        speed = acc[2];
                    }
                }else if(ctrSpeed == "1"){
                    if(speed > acc[1]){
                        speed = acc[1];
                    }
                }else if(ctrSpeed == "0"){
                    if(speed > acc[0]){
                        speed = acc[0];
                    }
                }
			}
			
			
			
		}
	}else {
		ofLog(OF_LOG_ERROR, "NO CONNECTION!");
        mobileController.close();
        system(adbPortForwardCmd.c_str());
        try {
            connected = mobileController.setup("127.0.0.1",12580);
        } catch (exception ex) {
            
        }
	}
	
}

//--------------------------------------------------------------
void ofApp::draw() {

	if (sequenceA.isLoading() || sequenceB.isLoading()) {
		ofBackground(20, 0, 0);
	}
	else {
		ofBackground(0);
		if (playing) {
			//get the frame based on the current time and draw it
			if (speed > 0.00016) {
				speed = speed * 0.973;
				if (speed < 0.0002) {
					speed = speedDefault;
				}
			}

			if (speed < 0) {
				speed *= 0.973;
				if (speed> -0.0002) {
					speed = speedDefault;
				}
			}
			percent += speed;
            //to loop the png seq from end to beginning
			if (percent>1.0) {
				percent = 0;
				seqA = !seqA;
			}
            if (percent < 0.0) {
                percent = 1;
                seqA = !seqA;
            }
			if (seqA == false) {
				sequenceB.getTextureForPercent(percent).draw(0, 0, 1920, 1080);
			}else if (seqA == true) {
				sequenceA.getTextureForPercent(percent).draw(0, 0, 1920, 1080);
			}



			//sequence.getFrameForTime(ofGetElapsedTimef()* speed)->draw(0,0, 1080, 1920);
		}
		else {
			//get the sequence frame that maps to the mouseX position

			percent = ofMap(mouseY, 0, ofGetHeight(), 0, 1.0, true);
			//draw it.
			sequenceA.getFrameAtPercent(percent)->draw(0, 0, 1920, 1080);
		}
	}

	ofSetColor(255, 255, 255);
	//ofDrawBitmapString(ofGetFrameRate(), 100, 100);
	//ofDrawBitmapString(speed, 100, 150);
    //ofDrawBitmapString(sequenceA.getCurrentFrame(), 100, 200);
    //ofDrawBitmapString(sequenceB.getCurrentFrame(), 100, 250);
    
	ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//hit any key to toggle playing


	switch (key) {
	case 'a':
		playing = !playing;
		
		break;
	case 's':

        break;
	case 'z':
		speed = -0.004;

		break;
	case 'x':
		speed = -0.007;
		break;
	case 'c':
		speed = 0.007;
		break;
	case 'v':
		speed = 0.004;
		break;


	}
}


