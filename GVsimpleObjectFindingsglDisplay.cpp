/*
 *  GVsimpleObjectFindingsglDisplay.cpp
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVsimpleObjectFindingsglDisplay.h"


#include <QRegExp>

GVsimpleObjectFindingsglDisplay::GVsimpleObjectFindingsglDisplay (QWidget *parent, QGLWidget *shareWidget, GVfindings   *findings) 
: QGLWidget(QGLFormat( QGL::DepthBuffer | QGL::AlphaChannel ), parent, shareWidget)
{
    
    this->findings =  findings;
    om = 0;    
    
    scale   = 1.0;
    isx     = 0.0; 
    isy     = 0.0;
    
    
    elementID      = 0;
    currentElement = 0;
    bgr = 0.7;
    bgg = 0.7;
    bgb = 0.7;
    bga = 1.0;
    
    showRaster = true;
    
    xrot = 0;
    zrot = 0;
    
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void GVsimpleObjectFindingsglDisplay::setScale(float s)
{
    scale   = s;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void GVsimpleObjectFindingsglDisplay::setIsoShift(float x, float y)
{
    isx = x; 
    isy = y; 
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void GVsimpleObjectFindingsglDisplay::setShowRaster ( bool v)
{
    showRaster = v;
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
QSize GVsimpleObjectFindingsglDisplay::minimumSizeHint() const
{
    return QSize(50, 50);
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

QSize GVsimpleObjectFindingsglDisplay::sizeHint() const
{
    return QSize(512, 512);
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void GVsimpleObjectFindingsglDisplay::initLights( void )
{
    float xm = 1.0;
    float ym = 1.0;
    
    
    glEnable( GL_LIGHT0 );
 //   glEnable( GL_LIGHT1 );
 //   glEnable( GL_LIGHT2 );
    
    
    GLfloat diffuse_light0[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position_light0[] = { 0, 4.0, 0.0, 1.0f };
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light0 );
    glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );
    
     GLfloat diffuse_light1[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat position_light1[] = { -3.0, 0.0, 4.0, 1.0f };
    glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse_light1 );
    glLightfv( GL_LIGHT1, GL_POSITION, position_light1 );
    
     GLfloat diffuse_light2[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat position_light2[] = { 0.5, 0.5, 8.0, 1.0f };
    glLightfv( GL_LIGHT2, GL_DIFFUSE, diffuse_light2 );
    glLightfv( GL_LIGHT2, GL_POSITION, position_light2 );
    
    GLfloat ambient_lightModel[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void GVsimpleObjectFindingsglDisplay::initializeGL()
{
    
    GLint TMUs, ABs;
    int i,j;
    
    QGLFormat glFormat = format();
    
    
    qDebug ("INIT OF GVsimpleObjectFindingsglDisplay");
    qDebug ("      depthBufferSize = %d ", glFormat.depthBufferSize());
    
    
    rasterDl         = makeRaster();
    
    
    glEnable        ( GL_DEPTH_TEST);
    glDepthFunc     ( GL_LESS);	/* pedantic, GL_LESS is the default */
    glClearDepth    ( 1.0);	/* pedantic, 1.0 is the default */
    
    
    glEnable    ( GL_AUTO_NORMAL);
    glEnable    ( GL_NORMALIZE);
    
    glEnable    ( GL_CULL_FACE);
    glEnable    ( GL_BLEND);
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable    ( GL_LINE_SMOOTH);
    
    glEnable     ( GL_COLOR_MATERIAL ) ;
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    initLights();
    
    QGLContext *cx = (QGLContext *)this->context();
    om = new GV__simpleObjectFindings (findings, cx);
    
    if (elementID) generate3DdisplayList (elementID);
}



//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void GVsimpleObjectFindingsglDisplay::generate3DdisplayList  (int elementID)
{
    if (!om) {
        this->elementID = elementID;
        return;
    }
    if (currentElement) glDeleteLists(currentElement,1);
    
    
    currentElement = om->generateObject (elementID, 1024);
}



