/*
 *  GVmainGlVisuPlane.h
 *  paintTest
 *
 *  Created by mue on 11.11.05.
 *  Copyright 2005 __hmmc__. All rights reserved.
 *
 */


#ifndef GVmainGlVisuPlane_H
#define GVmainGlVisuPlane_H

#include <QGLWidget>
#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QMutex>

#include "GLmaster.h"
#include "GVisoGrid.h"
#include "GVelemetBox.h"
#include "GVfindings.h"
#include "GVsettings.h"

#include "GVsimpleObjectFindings.h"


class GVmainGlVisuPlane : public QGLWidget
{
    Q_OBJECT
    
public:
    GVmainGlVisuPlane(GVsettings    *settings, 
					  QWidget       *parent = 0, 
                      QGLWidget     *shareWidget =0, 
                      GVfindings    *findings=0,
					  GVisoGrid     *objectGrid=0,
					  QMutex        *m=0);
    
    ~GVmainGlVisuPlane();
    
	QGLContext* openGLcontext ();

    void generateGlyphs	();
    void resetFindingsToUpdate();

    void resizeView    	();

    QSize minimumSizeHint() const;
 //   QSize sizeHint() const;
    
public 

slots:
    
   void  updateObjectTextureHR      ();

   void  zoom ( QRect r);
   void  zoomFull                   ();
   void  zoomAllElements            ();
   void  zoomSelectedElements       ();
 
   void saveToFile ();
  
signals:

    void zoomChanged            (int value);
 
	void reportObject		 ( int index);
	void setManualSelection  ( QList <long> list);
    void setSelectedElements ( int s);
	void showMessage         ( QString m, int t);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent     (QWheelEvent *event);
#if 0
    int  sortElementsInYears(int ymin, int ymax, int xmin, int xmax, int xdelta);
    void doAntAnimation ();
    void doAntAnimationToPoint ();
    void initAntAnimationtoPoint();

    void resetAllElementsToInitialPosition();
#endif
    
    
private:
    void	 initLights();
	
	
	void	 recompMINMAX();
	bool     elementsInitialized;
	
	QPoint	 screen2index(QPoint p);
	QPointF  index2gl (QPoint p);
	QPointF  screen2gl (QPoint p);

        
    GVsettings       *_settings;
		
    GLmaster    *gmaster;    
    GVfindings   *findings; 
    
    GVisoGrid   *objectGrid;
    
    
    GLuint makeRaster();
    GLuint rasterDl;
    
    GLuint backgroundGrid;

    void   drawRect (int x, int y);
    void   drawCube (int x, int y, int age);
    void   drawSelection (int x, int y, int age);

	QGLPixelBuffer *pbuffer;
	bool saveToFileOperation;
	
    double w,h;
    double scale;
    double screenScale;
    	
    bool doHighLighting;
    bool antAnimation;
    
    bool antAnimationToPoint;
	int  antAnimationPoints;
	int  sammelpunktX;
	int  sammelpunktY;
    int  deltaMX;
	int  deltaMY;

    int    minAge;
    int    maxAge;
    
    double dx, dy;
    double dxbg, dybg;
    double xp, yp;
    
    QPoint lastPos;
    QColor trolltechBlack; 
    
    
    GVelemetBox *testBox;
    GVelemetBox *testBox2;
    

	// selection modes
	QPoint  indexMousePressed;
	QPoint  indexMousePosition;
	QPointF glMousePressed;
	QPointF glMousePosition;

	bool   selectionActive;

    GVbasicElement *movingObject;
	int				selStartX;
	int				selStartY;
    
    bool startMovement;
	bool startObjectMovement;
	
    QPoint lastEventPos;
    QPoint lastEventPosAbsolut;
    QPoint lastObjectPosition;
    
    
    int numberOfObjects;
    int currentObject;
    
    int sizehint;
    
    int antIndex;
    
    QList <GVsimpleObjectFindings *> findingsToUpdate;

    int Xmin, Ymin, Xmax, Ymax;

	QMutex       *_m;    
};

#endif
