/*
 *  GLmaster.h
 *  sequencepaint
 *
 *  Created by mue on 05.02.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GLmaster_H
#define GLmaster_H

#include <QGLWidget>
#include <QTimer>
#include <QTime>

#define GL_GLEXT_PROTOTYPES
#include "ogl/OglExt.h"

class GLmaster : public QGLWidget
{
    Q_OBJECT
    
public:
    
     GLmaster(QWidget *parent = 0);
    ~GLmaster();
            
    static QTime getTime();
    
    static void  fastTimer      ( QObject * receiver, const char * member );  
    static void  ultrafastTimer ( QObject * receiver, const char * member );  
    
    static void       disconnectFastTimer ( QObject * receiver, const char * member );  
    static void  disconnectUltraFastTimer ( QObject * receiver, const char * member );  
    
    static bool     glWasInitialized;

    static GLint    GLmaxTextureSize;
    static GLint    GLEnumberOfTextureUnits;
    static GLint    GLEnumberOfAuxBuffers;

    static bool     GLEblendColor;
    static bool     GLEmultiTexture;
    static bool     GLEimaging;
    static bool	    GLEtextureRectangle;
    static bool	    GLEtextureRectangleNV;
        
    static GLuint   GMA_2Dunit;
    static GLuint   GMA_2Dcross;
    

signals:

//    static void tickultrafast();             
//    static void tickfast();     
//    static void tickslow();     
    
protected:        
    void initializeGL();
private:
    
    static QTime   masterTime;    
    static QTimer *ultrafastTimerIntern;
    static QTimer *fastTimerIntern;
    static QTimer *slowTimerIntern;
};    

#endif