//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
GLuint GVsimpleObjectFindingsglDisplay::makeRaster()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);	
    
    int r_max_x = 50;
    int r_max_y = 500;
    int xi,yi, i;
    
    int xr, yr; 
    glColor4f(0.0, 0.0, 1.0, 0.4);
    glLineWidth (0.2);
    
    for (xr=-4*r_max_x; xr< 5*r_max_x; xr++){
        glBegin(GL_LINES);
        {   
            glVertex3d  ( (float)(xr) , -4 * r_max_y, 0);  
            glVertex3d  ( (float)(xr) ,  5 * r_max_y, 0);  
        }
        glEnd();  
    }
    
    for (yr=-4*r_max_y; yr< 5*r_max_y; yr++){
        glBegin(GL_LINES);
        {   
            glVertex3d  ( -4*r_max_x,  (float)(yr) , 0);  
            glVertex3d  (  5*r_max_x,  (float)(yr) , 0);  
        }
        glEnd();      
    }
    
    
    qDebug ("Raster created");
    
    glEndList();
    return list;
    
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void   GVsimpleObjectFindingsglDisplay::setBGcolor (float r, float g, float b, float a) 
{
    
    bgr = (GLclampf)r;
    bgg = (GLclampf)g;
    bgb = (GLclampf)b;
    bga = (GLclampf)a;
    
}



//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void GVsimpleObjectFindingsglDisplay::paintGL()
{
    int i,j;
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();   
    
    //    std::cout << "GVmainGlVisuPlane::paintGL() \n";
    glClearColor(bgr,bgg, bgb, bga);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //  background
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    //    glTranslated(r_max_x/2, r_max_y/2, 0.0); 
    glScaled    (scale, scale * w / h, 1.0 ); 
    //    glTranslated(-r_max_x/2, -r_max_y/2, 0.0); 
    
    glScaled    (0.70710681, 0.70710681, 1.0); 
    
    glTranslated(0.70710681, 0, 0.0); 
    
    glRotated (-45, 1.0, 0.0, 0.0);
    glRotated (45,  0.0, 0.0, 1.0);
    
    
    if (currentElement) {
        glPushMatrix();
        glTranslated    ( isx,    isy,  0.0); 
        glTranslated    ( 0.5,    0.5,  0.0); 
        glRotated       ( xrot, 1.0, 0.0, 0.0);
        glRotated       ( zrot,   0.0,  0.0, 1.0);
        glTranslated    ( -0.5,  -0.5,  0.0); 
      
		if (showRaster) {
            glDisable ( GL_LIGHTING ) ; 
            glCallList(rasterDl);
            glEnable ( GL_LIGHTING ) ;
        }      

	
        glCallList(currentElement);
        glPopMatrix();
    }
    glPopMatrix();    
    glPopAttrib();
    
}



//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void GVsimpleObjectFindingsglDisplay::resizeGL(int width, int height)
{
//    qDebug ("hello in resizeGk with new dimension (%d,%d)", width, height);
    w = width;
    h = height;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
    glOrtho(0, 1, 0, 1, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    updateGL();
    
}


void GVsimpleObjectFindingsglDisplay::mouseDoubleClickEvent(QMouseEvent *event)
{
    xrot = 0;
    zrot = 0;
    
}   

void GVsimpleObjectFindingsglDisplay::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    
}   

void GVsimpleObjectFindingsglDisplay::mouseMoveEvent(QMouseEvent *event)
{
    
    if ( (event->buttons() & Qt::LeftButton)) {
        int edx = event->x() - lastPos.x();
        int edy = event->y() - lastPos.y();
        zrot = zrot + edx/15;
        updateGL();        
    }   
    if ( (event->buttons() & Qt::RightButton)) {
        int edx = event->x() - lastPos.x();
        int edy = event->y() - lastPos.y();
        xrot = xrot + edx/15;
        updateGL();        
    }   
    
}    



