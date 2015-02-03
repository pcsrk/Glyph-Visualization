/*
 *  GVattributeSelectionWidget.h
 *  diseaseViewer
 *
 *  Created by mue on 21.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVattributeSelectionWidget_H
#define GVattributeSelectionWidget_H

#include <QColor>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>


#include <QMouseEvent>

#include "GVattributeType.h"

class GVattributeSelectionWidget: public QWidget
{
    
    Q_OBJECT
    
public:
    
     GVattributeSelectionWidget (  GVattributeType *attributeType,
								   QWidget         *parent = 0);
	 
	 
	 void highlightValue (long index);

public slots:

    void showNaV  (bool v);
	void distanceWeightFieldTextChanged (QString t);

signals:

	void updateSelection ( );
	void showMessage     ( QString m, int t);


protected:
    
    void paintEvent				( QPaintEvent *event);
	void mousePressEvent		( QMouseEvent *event);
	void mouseDoubleClickEvent	( QMouseEvent *event); 
    void mouseReleaseEvent		( QMouseEvent *event);
    void mouseMoveEvent			( QMouseEvent *event);
    void leaveEvent				( QEvent      *event );
	
private:
   
    GVattributeType *_attributeType; 
	QString          _valueS;
	long             _currenIndex;
   
    bool             _considerNaVinDistribution;
	int              _navDistance;
	int              _bb;
	int              _tb;
	int              _lb;
	int              _rb;
	int				 _selectorWidth;

	long			 _minIndex;
	long			 _maxIndex;
	long			 _highlightIndex;
	qreal			 _minLinePosY;
	qreal			 _maxLinePosY;
	bool			 _minLineMoving;
	bool			 _maxLineMoving;
	bool             _minMaxMoving;
	int			     _currentY;
    int dx;
    int dy;
  
	QLineEdit        *_distanceWeightField;
  
    QColor    fontWhite;
    QColor    fontNonZero;
    QColor    red;
    QColor    background;
    QColor    lightGrey;
    QColor    darkGrey;
    
};
#endif