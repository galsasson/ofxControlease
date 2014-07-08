//
//  ofxControlease.h
//  drawingApp
//
//  Created by Gal Sasson on 3/25/14.
//
//

#ifndef __ofxControlease__
#define __ofxControlease__

#include <iostream>
#include <sstream>
#include "ofMain.h"
#include "ofxOsc.h"

#include "ControleaseType.h"
#include "ofxControleaseInput.h"
#include "ofxControleaseOutput.h"

class ofxControlease : public ofThread
{
public:
    ofxControlease();
    ~ofxControlease();
    
    void setup(std::string name, int listenPort);
    void update();
    
    void addInput(std::string name, float* val);
    void addInput(std::string name, int* val);
    void addInput(std::string name, bool* val);
	void addInput(std::string name, unsigned char *val);
    void addInput(std::string name, ofVec2f* val);
    void addInput(std::string name, ofFloatColor* color);
    void addInput(std::string name, ofColor* color);
    void addInputEvent(std::string name, void (*event)(float));
    
    void addOutput(std::string name, float* val);
    void addOutput(std::string name, int* val);
    void addOutput(std::string name, bool* val);
    
    void sendOutputs();

private:
    void handleHelloMessage(ofxOscMessage &msg);
    void handleAliveMessage(ofxOscMessage &msg);
    void threadedFunction();
    
    bool bSetup;
    bool bRun;
    bool bOscSenderInitialized;
    
    std::string programName;
    vector<ofxControleaseInput*> inputs;
    vector<ofxControleaseOutput*> outputs;
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;

};


#endif /* defined(__ofxControlease__) */
