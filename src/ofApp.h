#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "Math/MathUtils.h"

#include "Utils/ofTrueTypeFontExt.h"
#include "Utils/Cameras/ofxWalkingFirstPersonCamera.h"

class ofApp : public ofBaseApp
{
	public:
		
		void setup();
		void update();
		void draw();
		void setNormals(ofMesh &mesh);
    void fSlideChanged(float & val);

		void keyPressed(int key);
	
		ofxFirstPersonCamera	camera;
	
		ofTrueTypeFontExt		fontSmall;
    
    vector<ofMesh> mesh;
    ofMesh tempMesh;
    ofxPanel gui;
    ofxVec3Slider o;
    int x,y,z,x2,y2,z2;
    int w;
    int h;
    ofxFloatSlider fSlide;
    float fval;
};
