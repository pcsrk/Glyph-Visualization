/*
 *  GVmainGlVisuPlane.cpp
 *  paintTest
 *
 *  Created by mue on 11.11.05.
 *  Copyright 2005 __hmmc__. All rights reserved.
 *
 */

#include <QtGui>
#include <QtOpenGL>
#include <QMutex>
#include <QCoreApplication>

#include <math.h>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <AGL/agl.h>
#endif

#define     SELECTION_BUFSIZE   20    
#define     NOOBJECT            77
#define     BOXNAME             88

#include "GVmainGlVisuPlane.h"
#include "GVelementThreeStateDistribution.h"

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
// GVmainGlVisuPlane::GVmainGlVisuPlane(QWidget *parent): QGVmainGlVisuPlane(parent)

GVmainGlVisuPlane::GVmainGlVisuPlane( GVsettings    *settings, 
									 QWidget      *parent, 
                                     QGLWidget    *shareWidget, 
                                     GVfindings   *findings,
									 GVisoGrid    *objectGrid,
									 QMutex       *m) : QGLWidget(parent, shareWidget)
{
    _m = m;
	_settings = 	settings;			  
	
	makeCurrent();
	
    antIndex = 0;
    
    this->findings    = findings;
    this->objectGrid  = objectGrid;
    this->gmaster     = (GLmaster*)shareWidget;
    
    startObjectMovement = false;
    
      
	   
	qDebug ("Xmax = %d, Ymax = %d ", Xmax, Ymax);	  
			 
    sizehint = 16;
    
    backgroundGrid = 0;
    dx = 0;
    dy = 0; 
    xp =  0;
    yp = 0;
    scale = 1.0;
//    scale =sqrt(2) / 2;
	
    int xs,ys;
    int xr,yr;
    float xhalf;
    
    int *imp;
    int *rmp;
    

    doHighLighting = false;
    antAnimation   = false;
    
	deltaMX = 2;
	deltaMX = 2;

    glInitNames();
    startObjectMovement = false;
	selectionActive = false;

    
    // we want to recive keyboard events in this window
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    trolltechBlack  = QColor::fromCmykF(0.00, 0.00, 0.0, 0.0);

    gmaster->ultrafastTimer (this, SLOT(updateGL()));     

    elementsInitialized = false;
	
	
	saveToFileOperation = false;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

GVmainGlVisuPlane::~GVmainGlVisuPlane()
{
    makeCurrent();
    glDeleteLists(backgroundGrid, 1);
    // here some code is missing
}



//
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------


void GVmainGlVisuPlane::initLights( void )
{

    
    float xm = (float)objectGrid->width();
    float ym = (float)objectGrid->height();
    
    glDisable(GL_LIGHTING);
 //   glEnable( GL_LIGHT0 );
 //   glEnable( GL_LIGHT1 );
  //  glEnable( GL_LIGHT2 );

    
    GLfloat diffuse_light0[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position_light0[] = { 2* xm, 2 * ym , (xm+ym)/2, xm+ym };
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light0 );
    glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );
    
    GLfloat diffuse_light1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position_light1[] = { -2* xm, 2 * ym , (xm+ym)/2, xm+ym };
    glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse_light1 );
    glLightfv( GL_LIGHT1, GL_POSITION, position_light1 );
    

     GLfloat ambient_lightModel[] = { 0.3f, 0.3f, 0.3f, 1.0f };
     glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );
}

void GVmainGlVisuPlane::initializeGL()
{
    
     
    GLint TMUs, ABs;
    int i,j;
    
    qglClearColor(trolltechBlack.dark());
    
    rasterDl         = makeRaster();
    
    glDisable    (GL_DEPTH_TEST);
//    glEnable    (GL_DEPTH_TEST);
    
    glEnable    (GL_AUTO_NORMAL);
    glEnable    (GL_NORMALIZE);
    
    glDisable   (GL_CULL_FACE);
    glEnable    (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable    ( GL_LINE_SMOOTH);
    
    glEnable     ( GL_COLOR_MATERIAL ) ;
    glPolygonMode( GL_FRONT, GL_FILL );
    
    initLights();
    
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &TMUs);
    glGetIntegerv(GL_AUX_BUFFERS, &ABs);
    
    // std::cout << "GL_MAX_TEXTURE_UNITS = " << TMUs << "\n";
    // std::cout << "GL_AUX_BUFFERS       = " << ABs << "\n";
    
    
    QGLContext *cx = (QGLContext *)this->context();
    
    
    GLUquadricObj* gq;
    gq = gluNewQuadric();

   
#ifdef __APPLE__
         AGLContext aglContext;
         aglContext = aglGetCurrentContext();
         GLint swapInt = 1;
         aglSetInteger(aglContext, AGL_SWAP_INTERVAL, &swapInt);
#endif
	
	

}

//------------------------------------------------------------------------------------
QGLContext* GVmainGlVisuPlane::openGLcontext () {
	return (QGLContext *)this->context();
}


void GVmainGlVisuPlane::resetFindingsToUpdate() {
  findingsToUpdate.clear();    
}


