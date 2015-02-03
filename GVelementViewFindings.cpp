/*
 *  GVelementViewFindings.cpp
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVelementViewFindings.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QPushButton>


GVelementViewFindings::GVelementViewFindings( QGLWidget *shareWidget,  GVfindings  *findings )
{
    
    this->findings = findings;
    this->gmaster  = shareWidget;
    recNumber = 0;
    
    
    int numberOfRecords = findings->numberOfRecords;
    
	QGridLayout *attributeLayout  = new QGridLayout;
    QSlider     *elementSlider    = new QSlider(Qt::Horizontal);

    elementSlider->setRange(0, numberOfRecords-1);
    elementSlider->setValue(recNumber);
    elementSlider->setFixedWidth (512);
    
    connect(elementSlider,   SIGNAL(valueChanged(int)),              this, SLOT(sliderValueChanged(int)) );
    
	selectedElements  = new QLabel ("0");
    selectedElements->setFixedWidth (150);
  


    int row = 1;
    
    attributeLayout->addWidget(selectedElements, 0,0,1,2);
    attributeLayout->addWidget(elementSlider,    0,1,1,3);
    
    
    QPushButton *antAnimationB = new QPushButton("Auto", this);
    antAnimationB->setCheckable (true); antAnimationB->setFixedWidth (80);
    connect( antAnimationB,   SIGNAL(toggled ( bool) ),  this, SIGNAL(antAnimationToogled( bool )) );

    QPushButton *animation_1B= new QPushButton("1", this);
    animation_1B->setCheckable (false); animation_1B->setFixedWidth (80);
    connect( animation_1B,   SIGNAL(pressed () ),  this, SIGNAL(startAnimation_1()) );

    QPushButton *animation_2B= new QPushButton("2", this);
    animation_2B->setCheckable (false); animation_2B->setFixedWidth (80);
    connect( animation_2B,   SIGNAL(pressed () ),  this, SIGNAL(startAnimation_2()) );

	QPushButton *animation_3B= new QPushButton("3", this);
    animation_3B->setCheckable (false); animation_3B->setFixedWidth (80);
    connect( animation_3B,   SIGNAL(pressed () ),  this, SIGNAL(startAnimation_3()) );

	QPushButton *animation_4B= new QPushButton("4", this);
    animation_4B->setCheckable (false); animation_4B->setFixedWidth (80);
    connect( animation_4B,   SIGNAL(pressed () ),  this, SIGNAL(startAnimation_4()) );
    


    QPushButton *resetAnimationB = new QPushButton("Reset", this);
    resetAnimationB->setCheckable (false);
    resetAnimationB->setFixedWidth (150);
    connect( resetAnimationB,   SIGNAL(pressed () ),  this, SIGNAL(resetAnimation()) );
 
    QGridLayout *aninButtonsLayout  = new QGridLayout;
    aninButtonsLayout->addWidget(antAnimationB, 0, 0);
	aninButtonsLayout->addWidget(animation_1B, 0,  1);
	aninButtonsLayout->addWidget(animation_2B, 0,  2);
	aninButtonsLayout->addWidget(animation_3B, 0,  3);	
	aninButtonsLayout->addWidget(animation_4B, 0,  4);	
	
   
    
    QPushButton *resetSelectionB = new QPushButton("Reset Selction", this);
    resetSelectionB->setCheckable (false);
    resetSelectionB->setFixedWidth (150);
    connect( resetAnimationB,   SIGNAL(pressed () ),  this, SIGNAL(resetSelection()) );
 
    QPushButton *selectVerstB = new QPushButton("Verstorben", this);
    selectVerstB->setCheckable (false); selectVerstB->setFixedWidth (80);
    connect( selectVerstB,   SIGNAL(pressed () ),  this, SIGNAL(selectVerst()) );

    QPushButton *selectNichtVerstB= new QPushButton("Nicht Verst.", this);
    selectNichtVerstB->setCheckable (false); selectNichtVerstB->setFixedWidth (80);
    connect( selectNichtVerstB,   SIGNAL(pressed () ),  this, SIGNAL(selectNichtVerst()) );
	
	QPushButton *selectSurvialSehrKleinB = new QPushButton("Surv < 3 Jahre", this);
    selectSurvialSehrKleinB->setCheckable (false); selectSurvialSehrKleinB->setFixedWidth (80);
    connect( selectSurvialSehrKleinB,   SIGNAL(pressed () ),  this, SIGNAL(selectSurvialSehrKlein()) );
	
	QPushButton *selectSurvialKleinB = new QPushButton("Surv < 5 Jahre", this);
    selectSurvialKleinB->setCheckable (false); selectSurvialKleinB->setFixedWidth (80);
    connect( selectSurvialKleinB,   SIGNAL(pressed () ),  this, SIGNAL(selectSurvialKlein()) );

	QPushButton *selectSurvialGrossB = new QPushButton("Surv > 5 Jahre", this);
    selectSurvialGrossB->setCheckable (false); selectSurvialGrossB->setFixedWidth (80);
    connect( selectSurvialGrossB,   SIGNAL(pressed () ),  this, SIGNAL(selectSurvialGross()) );

    QGridLayout *selectButtonsLayout  = new QGridLayout;
    selectButtonsLayout->addWidget(selectVerstB,			0, 0);
	selectButtonsLayout->addWidget(selectNichtVerstB,		0,  1);
	selectButtonsLayout->addWidget(selectSurvialSehrKleinB, 0,  2);
	selectButtonsLayout->addWidget(selectSurvialKleinB,		0,  3);
	selectButtonsLayout->addWidget(selectSurvialGrossB,		0,  4);

    
	minValue = 0;
	maxValue = 30;


	sliderActive = 	false;

    QPushButton *activateSliderB = new QPushButton("Activate Slider", this);
	activateSliderB->setCheckable (true);
    activateSliderB->setChecked (sliderActive);
 
    activateSliderB->setFixedWidth (150);
    connect( activateSliderB,   SIGNAL(toggled ( bool) ),  this, SLOT(activateSliders( bool )) );
  
	minSlider    = new QSlider(Qt::Horizontal);
    minSlider->setRange(minValue, maxValue);
    minSlider->setValue(minValue);
    minSlider->setFixedWidth (200 );
    connect(minSlider,   SIGNAL(valueChanged(int)),  this, SLOT(minChanged(int)) );
  
	minL  = new QLabel ("0");
    minL->setFixedWidth (40);
   	
	maxSlider    = new QSlider(Qt::Horizontal);
    maxSlider->setRange(minValue, maxValue);
    maxSlider->setValue(maxValue);
    maxSlider->setFixedWidth (200 );
    connect(maxSlider,   SIGNAL(valueChanged(int)),  this, SLOT(maxChanged(int)) );
  
	maxL  = new QLabel ("25");
    maxL->setFixedWidth (40);
 
    QGridLayout *sliderLayout  = new QGridLayout;
	sliderLayout->addWidget(minL,		0,  0);
	sliderLayout->addWidget(minSlider,	0,  1);
	sliderLayout->addWidget(maxSlider,  0,  2);
	sliderLayout->addWidget(maxL,		0,  3);
	
    minL->setText (QString::number(minValue)); 
	maxL->setText (QString::number(maxValue)); 


    minYValue = 1985;
	maxYValue  = 2007;

  
	minYSlider    = new QSlider(Qt::Horizontal);
    minYSlider->setRange(minYValue, maxYValue);
    minYSlider->setValue(minYValue);
    minYSlider->setFixedWidth (200 );
    connect(minYSlider,   SIGNAL(valueChanged(int)),  this, SLOT(minYChanged(int)) );
  
	minYL  = new QLabel ("0");
    minYL->setFixedWidth (40);
   	
	maxYSlider    = new QSlider(Qt::Horizontal);
    maxYSlider->setRange(minYValue, maxYValue);
    maxYSlider->setValue(maxYValue);
    maxYSlider->setFixedWidth (200 );
    connect(maxYSlider,   SIGNAL(valueChanged(int)),  this, SLOT(maxYChanged(int)) );
  
	maxYL  = new QLabel ("25");
    maxYL->setFixedWidth (40);
 
    QGridLayout *sliderYLayout  = new QGridLayout;
	sliderYLayout->addWidget(minYL,		0,  0);
	sliderYLayout->addWidget(minYSlider,	0,  1);
	sliderYLayout->addWidget(maxYSlider,  0,  2);
	sliderYLayout->addWidget(maxYL,		0,  3);
	
    minYL->setText (QString::number(minYValue)); 
	maxYL->setText (QString::number(maxYValue)); 

    attributeLayout->addWidget(resetAnimationB,   row,   0);
    attributeLayout->addLayout(aninButtonsLayout, row++, 1);

  

	attributeLayout->addWidget(resetSelectionB,     row,   0);
    attributeLayout->addLayout(selectButtonsLayout, row++, 1);

    attributeLayout->addWidget(activateSliderB,     row,   0);
    attributeLayout->addLayout(sliderLayout, row++, 1);

     attributeLayout->addLayout(sliderYLayout, row++, 1);


    QLabel *recnumberLL = new QLabel ("Record Number");
    recnumberL  = new QLabel ("");
    attributeLayout->addWidget(recnumberLL, row,0);
    attributeLayout->addWidget(recnumberL,  row++,1);
    
    QLabel *sampeAgeLL = new QLabel ("Age");
    sampeAgeL  = new QLabel ("");
    attributeLayout->addWidget(sampeAgeLL, row,0);
    attributeLayout->addWidget(sampeAgeL,  row++,1);

    QLabel *sendDateLL = new QLabel ("Send Date");
    sendDateL  = new QLabel ("");
    attributeLayout->addWidget(sendDateLL, row,0);
    attributeLayout->addWidget(sendDateL,  row++,1);
 
    QLabel *localisationLL = new QLabel ("Localisation");
    localisationL  = new QLabel ("");
	localisationL->setFixedWidth (400);  
    localisationL->setFixedHeight(50);
    localisationL->setWordWrap (true);
  
    attributeLayout->addWidget(localisationLL, row,0);
    attributeLayout->addWidget(localisationL,  row++,1);

    QLabel *stagingtLL = new QLabel ("Staging T");
    stagingtL  = new QLabel ("");
    attributeLayout->addWidget(stagingtLL, row,0);
    attributeLayout->addWidget(stagingtL,  row++,1);
    
    QLabel *stagingnLL = new QLabel ("Staging N");
    stagingnL  = new QLabel ("");
    attributeLayout->addWidget(stagingnLL, row,0);
    attributeLayout->addWidget(stagingnL,  row++,1);
    
    QLabel *stagingmLL = new QLabel ("Staging M");
    stagingmL  = new QLabel ("");
    attributeLayout->addWidget(stagingmLL, row,0);
    attributeLayout->addWidget(stagingmL,  row++,1);
    
    QLabel *stagingrLL = new QLabel ("Staging R");
    stagingrL  = new QLabel ("");
    attributeLayout->addWidget(stagingrLL, row,0);
    attributeLayout->addWidget(stagingrL,  row++,1);
    
	QLabel *ICDNLL = new QLabel ("ICDN");
    ICDNL  = new QLabel ("");
    attributeLayout->addWidget(ICDNLL, row,0);
    attributeLayout->addWidget(ICDNL,  row++,1);

	QLabel *VerstorbenLL = new QLabel ("Verstorben");
    VerstorbenL  = new QLabel ("");
    attributeLayout->addWidget(VerstorbenLL, row,0);
    attributeLayout->addWidget(VerstorbenL,  row++,1);

   	QLabel *SurvivalLL = new QLabel ("Survival");
    SurvivalL  = new QLabel ("");
    attributeLayout->addWidget(SurvivalLL, row,0);
    attributeLayout->addWidget(SurvivalL,  row++,1);

   

    QLabel *diagnLL = new QLabel ("Diagnose");
    diagnL  = new QLabel ("");
    diagnL->setFixedWidth (400);  
    diagnL->setFixedHeight(350);
    diagnL->setWordWrap (true);
  
    diagnL->setAlignment (Qt::AlignTop);
    diagnLL->setAlignment (Qt::AlignTop);
    

    attributeLayout->addWidget(diagnLL, row,0);
    attributeLayout->addWidget(diagnL,  row++,1);
    
    QLabel *commentLL = new QLabel ("Pathologe");
    commentLL->setAlignment (Qt::AlignTop);
    commentL  = new QLabel ("");    
    commentL->setWordWrap (true);
    attributeLayout->addWidget(commentLL, row,0);
    attributeLayout->addWidget(commentL,  row++,1);
    
    
    QLabel *freitextSucheLL = new QLabel ("Freitextsuche");
    freitextSucheLL->setAlignment (Qt::AlignTop);
    freitextSucheL  = new QLineEdit ();    
    freitextSucheL->setFixedWidth (400);  
    attributeLayout->addWidget(freitextSucheLL, row,0);
    attributeLayout->addWidget(freitextSucheL,  row++,1);
    
    connect(freitextSucheL,   SIGNAL(textChanged( const QString &)),  this, SIGNAL(search( const QString &)) );
    
    
    glWindow          = new GVsimpleObjectFindingsglDisplay (this, gmaster, findings);  
    glWindow->setFixedWidth  (256);
    glWindow->setFixedHeight (256);
    glWindow->setScale       (1/2.0);
    glWindow->setIsoShift    (1, 0);
    
    
    QVBoxLayout *attributemasterLayout  = new QVBoxLayout;
    
    attributemasterLayout->addLayout(attributeLayout);
    attributemasterLayout->addStretch(10);
    
    //    gmaster           = new GLmaster ();
    
    //    gmaster->fastTimer (mainsurface, SLOT(updateGL()));     
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    
    
    mainLayout->addLayout(attributemasterLayout);
    mainLayout->addWidget(glWindow);
    
#if 0    
    mainLayout->addWidget(zoomSlider);
    mainLayout->addWidget(slider2);
    mainLayout->addWidget(slider3);
    mainLayout->addWidget(slider4);
    mainLayout->addWidget(slider5);
#endif
    
    setLayout(mainLayout);
    
    setWindowTitle(tr("GENVIEW - sample DB object config"));
    
    updateValueFields();
}


void GVelementViewFindings::sliderValueChanged(int v) {
    recNumber = v;
    updateValueFields();
    
}


void GVelementViewFindings::updateValueFields ()
{
    recnumberL->setText (QString::number(recNumber) );
    
    sampeAgeL->setText (QString::number (findings->el.value(recNumber)->age));
    sendDateL->setText (QString::number (findings->el.value(recNumber)->year));
    
    stagingtL->setText (findings->el.value(recNumber)->stTs);
    stagingnL->setText (findings->el.value(recNumber)->stNs);
    stagingmL->setText (findings->el.value(recNumber)->stMs);
    stagingrL->setText (findings->el.value(recNumber)->stGs);
    
    localisationL->setText  (findings->el.value(recNumber)->localisation);
    
    diagnL->setText    (findings->el.value(recNumber)->diagnosis);
    commentL->setText  (findings->el.value(recNumber)->doctor);
    
	ICDNL->setText    (findings->el.value(recNumber)->icdn );

	VerstorbenL->setText    (findings->el.value(recNumber)->isDeath?"ja":"nein");

    if (findings->el.value(recNumber)->isDeath )
       SurvivalL->setText    (QString::number( findings->el.value(recNumber)->survival ) + " Jahre");
	else
       SurvivalL->setText    ("");
   
    glWindow->generate3DdisplayList (recNumber);
    glWindow->updateGL();
    
}


void GVelementViewFindings::minChanged (int v)
{
	if (minValue == v) return;
	minValue = v; 
 
	if (minValue > maxValue) 
	    maxSlider->setValue (minValue);

	if (sliderActive) {
		filterWithRange (minValue, maxValue, minYValue, maxYValue);
	}
	minL->setText (QString::number(minValue)); 
	maxL->setText (QString::number(maxValue)); 
}
void GVelementViewFindings::maxChanged (int v)
{
	if (maxValue == v) return;
	maxValue = v; 

	if (minValue > maxValue) 
	    minSlider->setValue (maxValue);
 
	if (sliderActive) {
		filterWithRange (minValue, maxValue, minYValue, maxYValue);
	}
	minL->setText (QString::number(minValue)); 
	maxL->setText (QString::number(maxValue)); 
}


void GVelementViewFindings::minYChanged (int v)
{
	if (minYValue == v) return;
	minYValue = v; 
 
	if (minYValue > maxYValue) 
	    maxYSlider->setValue (minYValue);

	if (sliderActive) {
		filterWithRange (minValue, maxValue, minYValue, maxYValue);
	}
	minYL->setText (QString::number(minYValue)); 
	maxYL->setText (QString::number(maxYValue)); 
}
void GVelementViewFindings::maxYChanged (int v)
{
	if (maxYValue == v) return;
	maxYValue = v; 

	if (minYValue > maxYValue) 
	    minYSlider->setValue (maxYValue);
 
	if (sliderActive) {
		filterWithRange (minValue, maxValue, minYValue, maxYValue);
	}
    minYL->setText (QString::number(minYValue)); 
	maxYL->setText (QString::number(maxYValue)); 
}

void GVelementViewFindings::activateSliders (bool v) {
	sliderActive = 	v;
	if (!sliderActive) {
		resetSelection ();
	}
	if (sliderActive) {
		filterWithRange (minValue, maxValue, minYValue, maxYValue);
	}
}

void GVelementViewFindings::setSelectedElements (int v) {
   selectedElements->setText (QString::number(v));
}
