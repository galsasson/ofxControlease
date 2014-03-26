//
//  ofxControlease.h
//  drawingApp
//
//  Created by Gal Sasson on 3/25/14.
//
//

#ifndef __drawingApp__ofxControlease__
#define __drawingApp__ofxControlease__

#include <iostream>
#include "ofMain.h"
#include "ofxOsc.h"

class ofxControleaseInput;

class ofxControlease : public ofThread
{
public:
    ofxControlease();
    ~ofxControlease();
    
    void setup(std::string name, int listenPort);
    void update();
    
    void addInput(std::string name, float* val);
    void addInput(std::string name, int* val);
    

    typedef enum ControleaseType_t {
        FLOAT = 0,
        INT = 1,
        BOOL = 2
    } Type;
    
    

private:
    void handleAliveMessage(ofxOscMessage &msg);
    void threadedFunction();
    
    bool bSetup;
    bool bRun;
    
    std::string programName;
    vector<ofxControleaseInput*> inputs;
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;

};


class ofxControleaseInput
{
public:
    
    ofxControlease::Type type;
    std::string name;
    void *val;
    
    ofxControleaseInput(ofxControlease::Type t, std::string n, void *v) { type = t; name = n; val = v; }
};


#endif /* defined(__drawingApp__Controlease__) */
