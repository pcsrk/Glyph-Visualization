/*
 *  GLmaster.cpp
 *  sequencepaint
 *
 *  Created by mue on 05.02.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GLmaster.h"
#include  "QObject"


//------------------------------------------------------------------------------------
GLmaster::GLmaster(QWidget *parent) : QGLWidget(QGLFormat( QGL::DepthBuffer | QGL::AlphaChannel ), parent) {
    
    if (!glWasInitialized) {
        
        makeCurrent();
        //------------------------------------------------------------------------------------
        // GL extensions 
        //
        glGetIntegerv(GL_MAX_TEXTURE_SIZE,  &GLmaxTextureSize);
        glGetIntegerv(GL_MAX_TEXTURE_UNITS, &GLEnumberOfTextureUnits);
        glGetIntegerv(GL_AUX_BUFFERS,       &GLEnumberOfAuxBuffers);
        
       // qDebug ("GL_MAX_TEXTURE_SIZE  = %d", GLmaxTextureSize );
       // qDebug ("GL_MAX_TEXTURE_UNITS = %d", GLEnumberOfTextureUnits );
       // qDebug ("GL_AUX_BUFFERS       = %d", GLEnumberOfAuxBuffers   );
        
        GLEblendColor         =     glexExtensionsSupported ("GL_EXT_blend_color");
        GLEimaging            =     glexExtensionsSupported ("GL_ARB_imaging");
        GLEmultiTexture       =     glexExtensionsSupported ("GL_ARB_multitexture");
        
        GLEtextureRectangle   =     glexExtensionsSupported ("GL_ARB_texture_rectangle");
        GLEtextureRectangleNV =     glexExtensionsSupported ("GL_NV_texture_rectangle");
        
        if (GLEblendColor)          qDebug ("GLmaster:: constAlphaBlending  = enabled");  
        else                        qDebug ("GLmaster:: constAlphaBlending  = disabled");  
        
        if (GLEmultiTexture)        qDebug ("GLmaster:: GLEmultiTexture     = enabled");  
        else                        qDebug ("GLmaster:: GLEmultiTexture     = disabled");  
        
        if (GLEtextureRectangleNV)  qDebug ("GLmaster:: GLEtextureRectangle = enabled");  
        else                        qDebug ("GLmaster:: GLEtextureRectangle = disabled");  
        
        if (GLEimaging)             qDebug ("GLmaster:: imaging             = enabled");  
        else                        qDebug ("GLmaster:: imaging             = disabled");  
        
        //------------------------------------------------------------------------------------
        // common display list for debugging
        //
        GMA_2Dunit = glGenLists(1);  glNewList(GMA_2Dunit, GL_COMPILE); 
        {
            glDisable     ( GL_TEXTURE_2D);  glDisable     ( GL_BLEND);
            glColor3f(0.5, 0.0, 0.0);
            glBegin(GL_LINE_STRIP);
            { glVertex3d   (0, 0, 0); glVertex3d   (0, 1, 0); glVertex3d   (1, 1, 0); glVertex3d   (1, 0, 0); glVertex3d   (0, 0, 0);  }
            glEnd();    
        }
        glEndList();
        //------------------------------------------------------------------------------------    
        GMA_2Dcross = glGenLists(1); glNewList(GMA_2Dcross, GL_COMPILE); 
        {
            glDisable     ( GL_TEXTURE_2D);  glDisable     ( GL_BLEND);
            glBegin(GL_LINES);
            {   glColor3f(1.0, 0.0, 0.0);  glVertex3d   (0, 0, 0);  glVertex3d   (1, 1, 0);	
            glColor3f(0.0, 1.0, 0.0);  glVertex3d   (1, 0, 0);	glVertex3d   (0, 1, 0); }
            glEnd();    
        }
        glEndList();
        //------------------------------------------------------------------------------------
        
        
        //------------------------------------------------------------------------------------
        // timers and clock
        //
        // the ultrafast is 100 Hz
        ultrafastTimerIntern = new QTimer(this);
        //    connect(ultrafastTimer, SIGNAL(timeout()), this, SIGNAL(tickultrafast()));
        ultrafastTimerIntern->start(17);

        fastTimerIntern = new QTimer(this);
        //    connect(fastTimer, SIGNAL(timeout()), this, SIGNAL(tickfast()));
        fastTimerIntern->start(34);
        
        // and the slow on 4 frmaes per second
        slowTimerIntern = new QTimer(this);
        //    connect(slowTimer, SIGNAL(timeout()), this, SIGNAL(tickslow()));
        slowTimerIntern->start(250);
        
        masterTime.start();
        
        

        
    }
    glWasInitialized = true;
}

//------------------------------------------------------------------------------------
void GLmaster::fastTimer ( QObject * receiver, const char * member )  
{
    connect(fastTimerIntern, SIGNAL(timeout()), receiver, member);
}

//------------------------------------------------------------------------------------
void  GLmaster::ultrafastTimer ( QObject * receiver, const char * member )  
{
    connect(ultrafastTimerIntern, SIGNAL(timeout()), receiver, member);
}

//------------------------------------------------------------------------------------
void GLmaster::disconnectFastTimer ( QObject * receiver, const char * member )  
{
    disconnect(fastTimerIntern, SIGNAL(timeout()), receiver, member);
}

//------------------------------------------------------------------------------------
void GLmaster::disconnectUltraFastTimer ( QObject * receiver, const char * member )  
{
    disconnect(ultrafastTimerIntern, SIGNAL(timeout()), receiver, member);
}


//------------------------------------------------------------------------------------
GLmaster::~GLmaster() {
}

//------------------------------------------------------------------------------------
QTime GLmaster::getTime() {
    return masterTime.currentTime();
}

//------------------------------------------------------------------------------------
void GLmaster::initializeGL() {
    qDebug("GLmaster:: open GL is ready");   
}

bool     GLmaster::glWasInitialized = false;


GLint    GLmaster::GLmaxTextureSize          = 0;
GLint    GLmaster::GLEnumberOfTextureUnits   = 0;
GLint    GLmaster::GLEnumberOfAuxBuffers     = 0;

bool     GLmaster::GLEblendColor             = false;
bool     GLmaster::GLEmultiTexture           = false;
bool     GLmaster::GLEimaging                = false;
bool	 GLmaster::GLEtextureRectangle       = false;
bool	 GLmaster::GLEtextureRectangleNV     = false;

GLuint   GLmaster::GMA_2Dunit   = 0;
GLuint   GLmaster::GMA_2Dcross  = 0;

QTime    GLmaster::masterTime;    
QTimer *GLmaster::ultrafastTimerIntern = 0;
QTimer *GLmaster::fastTimerIntern      = 0;
QTimer *GLmaster::slowTimerIntern      = 0;

