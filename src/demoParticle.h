#pragma once
#include "ofMain.h"

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE
};

class demoParticle{
    
public:
    demoParticle();
    
    void setMode(particleMode newMode);
    void setAttractPoints( vector <ofPoint> * attract );
    
    void reset();
    void update(ofPoint centerOfFace, vector <ofPoint> attractPoints);
    void draw();
    
    ofPoint pos;
    ofPoint vel;
    ofPoint frc;
    ofFloatColor color;
    
    float drag;
    float uniqueVal;
    float scale;
    
    particleMode mode;
    
    vector <ofPoint> * attractPoints;
};
