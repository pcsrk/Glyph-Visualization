/*
 *  G__texture.cpp
 *  isoProjTest
 *
 *  Created by mue on 12.02.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "G__texture.h"
#include <QImage>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

static    QHash<QString, GLuint> textureHash;

//------------------------------------------------------------------------------------
G__texture::G__texture (QGLContext *cx) {
    GLcontext = cx;
}


//------------------------------------------------------------------------------------
G__texture::~G__texture () {
    QHash<QString, GLuint>::const_iterator i = textureHash.constBegin();
    while (i != textureHash.constEnd()) {
        GLcontext->deleteTexture ( (GLuint)  i.value() );
    }
    textureHash.clear();
}

//------------------------------------------------------------------------------------
GLuint   G__texture::generateTexture(QString filename)  {

    if (textureHash.contains(filename)) {
         return textureHash.value(filename);
    }
    else {
        QImage textureImage = QImage (filename);
        // converting the pixels
        int i,j;
        int h =  textureImage.height();
        int w =  textureImage.width();
        int rbytes =  4 * w;
        unsigned char *dstp;
        unsigned char *dst = (unsigned char *)malloc (w * h * 4);
        
        dstp = dst;
        
        for (i=h-1; i>=0;i--) {
            QRgb* srcp = (QRgb*)textureImage.scanLine(i);
            for (j=0; j<w;j++) {
                *dstp++ =  qRed   (*srcp);
                *dstp++ =  qGreen (*srcp);
                *dstp++ =  qBlue  (*srcp);
                *dstp++ =  qAlpha (*srcp);
                srcp++;
            }
        }
        
		GLcontext->makeCurrent();
		
        GLuint textureID;    
        glGenTextures( 1, &textureID );        
        glBindTexture( GL_TEXTURE_2D,  textureID);  
        
        int err = gluBuild2DMipmaps(GL_TEXTURE_2D,
                              4,
                              textureImage.width(),
                              textureImage.height(),
                              GL_RGBA,
                              GL_UNSIGNED_BYTE,
                              dst);

        free (dst);
        textureHash.insert(filename, textureID);
        return textureID;
    }
}

//------------------------------------------------------------------------------------
GLuint   G__texture::getID          (QString name)      {
    if (textureHash.contains(name)) {
        return textureHash.value(name);
    }
    else
        return 0;
}

//------------------------------------------------------------------------------------
void  G__texture::freeTexture     ( QString name)
{
    if (textureHash.contains(name)) {
          GLuint textureID    = textureHash.value(name);
          GLcontext->deleteTexture ( (GLuint) textureID );
          textureHash.remove(name);
    }
}
