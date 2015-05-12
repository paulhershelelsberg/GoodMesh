#include "ofApp.h"
#include <iostream>
using namespace std;
//-----------------------------------------------------------------------------------------
//
void ofApp::setup()
{
    fval = 1;
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
    gui.setup();
    gui.add(fSlide.setup("multiplier", 1, 0, 10));
    fSlide.addListener(this, &ofApp::fSlideChanged);
    //camera orientation data
    x = -104.688;
    y = 4.33332;
    z = -2.34788e-06;
    
    //camera position data
        //outer perspective
        x2 = -0.228592;
        y2 = 32.3089;
        z2 = -4.47262;
    
        //inner perspective
//        x2 = -0.00958237;
//        y2 = 3.55096;
//        z2 = -0.137034;
    
	// Give us a starting point for the camera
	camera.setNearClip(0.01f);
	camera.setMovementMaxSpeed( 0.1f );

    //I'd like to move this into update in the hopes of added camera motion but adjusting at correct rates is tricky is there a way I can override the 's' and 'w' keys to get the motion I want
    camera.setPosition(ofVec3f(x2,y2,z2));
    camera.setOrientation(ofVec3f(x,y,z));

    //set width and height of plane (playing with these creates crazy effects) possibly integrate into gui
        w = 10;
        h = 10;
    //i represents how many meshes we have in our vector (could also have chosen an array)
    for(int i = 0; i < 12;i++)
    {
        //add mesh to mesh vector
        mesh.push_back(tempMesh);
    
   
        for (int x = 0; x < w ; x++)
        {
            for (int y = 0; y < h ; y++)
            {
                    mesh[i].addVertex(ofPoint(x,y,0)); //create a vertex for every possible point
            }
        }
        //w-1 and h-1 because the bottom and far right indeces account for the largest w and h
        for (int x = 0; x < w-1 ; x++)
        {
            for (int y = 0; y < h-1 ; y++)
            {
                int ind1 = x + w * y; // top left point
                int ind2 = x + 1 + w * y; //top right point
                int ind3 = x  + w   * (y+1);//bottom left point
                int ind4 = (x + 1) + w   * (y+1);//bottom right point
                
                //together the triangles create a rectangle
                mesh[i].addTriangle(ind1, ind3, ind2);
                mesh[i].addTriangle(ind2, ind3, ind4);

            }
        }
        //?still a bit fuzzy on setting normals
        setNormals(tempMesh);
    }
    
	
}

//-----------------------------------------------------------------------------------------
//
void ofApp::update()
{
    
    
    //testing perspectives
//    cout<<camera.getOrientationEuler()<<endl;
    cout<<camera.getPosition()<<endl;

    float time = ofGetElapsedTimef();
    for(int i = 0; i < 12;i++)
    {
        for (int x = 0; x < w ; x++)
        {
            for (int y = 0; y < h ; y++)
            {
                
                    int id = x + w * y;//create id for every vertex
                    ofVec3f p = mesh[i].getVertex(id);//make a point that we can influence
                
                    p.z = ofMap(ofNoise(sin(x)*fval,sin(y)*fval,time),0,1,-6,5);//noise movement
//                    p.z = cos(x+y*time)+sin(x+y*time);//wavey movement
                //create point independent color
                    float colVal = ofMap(ofNoise(sin(x),sin(y),time),0,1,0,255);
                    mesh[i].setVertex(id, p);//reenter changing vertices into mesh
                
                    mesh[i].addColor(ofColor(ofRandom(255), colVal, ofRandom(255)));//create vertex color
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
//
void ofApp::draw()
{
    gui.draw();
	ofBackgroundGradient( ofColor(40,40,40), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);	
	
	ofEnableDepthTest();
	
	camera.begin();
	
    //janky arranging
    ofRotate(180, 1, 1, 0);
    for(int i = 0;i<12;i++)
    {
        ofPushMatrix();
        //kaleidoscope meshes
        ofRotate(i*30, -1, 0, 0);
        ofTranslate(-5, -10);
        mesh[i].draw();//draw finished work
        ofPopMatrix();
    }

	camera.end();

	ofSetColor( ofColor::white );
	ofDisableDepthTest();


	fontSmall.drawStringShadowed(ofToString(ofGetFrameRate(),2), ofGetWidth()-35, ofGetHeight() - 6, ofColor::whiteSmoke, ofColor::black );
}


//-----------------------------------------------------------------------------------------//
//Universal function which sets normals for the triangle mesh
void ofApp::setNormals( ofMesh &mesh ){
    
    //The number of the vertices
    int nV = mesh.getNumVertices();
    
    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;
    
    vector<ofPoint> norm( nV ); //Array for the normals
    
    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t=0; t<nT; t++) {
        
        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );
        
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );
        
        //Compute the triangle's normal
        ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
        
        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }
    
    //Normalize the normal's length
    for (int i=0; i<nV; i++) {
        norm[i].normalize();
    }
    
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals( norm );
}



void ofApp::fSlideChanged(float & val)
{
    fval=val;
}

void ofApp::keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}