//------------------------------------------------------------------------------------
void   GVmainGlVisuPlane::generateGlyphs () {

	QGLContext *cx = (QGLContext *)this->context();

	if (! findings->numberOfRecords) return;

	showMessage ("Generate Glyphs", 0);

	int side = sqrt ( findings->numberOfRecords);
	int x_start = (objectGrid->areas[0].w - side ) / 2;
	int y_start = (objectGrid->areas[0].h - side ) / 2;
	int x_end   =  x_start + side;
	int y_end   =  y_start + side;

	if (x_start < 0) x_start = 0;
	if (y_start < 0) y_start = 0;
	if (x_start > objectGrid->width()) x_start =  objectGrid->width();
	if (y_start > objectGrid->width()) y_start =  objectGrid->width();

	qDebug ("start point = %d,%d", x_start, y_start);
	qDebug ("end point = %d,%d", x_end, y_end);

	int x = x_start;
	int y = y_start;
 
    int cc = 0;
    for (int i = 0; i< findings->numberOfRecords ; i++) {	

	    GVsimpleObjectFindings *so = new GVsimpleObjectFindings (_settings, gmaster, findings, cx);
		so->recordID = i;
		so->fo = findings->el[i];	
		so->w = 1;
		so->h = 1;
		so->x = x;
		so->y = y;
		objectGrid->setElement (so);
		if (x++ > x_end) { x = x_start; y++; }
		if (cc++ >500) {cc=0; QCoreApplication::processEvents (QEventLoop::AllEvents, 1);}
	}
	
	 for (int i = 0; i< objectGrid->count(); i++) {	
	    GVsimpleObjectFindings *so = (GVsimpleObjectFindings *)objectGrid->elementAt(i);
		so->generateOrUpdateObject (16);
		if (cc++ >500) {cc=0; QCoreApplication::processEvents (QEventLoop::AllEvents, 1);}
	}

#if 0	
	int e=0;
	for (int i = 0; i<4; i++) {
		objectGrid->moveElementTo (objectGrid->elementAt(e++), objectGrid->areas[i].x,							 objectGrid->areas[i].y);
		objectGrid->moveElementTo (objectGrid->elementAt(e++), objectGrid->areas[i].x+objectGrid->areas[i].w-1,  objectGrid->areas[i].y);
		objectGrid->moveElementTo (objectGrid->elementAt(e++), objectGrid->areas[i].x+objectGrid->areas[i].w-1,  objectGrid->areas[i].y +  objectGrid->areas[i].h -1);
		objectGrid->moveElementTo (objectGrid->elementAt(e++), objectGrid->areas[i].x,                           objectGrid->areas[i].y +  objectGrid->areas[i].h -1);
    }
#endif
    elementsInitialized    = true;
	
	
	qDebug ("GVmainGlVisuPlane::generateGlyph finished");
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void   GVmainGlVisuPlane::updateObjectTextureHR()
{
    if ( !findingsToUpdate.isEmpty() && elementsInitialized) {
//        qDebug ("updateObjectTexture HIGH RES  %d elements to process ", findingsToUpdate.count());
        makeCurrent();
        GVsimpleObjectFindings *so;
        int i = 0;
        while (i<5 && !findingsToUpdate.isEmpty()) {
            so = findingsToUpdate.takeFirst();
            so->generateOrUpdateObject (sizehint);
            i++;
        }
        if (!findingsToUpdate.isEmpty())
        {        
            QTimer::singleShot(10, this, SLOT(updateObjectTextureHR()));
        }
    }
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void GVmainGlVisuPlane::paintGL()
{
    makeCurrent();
    w = width();
    h = height();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    if (objectGrid) {
		QPointF maxScreen = index2gl (QPoint (objectGrid->width(), objectGrid->height()));
        qreal maxSide = std::max (maxScreen.x(), maxScreen.y());
		glOrtho(0, maxSide, 0, maxSide, -100000, 100000.0);
		//qDebug ("GL SPACE 0 -> %f", maxSide);
	}
	else {
	    glOrtho(0, 1, 0, 1, -100000, 100000.0);
	}
	
    glMatrixMode(GL_MODELVIEW);
    
	Xmin = 0;
    Ymin = 0;
    Xmax = objectGrid->width();
    Ymax = objectGrid->height();
 	
	qreal scale_x =      (qreal)objectGrid->width() /  w;
	qreal scale_y =      (qreal)objectGrid->height() / (h*  0.70710681 / 2 );
    
	screenScale = std::min (scale_x, scale_y);
	
	if (saveToFileOperation) {
		pbuffer->makeCurrent();
		glClearColor     (1.0, 0.0, 0.0, 0.0) ;    	 
	}
	else {
	    makeCurrent();
		glClearColor     (0.6, 0.6, 0.6, 0.0) ;    	 

	}
	
	int i,j;
    GVbasicElement *be;
	
    glInitNames();
    glPushName(NOOBJECT);
	
	
	glClear      ( GL_COLOR_BUFFER_BIT );
	
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glPushMatrix();
		
//    std::cout << " " << scale << ", " << scale << "\n";
#if 0	
    glTranslated ( objectGrid->w/2, objectGrid->h/2, 0.0); 
    glScaled     ( scale, scale * w / h, 1.0 );  
    glTranslated ( -objectGrid->w/2, -objectGrid->h/2, 0.0); 
    glTranslated ( dx, dy, 0.0); 
    
    be = objectGrid->elementAt(0);

		glPopMatrix();    
		glPushMatrix();
#endif
	
    GLuint textureIDlokal;
    
//    glTranslated ( objectGrid->w/2, objectGrid->h/2, 0.0); 
    
	
	if (saveToFileOperation) {
		glScaled     ( scale, scale, 1.0 );  

	}
	else {
		glScaled     ( scale, scale * w / h, 1.0 );  
	}
	
//    glTranslated ( -objectGrid->w/2, -objectGrid->h/2, 0.0); 

    glTranslated(dx, dy, 0.0); 
    
    GVsimpleObjectFindings *so;
	
    glDisable       ( GL_LIGHTING ) ;
    glDisable       ( GL_TEXTURE_2D);
    glDisable       ( GL_BLEND);
  	
	glEnable       (GL_LINE_SMOOTH); 
    glEnable       ( GL_BLEND);
	
	QPointF fe  =  index2gl ( QPoint ( objectGrid->width(), objectGrid->height() ) ); 

	glColor4f     (0.6, 0.6, 0.6, 0.0) ;    	 
    glBegin(GL_QUADS);
		glVertex3f   (0,      0,       0); 
		glVertex3f   (fe.x(), 0,      0); 
		glVertex3f   (fe.x(), fe.y(), 0); 
		glVertex3f   (0,      fe.y(), 0); 

	glEnd( );

	glColor4f     (0.7, 0.7, 0.7, 0.0) ;    
	glBegin(GL_LINE_STRIP);
		glVertex3d   (0,      0,      0); 
		glVertex3d   (fe.x(), 0,      0);
		glVertex3d   (fe.x(), fe.y(), 0); 
		glVertex3d   (0,      fe.y(), 0);  
		glVertex3d   (0,      0,      0); 
     glEnd(); 

	QPointF vline  =  index2gl ( QPoint ( objectGrid->areas[1].x, objectGrid->areas[1].y ) ); 
    


	glBegin(GL_LINE_STRIP);
		glVertex3d   (0,            vline.y(), 0); 
		glVertex3d   (fe.x(),       vline.y(), 0); 
     glEnd(); 

	QPointF vvl1  =  index2gl ( QPoint ( objectGrid->areas[2].x, objectGrid->areas[2].y ) ); 

	glBegin(GL_LINE_STRIP);
		glVertex3d   (vvl1.x(), vvl1.y(), 0); 
		glVertex3d   (vvl1.x(), fe.y(),   0); 
     glEnd(); 

	QPointF vvl2  =  index2gl ( QPoint ( objectGrid->areas[3].x, objectGrid->areas[3].y ) ); 

	glBegin(GL_LINE_STRIP);
		glVertex3d   (vvl2.x(), vvl2.y(), 0); 
		glVertex3d   (vvl2.x(), fe.y(),   0); 
     glEnd(); 


	if (findings->numberOfRecords == 0) return; 

	glLineWidth (0.5);

  	int mCode = objectGrid->areas[0].movementType;
 	if ( (mCode == kMoveToYAS_Single) || (mCode == kMoveToYAS_Multiple) ) {
		for (int yearAreaCount = 0; yearAreaCount<12; yearAreaCount++) {
			QPointF verticalLine  =  index2gl ( QPoint ( objectGrid->width(), 1 + objectGrid->areas[0].y + objectGrid->areas[0].h / 10 + yearAreaCount * objectGrid->cellsPerYear*10)); 
			
			if (  (yearAreaCount ==  0) || (yearAreaCount ==  5) || (yearAreaCount ==  10) ) {
				glLineStipple (3, 0xFFFF);
				glColor4f     (0.8, 0.0, 0.8, 0.5) ;  
			}
			else
			{
				glLineStipple (3, 0xAAAA);
				glColor4f     (0.8, 0.8, 0.8, 0.5) ;  

			}				
			glBegin(GL_LINE_STRIP);
				glVertex3d   (0,                      verticalLine.y(), 0); 
				glVertex3d   (verticalLine.x(),       verticalLine.y(), 0); 
			glEnd(); 
		}
	}
	
	
	
	glEnable    ( GL_BLEND);
    glEnable	( GL_TEXTURE_2D);
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      	
#if 1	
	int objectsInGrid = 0;
	for (int l = Ymax-1; l>=0; l=l-1) {
      for (int k = 0; k < Xmax; k=k+1) {

			int index = objectGrid->getIndex(k,l);
            if (index != -1) {
				be = objectGrid->elementAt (index);
                objectsInGrid++;
                int name = objectGrid->getIndex(k,l);
                
                glLoadName   (name+1000);                
                glPushMatrix();
    
                if (be->y%2) 
                    glTranslated (be->x,       be->y * 0.70710681/2, 0.0); 
                else
                    glTranslated (be->x+ 0.5 , be->y * 0.70710681/2, 0.0); 

                textureIDlokal = be->textureID;
				
				
			    glBindTexture  ( GL_TEXTURE_2D, textureIDlokal);
                
                glColor4f     ( 0.5, 0.5, 0.5, 1.0); 
				bool isActive =  findings->el.value(be->recordID)->isSelected; 
			    if ( isActive) {
                    glTexEnvf       ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                }
                else {
                     glTexEnvf     ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                }
				
                glEnable    ( GL_BLEND);
                glEnable	( GL_TEXTURE_2D);
                glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST  );   
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );    
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

                glBegin(GL_QUADS);
                glNormal3i   (0, 0, 1);
                glTexCoord2d (0,  0);   glVertex3f(0.0, 0.0, 0.0);
                glTexCoord2d (1,  0);   glVertex3f(1.0, 0.0, 0.0);
                glTexCoord2d (1,  1);   glVertex3f(1.0, 2.0, 0.0);
                glTexCoord2d (0,  1);   glVertex3f(0.0, 2.0, 0.0);
                glEnd( );
				
				
				
                glPopMatrix();
            } 
            
        }
        
        
    }
	
//	qDebug ("in paint widget painted %d", 	objectsInGrid);
	
#endif

    glDisable       ( GL_LIGHTING ) ;
    glDisable       ( GL_TEXTURE_2D);
    glDisable       ( GL_BLEND);

    if (selectionActive) {
		QPointF start = glMousePressed;
		QPointF end   = glMousePosition;
		glColor4f     (0.7, 0.7, 0.7, 1.0) ;    
		glBegin(GL_LINE_STRIP);
			glVertex3f   (start.x(),	start.y(),  0); 
			glVertex3f   (end.x(),	    start.y(),  0); 
			glVertex3f   (end.x(),	    end.y(), 0); 
			glVertex3f   (start.x(),	end.y(), 0); 
			glVertex3f   (start.x(),	start.y(),  0); 		
		glEnd(); 
	}



    glPopMatrix();
   
 
	

}


