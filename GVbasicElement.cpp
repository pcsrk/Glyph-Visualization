/*
 *  GVbasicElement.cpp
 *  isoProjTest
 *
 *  Created by mue on 08.05.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVbasicElement.h"

GVbasicElement::GVbasicElement  () 

{
//    GLcontext = cx;
    x = -1;
    y = -1;
    
    w = 1;
    h = 1;
    
    isContainer    = false;
//    isSelected     = false;
    isHighLighted  = false;

    
    pBufferNeedsUpdate = true;

    object3dDl         = 0;
    object2dTextureDl  = 0;
    updatePbufferDL    = 0;
    textureID          = 0;
    HRtextureID        = 0;

}    


void GVbasicElement::generateOrUpdateDL () {}

