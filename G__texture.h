/*
 *  G__texture.h
 *  isoProjTest
 *
 *  Created by mue on 12.02.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef G__TEXTURE_H
#define G__TEXTURE_H

#include <QGLContext>
#include <QHash>

class G__texture 
{
    
public:    
    
    G__texture (QGLContext *cx);
   ~G__texture ();
    
    GLuint   generateTexture ( QString filename); 
    GLuint   getID           ( QString name); 
    void     freeTexture     ( QString name); 

private:        
    QGLContext   *GLcontext;

};
#endif
