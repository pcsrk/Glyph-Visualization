/*
 *  GVsimpleObjectFindings.cpp
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <QDebug>
#include "GVsimpleObjectFindings.h"
//#include <QDebug>

/*
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
*/

GVsimpleObjectFindings::GVsimpleObjectFindings  (  GVsettings *settings, QGLWidget *shareWidget, GVfindings   *findings, QGLContext *cx) : GVbasicElement ()    
{

    _settings = 	settings;			
	
    
    if (!soDLgenerator) {
    
        qDebug() << "MAKE A GENERATOR";
		this->gmaster = shareWidget;
		this->GLcontext = cx;  
        this->findings = findings;
		
        soDLgenerator = new GV__simpleObjectFindings(findings, cx);

        QSurfaceFormat surfaceFormat;
        surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);
        
        offscreenSurface = new QOffscreenSurface();
        offscreenSurface->setFormat(surfaceFormat);
        
        sfmlContext = new QOpenGLContext();

        if(sfmlContext->create() == false)
            qDebug()<<Q_FUNC_INFO<<"(1) Shit happens with QOpenGLContext create";
        
        
        if(sfmlContext->makeCurrent(offscreenSurface) == false)
            qDebug()<<Q_FUNC_INFO<<"(1) Shit happens with QOpenGLContext makeCurrent";
        
        
        QOpenGLFramebufferObjectFormat fboFormat;
        fboFormat.setAttachment(QOpenGLFramebufferObject::NoAttachment);

        openGLFramebufferObjectList[16] =new QOpenGLFramebufferObject(16, 32, fboFormat);
        openGLFramebufferObjectList[32] = new QOpenGLFramebufferObject(32, 64, fboFormat);
        openGLFramebufferObjectList[64] = new QOpenGLFramebufferObject(64, 128, fboFormat);
        openGLFramebufferObjectList[128] = new QOpenGLFramebufferObject(128, 256, fboFormat);
        openGLFramebufferObjectList[256] = new QOpenGLFramebufferObject(256, 512, fboFormat);
        openGLFramebufferObjectList[512] = new QOpenGLFramebufferObject(512, 1024, fboFormat);

        sfmlContext->makeCurrent(0);
    }
    textureID = defaulttextureID;
    object3dDl = 0;
    this->w = 1;
    this->h = 1; 
    this->recordID = 0;
}


void GVsimpleObjectFindings::generateOrUpdateObject (int level)
{     
    
    bool generate = false;
    
    if ((level >  16)) 
        generate = true;
    
    if ((level ==  16) && (textureID != 0)) 
        generate = true;
    
    
    generate = true;
    if ( generate) {
        
	
        // generate a hash ID with all attributes were visu = visu2attribute
        
        QString hashID = QString::number(level);
        
        foreach (QString visuAttr, GVfindingsObject::visu2attribute) {
            hashID += "-";
            hashID += findings->el[recordID]->value[visuAttr];
        }
        
        if (textureHash.contains(hashID)) {
			textureID  = textureHash[hashID];			
			object3dDl = objectHash[hashID];
        }
        else
        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            
            if(sfmlContext->makeCurrent(offscreenSurface) == false)
                qDebug()<<Q_FUNC_INFO<<"(2)  Shit happens with QOpenGLContext makeCurrent";
            
            QOpenGLFramebufferObject *fbo = openGLFramebufferObjectList[level];
            
            QOpenGLFramebufferObjectFormat fboFormat;
            fboFormat.setAttachment(QOpenGLFramebufferObject::NoAttachment);
            
            //fbo = new QOpenGLFramebufferObject(level, 2*level, fboFormat);
            
            if(fbo->bind() == false)
                qDebug()<<Q_FUNC_INFO<<"Shit happens with QOpenGLFramebufferObject bind";
            
            
            glEnable    ( GL_AUTO_NORMAL);
            glEnable    ( GL_NORMALIZE);
            
            glEnable    ( GL_CULL_FACE);
            glEnable    ( GL_BLEND);
            glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable    ( GL_LINE_SMOOTH);
            
            glEnable     ( GL_COLOR_MATERIAL ) ;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            
            float xm = 1.0;
            float ym = 1.0;
            
            //			RR_CONFIG
            
            
            glEnable(GL_LIGHTING);
            glEnable( GL_LIGHT0 );
            glEnable( GL_LIGHT1 );
            glEnable( GL_LIGHT2 );
            
            float lc = 0.1;
            
            GLfloat diffuse_light0[]  = { 5*lc,5*lc,5*lc, 1.0f };
            GLfloat position_light0[] = { 0.0f, 4.0f, 0.0f, 1.0f };
            glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light0 );
            glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );
            
            GLfloat diffuse_light1[]  =  { 3*lc,3*lc,3*lc, 1.0f };
            GLfloat position_light1[] = { -3.0f, 0.0f, 2.0f, 1.0f };
            glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse_light1 );
            glLightfv( GL_LIGHT1, GL_POSITION, position_light1 );
            
            GLfloat diffuse_light2[]  =  { 2*lc,2*lc,2*lc, 1.0f };
            GLfloat position_light2[] = { 5.5f, 0.0f, 2.0f, 1.0f };
            glLightfv( GL_LIGHT2, GL_DIFFUSE, diffuse_light2 );
            glLightfv( GL_LIGHT2, GL_POSITION, position_light2 );
            
            GLfloat ambient_lightModel[] =  { 5*lc,5*lc,5*lc, 1.0f };
            glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );
            
            
            glClearColor(0.0, 1.0, 0.0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glEnable        ( GL_DEPTH_TEST);
            
            glViewport(0, 0, this->openGLFramebufferObjectList[level]->size().width(), this->openGLFramebufferObjectList[level]->size().height());
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, 1, 0, 1, -5, 5);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glScaled    (1.0, 0.5, 1.0 );
            glScaled    (0.70710681, 0.70710681, 1.0);
            
            glTranslated(0.70710681, 0, 0.0);
            
            glRotated (-45, 1.0, 0.0, 0.0);
            glRotated (45,  0.0, 0.0, 1.0);
            
            
            if (object3dDl) glDeleteLists(object3dDl,1);
            object3dDl =  soDLgenerator->generateObject (recordID,  level);
            
            glCallList(object3dDl);
            QImage QOpenGLFramebufferObjectFormatImage = this->openGLFramebufferObjectList[level]->toImage();
            if(QOpenGLFramebufferObject::bindDefault() == false)
                qDebug()<<Q_FUNC_INFO<<"Shit happens with QOpenGLFramebufferObject unbind";
            
            sfmlContext->makeCurrent(0);
            GLcontext->makeCurrent();

            glPopMatrix();
            glPopAttrib();
            

