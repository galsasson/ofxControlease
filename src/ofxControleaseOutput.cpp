//
//  ofxControleaseOutput.cpp
//  Crystalfilm
//
//  Created by Gal Sasson on 4/11/14.
//
//

#include "ofxControleaseOutput.h"

ofxControleaseOutput::ofxControleaseOutput(std::string n, float *v)
{
    type = ControleaseType::FLOAT;
    name = n;
    val = (void*)v;
    lastValSent = *v;
}

ofxControleaseOutput::ofxControleaseOutput(std::string n, int *v)
{
    type = ControleaseType::FLOAT;
    name = n;
    val = (void*)v;
    lastValSent = (int)(*v);
}

ofxControleaseOutput::ofxControleaseOutput(std::string n, bool *v)
{
    type = ControleaseType::FLOAT;
    name = n;
    val = (void*)v;
    lastValSent = (bool)(*v);
}
