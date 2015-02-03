/*
 *  GVattributeSelectionWidget.cpp
 *  diseaseViewer
 *
 *  Created by mue on 21.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVattributeSelectionWidget.h"
#include <QPainter>


//------------------------------------------------------------------------------
GVattributeSelectionWidget::GVattributeSelectionWidget ( GVattributeType *attributeType, 
														 QWidget         *parent) : QWidget (parent) {
	
	_attributeType = attributeType;
	
	setFocusPolicy(Qt::ClickFocus);
	
	red         =  QColor(191,91,91);
 
    background  =  QColor(91,91,91);
    darkGrey    =  QColor(64,64,64);
    lightGrey   =  QColor(75,75,75);    
    
    fontWhite   =  QColor(134,134,134);    
    fontNonZero =  QColor(157,135,67);    
	setMouseTracking(true);
	
	_currenIndex    = -99;
	_bb             =  6;
	_tb             = 16;
	_lb             = 30;
	_rb             =  6;
	_selectorWidth =  12;

	_considerNaVinDistribution = false;

	_minIndex = 0;
	_maxIndex = _attributeType->numberOfElements() - 1;

	_highlightIndex = -99;
	
	_minLineMoving = false;
	_maxLineMoving = false;
	_minMaxMoving = false;

	_distanceWeightField = new QLineEdit ("0,0", this);
	_distanceWeightField->setFont(QFont("Helvetica",10));
	
	_distanceWeightField->setAlignment (Qt::AlignRight);
	_distanceWeightField->setText (QString::number ((double)attributeType->distanceWeight(), 'f', 3));

	connect(_distanceWeightField,   SIGNAL(textChanged(QString)),  this, SLOT   (distanceWeightFieldTextChanged(QString)) );

	
}

//------------------------------------------------------------------------------
void GVattributeSelectionWidget::showNaV (bool v) {
	_considerNaVinDistribution = v;
}

//------------------------------------------------------------------------------

void GVattributeSelectionWidget::distanceWeightFieldTextChanged (QString t) {
	qreal w = t.toDouble();
	_attributeType->setDistanceWeight (w);
	qDebug ("new distaceWeight %f",w);
 }


//------------------------------------------------------------------------------
void GVattributeSelectionWidget::highlightValue (long index) {
	_highlightIndex = index;
	update();
}

//------------------------------------------------------------------------------
void GVattributeSelectionWidget::paintEvent(QPaintEvent *event) {

	bool leftToRight = false;
    
	int w = this->width  ();
    int h = this->height ();
	
	
	int    fs = 9;

	_distanceWeightField->setFixedSize (w-30, 14);
	_distanceWeightField->move         (30, h-14);
	

	h = h-16;
	
	_minIndex =  _attributeType->minIndex();
	_maxIndex =  _attributeType->maxIndex();

    _navDistance = 12;

	QPainter painter(this);
	painter.setRenderHint(	QPainter::Antialiasing, true);
	
    QFont textFont ("Helvetica", fs ); 
    painter.setPen( fontWhite );  
    painter.setFont(QFont("Helvetica",fs));        
   	painter.setPen   (red);
    
	QString title =  _attributeType->name();
	QString info;
	if (_currenIndex >= -1) title += " = " + _attributeType->stringValue(_currenIndex); 
								 
	if (_currenIndex >= -1) info = _attributeType->name() + 
								 + ": for value  "
								 + _attributeType->stringValue(_currenIndex) 
								 + "  we have "
								 + QString::number (_attributeType->distValueAt(_currenIndex))
								 + " overall and "
								 + QString::number (_attributeType->distSValueAt(_currenIndex))
								 + " selected elements equiv. to "
								 + QString::number (100 * (qreal) _attributeType->distValueAt(_currenIndex) / _attributeType->numberOfDistValues(), 'f', 1  )
								 + "%, "
								 + QString::number (100 * (qreal) _attributeType->distSValueAt(_currenIndex) / _attributeType->numberOfDistSValues(), 'f', 1  )
								 + "%";


//	if (!_maxLineMoving && !_minLineMoving) showMessage (info, 0);
	
   
    
	painter.drawText (  QRect( 0,    0,  w,  _tb),
                        Qt::AlignLeft | Qt::AlignTop,
                        title );
	
    int paramterH = 12;
    int paramterW = 28;
    int parameterIndex = 0;
 
 #if 0      
    painter.drawText (  QRect( 0,    _tb+parameterIndex*paramterH,  paramterW,  paramterH),
                        Qt::AlignLeft | Qt::AlignTop,
                        "dev" );
    parameterIndex++;                    
	
    painter.drawText (  QRect( 0,    _tb+parameterIndex*paramterH,  paramterW,  paramterH),
                        Qt::AlignLeft | Qt::AlignTop,
                        "mean" );
    parameterIndex++;                    
    
    painter.drawText (  QRect( 0,    _tb+parameterIndex*paramterH,  paramterW,  paramterH),
                        Qt::AlignLeft | Qt::AlignTop,
                        "#" );


    parameterIndex++;                    
#endif
    
    int starty =  _tb + parameterIndex * paramterH + 24;
    int distributionH = h - _bb - starty;
    
    
	painter.setPen   ( red);
    painter.setBrush ( lightGrey   );
	if (_considerNaVinDistribution) {
		painter.drawRect ( QRect(_lb, starty, w, distributionH));
	}
	else {
		painter.drawRect ( QRect(_lb, starty, w, distributionH -_navDistance));
		painter.setPen   ( lightGrey);
		painter.setBrush ( QColor( 150,150,150,255)  );
		painter.drawRect ( QRect(_lb, starty + distributionH -  _navDistance, w, _navDistance));	
	
	}
	
	int hfordist      =  distributionH - _navDistance;
	int numberOfTicks =  hfordist / 12;
	
	int noe = _attributeType->numberOfElements();  
	if (numberOfTicks > noe) numberOfTicks = noe;

	painter.drawText (  QRect( 0,   starty + distributionH - _navDistance, 28,  _navDistance),
							Qt::AlignRight | Qt::AlignVCenter,
							"NaV" );

	for (int i= 0; i<numberOfTicks; i++) {
	
		int valueIndex = _attributeType->numberOfElements() - noe *(i+1) / numberOfTicks;
		

		int y = starty + (i*hfordist)/numberOfTicks;
		if (leftToRight) 
			y = starty +  ((numberOfTicks-1)*hfordist)/numberOfTicks  -  (i*hfordist)/numberOfTicks;
		
		painter.drawText (  QRect( 0,    y, 28, hfordist/numberOfTicks),
                            Qt::AlignRight | Qt::AlignVCenter,
                            _attributeType->stringValue (valueIndex) );

	}

	qreal maxN;
	if (_considerNaVinDistribution) {
		maxN = _attributeType->maxEntryInDist();
		}
		else {
		maxN = _attributeType->maxEntryExeptNaV();
	}
	



	qreal dw = w - 30;
	qreal dh = (qreal) hfordist / qreal (noe);
	

	if (_attributeType->numberOfDistValues() > 0) {
	
        
            	
		qreal dist; 
		dist = 0.8 * _attributeType->distValueAt (-1) / maxN; // die NaVs;

		if (_considerNaVinDistribution) {
			painter.setPen   ( QColor( 0,0,0,100));
			painter.setBrush ( QColor( 91,91,191,255)  );
			painter.drawRect (QRectF (_lb, h - _bb - _navDistance, dw *dist,_navDistance) );
		}
		else {
			if (_attributeType->distValueAt(-1) > 0) {
				qreal perzentNaV = 100 * qreal (_attributeType->distValueAt(-1)) / _attributeType->numberOfDistValues();
				QString navInfo =  QString::number (_attributeType->distValueAt(-1)) + " - " +
					QString::number (perzentNaV,'f',1) + "%";
				
				painter.setFont(QFont("Helvetica",fs-1));        
				painter.setPen   (  QColor( 20,20,20,255) );
				
				painter.drawText (   QRectF (_lb+2, h - _bb - _navDistance+1, w-_lb-2, _navDistance),
									 Qt::AlignLeft | Qt::AlignVCenter,
									 navInfo );
			}
			
   		}
		
		// blaue balken = grundgesamtheit
    for (int i = 0; i< _attributeType->numberOfElements(); i++) {
			
			qreal nodstelements = _attributeType->distValueAt (i);
			if (nodstelements > 0) {
				dist = 0.8 * nodstelements / maxN; 
				qreal y = h - _bb - _navDistance - dh * (i+1);
                
                                				
				if ( (i >= _minIndex) && (i <= _maxIndex) ){
					painter.setBrush (  QColor( 91,91,191,255)  );
					if (dh > 5) painter.setPen (QColor( 0,0,0,100) ); else painter.setPen   ( QColor( 91,91,191,255));
				}
				else {
					painter.setBrush (  QColor( 91,91,191,200)   );				
					painter.setPen   (  QColor( 91,91,191,200)   );
				}
				painter.drawRect (QRectF (_lb, y, dw *dist, dh) );
			}
		}
	}

	// grüne balken = selectierte
	if (_attributeType->numberOfDistSValues() > 0) {
		
		qreal dist; 
		dist = 0.8 * _attributeType->distSValueAt (-1) / maxN; // die NaVs;
		
		if (_considerNaVinDistribution) {
			painter.setPen   (QColor( 0,0,0,100));
			painter.setBrush (QColor( 91,191,91,255)  );
			painter.drawRect (QRectF (_lb, h - _bb - _navDistance, dw *dist,_navDistance) );
		}
		
		for (int i = 0; i< _attributeType->numberOfElements(); i++) {
			
			qreal nodstelements = _attributeType->distSValueAt (i);
			if (nodstelements > 0) {
				dist = 0.8 * nodstelements / maxN; 
				qreal y = h - _bb - _navDistance - dh * (i+1);
				
				if ( (i >= _minIndex) && (i <= _maxIndex) ){
					painter.setBrush (  QColor( 91,191,91,255)  );
					if (dh > 5) painter.setPen (QColor( 0,0,0,100) ); else painter.setPen   ( QColor( 91,191,91,255));
				}
				else {
					painter.setBrush (  QColor( 75,100,75) );				
					painter.setPen   (  QColor( 75,100,75) );
				}
				painter.drawRect (QRectF (_lb, y, dw *dist, dh) );
			}
		}
	}
	
	if (_highlightIndex >= -1) {
		qreal dhlokal;
		qreal y;
		if (_highlightIndex == -1) dhlokal = _navDistance; else dhlokal = dh;
		qreal nodstelements = _attributeType->distValueAt (_highlightIndex);
		if (nodstelements > 0) {
		   qreal dist = 0.8 * nodstelements / maxN; 
		  
		   y = h - _bb - _navDistance - dh * (_highlightIndex+1);	
		   if ( (_highlightIndex >= _minIndex) && (_highlightIndex <= _maxIndex) ){
					painter.setBrush (  QColor( 200,100,110,150)  );
					if (dh > 5) painter.setPen (QColor( 0,0,0,100) ); else painter.setPen   (   QColor( 255,100,110,150)  );
				}
				else {
					painter.setBrush (  QColor( 191,91,91,200) );				
					painter.setPen   (  QColor( 191,91,91,200) );
				}
				
				painter.drawRect (QRectF (_lb, y, dw *dist, dhlokal) );
	      }
		nodstelements = _attributeType->distSValueAt (_highlightIndex);
		if (nodstelements > 0) {
		   qreal dist = 0.8 * nodstelements / maxN; 
		   qreal y = h - _bb - _navDistance - dh * (_highlightIndex+1);	
		   if ( (_highlightIndex >= _minIndex) && (_highlightIndex <= _maxIndex) ){
					painter.setBrush (  QColor( 255,100,110,200)  );
					if (dh > 5) painter.setPen (QColor( 0,0,0,100) ); else painter.setPen   (   QColor( 255,100,110,200)  );
				}
				else {
					painter.setBrush (  QColor( 100, 75,175) );				
					painter.setPen   (  QColor( 100, 75,175) );
				}
				painter.drawRect (QRectF (_lb, y, dw *dist, dhlokal) );
	      }
	}

	// min line
	int minDisplayIndex;
	int maxDisplayIndex;
    if (_minIndex <= _maxIndex) {
		minDisplayIndex = _minIndex;
		maxDisplayIndex = _maxIndex + 1;
	}
	else {
		minDisplayIndex = -1;
		maxDisplayIndex = -1;	
	}	

	qreal miny;
	qreal maxy;
	{
	
		if (minDisplayIndex == -1) {
			_minLinePosY = h - _bb;
		}
		else {
			_minLinePosY = h - _bb - _navDistance - dh * (minDisplayIndex); 
	    }
		if (_minLineMoving) {
			miny = _currentY;
			if (miny >  (h - _bb) ) miny = h - _bb;
		}
		else {
			miny = _minLinePosY;
		}
		painter.setPen      ( QColor(255, 0, 0, 200) );
		painter.drawLine (QLineF (_lb, miny, w, miny) );
		
		painter.setPen      ( QColor(255, 0, 0, 150) );
		painter.setBrush    ( QColor(255, 0, 0, 100) );
	

		const QPointF triangle[3] = {
			QPointF(w- _selectorWidth,  miny),
			QPointF(w,                  miny),
			QPointF(w,                  miny + _bb),
		};		
		painter.drawPolygon ( triangle, 3);
	    const QPointF triangleLeft[3] = {
			QPointF(_lb+_selectorWidth,  miny),
			QPointF(_lb+0,               miny),
			QPointF(_lb+0,               miny + _bb),
		};		
		painter.drawPolygon ( triangleLeft, 3);
	}
	
	// max line
	{
	
		if (maxDisplayIndex == -1) {
			_maxLinePosY = h - _bb;
		}
		else {
			_maxLinePosY = h - _bb - _navDistance - dh * (maxDisplayIndex); 
	    }
	    if (_maxLineMoving) {
			maxy = _currentY;
			if (maxy < (_tb) ) maxy =_tb;
		}
		else {
			maxy = _maxLinePosY;
		}

		painter.setPen      ( QColor(255, 0, 0, 200) );
		painter.drawLine (QLineF (_lb, maxy, w, maxy) );
		
		painter.setPen      ( QColor(255, 0, 0, 150) );
		painter.setBrush    ( QColor(255, 0, 0, 100) );
				
		const QPointF triangle[3] = {
			QPointF(w-_selectorWidth,  maxy),
			QPointF(w,                 maxy),
			QPointF(w,                 maxy - _bb),
		};		
		painter.drawPolygon ( triangle, 3);
	    const QPointF triangleLeft[3] = {
			QPointF(_lb+_selectorWidth,  maxy),
			QPointF(_lb+0,               maxy),
			QPointF(_lb+0,               maxy - _bb),
		};		
		painter.drawPolygon ( triangleLeft, 3);

	}	
	
	
}
//------------------------------------------------------------------------------
void GVattributeSelectionWidget::mousePressEvent	( QMouseEvent *event) {

		if (  ( event->y() > _minLinePosY) && ( event->y() < _minLinePosY +2*_bb) ){ _minLineMoving = true;}
		if (  ( event->y() < _maxLinePosY) && ( event->y() > _maxLinePosY -2*_bb) ){ _maxLineMoving = true;}
}
//------------------------------------------------------------------------------

void GVattributeSelectionWidget::mouseReleaseEvent	( QMouseEvent *event) {
	_minLineMoving = false;
	_maxLineMoving = false;
}

//------------------------------------------------------------------------------
void GVattributeSelectionWidget::mouseMoveEvent		( QMouseEvent *event) {
	
	int index;
	int indexVonOben; 
    double mouseX =  event->x();    
	double mouseY =  event->y();
	
	_currentY = event->y();

	index = -99;
	if (  (event->y() > ( this->height () - _navDistance )) ) {
		index = -1;
	}
	else {
		qreal hfordist = this->height () - _tb - _bb - _navDistance;
		int   noe = _attributeType->numberOfElements();
		qreal dh = (qreal) hfordist / noe;		
		indexVonOben =    (mouseY - _tb) / dh;
		index=   _attributeType->numberOfElements() - indexVonOben - 1;
	}
	
	
	_currenIndex = index;

    if (_currenIndex >= -1) {
		if (_minLineMoving) {
			_minIndex = _currenIndex;
			
			if ( !_attributeType->isIndexRangeEmpty() && (_minIndex > _attributeType->maxIndex() ))	{
				_minIndex =   _attributeType->maxIndex ();
			}
			if (_minIndex > _attributeType->maxMaxIndex() ) _minIndex =  _attributeType->maxMaxIndex();
	
			_attributeType->setMinIndex (_minIndex);			
			updateSelection ();

		}
		if (_maxLineMoving) {
			_maxIndex = _currenIndex;			
			if (! _attributeType->isIndexRangeEmpty() ) {
				if (_maxIndex < _attributeType->minIndex () )	{
					_maxIndex =   _attributeType->minIndex ();
				}
			}
			else {
					_minIndex = -1;
					_attributeType->setMinIndex (_minIndex);
			}
			if (_maxIndex > _attributeType->maxMaxIndex() ) _maxIndex =  _attributeType->maxMaxIndex();

			if (_attributeType->isIndexRangeEmpty() ) qDebug ("*** 2 Index range is empty MI = %d", _maxIndex);
			_attributeType->setMaxIndex (_maxIndex);
		    
			updateSelection ();
		}
	}
	
//	qDebug ("GVattributeSelectionWidget -> mouse pointer at position %f, %f  -- %d %d ", mouseX,  mouseY, indexVonOben, index);
	update();
}

//------------------------------------------------------------------------------
void GVattributeSelectionWidget::mouseDoubleClickEvent	( QMouseEvent *event) {


	int index;
	int indexVonOben; 
    double mouseX =  event->x();    
	double mouseY =  event->y();
	
	_currentY = event->y();

	// reset the slider
	if (event->y() < _tb) {
		_currenIndex = 0;
		_minIndex = 0;
		_maxIndex = _attributeType->maxMaxIndex();	
		_attributeType->setMinIndex (_minIndex);
		_attributeType->setMaxIndex (_maxIndex);
	    updateSelection ();	
	}

	index = -99;
	if ( (event->y() > ( this->height () - _navDistance )) ) {
		index = -1;
	}
	else {
		qreal hfordist = this->height () - _tb - _bb - _navDistance;
		int   noe = _attributeType->numberOfElements();
		qreal dh = (qreal) hfordist / noe;		
		indexVonOben =    (mouseY - _tb) / dh;
		index=   _attributeType->numberOfElements() - indexVonOben - 1;
	}
	
	
	if ((index > -1) & (index <= _attributeType->maxMaxIndex())) {
		_currenIndex = index;
		_minIndex = _currenIndex;
		_maxIndex = _currenIndex;	
		_attributeType->setMinIndex (_minIndex);
		_attributeType->setMaxIndex (_maxIndex);
	    updateSelection ();
	}
		
//	qDebug ("GVattributeSelectionWidget -> mouse pointer at position %f, %f  -- %d %d ", mouseX,  mouseY, indexVonOben, index);
	update();


}


//------------------------------------------------------------------------------
void GVattributeSelectionWidget::leaveEvent         ( QEvent * event ) {
	_currenIndex = -99;
	update();
//	showMessage ("", 0);
}


