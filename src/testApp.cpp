#include "testApp.h"
#include "Poco/RegularExpression.h"

using Poco::RegularExpression;


vector < string > getMatchedStrings (string stringToParse, string regex );
vector < string > getMatchedStrings (string contents, string regex, int & lastPos ){
    
    vector < string > results;
    RegularExpression regEx(regex);
    RegularExpression::Match match;
    
    lastPos = 0;
    while(regEx.match(contents, match) != 0) {
        
        // we get the sub string from the content
        // and then trim the content so that we
        // can continue to search
        string foundStr = contents.substr(match.offset, match.length);
        contents = contents.substr(match.offset + match.length);
        
        
        lastPos += match.offset + match.length;
        
        results.push_back(foundStr);
        
    }
    return results;
}

using namespace ofxCv;

void testApp::setup() {
    ofBackground(0);
	ofSetVerticalSync(true);
	ofSetFrameRate(24);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	cam.initGrabber(640, 480);
    
	tracker.setup();
	tracker.setRescale(.5);
    
	int num = 10000;
	p.assign(num, demoParticle());
	currentMode = PARTICLE_MODE_NEAREST_POINTS;
	resetParticles();
    
	centerOfFace.set(0,0);
	centerOfRightEye.set(0,0);
	centerOfLeftEye.set(0,0);
    
//--------
    //serial setup
    bSendSerialMessage = false;
	ofBackground(100);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	//serial.setup(5, 9600); //open the first device
	//serial.setup("COM4"); // windows example
	serial.setup("/dev/tty.usbmodemfd131",9600); // mac osx example
	//serial.setup("/dev/ttyUSB0", 9600); //linux example
	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);
}

void testApp::resetParticles(){
    
	//these are the attraction points used in the forth demo
	attractPoints.clear();
	for(int i = 0; i < 3; i++){
		attractPoints.push_back( ofPoint(0,0) );
	}
    
	attractPointsWithMovement = attractPoints;
    
	for(int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}
}

void testApp::update() {
    int howMany = serial.available();
    
    if (howMany > 0){
        
        unsigned char bytes[howMany];
        serial.readBytes(bytes, howMany);
        //cout << "numBytes recvd: " << ofToString(howMany) << endl;
        
        for (int i = 0; i < howMany; i++){
            message += (bytes[i]);
        }
        
        //cout << message << endl;
        int foundHash = 0;
        foundHash = message.find("#");
        
        if (foundHash > 0){
            cout << message << endl;
            parsedMsg = ofSplitString(message, ",");
            
            
            att = ofToFloat(parsedMsg[1]);
            med = ofToFloat(parsedMsg[2]);
            
            cout << "att: "<< att << "  med: " << med << endl;
            
            //            cout << "Signal: " << parsedMsg[0] << endl;
            //            cout << "Attention: " << parsedMsg[1] << endl;
            //            cout << "Meditation: " << parsedMsg[2] << endl;
            //            cout << "Delta: " << parsedMsg[3] << endl;
            //            cout << "Theta: " << parsedMsg[4] << endl;
            //            cout << "Low Alpha: " << parsedMsg[5] << endl;
            //            cout << "High Alpha: " << parsedMsg[6] << endl;
            //            cout << "Low Beta: " << parsedMsg[7] << endl;
            //            cout << "High Beta: " << parsedMsg[8] << endl;
            //            cout << "Low Gamma: " << parsedMsg[9] << endl;
            //            cout << "High Gamma: " << parsedMsg[10] << endl;
            //
            //            cout << "---------" << endl;
            message.clear();
            
        }
        
        howMany = 0;
    }
    
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
		ofTexture _text;
		_text = cam.getTextureReference() ;
		_text.readToPixels(pixels);
        
	}
    
    for(int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].update(centerOfFace, attractPoints);
		p[i].color.set( pixels.getColor(ofMap(p[i].pos.x, 0, ofGetWindowWidth(), 0, 640), ofMap( p[i].pos.y, 0, ofGetWindowHeight(), 0, 480)) );
	}
}

void testApp::draw() {
    ofBackground(0, 0, 0);
	ofSetColor(255);
	//cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    
    ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
    ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
    ofPolyline faceOutline = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
    
    if(faceOutline.size()>0){
        centerOfFace.set(faceOutline.getCentroid2D());
        centerOfRightEye.set(rightEye.getCentroid2D());
        centerOfLeftEye.set(leftEye.getCentroid2D());
        attractPoints[0] = ( centerOfFace );
        attractPoints[1] = ( centerOfRightEye );
        attractPoints[2] = ( centerOfLeftEye );
    }
    
    for(int i = 0; i < p.size(); i++){
		p[i].draw();
	}
    
    ofSetColor(190);
    //needed only for nearest point mode
	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
		for(int i = 0; i < attractPoints.size(); i++){
			ofNoFill();
			ofCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofCircle(attractPointsWithMovement[i], 4);
		}
	}
    
//	ofSetLineWidth(2);
//	ofSetColor(ofColor::red);
//	leftEye.draw();
//	ofSetColor(ofColor::green);
//	rightEye.draw();
//	ofSetColor(ofColor::blue);
//	faceOutline.draw();
//    
//	ofSetLineWidth(1);
//	ofSetColor(255);
//	tracker.draw();
    
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
    

}
