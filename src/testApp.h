#pragma once

#include "ofMain.h"
#define NUM_STRIP 1000
#define LOC_LENGTH 10
#define LENGTH LOC_LENGTH*2

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		ofVbo vbo;
    ofVec3f pos[NUM_STRIP];
    ofVec3f acc[NUM_STRIP];
        ofVec3f vec[NUM_STRIP];
    float age[NUM_STRIP];
    
    
    ofVec3f strip[NUM_STRIP*LENGTH];
    ofVec3f loc[NUM_STRIP*LOC_LENGTH];
//	ofVec2f tex_coord[NUM_STRIP*LENGTH];
	ofFloatColor color[NUM_STRIP*LENGTH];
    int total;
//    ofEasyCam cam;
        int count;
    ofVec3f pMouse;
};
