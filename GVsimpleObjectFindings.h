/*
 *  GVsimpleObjectFindings.h
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GVsimpleObjectFindings_H
#define GVsimpleObjectFindings_H


#include <QWidget>
#include <QImage>
#include "GVbasicElement.h"
#include "GVfindings.h"
#include "GVsettings.h"
#include "GV__simpleObjectFindings.h"

#define QT_OPENGL_ES_2
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFramebufferObject>



class GVsimpleObjectFindings : public GVbasicElement 
{
    
    
public:
    
    
    GVsimpleObjectFindings  (  GVsettings    *settings,   QGLWidget *shareWidget, GVfindings   *findings, QGLContext *cx);
    
    void generateOrUpdateObject (int level );
    
    
    static GLuint           defaulttextureID;

private :
  
  
    GVsettings       *_settings;
    
    static QHash<int, QOpenGLFramebufferObject*> openGLFramebufferObjectList;
    static QOpenGLContext     *sfmlContext;
    static QOffscreenSurface *offscreenSurface;
    
    static QGLWidget  *gmaster;
	static QGLContext *GLcontext;
    static QHash<int, QGLPixelBuffer*> pixelbufferList;

    
    static     GVfindings   *findings;
    static int count;
    static GV__simpleObjectFindings *soDLgenerator;
    
    static GLuint           baseDL;
    static QHash<QString, GLuint>  textureHash;
    static QHash<QString, GLuint>  objectHash;

    
};


#endif