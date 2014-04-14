//
//  ofxControleaseInput.cpp
//  Crystalfilm
//
//  Created by Gal Sasson on 4/11/14.
//
//

#include "ofxControleaseInput.h"

ofxControleaseInput::ofxControleaseInput(std::string n, float *v)
{
    type = ControleaseType::FLOAT;
    name = n;
    val = (void*)v;
}

ofxControleaseInput::ofxControleaseInput(std::string n, int *v)
{
    type = ControleaseType::INT;
    name = n;
    val = (void*)v;
}

ofxControleaseInput::ofxControleaseInput(std::string n, bool *v)
{
    type = ControleaseType::BOOL;
    name = n;
    val = (void*)v;
}

