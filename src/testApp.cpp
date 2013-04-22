#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    for (int j=0; j<NUM_STRIP; j++)
    {
        pos[j].set(ofRandom(0, ofGetWidth()),ofRandom(0,ofGetHeight()),0);
        
        for (int i=0; i<LENGTH; i++)
        {
            int index = i+(j*LENGTH);
            strip[index].set(ofGetWidth()*0.5,ofGetHeight()*0.5,0);
            float brightness = sinf(PI*float((i*0.5)*1.f/LENGTH*0.5f))*255;
            color[index].set(ofColor::fromHsb(brightness,0, 255,brightness));
        }
        
        for (int i=0; i<LOC_LENGTH; i++)
        {
            int index = i+(j*LOC_LENGTH);
            loc[index].set(0,0,0);
        }
        
        
    }
    total = NUM_STRIP*LENGTH;
    vbo.setVertexData(strip, total, GL_DYNAMIC_DRAW);
	vbo.setColorData(color, total, GL_DYNAMIC_DRAW);
    //	vbo.setTexCoordData(tex_coord, total, GL_DYNAMIC_DRAW);

}

//--------------------------------------------------------------
void testApp::update(){
    
    float t = (ofGetElapsedTimef()) * 0.9f;
    float div = 250.0;
    
    for (int j=0; j<NUM_STRIP; j++)
    {
        //
        
        
        ofVec3f vec(0,//ofSignedNoise(t, pos[j].y/div, pos[j].z/div),
                    ofSignedNoise(pos[j].x/div, t, pos[j].z/div),
                    ofSignedNoise(pos[j].x/div, pos[j].y/div, t));
        vec *=  ofGetLastFrameTime()*1000;
        vec.x+=ofGetLastFrameTime()*1000;
        
        if(pos[j].y<-10)
        {
            pos[j].y = ofGetHeight();
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        else if(pos[j].x<-10)
        {
            
            pos[j].x = ofGetWidth();
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        else if(pos[j].y>ofGetHeight()+10)
        {
            pos[j].y = 0;
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        else if(pos[j].x>ofGetWidth()+10)
        {
            pos[j].x = 0;
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        else
        {
            ofVec3f Off;
            float radius = 10;
            for (int i=LOC_LENGTH-1; i>=1; i--)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(loc[index-1]);
            }
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                int index2 = (i*2)+(j*LENGTH);
//                loc[index].set(loc[index-1]);s
                //
//                if(i>1)
                radius = sinf(PI*float(i*1.f/LOC_LENGTH))*5;
                {
                    ofVec3f perp0 = loc[index] - loc[index+1];
                    ofVec3f perp1 = perp0.getCrossed( ofVec3f( 0, 1, 0 ) ).getNormalized();
                    ofVec3f perp2 = perp0.getCrossed( perp1 ).getNormalized();
                    perp1 = perp0.getCrossed( perp2 ).getNormalized();
                    Off.x        = perp1.x * radius;// * agePer * per * .05;
                    Off.y       = perp1.y * radius;// * agePer * per * .05;
                    Off.z        = perp1.z * radius;// * agePer * per * .05;
                    
                    strip[(index2)]=loc[index]-Off;
                    
                    strip[(index2+1)]=loc[index]+Off;
                }
                //                }
                //                if(i==1)
                //                {
                //                    strip[j*LENGTH]=loc[index];
                ////                    strip[j*LENGTH-1]=loc[index];
                //                }
                //                else if(i==LOC_LENGTH-1)
                //                {
                //                    strip[(j*LENGTH)+(index2+1)]=loc[index];
                //                }
                
                
            }
        }
        loc[j*LOC_LENGTH] = pos[j];
        pos[j]+=vec;
//        pos[j].x += vec.x;
//        pos[j].y += vec.y;
        
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    vbo.bind();
	vbo.updateVertexData(strip, total);
	vbo.updateColorData(color, total);
//	vbo.updateTexCoordData(tex_coord, total);

    for (int j=0; j<NUM_STRIP; j++)

    {
        int index = j * LENGTH;

        vbo.draw(GL_TRIANGLE_STRIP, index,LENGTH);
//        vbo.draw(GL_LINE_STRIP, index,LENGTH);
    }

	vbo.unbind();
    

    if(ofGetLogLevel()==OF_LOG_VERBOSE)
    {
    for (int j=0; j<NUM_STRIP; j++)
    {
        ofSetColor(255);
        ofCircle(pos[j] , 10);
        for (int i=LENGTH-1; i>=0; i--)
        {
            int index = i+(j*LENGTH);
            if(i%2==0)
            {
                ofSetColor(255, 0, 0);
                ofCircle(strip[index] , 2);
//                ofDrawBitmapString(ofToString(i), strip[index]);
            }
            else
            {
                ofSetColor(0, 0, 255);
                ofCircle(strip[index] , 2);
//                ofDrawBitmapString(ofToString(i), strip[index]);                
            }
        }
        //        for (int i=LOC_LENGTH-1; i>=1; i--)
        //        {
        //            int index = i+(j*LOC_LENGTH);
        //
        //            ofCircle(loc[index] , 2);
        //        }
        
        
    }
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key)
    {
        case 'd':
            if(ofGetLogLevel()!=OF_LOG_VERBOSE)
            {
                ofSetLogLevel(OF_LOG_VERBOSE);
            }
            else{
                ofSetLogLevel(OF_LOG_NOTICE);
            }
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}