//------------------------------------------------------------------------------------
 void GVmainGlVisuPlane::resizeView    	() { 

	w = width();
    h = height();

    makeCurrent();
	glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
	
    if (objectGrid) {
		QPointF maxScreen = index2gl (QPoint (objectGrid->width(), objectGrid->height()));
        qreal maxSide = std::max (maxScreen.x(), maxScreen.y());		
		glOrtho(0, maxSide, 0, maxSide, -100000, 100000.0);
		//qDebug ("GL SPACE 0 -> %f", maxSide);
	}
	else {
	    glOrtho(0, 1, 0, 1, -100000, 100000.0);
	}
	
    glMatrixMode(GL_MODELVIEW);
 
	Xmin = 0;    
    Ymin = 0;    
    Xmax = objectGrid->width();    
    Ymax = objectGrid->height();
 	
	qreal scale_x =      (qreal)objectGrid->width() /  w;
	qreal scale_y =      (qreal)objectGrid->height() / (h*  0.70710681 / 2 );

	screenScale = std::min (scale_x, scale_y);
 
//	qDebug ("********** screenScale = %f", screenScale);
    updateGL();
 }


//------------------------------------------------------------------------------------
void GVmainGlVisuPlane::saveToFile  	() { 
	
	
	GVbasicElement *be;

	int w = 4096;
    int h = 4096;
			
	
//	w = 1024;
//	h = 1024;
	
	saveToFileOperation = true;
	
	pbuffer = new QGLPixelBuffer( QSize(w, h), this->context()->format(), gmaster );	
    pbuffer->makeCurrent();
	glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
	
    if (objectGrid) {
		QPointF maxScreen = index2gl (QPoint (objectGrid->width(), objectGrid->height()));
        qreal maxSide = std::max (maxScreen.x(), maxScreen.y());		
		glOrtho(0, maxSide, 0, maxSide, -100000, 100000.0);
		//qDebug ("GL SPACE 0 -> %f", maxSide);
	}
	else {
	    glOrtho(0, 1, 0, 1, -100000, 100000.0);
	}
	
    glMatrixMode(GL_MODELVIEW);
	
	Xmin = 0;    
    Ymin = 0;    
    Xmax = objectGrid->width();    
    Ymax = objectGrid->height();
	
	
 	
	qreal scale_x =      (qreal)objectGrid->width() /  w;
	qreal scale_y =      (qreal)objectGrid->height() / (h*  0.70710681 / 2 );
	
	screenScale = std::min (scale_x, scale_y);

#if 1

	glClearColor     (1.0, 1.0, 1.0, 0.0) ;    	 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glScaled     ( scale, scale, 1.0 );  
	glTranslated (dx, dy, 0.0); 
	
	
	int mCode = objectGrid->areas[0].movementType;
    if (1) {
// 	if ( (mCode == kMoveToYAS_Single) || (mCode == kMoveToYAS_Multiple) ) {
		for (int yearAreaCount = 0; yearAreaCount<12; yearAreaCount++) {
			QPointF verticalLine  =  index2gl ( QPoint ( objectGrid->width(), 1 + objectGrid->areas[0].y + objectGrid->areas[0].h / 10 + yearAreaCount * objectGrid->cellsPerYear*10)); 
			
			if (  (yearAreaCount ==  0) || (yearAreaCount ==  5) || (yearAreaCount ==  10) ) {
				glLineStipple (3, 0xFFFF);
				glColor4f     (0.8, 0.0, 0.8, 0.5) ;  
			}
			else
			{
				glLineStipple (3, 0xAAAA);
				glColor4f     (0.8, 0.8, 0.8, 0.5) ;  
				
			}				
			glBegin(GL_LINE_STRIP);
			glVertex3d   (0,                      verticalLine.y(), 0); 
			glVertex3d   (verticalLine.x(),       verticalLine.y(), 0); 
			glEnd(); 
		}
	}
	
	
	
//	glEnable     ( GL_DEPTH_TEST);
	glEnable     ( GL_AUTO_NORMAL);
	glEnable     ( GL_NORMALIZE);
	glEnable     ( GL_COLOR_MATERIAL ) ;
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	

#if 1
	glEnable     ( GL_CULL_FACE);
	glEnable     ( GL_BLEND);
	glBlendFunc  ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable     ( GL_LINE_SMOOTH);
#endif	

	
	glEnable(GL_LIGHTING);


	glEnable( GL_LIGHT0 );
//	glEnable( GL_LIGHT1 );
//	glEnable( GL_LIGHT2 );


	float lc = 0.1;
	
	GLfloat diffuse_light0[]  = { 50*lc, 50*lc, 50*lc, 1.0f };
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light0 );
	
	GLfloat diffuse_light1[]  =  { 3*lc,3*lc,3*lc, 1.0f };
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse_light1 );
	
	GLfloat diffuse_light2[]  =  { 2*lc,2*lc,2*lc, 1.0f };
	glLightfv( GL_LIGHT2, GL_DIFFUSE, diffuse_light2 );
	
	GLfloat ambient_lightModel[] =  { 7*lc,7*lc,7*lc, 1.0f };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );

	
	int objectsInGrid = 0;
	for (int l = Ymax-1; l>=0; l=l-1) {
		for (int k = 0; k < Xmax; k=k+1) {
			
			int index = objectGrid->getIndex(k,l);
            if (index != -1) {
	
				be = objectGrid->elementAt (index);
                objectsInGrid++;
                
                glPushMatrix();
               if (be->y%2) 
                    glTranslated (be->x,       be->y * 0.70710681/2, 0.0); 
                else
                    glTranslated (be->x+ 0.5 , be->y * 0.70710681/2, 0.0); 
			
				
				
//
				
				
//				glScaled    (1.0, 0.5, 1.0 ); 
				glScaled    (0.70710681, 0.70710681, 1.0); 
				
//				glScaled    (0.5, 0.5, 1.0); 
				glTranslated(0.70710681, 0, 0.0); 
				
				glRotated (-45, 1.0, 0.0, 0.0);
				glRotated (45,  0.0, 0.0, 1.0);
				
//				GLfloat position_light0[] = { 0.0f, 4.0f, 0.0f, 1.0f };
				GLfloat position_light0[] = { 0.0f, 1.0f, 1.0f, 1.0f };
				
				glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );
				
				// 			GLfloat position_light1[] = { -3.0f, 0.0f, 2.0f, 1.0f };
				GLfloat position_light1[] = { -5.0f, 5.0f, 0.0f, 1.0f };
				
				glLightfv( GL_LIGHT1, GL_POSITION, position_light1 );
				
				//				GLfloat position_light2[] = { 5.5f, 0.0f, 2.0f, 1.0f };
				GLfloat position_light2[] = { 5.5f, 0.0f, 8.0f, 1.0f };
				
				glLightfv( GL_LIGHT2, GL_POSITION, position_light2 );
				
				
				
				if (be->object3dDl)  {
					glCallList(be->object3dDl);
				}
                glPopMatrix();
            } 
            
        }
        
        
    }
	
	
	
	
