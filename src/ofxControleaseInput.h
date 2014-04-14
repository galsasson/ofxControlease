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
    
    ofxControleaseInput(std::string n, float *v);
    ofxControleaseInput(std::string n, int *v);
    ofxControleaseInput(std::string n, bool *v);
};


#endif /* defined(__Crystalfilm__ofxControleaseInput__) */