//            QString name = "yy_" + hashID + ".png";
//            QOpenGLFramebufferObjectFormatImage.save (name, "png");
//            qDebug()<< "SAVE <- " <<name;
            
            QImage *textureImage = &QOpenGLFramebufferObjectFormatImage;
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            
            glEnable            ( GL_TEXTURE_2D);
            glGenTextures        (  1, &textureID );
            glBindTexture        (  GL_TEXTURE_2D,  textureID);  
            int h =  textureImage->height();
            int w =  textureImage->width();
            int rbytes =  4 * w;
            unsigned char *dstp;
            unsigned char *dst = (unsigned char *)malloc (w * h * 4);
            
            dstp = dst;
   			
			if (_settings->flipPBuffer) {
				for (int i=0; i<h;i++) {
					
					QRgb* srcp = (QRgb*)textureImage->scanLine(i);
					for (int j=0; j<w;j++) {
						*dstp++ =  qRed   (*srcp);
						*dstp++ =  qGreen (*srcp);
						*dstp++ =  qBlue  (*srcp);
						*dstp++ =  qAlpha (*srcp);
						srcp++;
					}
				}
				
			}
			else
			{
				for (int i=h-1; i>=0;i--) {
					
					QRgb* srcp = (QRgb*)textureImage->scanLine(i);
					for (int j=0; j<w;j++) {
						*dstp++ =  qRed   (*srcp);
						*dstp++ =  qGreen (*srcp);
						*dstp++ =  qBlue  (*srcp);
						*dstp++ =  qAlpha (*srcp);
						srcp++;
					}
				}
				
			}
			
            int err = gluBuild2DMipmaps(GL_TEXTURE_2D,
                                        4,
                                        textureImage->width(),
                                        textureImage->height(),
                                        GL_RGBA,
                                        GL_UNSIGNED_BYTE,
                                        dst);
            
            
            free (dst);
            
            glPopMatrix();
            glPopAttrib();
//            qDebug ("*** NEW Object created %d WITH 2D texture, textureID = %d", count++, textureID);
        }        
        textureHash[hashID] = textureID;
		objectHash[hashID]  = object3dDl;  		
    }
    
    if (level > 16)
        HRtextureID = textureID;
   
}



int GVsimpleObjectFindings::count = 0;

GV__simpleObjectFindings    *GVsimpleObjectFindings::soDLgenerator = 0;

QHash<int, QGLPixelBuffer*> GVsimpleObjectFindings::pixelbufferList;
QHash<QString, GLuint>      GVsimpleObjectFindings::textureHash;
QHash<QString, GLuint>      GVsimpleObjectFindings::objectHash;


QHash<int, QOpenGLFramebufferObject*> GVsimpleObjectFindings::openGLFramebufferObjectList;
QOpenGLContext*  GVsimpleObjectFindings::sfmlContext;
QOffscreenSurface* GVsimpleObjectFindings::offscreenSurface;


GVfindings   *GVsimpleObjectFindings::findings = 0;
QGLWidget  *GVsimpleObjectFindings::gmaster   = 0;      
QGLContext *GVsimpleObjectFindings::GLcontext = 0;      

GLuint           GVsimpleObjectFindings::baseDL           = 0;
GLuint           GVsimpleObjectFindings::defaulttextureID = 0;