#endif	
	
	
	
//	paintGL();

	this->makeCurrent();
	
	QImage pBufferImage = pbuffer->toImage();
	
	pBufferImage.save ("test.png");

	delete (pbuffer);
	
  	saveToFileOperation = false;
	resizeView    	();
	
}


//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void GVmainGlVisuPlane::resizeGL(int width, int height)
{
	resizeView();
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------


void GVmainGlVisuPlane::keyPressEvent(QKeyEvent *event)
{
    
#if 0
    //    std::cout << "keyPressEvent \n";
    switch ( event->key() )
    {
        case Qt::Key_M:
            paintMode = MOVE_MODE;
            std::cout << "MOVE_MODE ON \n";
            break;
        case Qt::Key_B:
            break;
        case Qt::Key_P:
            break;
        case Qt::Key_Q:
            zoomChanged (20 * (scale * 1.2 -1) );
            //            setScaleIntern ( scale * 1.2);
            break;
        case Qt::Key_W:
            if (scale < 1.0)
                scale = 1.0;
            zoomChanged (20 * (scale/1.2 -1) );			
            //			setScaleIntern ( scale / 1.2);
            break;
        case Qt::Key_U:
            updateGL();
            break;
        case Qt::Key_R:
            updateResultFullImage();
            break;
            
        case Qt::Key_I:
            dx = 0;		
            dy = 0;
            dxbg = 0;	
            dybg = 0;
            scale = 1.0;
            updateGL();
            break;
        default:;
    }
    
    if ( paintMode == MOVE_MODE || paintMode == MOVE_ONLY_BACKGROUND_MODE) {
        
        switch ( event->key() )
        {
            case Qt::Key_Up:
                if (  paintMode == MOVE_MODE) {
                    dy   = dy + 1 ;
                }                
                dybg = dybg + 1 ;
                updateGL();
                break;
            case Qt::Key_Down:
                if (  paintMode == MOVE_MODE) {
                    dy   = dy - 1 ;
                }                
                dybg = dybg - 1 ;
                updateGL();
                break;
                
            case Qt::Key_Left:
                if (  paintMode == MOVE_MODE) {
                    dx   = dx - 1 ;
                }                
                dxbg = dxbg - 1 ;
                updateGL();
                break;
            case Qt::Key_Right:
                if (  paintMode == MOVE_MODE) {
                    dx   = dx + 1 ;
                }                
                dxbg = dxbg + 1 ;
                updateGL();
                break;
            default:;
        }
    }   
#endif    
}

//------------------------------------------------------------------------------
void  GVmainGlVisuPlane::zoomFull(){
	scale = 1; 
	dx = 0; 
	dy = 0;
}


//------------------------------------------------------------------------------
void  GVmainGlVisuPlane::zoomAllElements () {
	int b = 10;
	if (objectGrid)   zoom ( objectGrid->minMax().adjusted (-b,-b, b, b) );
}
//------------------------------------------------------------------------------
void  GVmainGlVisuPlane::zoomSelectedElements() {
	int b = 10;
	if (objectGrid)  zoom ( objectGrid->minMaxSelected().adjusted (-b,-b, b, b) );
}

//------------------------------------------------------------------------------
void  GVmainGlVisuPlane::zoom (QRect r) {

	QPointF delta  =  index2gl ( QPoint ( r.x(), r.y()) ) ; 
	QPointF size   =  index2gl ( QPoint ( r.width (), r.height()) ); 
	
	qreal scale_x =      (qreal)objectGrid->width() /  r.width ();
	qreal scale_y =      (qreal)objectGrid-> height () / (r.height() *  0.70710681 / 2 );


	scale = std::min (scale_x, scale_y);
	dx = - delta.x();
	dy = - delta.y();


//	qDebug ("RR  scale_x= %f, scale_y = %f",scale_x, scale_y );

//	qDebug ("scale = %f, dx = %f, dy = %f", scale, dx, dy);

	recompMINMAX();
 //   updateGL();
	
}





//------------------------------------------------------------------------------
void GVmainGlVisuPlane::wheelEvent     (QWheelEvent *event) {
    int delta = event->delta() / 120;
    double scaleOld = scale;
    
    
    if (delta < 0) {
        for (int i = 0; i< -delta; i++)   
            scale =  scale/1.05;	
    }
    else
    {
        for (int i = 0; i< delta; i++)   
            scale =  scale*1.05;	
    }
	
	
	double scaleMx = (objectGrid->width())/ w /  (scale) ;
    double scaleMy = (objectGrid->height())/ h /  (scale * w / h);
	
    double mouseX =  event->x()     * scaleMx  - dx;;
    double mouseY =  (h-event->y()) * scaleMy  - dy;

    double mouseOldX =  event->x()     * ( objectGrid->width())/ w /  (scaleOld)         - dx;
    double mouseOldY =  (h-event->y()) * ( objectGrid->height())/ h /  (scaleOld * w / h) - dy;
    double mouseNewX =  event->x()     * ( objectGrid->width())/ w /  (scale)          - dx;
    double mouseNewY =  (h-event->y()) * ( objectGrid->height())/ h /  (scale * w / h)  - dy;

//    qDebug ("mouseOldY = %f, mouseNewY = %f, DIFF = %f ", mouseOldY, mouseNewY, mouseNewY - mouseOldY);
        
		
		
    dx   = dx +  (mouseNewX - mouseOldX);
    dy   = dy +  (mouseNewY - mouseOldY);
    
//	qDebug ("scale = %f, dx = %f, dy = %f", scale, dx, dy);

    recompMINMAX();
    updateGL();
}




void GVmainGlVisuPlane::mousePressEvent(QMouseEvent *event)
{
  
	indexMousePressed = screen2index (event->pos() );
	glMousePressed    = screen2gl    (event->pos());

	
	
//	pMousePressed   =  screen2index (event->pos());	
	
   if (( event->modifiers () == Qt::NoModifier) ) selectionActive = true;
	
			
	lastPos = event->pos();
  
	double scaleMx = (objectGrid->width())/ w /  (scale) ;
    double scaleMy = (objectGrid->height())/ h /  (scale * w / h);

    double mouseX =  event->x()     * scaleMx  - dx;;
    double mouseY =  (h-event->y()) * scaleMy  - dy;

	selStartX = mouseX+0.5;
	selStartY = mouseY+0.5;

    startMovement = true;
    
    makeCurrent();
    QPoint   screenPos = event->pos();
    
    static GLuint selectionBuffer[SELECTION_BUFSIZE];
    GLint  hits, viewport[4];
    
    glSelectBuffer (SELECTION_BUFSIZE, selectionBuffer);
    glGetIntegerv(GL_VIEWPORT,viewport);
   
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    glRenderMode(GL_SELECT);
    glLoadIdentity();
  
    gluPickMatrix(screenPos.x(),  viewport[3] - screenPos.y(), 5, 5, viewport);
    glOrtho(0, objectGrid->width(), 0, objectGrid-> height(), -100000, 100000.0);
    
    glMatrixMode(GL_MODELVIEW);
    glInitNames();
    updateGL();
    glFlush();
    
    hits = glRenderMode(GL_RENDER);
    
	
	if (hits == 0) {
		
	}
	else
	{
		GLuint *ptr = (GLuint *) selectionBuffer;
		for (int i=0;i<hits;i++) {    
			GLuint names = *ptr; 
//			std::cout << "                      names = " << *ptr << "\n"; 
			ptr +=3;
			for (int j=0;j<names;j++) {
				std::cout << "                      name = " << *ptr << "\n"; 
				if (  (*ptr >= (i+1000)) &&    (*ptr < (1000+objectGrid->count())) ) {          
                    movingObject = objectGrid->elementAt(*ptr-1000);
					
					
					qDebug ("Object Nr. %d, %d selected", movingObject->x, movingObject->y);
                    startObjectMovement = true;
                    qDebug  ("Object %d Selected", *ptr);
                    lastEventPos        = screenPos;
                    lastEventPosAbsolut = screenPos;
                    lastObjectPosition.setX (movingObject->x);
                    lastObjectPosition.setY (movingObject->y);
				}
				ptr++;            
			}
		}
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
}

void GVmainGlVisuPlane::mouseReleaseEvent(QMouseEvent *event)
{
    std::cout << "release mouse\n";    
    startMovement = false;
	startObjectMovement = false; 
	selectionActive = false;
}

//------------------------------------------------------------------------------------
void GVmainGlVisuPlane::recompMINMAX()
{
    double eh = 0.70710681/2.0;

    double scaleMx = (objectGrid->width())/ w /  (scale) ;
    double scaleMy = (objectGrid->height())/ h /  (scale * w / h);
    

    double   xdmin = -dx;
    double   xdmax =  w *  scaleMx -dx;;
  
    double   ydmin = -dy;
    double   ydmax =  2 * h *  scaleMy -dy;  // hack
    
    
//    qDebug ("DISPLAY coordinates x-> %f, %f  --- y-> %f, %f ", xdmin,xdmax, ydmin, ydmax);

    
    int Xmin, Ymin, Xmax, Ymax;
    
    Xmin = (xdmin - 0.25);
    Ymin = (ydmin - eh/4) / eh;
    Xmax = (xdmax - 0.25);
    Ymax = (ydmax - eh/4) / eh;
    
    if (Xmin < 0) Xmin = 0;    
    if (Ymin < 0) Ymin = 0;    
    if (Xmin > objectGrid->width())Xmin = objectGrid->width();    
    if (Ymin > objectGrid->height())Ymin = objectGrid->height();    

    if (Xmax < 0) Xmax = 0;    
    if (Ymax < 0) Ymax = 0;    

    if (Xmax > objectGrid->width())Xmax = objectGrid->width();    
    if (Ymax > objectGrid->height())Ymax = objectGrid->height();    

 
//    qDebug ("still %d objects to Update open", findingsToUpdate.count());
    
    findingsToUpdate.clear();    

	sizehint =   4 * w / (Ymax-Ymin);
	if (sizehint > 512)       sizehint = 512;
    else if  (sizehint > 256) sizehint = 256;
    else if (sizehint > 128) sizehint = 128;
    else if (sizehint > 64)  sizehint = 64;
    else if (sizehint > 32)  sizehint = 32;
    else if (sizehint > 16)  sizehint = 16;
    else sizehint = 16;

    if (sizehint == 16) return;
    
    for (int l =  Xmax-1; l>=Xmin; l=l-1) {
        for (int k = Ymin; k < Ymax; k=k+1) {
		    int index = objectGrid->getIndex(l,k);
			GVsimpleObjectFindings *so = 0;
			if (index != -1) 
            so = ( GVsimpleObjectFindings *)    objectGrid->elementAt  (index);      
			if (so)
            {
                findingsToUpdate.append (so);
            }
        }
    }

    int numberofCells = (Xmax-Xmin)*(Ymax-Ymin);
 //   qDebug ("now we have  %d objcets to Update on %d Cells", findingsToUpdate.count(), numberofCells);
	
	updateObjectTextureHR(); 
 
 //   qDebug ("new x-> %d, %d  --- y-> %d, %d - sizehint ***** %d ****** ", Xmin,Xmax, Ymin, Ymax, sizehint);
 //   qDebug ("now we have  %d objcets to Update on %d Cells", findingsToUpdate.count(), numberofCells);

}


//------------------------------------------------------------------------------------
QSize GVmainGlVisuPlane::minimumSizeHint() const
{
	return (QSize (300,300));
}


//------------------------------------------------------------------------------------
QPointF  GVmainGlVisuPlane::index2gl (QPoint p) {

	qreal localX;
	qreal localY;

    localY = p.y()  * 0.70710681/2;
	if (p.y() % 2) {
	   localX = p.x();
	}
	else {
	   localX = p.x() +0,5;
	}
	return QPointF(localX, localY);
}

//------------------------------------------------------------------------------------
QPointF  GVmainGlVisuPlane::screen2gl (QPoint p) {

	qreal localX;
	qreal localY;
	qreal scalex = objectGrid->width()    / w /  (scale) ;
    qreal scaley = objectGrid-> height() / h /  (scale * w / h);
  
    localX =  p.x()  *  scalex  - dx;
    localY =  ( height() - p.y())  *  scaley  - dy; 

	return QPointF(localX, localY);
}

//------------------------------------------------------------------------------------
QPoint  GVmainGlVisuPlane::screen2index(QPoint p) {
	
	const double eh = 0.70710681/2.0;

    double scalex = objectGrid->width() / w /  (scale) ;
    double scaley = objectGrid->height () / h /  (scale * w / h);
    
    double   xlocalD =              p.x() *  scalex  - dx;
    double   ylocalD =  (height() - p.y()) *  scaley  - dy;  // hack
	
    int xlocal = (xlocalD - 0.25);
    int ylocal = (ylocalD - eh/4) / eh;
#if 0
	qDebug ("retransform p(%d,%d) scale (%f,%f), xlcoalD (%f,%f), xlocal (%d,%d)", 
				 p.x(),  p.y(),
				 scalex, scaley,
				 xlocalD, ylocalD,
				 xlocal,  ylocal);		
#endif
    if (xlocal < 0) xlocal = 0;    
    if (ylocal < 0) ylocal = 0;    	
    if (xlocal > (objectGrid->width()-1))     xlocal = objectGrid->width()-1;    
    if (ylocal > (objectGrid-> height ()-1)) ylocal = objectGrid-> height ()-1;    

	return QPoint (xlocal, ylocal);
}

//------------------------------------------------------------------------------------
void GVmainGlVisuPlane::mouseMoveEvent(QMouseEvent *event)
{
  
    makeCurrent();
   
	
	
    double scaleMx = (objectGrid->width())/ w /  (scale) ;
    double scaleMy = (objectGrid->height())/ h /  (scale * w / h);
    double mouseX =  event->x()     * scaleMx  - dx;;
    double mouseY =  (height()-event->y()) * scaleMy  - dy;
	
	indexMousePosition = screen2index (event->pos());
	glMousePosition    = screen2gl    (event->pos());

	// wir zeigen auf jeden fall an wenn die maus über einem element ist
	
    int index = objectGrid->getIndex(indexMousePosition.x(),indexMousePosition.y());
    if (index > -1) {
       GVsimpleObjectFindings *so = (GVsimpleObjectFindings *)objectGrid->elementAt(index);
       int si = so->recordID;
        reportObject (si);
	}
	else {
	         reportObject (-99);
	}
	

	if (( event->modifiers () == Qt::ControlModifier) ) {
            int edx = event->x() - lastPos.x();
            int edy = event->y() - lastPos.y();

            dx   = dx + scaleMx * (double)edx;
            dy   = dy - scaleMy * (double)edy;

			qDebug ("dx = %f, dy = %f", dx, dy);
            recompMINMAX();
            updateGL();    
    }   

	
	
    if (( event->modifiers () == Qt::NoModifier) && selectionActive) {
		int se = 0;
		
		int startIndexX = std::min ( indexMousePressed.x(), indexMousePosition.x());
		int endIndexX   = std::max ( indexMousePressed.x(), indexMousePosition.x());
		int startIndexY = std::min ( indexMousePressed.y(), indexMousePosition.y());
		int endIndexY   = std::max ( indexMousePressed.y(), indexMousePosition.y());
		
		QList<long> selectedElements;
		GVsimpleObjectFindings *so;
		for (int xi = startIndexX ; xi <= endIndexX ; xi++) {
			for (int yi = startIndexY ; yi <=  endIndexY  ; yi++) {	
			  int index = objectGrid->getIndex(xi,yi);
				GVsimpleObjectFindings *so = 0;
				if (index != -1) 
				so = ( GVsimpleObjectFindings *)    objectGrid->elementAt  (index);    
				if  (so) selectedElements.append (so->recordID);
			}
		}
		
		setManualSelection (selectedElements);	
		qDebug ("SELECTION MODE %d selected",selectedElements.size() );	
    }
	
    QPoint p = screen2index ( event->pos() );	 	
	QString elementInfo = "";
	int objectIndex= objectGrid->getIndex(p.x(),p.y());
	if (objectIndex != -1) elementInfo = "Empty";
//	qDebug ("mouse over  %s element %d, %d", qPrintable (elementInfo), p.x(), p.y() );	
	
	
//	qDebug  ("mouse pressed at event (%d, %d) -  object coord (%f,%f)   -  transformed to (%d,%d)", event->x(), height()-event->y(),  mouseX, mouseY, p.x(), p.y()   );

#if 0    
        
    else if ( (event->buttons() & Qt::LeftButton) && startObjectMovement ) 
    {   
        
        float edx =  ((float)objectGrid->w / (float)w) * ( event->x() - lastEventPosAbsolut.x());
        float edy = -((float)objectGrid->h / (float)h) * ( event->y() - lastEventPosAbsolut.y());
        
     //   float  dxc =  (edx * 0.707106781 + edy * 0.707106781) / scale;  
     //   float  dyc =  (edx * 0.707106781 - edy * 0.707106781) / scale;  
     
        float  dxc =  edx / scale;  
        float  dyc =  edy / scale;  
        
        
        qDebug ("shift in screen coord -> %f, %f, shift in object coord -> %f, %f ", edx, edy, dxc, dyc);
        if (dxc > 0.0) dxc += 0.5;
        if (dyc > 0.0) dyc += 0.5;
        if (dxc < 0.0) dxc -= 0.5;
        if (dyc < 0.0) dyc -= 0.5;
        
        movingObject->x = lastObjectPosition.x() + (int)dxc;
        movingObject->y = lastObjectPosition.y() + (int)dyc;

        updateGL();
        
    }    
    else if ( 1 ) {
#if 0
        glTranslated(objectGrid->w/2, objectGrid->h/2, 0.0); 
        glScaled (scale, scale * w / h, 1.0 );  
        glTranslated(dx, dy, 0.0); 
        if (be->y%2) 
            glTranslated (be->x,       be->y * 0.5, 0.0); 
        else
            glTranslated (be->x+ 0.5 , be->y * 0.5, 0.0); 
        
   
        int    xi =  (int) (event->x() - dx) / scale - objectGrid->w/2;  
        int    yi =  (int) (event->x() - dy) / scale  -objectGrid->h/2;  
        
#endif     
        
        
//        double scaleMx = (objectGrid->width())/ w /  (scale) ;
//        double scaleMy = (objectGrid->height())/ h /  (scale * w / h);
        
        
        int    xi =  -1;
        int    yi =  -1;
        
        double    xd =  event->x()       *  scaleMx -dx;
        double    yd =  (h - event->y()) *  scaleMy -dy;  
        

        double eh = 0.70710681/2.0;
        
        xi = (xd - 0.25);
        yi = (yd - eh/4) / eh;
        
        int index = objectGrid->getIndex(xi,yi);
        if (index > -1) {
            GVsimpleObjectFindings *so;
            so = (GVsimpleObjectFindings *)objectGrid->elementAt(index);
            int si = so->recordID;            
            reportObject (si);
        }
//        qDebug ("mouse am Feld %f, %f  --- %d, %d --- %d", xd,yd, xi, yi, index);
        
    }
#endif
    lastPos = event->pos();
//    update();
}




GLuint GVmainGlVisuPlane::makeRaster()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);	
    
    int xi,yi, i;
    
    float height;
    
    int xr, yr; 
    glColor4f(0.0, 0.0, 1.0, 0.4);
    glLineWidth (0.2);
    
    for (xr=-49*objectGrid->width(); xr< 50*objectGrid->width(); xr++){
        glBegin(GL_LINES);
        {   
            glVertex3d  ( (float)(xr) , -49 * objectGrid->height(), 0);  
            glVertex3d  ( (float)(xr) ,  50 * objectGrid->height(), 0);  
        }
        glEnd();  
    }
        
    for (yr=-49*objectGrid->height(); yr< 50*objectGrid->height(); yr++){
        glBegin(GL_LINES);
        {   
            glVertex3d  ( -49*objectGrid->width(),  (float)(yr) , 0);  
            glVertex3d  (  50*objectGrid->width(),  (float)(yr) , 0);  
        }
        glEnd();      
    }
    

    qDebug ("Raster created");
    
    glEndList();
    return list;
    
}





