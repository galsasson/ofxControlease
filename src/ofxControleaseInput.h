//
//  ofxControleaseInput.h
//  Crystalfilm
//
//  Created by Gal Sasson on 4/11/14.
//
//

#ifndef __Crystalfilm__ofxControleaseInput__
#define __Crystalfilm__ofxControleaseInput__

#include <iostream>
#include "ControleaseType.h"

class ofxControleaseInput
{
public:
    
    ControleaseType::Type type;
    std::string name;
    void *val;
    void (*eventFunc)(float);
    
    ofxControleaseInput(std::string n, float *v);
    ofxControleaseInput(std::string n, int *v);
    ofxControleaseInput(std::string n, bool *v);
    ofxControleaseInput(std::string n, unsigned char *v);
    ofxControleaseInput(std::string n, void (*func)(float));
};


#endif /* defined(__Crystalfilm__ofxControleaseInput__) */
