/*
 *  GV__simpleObjectFindings.h
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GV__simpleObjectFindings_H
#define GV__simpleObjectFindings_H

#include <QGLContext>
#include "G__texture.h"
#include "GVfindings.h"
#include "gl.hpp"

class GV__simpleObjectFindings 
{
    
public:    
    
    GV__simpleObjectFindings ( GVfindings   *findings, QGLContext *cx);
    
    
    GLuint generateObject    ( int elementIndex, int level);
    GLuint makeBase ();
    
private:        
        
    G__texture     *tm; 
    GLuint      nodataT;

    GLuint      level1tex;
    GLuint      level2tex;
    GLuint      level3tex;
    GLuint      level4tex;


    GLuint      level1atex;
    GLuint      level2atex;
    GLuint      level3atex;
    GLuint      level1dtex;
    GLuint      level2dtex;
    GLuint      level3dtex;

    
        
    GVfindings   *findings;
    QGLContext      *cx;
    
    GLUquadricObj   *gq;
    
    void  generateBOX  (float xmin, float ymin, float zmin, float xmax, float ymax, float zmax,  QColor topcolor);
    void  generateBase (float height, GLuint textureID, bool borderHighlight); 
    
};
#endif
