/*
 *  GVelementThreeStateDistribution.h
 *  isoProjTest
 *
 *  Created by mue on 19.06.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVelementThreeStateDistribution_H
#define GVelementThreeStateDistribution_H


#include <QWidget>
#include "GVbasicElement.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

class GVelementThreeStateDistribution : public GVbasicElement 
{
    
    
public:
    
    GVelementThreeStateDistribution  (QGLContext *cx);    
    
    void generateOrUpdateElement ( GLUquadricObj* gq, double v1,  double v2, double v3, float transparency);
    
private :
  
};



#endif