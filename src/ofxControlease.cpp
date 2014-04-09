//
//  ofxControlease.cpp
//  drawingApp
//
//  Created by Gal Sasson on 3/25/14.
//
//

#include "ofxControlease.h"

ofxControlease::ofxControlease()
{
    bSetup = false;
}

ofxControlease::~ofxControlease()
{
    bRun = false;
    waitForThread();
}

void ofxControlease::setup(std::string name, int listenPort)
{
    programName = name;
    oscReceiver.setup(listenPort);
    
    bSetup = true;
    bRun = true;
    startThread();
}



void ofxControlease::update()
{
    if (!bSetup) {
        return;
    }
    
    ofxOscMessage msg;
    while (oscReceiver.hasWaitingMessages())
    {
        oscReceiver.getNextMessage(&msg);
        
        
        if (msg.getAddress() == "/ic") {
            // update variable
            int index = msg.getArgAsInt32(0);
            float val = msg.getArgAsFloat(1);
            if (index >= inputs.size()) {
                return;
            }
            
            *((float*)inputs[index]->val) = val;
        }
        else if (msg.getAddress() == "/hello") {
            handleHelloMessage(msg);
        }
        else if (msg.getAddress() == "/alive?") {
            handleAliveMessage(msg);
        }
    }
}

void ofxControlease::addInput(std::string name, float *val)
{
    if (!bSetup) {
        return;
    }
    
    inputs.push_back(new ofxControleaseInput(ofxControlease::FLOAT, name, (void*)val));
}

void ofxControlease::addInput(std::string name, int *val)
{
    if (!bSetup) {
        return;
    }
    
    inputs.push_back(new ofxControleaseInput(ofxControlease::INT, name, (void*)val));
}

void ofxControlease::threadedFunction()
{
    while (bRun) {
        update();
//        usleep(10);
    }
}

void ofxControlease::handleHelloMessage(ofxOscMessage &msg)
{
    std::string host = msg.getRemoteIp();//msg.getArgAsString(0);
    int remotePort = msg.getArgAsInt32(0);
    
    // setup oscSender
    oscSender.setup(host, remotePort);
}

void ofxControlease::handleAliveMessage(ofxOscMessage &msg)
{
    // send alive! message
    ofxOscMessage aliveMsg;
    aliveMsg.setAddress("/alive!");
    aliveMsg.addStringArg(programName);
    oscSender.sendMessage(aliveMsg);
    
    // send all inputs
    for (int i=0; i<inputs.size(); i++)
    {
        ofxControleaseInput *input = inputs[i];
        ofxOscMessage iMsg;
        iMsg.setAddress("/input_node");
        iMsg.addStringArg(input->name);
        iMsg.addStringArg("/ic");
        iMsg.addIntArg(i);
        iMsg.addIntArg((int)input->type);
        iMsg.addFloatArg(*(float*)input->val);
        oscSender.sendMessage(iMsg);
    }
    
    ofxOscMessage endMsg;
    endMsg.setAddress("/end_nodes");
    oscSender.sendMessage(endMsg);
}

