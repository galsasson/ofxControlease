//
//  ofxControleaseOutput.h
//  Crystalfilm
//
//  Created by Gal Sasson on 4/11/14.
//
//

#ifndef __Crystalfilm__ofxControleaseOutput__
#define __Crystalfilm__ofxControleaseOutput__

#include <iostream>

#include "ControleaseType.h"

class ofxControleaseOutput
{
public:
    ControleaseType::Type type;
    std::string name;
    void *val;
    float lastValSent;
    
    ofxControleaseOutput(std::string n, float *v);
    ofxControleaseOutput(std::string n, int *v);
    ofxControleaseOutput(std::string n, bool *v);
    
    bool hasChanged() {return (lastValSent != *(float*)(val)); }
};



#endif /* defined(__Crystalfilm__ofxControleaseOutput__) */
