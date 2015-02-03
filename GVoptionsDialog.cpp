/*
 *  GVoptionsDialog.cpp
 *  dv
 *
 *  Created by mue on 09.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVoptionsDialog.h"
#include  <QLabel>
#include  <QGridLayout>
#include  <QVBoxLayout>
#include  <QComboBox>
#include  <QMessageBox>
#include  <QDoubleSpinBox>

GVoptionsDialog::GVoptionsDialog( GVsettings *settings,   
									  QWidget      *parent) : QWidget (0) {
	
	
	_settings = 	settings;			  
	
	const int COMBOBOXWIDTH = 128;


 	QGridLayout *mainlayout = new QGridLayout();
	
	
	QLabel *defaultCellsPerYearL = new QLabel (tr("scale (cells per year)"),	   this);                 
	QLabel *defaultAgeBandL      = new QLabel (tr("age band"),	   this);                 
	
	QLabel *numberOfAntsL       = new QLabel (tr("number of ants"),	   this);                 
	QLabel *minSpeedL           = new QLabel (tr("minimal Ant Speed"),   this);            
	QLabel *maxSpeedL           = new QLabel (tr("maximal Ant Speed"),      this);            
	QLabel *k1L					= new QLabel (tr("k1 (pick) factor"),   this);         
	QLabel *k2L					= new QLabel (tr("k2 (drop) factor"),   this);         
	QLabel *fBL					= new QLabel (tr("flip glyphs"),   this);         
	

	QDoubleSpinBox *setNumberOfAntsW = new QDoubleSpinBox(0);
	setNumberOfAntsW->setMinimum    (    1.0);
    setNumberOfAntsW->setMaximum    ( 10000.0);
    setNumberOfAntsW->setDecimals   (    0);
    setNumberOfAntsW->setAlignment  ( Qt::AlignLeft);
    setNumberOfAntsW->setSuffix     (" ants");
    setNumberOfAntsW->setValue       ( (double)_settings->numberOfAnts); 
    setNumberOfAntsW->setSingleStep  ( 1);
    setNumberOfAntsW->setFocusPolicy ( Qt::NoFocus );
    setNumberOfAntsW->setFixedSize    (QSize(COMBOBOXWIDTH,24) );
    setNumberOfAntsW->clearFocus     ();

    connect(setNumberOfAntsW,      SIGNAL   (valueChanged        (double)), 
            this,                SLOT     (setNumberOfAnts     (double)) );
      
	
 	QDoubleSpinBox *setminAntSpeedW = new QDoubleSpinBox(0);
	setminAntSpeedW->setMinimum    (    1.0);
    setminAntSpeedW->setMaximum    (  200.0);
    setminAntSpeedW->setDecimals   (    1);
    setminAntSpeedW->setAlignment  ( Qt::AlignLeft);
    setminAntSpeedW->setSuffix     (" ");
    setminAntSpeedW->setValue       ( (double)_settings->antMinSpeed); 
    setminAntSpeedW->setSingleStep  ( 1);
    setminAntSpeedW->setFocusPolicy ( Qt::NoFocus );
    setminAntSpeedW->setFixedSize    (QSize(COMBOBOXWIDTH,24) );
    setminAntSpeedW->clearFocus     ();

    connect(setminAntSpeedW,      SIGNAL   (valueChanged        (double)), 
            this,                SLOT     (setMinAntSpeed     (double)) );

	QDoubleSpinBox *setmaxAntSpeedW = new QDoubleSpinBox(0);
	setmaxAntSpeedW->setMinimum    (    1.0);
    setmaxAntSpeedW->setMaximum    (  200.0);
    setmaxAntSpeedW->setDecimals   (    1);
    setmaxAntSpeedW->setAlignment  ( Qt::AlignLeft);
    setmaxAntSpeedW->setSuffix     (" ");
    setmaxAntSpeedW->setValue       ( (double)_settings->antMaxSpeed); 
    setmaxAntSpeedW->setSingleStep  ( 1);
    setmaxAntSpeedW->setFocusPolicy ( Qt::NoFocus );
    setmaxAntSpeedW->setFixedSize    (QSize(COMBOBOXWIDTH,24) );
    setminAntSpeedW->clearFocus     ();

    connect(setmaxAntSpeedW,      SIGNAL   (valueChanged        (double)), 
            this,                SLOT     (setMaxAntSpeed     (double)) );


    QDoubleSpinBox *setK1W = new QDoubleSpinBox(0);
	setK1W->setMinimum    (    0.0);
    setK1W->setMaximum    (    0.99);
    setK1W->setDecimals   (    2);
    setK1W->setAlignment  ( Qt::AlignLeft);
    setK1W->setSuffix     (" ");
    setK1W->setValue       ( (double)_settings->antK1); 
    setK1W->setSingleStep  ( 0.05);
    setK1W->setFocusPolicy ( Qt::NoFocus );
    setK1W->setFixedSize    (QSize(COMBOBOXWIDTH,24) );
    setK1W->clearFocus     ();

    connect(setK1W,      SIGNAL   (valueChanged        (double)), 
            this,                SLOT     (setAntK1     (double)) );

    QDoubleSpinBox *setK2W = new QDoubleSpinBox(0);
	setK2W->setMinimum    (    0.0);
    setK2W->setMaximum    (    0.99);
    setK2W->setDecimals   (    2);
    setK2W->setAlignment  ( Qt::AlignLeft);
    setK2W->setSuffix     (" ");
    setK2W->setValue       ( (double)_settings->antK2); 
    setK2W->setSingleStep  ( 0.05);
    setK2W->setFocusPolicy ( Qt::NoFocus );
    setK2W->setFixedSize    (QSize(COMBOBOXWIDTH,24) );
    setK2W->clearFocus     ();

    connect(setK2W,      SIGNAL   (valueChanged        (double)), 
            this,        SLOT     (setAntK2     (double)) );

 
	
	QDoubleSpinBox *defaultCellsPerYearSB = new QDoubleSpinBox(0);
	defaultCellsPerYearSB->setMinimum    (    1.0);
    defaultCellsPerYearSB->setMaximum    (    10.0);
    defaultCellsPerYearSB->setDecimals   (    0);
    defaultCellsPerYearSB->setAlignment  ( Qt::AlignLeft);
    defaultCellsPerYearSB->setSuffix     (" ");
    defaultCellsPerYearSB->setValue       ( (double)_settings->defaultCellsPerYear); 
    defaultCellsPerYearSB->setSingleStep  ( 1);
    defaultCellsPerYearSB->setFocusPolicy ( Qt::NoFocus );
    defaultCellsPerYearSB->setFixedSize    (QSize(COMBOBOXWIDTH,24) );
    defaultCellsPerYearSB->clearFocus     ();
	
    connect(defaultCellsPerYearSB,      SIGNAL   (valueChanged        (double)), 
            this,                       SLOT     (setDefaultCellsPerYear     (double)) );
	
	
	
	QDoubleSpinBox *defaultAgeBandSB = new QDoubleSpinBox(0);
	defaultAgeBandSB->setMinimum    (    1.0);
    defaultAgeBandSB->setMaximum    (    10.0);
    defaultAgeBandSB->setDecimals   (    0);
    defaultAgeBandSB->setAlignment  ( Qt::AlignLeft);
    defaultAgeBandSB->setSuffix     (" ");
    defaultAgeBandSB->setValue       ( (double)_settings->defaultAgeBands); 
    defaultAgeBandSB->setSingleStep  ( 1);
    defaultAgeBandSB->setFocusPolicy ( Qt::NoFocus );
    defaultAgeBandSB->setFixedSize    (QSize(COMBOBOXWIDTH,24) );
    defaultAgeBandSB->clearFocus     ();
	
    connect(defaultAgeBandSB,      SIGNAL   (valueChanged        (double)), 
            this,                       SLOT     (setDefaultAgeBand     (double)) );
	
	
	
	
	
	
 
 	QComboBox *flipPBW = new QComboBox (this);
	flipPBW->addItem("NO");
    flipPBW->addItem("YES");
    flipPBW->setCurrentIndex ( _settings->flipPBuffer );
    flipPBW->setFixedSize    ( QSize(COMBOBOXWIDTH+6,24) );
	connect(flipPBW, SIGNAL(activated(int)), this, SLOT(setflipPBuffer (int)));
 
 
 
	int col = 0;
	   
	
	
	mainlayout->addWidget (defaultAgeBandL,      col,0 );   mainlayout->addWidget (defaultAgeBandSB,      col, 1 );   col++; 
	mainlayout->addWidget (defaultCellsPerYearL,      col,0 );   mainlayout->addWidget (defaultCellsPerYearSB,      col, 1 );   col++; 
	
	mainlayout->addWidget (numberOfAntsL,      col,0 );   mainlayout->addWidget (setNumberOfAntsW,      col, 1 );   col++; 
	mainlayout->addWidget (minSpeedL,          col,0 );   mainlayout->addWidget (setminAntSpeedW,          col, 1 );   col++; 
	mainlayout->addWidget (maxSpeedL,		   col,0 );   mainlayout->addWidget (setmaxAntSpeedW,      col, 1 );   col++; 
	mainlayout->addWidget (k1L,				   col,0 );   mainlayout->addWidget (setK1W,    col, 1 );   col++; 
	mainlayout->addWidget (k2L,                col,0 );   mainlayout->addWidget (setK2W,     col, 1 );   col++;
	mainlayout->addWidget (fBL,                col,0 );   mainlayout->addWidget (flipPBW,     col, 1 );   col++;




	QVBoxLayout *windowLayout = new QVBoxLayout; 
	
	windowLayout->setMargin(12);
	windowLayout->addLayout (mainlayout); 
	windowLayout->addStretch(4);    
    
	setLayout (windowLayout);
}

// -------------------------------------------------------------------------------
QSize GVoptionsDialog::minimumSizeHint() const {
	return QSize(100, 100);
}

// -------------------------------------------------------------------------------
QSize GVoptionsDialog::sizeHint() const {
	return QSize(100, 100);
}

// -------------------------------------------------------------------------------
void  GVoptionsDialog::setNumberOfAnts     ( double v)    {
	_settings->numberOfAnts = (int)v;
	antSettingsChanged();
}

void  GVoptionsDialog::setMinAntSpeed  (double value)
{
    _settings->antMinSpeed = value ;
    antSettingsChanged();
}

void  GVoptionsDialog::setMaxAntSpeed  (double value)
{
    _settings->antMaxSpeed = value ;
    antSettingsChanged();
}

void  GVoptionsDialog::setAntK1  (double value)
{
    _settings->antK1 = value ;
    antSettingsChanged();
}

void  GVoptionsDialog::setAntK2  (double value)
{
    _settings->antK2 = value ;
    antSettingsChanged();
}

void   GVoptionsDialog::setflipPBuffer					( int v) 
{
	_settings->flipPBuffer = v;
}

void  GVoptionsDialog::setDefaultCellsPerYear  (double value)
{
    _settings->defaultCellsPerYear = value ;
}

void  GVoptionsDialog::setDefaultAgeBand  (double value)
{
    _settings->defaultAgeBands = value ;
}




