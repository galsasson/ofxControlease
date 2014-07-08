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
    eventFunc = NULL;
}

ofxControleaseInput::ofxControleaseInput(std::string n, int *v)
{
    type = ControleaseType::INT;
    name = n;
    val = (void*)v;
    eventFunc = NULL;
}

ofxControleaseInput::ofxControleaseInput(std::string n, bool *v)
{
    type = ControleaseType::BOOL;
    name = n;
    val = (void*)v;
    eventFunc = NULL;
}

ofxControleaseInput::ofxControleaseInput(std::string n, unsigned char *v)
{
	type = ControleaseType::UCHAR;
	name = n;
	val = (void*)v;
	eventFunc = NULL;
}

ofxControleaseInput::ofxControleaseInput(std::string n, void (*func)(float))
{
    type = ControleaseType::FLOAT;
    name = n;
    val = NULL;
    eventFunc = func;
}

