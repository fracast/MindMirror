#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"

#include "ofxFaceTracker.h"
#include "demoParticle.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void resetParticles();
    
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
    
    particleMode currentMode;
    string currentModeStr;
    
    vector <demoParticle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;
    
    ofPoint centerOfFace;
    ofPoint centerOfRightEye;
    ofPoint centerOfLeftEye;
    
    //ofxCvColorImage colorImage;
    ofPixels pixels;
    
    bool		bSendSerialMessage;			// a flag for sending serial
    char		bytesRead[3];				// data from serial, we will be trying to read 3
    char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
    int			nBytesRead;					// how much did we read?
    int			nTimesRead;					// how many times did we read?
    float		readTime;					// when did we last read?
    ofSerial	serial;
    string message;
    
    float att;
    float med;
    
    vector <string> parsedMsg;
};
