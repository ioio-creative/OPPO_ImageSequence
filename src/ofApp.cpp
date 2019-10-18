#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	if (isLogToConsole) {
		ofLogToConsole();
	}
	ofSetVerticalSync(true);
	ofDisableArbTex();

	sequenceFbo.allocate(TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_RGB);
	sequenceFbo.begin();
	glClearColor(0,0,0,255);
	glClear(GL_COLOR_BUFFER_BIT);
	sequenceFbo.end();
	sequenceFbo.resetAnchor();
	sequenceFbo.setAnchorPercent(0.5, 0.5);

	guiSetup();
	
	//File format for the example frames is
	//frame01.png 
	//this creates a method call where the parameters
	//prefix is frame, file type is png, from frame 1 to 11, 2 digits in the number

	//sequence.loadSequence("frame", "png", 1, 11, 2);
	//sequence.preloadAllFrames();	//this way there is no stutter when loading frames

	connectToMobile();

	//ofSetVerticalSync(true);
	//ofSetWindowPosition(-1080, 0);

	sequenceA.setIsClearPixelsAfterLoadingTexture(isClearPixelsAfterLoadingTexture);
	sequenceB.setIsClearPixelsAfterLoadingTexture(isClearPixelsAfterLoadingTexture);

#if defined(TARGET_WIN32)
	sequenceA.enableThreadedLoad(isThreadedLoadImage);
	sequenceA.setExtension(imgExtenstion);
	sequenceA.loadSequence("frameU");

	sequenceB.enableThreadedLoad(isThreadedLoadImage);
	sequenceB.setExtension(imgExtenstion);
	sequenceB.loadSequence("frameV");
#elif defined (TARGET_OSX)
	sequenceA.loadSequence("frameO/poriffshop_video_smoke_", imgExtenstion, 0, 1499, 5);
	sequenceB.loadSequence("frameP/poriffshop_video_smoke_", imgExtenstion, 1500, 2999, 5);
#endif

	ofSetFrameRate(frameRate);
	speed = minSpeed;

	//int accSize = size(acc);
	//for (int i = 0; i < accSize; i++) {
	//	//acc[i] = (i / accSize - 0.5) * accFactor;
	//	acc[i] = ofMap(i, 0, accSize-1, -1, 1) * accFactor;
	//	if (!forwardUpward)
	//	{
	//		acc[i] *= -1;
	//	} 
	//	if(drawGui) ofLogNotice(ofToString(acc[i]));
	//}

	if (isToggleFullScreen) {
		ofToggleFullscreen();
	}
	ofBackground(10);
}

