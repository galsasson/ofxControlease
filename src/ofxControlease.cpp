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
    bOscSenderInitialized = false;
    bRun = false;
}

ofxControlease::~ofxControlease()
{
    bRun = false;
    cout<< "Waiting for ControlEase... ";
    waitForThread();
    cout<<"done"<<endl;
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
            
            ofxControleaseInput* input = inputs[index];
            if (input->val != NULL) {
				if (input->type == ControleaseType::FLOAT) {
                	*((float*)input->val) = val;
				}
				else if (input->type == ControleaseType::INT) {
					*((int*)input->val) = (int)val;
				}
				else if (input->type == ControleaseType::UCHAR) {
                	*((unsigned char*)input->val) = (unsigned char)val;
				}
				else if (input->type == ControleaseType::BOOL) {
					*((bool*)input->val) = (bool)!!val;
				}
            }
            if (input->eventFunc != NULL) {
                input->eventFunc(val);
            }
        }
        else if (msg.getAddress() == "/all") {
            if (msg.getNumArgs() != inputs.size()) {
                cout << "warning: got "<<msg.getNumArgs()<<" args, we have "<<inputs.size()<<" inputs"<<endl;
                continue;
            }
            
            for (int i=0; i<inputs.size(); i++)
            {
                *((float*)inputs[i]->val) = msg.getArgAsFloat(i);
            }
        }
        else if (msg.getAddress() == "/hello") {
            handleHelloMessage(msg);
        }
        else if (msg.getAddress() == "/alive?") {
            handleAliveMessage(msg);
        }
    }
}

void ofxControlease::sendOutputs()
{
    // update output values
    if (bOscSenderInitialized)
    {
        for (int i=0; i<outputs.size(); i++)
        {
            ofxControleaseOutput *out = outputs[i];
            //            if (out->hasChanged()) {
            ofxOscMessage oMsg;
            oMsg.setAddress("/oc");
            oMsg.addIntArg(i);
            oMsg.addFloatArg(*((float*)out->val));
            oscSender.sendMessage(oMsg);
            //                out->lastValSent = *(float*)out->val;
            //            }
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

void ofxControlease::addInput(std::string name, unsigned char *val)
{
	if (!bSetup) {
		return;
	}
	
	inputs.push_back(new ofxControleaseInput(name, val));
}

void ofxControlease::addInput(std::string name, ofVec2f *val)
{
    if (!bSetup) {
        return;
    }
    
    stringstream namex;
    namex << name << ".x";
    stringstream namey;
    namey << name << ".y";
    
    inputs.push_back(new ofxControleaseInput(namex.str(), &val->x));
    inputs.push_back(new ofxControleaseInput(namey.str(), &val->y));
}

void ofxControlease::addInput(std::string name, ofFloatColor *color)
{
    if (!bSetup) {
        return;
    }
    
    stringstream namer;
    namer << name << ".r";
    stringstream nameg;
    nameg << name << ".g";
    stringstream nameb;
    nameb << name << ".b";
    stringstream namea;
    namea << name << ".a";
    
    inputs.push_back(new ofxControleaseInput(namer.str(), &color->r));
    inputs.push_back(new ofxControleaseInput(nameg.str(), &color->g));
    inputs.push_back(new ofxControleaseInput(nameb.str(), &color->b));
    inputs.push_back(new ofxControleaseInput(namea.str(), &color->a));
}

void ofxControlease::addInput(std::string name, ofColor *color)
{
    if (!bSetup) {
        return;
    }
    
    stringstream namer;
    namer << name << ".r";
    stringstream nameg;
    nameg << name << ".g";
    stringstream nameb;
    nameb << name << ".b";
    stringstream namea;
    namea << name << ".a";
    
    inputs.push_back(new ofxControleaseInput(namer.str(), &color->r));
    inputs.push_back(new ofxControleaseInput(nameg.str(), &color->g));
    inputs.push_back(new ofxControleaseInput(nameb.str(), &color->b));
    inputs.push_back(new ofxControleaseInput(namea.str(), &color->a));
}

void ofxControlease::addInputEvent(std::string name, void (*event)(float))
{
    if (!bSetup) {
        return;
    }
    
    inputs.push_back(new ofxControleaseInput(name, event));
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
    bOscSenderInitialized = true;
    
}

void ofxControlease::handleAliveMessage(ofxOscMessage &msg)
{
	if (!bOscSenderInitialized) {
		ofLogError("ofxControlease", "oscSender not initialized");
		return;
	}

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
		if (input->type == ControleaseType::INT) {
			iMsg.addFloatArg(*(int*)input->val);
		}
		else if (input->type == ControleaseType::UCHAR) {
			iMsg.addFloatArg(*(unsigned char*)input->val);
		}
		else if (input->type == ControleaseType::BOOL) {
			iMsg.addFloatArg(*(bool*)input->val);
		}
		else {
			iMsg.addFloatArg(*(float*)input->val);
		}
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