void GVmainGlVisuPlane::drawRect (int x, int y)
{
    float xmin = x;
    float xmax = x + 1;
    float ymin = y;
    float ymax = y + 1;
    
    glBegin(GL_QUADS);
    glVertex3f(xmin, ymin, 0.0f);
    glVertex3f(xmax, ymin, 0.0f);
    glVertex3f(xmax, ymax, 0.0f);
    glVertex3f(xmin, ymax, 0.0f);
    glEnd( );
}    



void GVmainGlVisuPlane::drawSelection (int x, int y, int age)
{
    float d = 0;
    float height;
    float xmin = x+d;
    float xmax = x + 1-d;
    float ymin = y +d;
    float ymax = y + 1-d;
    float zmax = height;
    
    GLUquadricObj* gqs; 
    
    gqs = gluNewQuadric();
    
    height = 0.5 + (float)age / 50.0;
    
    float black[4]={0,0,0,0};
    
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );   
    glTranslated(x+0.5, y+0.5, 0.0); 
    glColor3f(0.9, 0.9, 0.8);
    glTranslated(0,0,  height); 
    gluSphere(gqs, 0.2, 15, 15);
    glTranslated(0,0, -height); 
    glTranslated(-(x+0.5), -(y+0.5), 0.0); 
    
}    


