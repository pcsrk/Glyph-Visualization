/*
 *  GVbasicElement.h
 *  isoProjTest
 *
 *  Created by mue on 08.05.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GVbasicElement_H
#define GVbasicElement_H


#include <QWidget>
#include <QGLPixelBuffer>

#include "GLmaster.h"

#include "GVfindingsObject.h"

class GVbasicElement 
{
    
    
public:
    
    GVbasicElement  ();    
  
    void    generateOrUpdateDL ();
    
	bool isSelected () {return fo->isSelected;}
	
	GVfindingsObject *fo;
    int               recordID;
    
    int     x;
    int     y;
    
    int     x_i;
    int     y_i;

	int     rc;

    int     w;
    int     h;

    int         x_togo;
    int         y_togo;

    signed char belongToGroup;

    bool    isContainer;
//    bool    isSelected;

	bool    pointReached;
    bool    positionFound;

	bool    isHighLighted;
    bool    pBufferNeedsUpdate;
    
    GLuint  object3dDl;
    GLuint  object2dTextureDl;
    GLuint  textureID;    
    GLuint  HRtextureID;    
    GLuint  updatePbufferDL;

protected:

};



#endif