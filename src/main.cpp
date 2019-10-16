#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	ofAppGLFWWindow window;

	// setup the GL context
	//ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);
	ofSetupOpenGL(&window, 540, 750, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
