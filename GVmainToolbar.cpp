/*
 *  GVanimationMaster.cpp
 *  diseaseViewer
 *
 *  Created by mue on 24.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVmainToolbar.h"

#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QString>
#include <QLabel>


//------------------------------------------------------------------------------
GVmainToolbar::GVmainToolbar (  QWidget           *parent,
								QList<QAction *>   moveSelectedToAreaA) : QWidget(parent) {


#ifdef __APPLE__
	const int macKorr = 7;
#else
	const int macKorr = 0;
#endif

	const int nOfSelectionGroups = 4;
	const int nOfSnapshots       = 4;
	const int lineditsize        = 78;
  
    const int buttonsize         = 18;
	// generate the GUI Elements
	int col = 0;
	int row = 0;
	
	col += 36;
	QPushButton *pb; 	
	pb =      new QPushButton (this);
	pb->setFixedSize (QSize (36,36));  	

	QIcon openI (":/icons/open.png");
	pb->setIcon  (openI);  
	pb->setIconSize        ( QSize (36,36) );
  	
	pb->move(col, row);
	connect(pb,     SIGNAL(pressed()),  this,    SIGNAL (open()) );

	col += 36;
	col += 12;


	readButton =      new QPushButton (this);
	readButton->setFixedSize (QSize (36,36));  
	readButton->move(col, row);
	
	QIcon readI (":/icons/read.png");
	readButton->setIcon  (readI);  
	readButton->setIconSize        ( QSize (36,36) );
 
	
	connect(readButton,     SIGNAL(pressed()),  this,    SIGNAL (read ()) );
	arw.append ((QWidget*) readButton);

	col += 36;
	col += 12;


	int lineditsizeX = lineditsize + 2*buttonsize;

    whereTextEdit = new QLineEdit (this);
	whereTextEdit->setFixedSize (QSize (3 * lineditsizeX, buttonsize));  
	whereTextEdit->move(col, buttonsize);
	arw.append ((QWidget*) whereTextEdit);


	QComboBox *cbdefault = new QComboBox (this);
	QStringList cbdefaultItems; 
    // http://www.dimdi.de/static/de/klassi/diagnosen/icd10/htmlamtl2006/fr-icd.htm?navi.htm+c00

	selectionH["--"]         = "Alle";

	selectionH["00"]         = "Lippe";
	selectionH["01"]         = "Zungengrund";
	selectionH["02"]         = "Zunge";
	selectionH["03"]         = "Zahnfleisch";
	selectionH["04"]         = "Mundboden";
	selectionH["05"]         = "Gaumen";
	selectionH["06"]         = "Mund";
	selectionH["07"]         = "Parotis";
	selectionH["08"]         =  tr("Speicheldrüsen");
	selectionH["09"]         = "Tonsille";
	selectionH["10"]         = "Oropharynx";
	
	selectionH["11"]         = "Nasopharynx";
	selectionH["12"]         = "Recessus piriformis";
	selectionH["13"]         = "Hypopharynx";
	selectionH["14"]         = tr("Lippe, Mundhöhle, Pharynx");
	selectionH["15"]         = tr("Ösophagus");
	selectionH["16"]         = "Magen";
	selectionH["17"]         = tr("Dünndarm");
	selectionH["18"]         = "Kolon";
	selectionH["19"]         = "Rektosigmoid";
	selectionH["20"]         = "Rektum";
	
	selectionH["21"]         = "Anus, Analkanal";
	selectionH["22"]         = tr("Leber, intrahepatischen Gallengänge");
	selectionH["23"]         = "Gallenblase";
	selectionH["24"]         = "Gallenwege";
	selectionH["25"]         = "Pankreas";
	selectionH["26"]         = "Verdauungsorgane";
	
	selectionH["30"]         = tr("Nasenhöhle, Mittelohr");
	selectionH["31"]         = tr("Nasennebenhöhlen");
	selectionH["32"]         = tr("Larynx");
	selectionH["33"]         = tr("Trachea");
	selectionH["34"]         = tr("Bronchien, Lunge");
	selectionH["37"]         = tr("Thymus");
	selectionH["38"]         = tr("Herzen, Mediastinum, Pleura");
	selectionH["39"]         = tr("Atmungssystem");
	
	selectionH["40"]         = tr("Knochen, Gelenkknorpel d. Extremitäten");
	selectionH["41"]         = tr("Knochen, Gelenkknorpel");

	selectionH["50"]         = tr("Brustdrüse [Mamma]");
	selectionH["51"]         = "Vulva";
	selectionH["52"]         = "Vagina";
	selectionH["53"]         = "Cervix uteri";
	selectionH["54"]         = "Corpus uteri";
	selectionH["55"]         = "Uterus";
	selectionH["56"]         = "Ovar";
	selectionH["57"]         = "webl. Genitalorgane";
	selectionH["58"]         = "Plazenta";
	
	selectionH["60"]         = "Penis";
	selectionH["61"]         = "Prostata";
	selectionH["62"]         = "Hoden";
	selectionH["63"]         = tr("männl. Genitalorgane");
	selectionH["64"]         = "Niere, ausg. Nierenbecken";
	selectionH["65"]         = "Nierenbecken";
	selectionH["66"]         = "Ureter";
	selectionH["67"]         = "Harnblase";
	selectionH["68"]         = "Harnorgane";
	selectionH["69"]         = "Auges, Augenanhangsgebilde";
	
	selectionH["70"]         = "Meningen";
	selectionH["71"]         = "Gehirns";
	selectionH["72"]         = tr("Rückenmark, Hirnnerven");
	selectionH["73"]         = tr("Schilddrüse");
	selectionH["74"]         = "Nebenniere";
	selectionH["75"]         = tr("endokrine Drüsen und verwandte Strukturen");
	selectionH["76"]         = "sonstiger und ungenau bezeichnete Lokalisationen";
	selectionH["77"]         = "Lymphknoten";
	selectionH["78"]         = "Atmungs- und Verdauungsorgane";
	selectionH["79"]         = tr("Sekundäre bösartige Neubildung an sonstigen Lokalisationen");

	selectionH["80"]         = "ohne Angabe der Lokalisation";
	selectionH["81"]         = "lymphatischen, blutbildenden und verwandten Gewebe";
	selectionH["82"]         = tr("Follikuläres [noduläres] Non-Hodgkin-Lymphom");
	selectionH["83"]         = "Diffuses Non-Hodgkin-Lymphom";
	selectionH["84"]         = "Periphere und kutane T-Zell-Lymphome";
	selectionH["85"]         = "Non-Hodgkin-Lymphoms";
	selectionH["88"]         = tr("Bösartige immunproliferative Krankheiten");
	selectionH["90"]         = "Plasmozytom und b√∂sartige Plasmazellen-Neubildungen";

	selectionH["51"]         = tr("Lymphatische Leukämie");
	selectionH["52"]         = tr("Myeloische Leukämie");
	selectionH["53"]         = tr("Monozytenleukämie");
	selectionH["54"]         = tr("Sonstige Leukämien näher bezeichneten Zelltyps");
	selectionH["55"]         = tr("Leukämie nicht näher bezeichneten Zelltyps");
	selectionH["56"]         = tr("sonst. bösartige Neubildungen d. lymphatischen blutbildenden und verwandten Gewebes");
	selectionH["57"]         = tr("Neubildungen als Primärtumoren an mehreren Lokalisationen");
	selectionH["58"]         = "Plazenta";
		
	
	QMapIterator<QString, QString> i(selectionH);
	QString whereClause;;
	QString newKey;
	while (i.hasNext()) {
     i.next();
	 if (i.key().contains("--",  Qt::CaseInsensitive)  ) {
	     newKey = "--";
		 whereClause =  "";
	 }
	 else {
		whereClause =  "where topologyid like '%C" + i.key() + "%'";
		newKey = "C." + i.key() + " " + i.value();
	    }
	 selectionLH [newKey] = whereClause;
	 cbdefault->addItem (newKey);
	}
		
	arw.append ((QWidget*) cbdefault);
	
	cbdefault->setCurrentIndex (38);
	whereTextEdit->setText ( selectionLH [cbdefault->currentText()]);
	defineWhere (selectionLH [cbdefault->currentText()]);

	cbdefault->setFixedSize (QSize (2*lineditsizeX + macKorr, buttonsize));  
	cbdefault->move(col-macKorr/2, row);
	connect(cbdefault, SIGNAL( activated(const QString &)), this, SLOT ( setWhereString (const QString &)));

	connect(whereTextEdit, SIGNAL( textChanged(const QString &)), this, SLOT ( changeWhereString (const QString &)));

	
	QComboBox *cbmaxElements = new QComboBox (this);
	QStringList cbmaxItems; 
	// achtung wenn die liste ge√§ndert wird dann auch die callback funktion √§ndern
	cbmaxItems << "max 100" << "max 500" << "max 1000" << "max 5000" << "max 10.000" << "max 20.000" << "max 30.000" << "max 50.000" << "unlimted";
	
	cbmaxElements->addItems (cbmaxItems);
	cbmaxElements->setFixedSize (QSize (lineditsizeX + macKorr, buttonsize));  
	cbmaxElements->move(2*lineditsizeX+ col-macKorr/2, row);

	maxEH[0] = 100;
	maxEH[1] = 500;
	maxEH[2] = 1000;
	maxEH[3] = 5000;
	maxEH[4] = 10000;
	maxEH[5] = 20000;
	maxEH[6] = 30000;
	maxEH[7] = 50000;
	maxEH[8] = 1000000;

	setMaxElementsS (maxEH[2]);
	cbmaxElements->setCurrentIndex (2);
	
	connect(cbmaxElements, SIGNAL( activated(int)), this, SLOT ( setMaxElements (int)));

	arw.append ((QWidget*) cbmaxElements);


	col += 3*lineditsizeX;
	col += 12;



#if 0
	pb =      new QPushButton ("ANT", this);
	pb->setFixedSize (QSize (36,36));  
	pb->move(col, row);

	col += 36;
	col += 12;
#endif 



   
	for (int i = 0; i<nOfSelectionGroups; i++) {

	
		QLineEdit *le = new QLineEdit (this);
		areaNames.append (le);
		le->setText ("area  " + QString::number (i));
		le->setFixedSize (QSize (lineditsize+buttonsize, buttonsize));  
		le->move(col, buttonsize);

		ase.append ((QWidget*) le);

		QPushButton *glp = new QPushButton ("+", this);
		glp->setFixedSize (QSize (buttonsize, buttonsize));  
		glp->move(col+lineditsize+buttonsize, buttonsize);
		connect(glp,     SIGNAL(pressed()),   moveSelectedToAreaA[i],    SLOT (trigger()) );
		glp->setFlat (false);

		ase.append ((QWidget*) glp);

#if 0
		QPushButton *glm = new QPushButton ("-", this);
		glm->setFixedSize  (QSize (buttonsize,buttonsize));   
		glm->move(col+lineditsize+buttonsize, buttonsize);
		glm->setFlat (false);

		ase.append ((QWidget*) glm);
#endif
		
		QComboBox *cb = new QComboBox (this);
		movCB.append (cb);


 		positionString[kRandomPosition]  = "random";
 		positionString[kMoveToCenter]    = "center";
		positionString[kMoveToRectange]  = "rectangle";
//		positionString[kMoveToYAS]		 = "year age staging";
		positionString[kMoveToYAS_Single]		 = "year age staging single";
		positionString[kMoveToYAS_Multiple]		 = "year age staging multiple";
		
     	positionString[kAntAnimtion]	 = "ant clustering";
	
		QMapIterator<int, QString> iterator(positionString);
		while (iterator.hasNext()) {
			iterator.next();
			cb->addItem (iterator.value());
		}	
	
		
		cb->setCurrentIndex  (kMoveToRectange);
		cb->setFixedSize (QSize (lineditsize+ 2*buttonsize + macKorr, buttonsize));  
		cb->move(col-macKorr/2, row);
		ase.append ((QWidget*) cb);

		
		col += lineditsize+ 2*buttonsize;
		col += buttonsize;

	}


	testMiningTextEdit = new QLineEdit (this);
	testMiningTextEdit->setFixedSize (QSize (col - 36-16, 24));  
	testMiningTextEdit->move(36, 2*buttonsize + 6+28);
	testMiningTextEdit->setAlignment (Qt::AlignTop);
	
	connect(testMiningTextEdit, SIGNAL( textChanged(QString)), this, SLOT ( selectionTextChanged (QString)));
	ase.append ((QWidget*) testMiningTextEdit);


	onlyPOFieldsCB = new QCheckBox ("Only PO", this);
	onlyPOFieldsCB->setFixedSize (QSize (100, 24));  
	onlyPOFieldsCB->move(36, 2*buttonsize + 6 );
	arw.append ((QWidget*) onlyPOFieldsCB);

	
	onlyWithTStagingCB = new QCheckBox ("With T St.", this);
	onlyWithTStagingCB->setFixedSize (QSize (100, 24));  
	onlyWithTStagingCB->move(36 + 100, 2*buttonsize + 6);
	arw.append ((QWidget*) onlyWithTStagingCB);

	
	negationCB = new QCheckBox ("Exclude Negations", this);
	negationCB->setFixedSize (QSize (150, 24));  
	negationCB->move(36 + 200, 2*buttonsize + 6);
	arw.append ((QWidget*) negationCB);

	negationTextEdit = new QLineEdit (this);
	negationTextEdit->setFixedSize (QSize (col - 400, 24));  
	negationTextEdit->move(34+350, 2*buttonsize + 6);
	negationTextEdit->setAlignment (Qt::AlignTop);
	arw.append ((QWidget*) negationTextEdit);


	connect(movCB[0],     SIGNAL( activated(int)),  this,    SLOT (setMovStratArea0(int)) );
	connect(movCB[1],     SIGNAL( activated(int)),  this,    SLOT (setMovStratArea1(int)) );
	connect(movCB[2],     SIGNAL( activated(int)),  this,    SLOT (setMovStratArea2(int)) );
    connect(movCB[3],     SIGNAL( activated(int)),  this,    SLOT (setMovStratArea3(int)) );


	areaNames[0]->setText ("DEFAULT");
	setFixedHeight (2*buttonsize+ 24 +  12+6 + 30);
	
	
	
}
	 
void GVmainToolbar::updateAreaMovementStrategieInMainWindow () {
     for (int i = 0; i<4; i++) {
		setAreaMovementStrategie (i, kMoveToCenter);
	}
}
	 
//------------------------------------------------------------------------------
QSize GVmainToolbar::minimumSizeHint() const{
	return QSize (400,150);
}

//------------------------------------------------------------------------------
QSize GVmainToolbar::sizeHint() const {
	return QSize (700,150);
}

//------------------------------------------------------------------------------
void GVmainToolbar::enableReadButton   (bool s) {
	for (int i = 0; i<arw.size(); i++) {
		arw[i]->setEnabled (s);
	}
}

//------------------------------------------------------------------------------

void  GVmainToolbar::enableSelectinMenu (bool s) {
	for (int i = 0; i<ase.size(); i++) {
		ase[i]->setEnabled (s);
	}
}


//------------------------------------------------------------------------------
//----------------------      Callbacks      -----------------------------------


void GVmainToolbar::setMovStratArea0 (int v) { setAreaMovementStrategie (0, v); }
void GVmainToolbar::setMovStratArea1 (int v) { setAreaMovementStrategie (1, v); }
void GVmainToolbar::setMovStratArea2 (int v) { setAreaMovementStrategie (2, v); }
void GVmainToolbar::setMovStratArea3 (int v) { setAreaMovementStrategie (3, v); }

void GVmainToolbar::setWhereString (const QString &v) {

	whereTextEdit->setText ( selectionLH [v] );
	defineWhere ( selectionLH [v]);
	qDebug ("GVmainToolbar::setWhereString  %s", qPrintable (v));
}

void GVmainToolbar::changeWhereString (const QString &v) {
	defineWhere (v);
	qDebug ("GVmainToolbar::changeWhereString  %s", qPrintable (v));
}


//------------------------------------------------------------------------------
void GVmainToolbar::setMaxElements (int v) {
	qDebug ("GVmainToolbar::setMaxElements  %d", v);
	setMaxElementsS (maxEH[v]);
}

//------------------------------------------------------------------------------
void GVmainToolbar::selectionTextChanged (QString text) {
	qDebug ("GVmainToolbar::setMaxEselectionTextChanged text = %s", qPrintable (text));
    search (text);
}


