/*
 *  GVelementViewFindings.h
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVelementViewFindings_H
#define GVelementViewFindings_H

#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QLineEdit>

#include  "GLmaster.h"
#include  "GVsimpleObjectFindingsglDisplay.h"
#include  "GVfindings.h"


class QSlider;
class GVmainGlVisuPlane;

class GVelementViewFindings : public QWidget
{
    Q_OBJECT
    
public:
    
    GVelementViewFindings( QGLWidget *shareWidget, GVfindings  *findings);
    
    public slots:
        
    void sliderValueChanged(int v);
    void updateValueFields();


    void minChanged(int v);
    void maxChanged(int v);
    void minYChanged(int v);
    void maxYChanged(int v);
 
	
	void activateSliders (bool v);
    void setSelectedElements (int v);

signals:
        
    void  search                  ( const QString & text);
    void  antAnimationToogled     ( bool checked );
    
	void  resetAnimation          ();
    
	void  resetSelection			();
 	void  selectVerst				();
 	void  selectNichtVerst          ();
 	void  selectSurvialSehrKlein    ();
 	void  selectSurvialKlein        ();
 	void  selectSurvialGross        ();
 	
   	void  startAnimation_1        ();
   	void  startAnimation_2        ();
   	void  startAnimation_3        ();
   	void  startAnimation_4        ();
 	
	void filterWithRange (int min, int max, int ymin, int ymax); 

private:
        
    QGLWidget   *gmaster;
    GVfindings   *findings;  
    
    GVsimpleObjectFindingsglDisplay    *glWindow;
    
    int recNumber;
    QLabel *recnumberL; 
    
    QLabel *sampeAgeL;
    QLabel *sendDateL;
    QLabel *stagingtL;
    QLabel *stagingnL;
    QLabel *stagingmL;
    QLabel *stagingrL;
    QLabel *localisationL;
    QLabel *olddiagnL;
    QLabel *diagnL;
    QLabel *commentL;
    QLabel *sterbejahrL;
    QLabel *ICDNL;
    QLabel *VerstorbenL;
    QLabel *SurvivalL;

	QLabel *selectedElements;

    QLabel *minL;
    QLabel *maxL;

    QLabel *minYL;
    QLabel *maxYL;

	QSlider     *minSlider; 
	QSlider     *maxSlider;
	QSlider     *minYSlider; 
	QSlider     *maxYSlider;

	bool   sliderActive;
	int    minValue;
	int    maxValue;

	int    minYValue;
	int    maxYValue;


	QLineEdit *freitextSucheL;



};

#endif