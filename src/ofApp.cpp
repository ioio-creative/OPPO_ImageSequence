#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	if (isLogToConsole) {
		ofLogToConsole();
	}

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

	int accSize = size(acc);
	for (int i = 0; i < accSize; i++) {
		//acc[i] = (i / accSize - 0.5) * accFactor;
		acc[i] = ofMap(i, 0, accSize-1, -1, 1) * accFactor;
		if (!forwardUpward)
		{
			acc[i] *= -1;
		}
		ofLogNotice(ofToString(acc[i]));
	}

	if (isToggleFullScreen) {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void ofApp::update() {
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
			ofLogNotice("raw:" + ctrSpeed);

			if (splitedCtrSpeed[0] == "d") {
				dSmooth++;
				//ofLogNotice("ddd:" + ofToString(dSmooth));
				if (dSmooth > 5) {					
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
				
				speed = forwardUpward ? std::stoi(ctrSpeed) : -std::stoi(ctrSpeed);

				//_ctrSpeed = ofClamp(_ctrSpeed, 0, (int)size(acc) - 1);
				//ofLogNotice("ctrSpeed: " + ofToString(_ctrSpeed));
				

				//if (speed * acc[_ctrSpeed] < 0 )
				//{					
				//	//momentarily stop when changing sliding direction
				//	speed = 0;
				//}
				//else
				//{
				//	speed = abs(speed) > abs(acc[_ctrSpeed]) ? speed : acc[_ctrSpeed];
				//}

				/*if (ctrSpeed == "10") {
						speed = acc[10];
				}
				else if (ctrSpeed == "9") {
						speed = acc[9];
				}
				else if (ctrSpeed == "8") {
						speed = acc[8];
				}
				else if (ctrSpeed == "7") {
						speed = acc[7];
				}
				else if (ctrSpeed == "6") {
						speed = acc[6];
				}
				else if (ctrSpeed == "4") {
						speed = acc[4];
				}
				else if (ctrSpeed == "3") {
						speed = acc[3];
				}
				else if (ctrSpeed == "2") {
						speed = acc[2];
				}
				else if (ctrSpeed == "1") {
						speed = acc[1];
				}
				else if (ctrSpeed == "0") {
						speed = acc[0];
				}*/
				//ofLogNotice("sliding speed: " + ofToString(speed));
			}
		}

		if (playing) {
			//get the frame based on the current time and draw it
			speed *= 0.7;

			if (!isDragging)
			{
				if (abs(speed) < abs(minSpeed)) {
					speed = minSpeed;
				}
			}

			float deltaFrame = speed / (float)frameRate * speedMultiplier;
			if (seqA)
			{
				percent += deltaFrame / sequenceA.getTotalFrames();
			}
			else
			{
				percent += deltaFrame / sequenceB.getTotalFrames();
			}

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
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (sequenceA.isLoading() || sequenceB.isLoading()) {
		ofBackground(20, 0, 0);
	}
	else {
		ofBackground(0);

		if (seqA) {
			drawTex = &sequenceA.getTextureForPercent(percent);
		}
		else {
			drawTex = &sequenceB.getTextureForPercent(percent);
		}
		//draw it.
		drawTex->draw(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}

	//ofSetColor(255, 255, 255);
	//ofDrawBitmapString(ofGetFrameRate(), 100, 100);
	//ofDrawBitmapString(speed, 100, 150);
	//ofDrawBitmapString(sequenceA.getCurrentFrame(), 100, 200);
	//ofDrawBitmapString(sequenceB.getCurrentFrame(), 100, 250);

	ofSetColor(255, 255, 255);

	if (drawGui)
	{
		gui.draw();
	}
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
			speed = -100;
			break;
		case 'x':
			speed = -180;
			break;
		case 'c':
			speed = 180;
			break;
		case 'v':
			speed = 100;
			break;

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
	speedParameters.add(speedMultiplier.set("Speed x", SpeedMultiplier, 0, 10));
	gui.add(speedParameters);
}