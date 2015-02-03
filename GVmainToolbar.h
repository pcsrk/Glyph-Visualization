/*
 *  GVmainToolbar.h
 *  diseaseViewer
 *
 *  Created by mue on 24.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVmainToolbar_H
#define GVmainToolbar_H

#include <QColor>
#include <QWidget>
#include <QList>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>




#include <QAction>
#include <QHash>


#include "GVglobals.h"

class GVmainToolbar: public QWidget
{
    
    Q_OBJECT
    
public:
    
     GVmainToolbar (  QWidget           *parent,
					  QList<QAction *>   moveSelectedToAreaA);
	 
    QSize minimumSizeHint() const;
	QSize sizeHint() const;
	 
	QCheckBox *negationCB;
	QCheckBox *onlyWithTStagingCB;
	QCheckBox *onlyPOFieldsCB;
	
	
	
public slots:

	void setWhereString (const QString & v);
	void changeWhereString (const QString & v);

	void setMaxElements (int v);

	void enableReadButton   (bool s);
	void enableSelectinMenu (bool s);
	
	void setMovStratArea0 (int s);
	void setMovStratArea1 (int s);
	void setMovStratArea2 (int s);
	void setMovStratArea3 (int s);

	void updateAreaMovementStrategieInMainWindow();		
	
	
	void selectionTextChanged (QString text);

signals:

	 void setToGroup          (int n);
	 void removeFromGroup     (int n);
	 void groupIsSelected     (int n);

	 void generateYearAgeStagingView ();
	 void generateRandomView         ();
			
 	 void makeASnapshot       (int n);
	 void restoreSnapShot     (int n);

	 void toggleAutoMove      ();
	 void startAntAnimation   ();
	 
	 void setMaxElementsS     (int n);

	 void setAreaMovementStrategie (int a, int s);

	 void open ();
	 void read ();
	 void defineWhere (const QString & v);

	 void search (const QString & v); 
	 	
	
protected:
    
	
private:

	QHash <int,int>          maxEH;

	QMap <int, QString>  positionString;


	QMap <QString,QString>  selectionH;
	QMap <QString,QString>  selectionLH;


	QPushButton *readButton;

	QLineEdit *whereTextEdit; 
	QLineEdit *testMiningTextEdit; 
	QLineEdit *negationTextEdit; 

	
	QList <QWidget   *> arw;      // all selection widgets			
	QList <QWidget   *> ase;      // all selection widgets

	QList <QLineEdit *> areaNames;
	QList <QComboBox *> movCB;
	QList <QComboBox *> posSel;

};
#endif