//--------------------------------------------------------------
void ofApp::update() {
	float _speed = 0;

	if (mobileController.isConnected()) {
				
		if (!connectedFlag) {
			ofLogNotice("CONNECTED!");
			connectedFlag = true;
		}

		ctrSpeed = mobileController.receive();
		/*controllerThread.lock();
		ctrSpeed = controllerThread.ctrSpeed;
		controllerThread.unlock();*/
		if (ctrSpeed == "") {
			//ofLogNotice("empty string");
			if ((isDragging && speed == 0))
			{
				if (couDefault <= numOfFramesToStopAfterDrag) {
					couDefault++;
				}
				if (couDefault > numOfFramesToStopAfterDrag) {
					speed = minSpeed;
					isDragging = false;
				}
			}


		}

		if (ctrSpeed != "") {
						
			vector<string> splitedCtrSpeed = ofSplitString(ctrSpeed, " ");
			if (drawGui) ofLogNotice("raw:" + ctrSpeed);

			if (splitedCtrSpeed[0] == "d") {
				dSmooth++;
				//ofLogNotice("ddd:" + ofToString(dSmooth));
				if (dSmooth > dragFrameThreshold) {					
					//this following part for determing drag movement
					speed = 0;
					couDefault = 0;
					isDragging = true;
					float ctrlSpeed = forwardUpward ? ofToFloat(splitedCtrSpeed[1]) : -ofToFloat(splitedCtrSpeed[1]);
					move = ctrlSpeed * dragSpeedMultiplier;
					if (seqA)
					{
						percent += move / sequenceA.getTotalFrames();
					}
					else
					{
						percent += move / sequenceB.getTotalFrames();
					}
					//ofLogNotice("d:" + ofToString(move));
					
				}
			}
			else if (splitedCtrSpeed[0] == "s") {
				// this following part for determining speed
				isDragging = false;
				dSmooth = 0;
				ctrSpeed = splitedCtrSpeed[1];
				
				_speed = forwardUpward ? std::stoi(ctrSpeed) : -std::stoi(ctrSpeed);
				_speed *= slideMultiplier;

				//_ctrSpeed = ofClamp(_ctrSpeed, 0, (int)size(acc) - 1);
				//ofLogNotice("ctrSpeed: " + ofToString(_ctrSpeed));
								
				//ofLogNotice("sliding speed: " + ofToString(speed));
			}
		}
		
	}
	else {
		ofLogError("NO CONNECTION!");
#if IS_RECONNECT_TO_MOBILE
		if (!drawGui)
		{
			connectToMobileIfTimeoutInUpdate();
		}
#endif
	}

	if (playing) {
		//get the frame based on the current time and draw it
		speed *= 0.75;

		if (abs(_speed) >= abs(speed))
		{
			speed = _speed;
		}

		if (!isDragging)
		{
			if (abs(speed) < abs(minSpeed)) {
				speed = minSpeed;
			}
		}

		float deltaFrame = speed / ofGetFrameRate();

		ofxImageSequence sequenceToUse = seqA ? sequenceA : sequenceB;
		percent += deltaFrame / sequenceToUse.getTotalFrames();


		//to loop the png seq from end to beginning
		if (percent > 1.0) {
			percent = 0;
			seqA = !seqA;
		}
		else if (percent < 0.0) {
			percent = 1;
			seqA = !seqA;
		}



		//ofLogNotice("speed: " + ofToString(speed));
	}
	else {
		//get the sequence frame that maps to the mouseX position
		percent = ofMap(mouseY, 0, ofGetHeight(), 0, 1.0, true);


	}

	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (sequenceA.isLoading() || sequenceB.isLoading()) {
		ofBackground(20, 0, 0);
	}
	else {
		ofBackground(0);

		ofxImageSequence sequenceToUse = seqA ? sequenceA : sequenceB;

		drawTex = &sequenceToUse.getTextureForPercent(percent);
		
		//draw it.
		sequenceFbo.begin();
		ofClear(0);
		drawTex->draw(0, 0, sequenceFbo.getWidth(), sequenceFbo.getHeight());
		sequenceFbo.end();

		
	}
	sequenceFbo.draw(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT/2, TEXTURE_WIDTH*FBO_SCALE, TEXTURE_HEIGHT * FBO_SCALE);
	
	if (drawGui)
	{
		//ofSetColor(0, 0, 255);
		//ofDrawBitmapString(ofGetFrameRate(), 100, 100);
		//ofDrawBitmapString(speed, 100, 150);
		//ofDrawBitmapString(sequenceA.getCurrentFrame(), 100, 200);
		//ofDrawBitmapString(sequenceB.getCurrentFrame(), 100, 250);
		gui.draw();
	}
	ofSetColor(255, 255, 255);
}

void ofApp::exit() {
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
			speed = -1000;
			break;
		case 'x':
			speed = -2000;
			break;
		case 'c':
			speed = 2000;
			break;
		case 'v':
			speed = 1000;
			break;
		case 'f':
			ofToggleFullscreen();
		case 'd':
			drawGui = !drawGui;
	}
}


void ofApp::connectToMobile() {
	system(adbPortForwardCmd.c_str());
	connected = mobileController.setup(mobileServerIp, mobileServerPort, false);
}

void ofApp::connectToMobileIfTimeoutInUpdate() {
	deltaTime = ofGetElapsedTimeMillis() - connectTime;
	if (deltaTime > reconnectTimeMillis) {
		connectToMobile();
		connectTime = ofGetElapsedTimeMillis();
	}	
}

void ofApp::guiSetup() {
	gui.setup();
	speedParameters.setName("Speed Params");
	speedParameters.add(minSpeed.set("Min. Speed", MinSpeed, -60, 60));
	speedParameters.add(dragSpeedMultiplier.set("Drag Speed x", DragSpeedMultiplier, 0, 10));
	speedParameters.add(slideMultiplier.set("Slide Speed x", SlideMultiplier, 0, 20));
	speedParameters.add(dragFrameThreshold.set("Drag Threshold", 3, 0, 5));
	gui.add(speedParameters);
}