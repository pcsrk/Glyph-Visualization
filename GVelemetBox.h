/*
 *  GVelemetBox.h
 *  isoProjTest
 *
 *  Created by mue on 07.06.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GVelemetBox_H
#define GVelemetBox_H


#include <QWidget>
#include "GVbasicElement.h"

class GVelemetBox : public GVbasicElement 
{
    
    
public:
    
    GVelemetBox  (QGLContext *cx);    
    
    void generateOrUpdateBox (int w, int h, int z, float transparency);

private :
    

};



#endif