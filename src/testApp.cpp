#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    for (int j=0; j<NUM_STRIP; j++)
    {
        pos[j].set(ofRandom(0, ofGetWidth()),ofRandom(0,ofGetHeight()),0);
        vec[j].set(0,0,0);
        acc[j].set(ofRandom(-1,1),ofRandom(-1,1),0);
        age[j] = 1;
        float h = ofRandom(200,255);
        for (int i=0; i<LENGTH; i++)
        {
            int index = i+(j*LENGTH);
            strip[index].set(ofGetWidth()*0.5,ofGetHeight()*0.5,0);
            float brightness = sinf(PI*float((i*0.5)*1.f/LENGTH*0.5f))*255;
            color[index].set(ofColor::fromHsb(h,255, 255,brightness));
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
    count = 0;
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    float t = (ofGetElapsedTimef()) * 0.9f;
    float div = 250.0;
    
    for (int j=0; j<NUM_STRIP; j++)
    {
        //
        
        
        ofVec3f _vec(ofSignedNoise(t, pos[j].y/div, pos[j].z/div),
                    ofSignedNoise(pos[j].x/div, t, pos[j].z/div),
                    0);
        _vec *=  ofGetLastFrameTime()*50;
//        vec.x+=ofGetLastFrameTime()*1000;
        vec[j]+=_vec;
        vec[j]+=acc[j];
        vec[j]*=0.9;
        if(pos[j].y<-200)
        {
            age[j] = 1;
            pos[j].y = ofGetHeight();
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        else if(pos[j].x<-200)
        {
                        age[j] = 1;
            pos[j].x = ofGetWidth();
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        else if(pos[j].y>ofGetHeight()+200)
        {
                        age[j] = 1;
            pos[j].y = 0;
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        else if(pos[j].x>ofGetWidth()+200)
        {
                        age[j] = 1;
            pos[j].x = 0;
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        else if(age[j]>0)
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
                

                radius = sinf(PI*float(i*1.f/LOC_LENGTH))*5;
                {
                    ofVec3f perp0 = loc[index] - loc[index+1];
                    ofVec3f perp1 = perp0.getCrossed( ofVec3f( 0, 1, 0 ) ).getNormalized();
                    ofVec3f perp2 = perp0.getCrossed( perp1 ).getNormalized();
                    perp1 = perp0.getCrossed( perp2 ).getNormalized();
                    Off.x        = perp1.x * radius*age[j];// * agePer * per * .05;
                    Off.y       = perp1.y * radius*age[j];// * agePer * per * .05;
                    Off.z        = perp1.z * radius*age[j];// * agePer * per * .05;
                    
                    strip[(index2)]=loc[index]-Off;
                    
                    strip[(index2+1)]=loc[index]+Off;
                }
            }
            loc[j*LOC_LENGTH] = pos[j];
            pos[j]+=vec[j];
            age[j]-=0.01;
        }
        else
        {
            for (int i=0; i<LOC_LENGTH; i++)
            {
                int index = i+(j*LOC_LENGTH);
                loc[index].set(pos[j]);
            }
        }
        
        
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    vbo.bind();
	vbo.updateVertexData(strip, total);
	vbo.updateColorData(color, total);
    
    
    for (int j=0; j<NUM_STRIP; j++)
        
    {
        int index = j * LENGTH;
        
        vbo.draw(GL_TRIANGLE_STRIP, index,LENGTH);
        
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
                    
                }
                else
                {
                    ofSetColor(0, 0, 255);
                    ofCircle(strip[index] , 2);
                }
            }
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
    count++;
    
    count%=NUM_STRIP;
    pos[count].set(x+ofRandom(-50,50), y+ofRandom(-50,50));
    vec[count].set(x-pMouse.x,y-pMouse.y,0);
    vec[count]*=2;
    acc[count].set((x-pos[count].x)*0.01, (y-pos[count].y)*0.01);
    age[count] = 1;
//    float h = 255*ofRandom(0,1);
    for (int i=0; i<LOC_LENGTH; i++)
    {
        int index = i+(count*LOC_LENGTH);
        loc[index].set(pos[count]);
        
//        float brightness = ((LOC_LENGTH-i*1.0)/LOC_LENGTH)*255;
//        color[index].set(ofColor::fromHsb(h,255, 255,brightness));
    }
    pMouse.set(x,y);

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