void GVmainGlVisuPlane::drawCube (int x, int y, int age)
{
    float d = 0;
    float height;
    float xmin = x+d;
    float xmax = x + 1-d;
    float ymin = y +d;
    float ymax = y + 1-d;
    float zmax = height;
    
    GLUquadricObj* gq; 
    GLUquadricObj* gqs; 
    
    gq  = gluNewQuadric();
    gqs = gluNewQuadric();
    
    
    height = 0.5 + (float)age / 50.0;
    
    float black[4]={0,0,0,0};
     
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    //    glColor3f(1.0, 0.0, 0.0);
    
    GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );   
    glTranslated(x+0.5, y+0.5, 0.0); 
 
    gluCylinder(     gq,
                     0.05,
                     0.05,
                     0.5 * height,
                     25,
                     25);
    
    glTranslated(-(x+0.5), -(y+0.5), 0.0); 
}

#if 0        
    glBegin(GL_QUADS);
    glNormal3i (0, 0, -1);
    glVertex3f(xmin, ymin, 0.0f);
    glVertex3f(xmax, ymin, 0.0f);    
    glVertex3f(xmax, ymax, 0.0f);
    glVertex3f(xmin, ymax, 0.0f);
    glEnd( );
    
    glBegin(GL_QUADS);
    glNormal3i (0, 0, 1);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmin, ymax, zmax);
    glEnd( );
 
    
    glBegin(GL_QUADS);
    glNormal3i (-1, 0, 0);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymin, 0.0f);
    glVertex3f(xmin, ymax, 0.0f);
    glVertex3f(xmin, ymax, zmax);
    glEnd( );
 
    glBegin(GL_QUADS);
    glNormal3i (1, 0, 0);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymin, 0.0f);
    glVertex3f(xmax, ymax, 0.0f);
    glVertex3f(xmax, ymax, zmax);
    glEnd( );

    glBegin(GL_QUADS);
    glNormal3i (0, -1, 0);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymin, 0.0f);
    glVertex3f(xmin, ymin, 0.0f);
    glEnd( );
    
    glBegin(GL_QUADS);
    glNormal3i (0, 1, 0);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmax, ymax, 0.0f);
    glVertex3f(xmin, ymax, 0.0f);
    glEnd( );

#endif
    
    


