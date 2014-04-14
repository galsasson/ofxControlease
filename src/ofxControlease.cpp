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
    
    // update output values
    for (int i=0; i<outputs.size(); i++)
    {
        ofxControleaseOutput *out = outputs[i];
        if (out->hasChanged()) {
            ofxOscMessage oMsg;
            oMsg.setAddress("/oc");
            oMsg.addIntArg(i);
            oMsg.addFloatArg(*(float*)out->val);
            oscSender.sendMessage(oMsg);
            out->lastValSent = *(float*)out->val;
        }
    }
}


void ofxControlease::addInput(std::string name, float *val)
{
    if (!bSetup) {
        return;
    }
    
    inputs.push_back(new ofxControleaseInput(name, val));
}

void ofxControlease::addInput(std::string name, int *val)
{
    if (!bSetup) {
        return;
    }
    
    inputs.push_back(new ofxControleaseInput(name, val));
}

void ofxControlease::addInput(std::string name, bool *val)
{
    if (!bSetup) {
        return;
    }
    
    inputs.push_back(new ofxControleaseInput(name, val));    
}

void ofxControlease::addOutput(std::string name, float *val)
{
    if (!bSetup) {
        return;
    }
    
    outputs.push_back(new ofxControleaseOutput(name, val));
}

void ofxControlease::addOutput(std::string name, int *val)
{
    if (!bSetup) {
        return;
    }
    
    outputs.push_back(new ofxControleaseOutput(name, val));
}

void ofxControlease::addOutput(std::string name, bool *val)
{
    if (!bSetup) {
        return;
    }
    
    outputs.push_back(new ofxControleaseOutput(name, val));
}


void ofxControlease::threadedFunction()
{
    while (bRun) {
        update();
        usleep(10000);
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
    
    for (int i=0; i<outputs.size(); i++)
    {
        ofxControleaseOutput *output = outputs[i];
        ofxOscMessage oMsg;
        oMsg.setAddress("/output_node");
        oMsg.addStringArg(output->name);
        oMsg.addIntArg(i);
        oMsg.addIntArg((int)output->type);
        oMsg.addFloatArg(*(float*)output->val);
        oscSender.sendMessage(oMsg);
    }
    
    ofxOscMessage endMsg;
    endMsg.setAddress("/end_nodes");
    oscSender.sendMessage(endMsg);
}

