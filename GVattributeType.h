/*
 *  GVattribute.h
 *  diseaseViewer
 *
 *  Created by mue on 21.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GVattributeType_H
#define GVattributeType_H


#include <QString>
#include <QHash>
#include <QMap>

class GVattributeType 
{
    
public:
    
    GVattributeType (QString attributeName);
    
	
	enum    {	kNoScale			= 1,
				kDiscreteScale		= 2,
				kContinousScale		= 3,
				kCategoricalScale	= 4,
				kOrdinalScale		= 5,
				kIntervalScale		= 6,
				kNominalScale		= 7
   	        };
	
	QString name ()             {return _name;}
	long    numberOfElements () {return _numberOfDiscreteValues; } 
	 
	
	void appendValue     (QString valueS, qreal valueR = 0.0);
	void setNominalValue (QString valueS, qreal valueR);

	
	QString		stringValue        (int     index) {return valueAsStringM[index]; }
	qreal		nominalValue       (int     index) {return nominalValueM [index]; }
	long        elementsAtIndex    (int     index) {return distributionM [index]; }
	long		indexOf            (QString valueS);

	qreal       nominalRange       () {return (_maxNominalValue - _minNominalValue); }


	void        resetDistribution ();
	void        incDistribution   (long index); 
	long        distValueAt       (long index) {return distributionM[index]; }
	long        numberOfDistValues()           {return _numberOfValuesInDist; }
	long        maxEntryInDist    ()           {return _maxEntryInDist; }
	long        maxEntryExeptNaV  ()           {return _maxEntryExeptNaV; }


	void        resetDistributionS       ();
	void        incDistributionS         (long index); 
	long        distSValueAt             (long index) {return distributionSM[index]; }
	long        numberOfDistSValues      ()           {return _numberOfValuesInDistS; }
	long        maxEntryInDistS          ()           {return _maxEntryInDistS; }
	long        maxEntryExeptNaVinDistS  ()           {return _maxEntryExeptNaVinDistS; }

	long        minIndex  ()           { return _minIndex; }
	long        maxIndex  ()           { return _maxIndex; }
	long        maxMaxIndex()          { return _numberOfDiscreteValues-1;}
	void        setMaxIndex (long v)   { _maxIndex = v; }
	void        setMinIndex (long v)   { _minIndex = v; }

	void        resetIndexRange ()     { _minIndex = -1;                       _maxIndex = _numberOfDiscreteValues-1; }
	void        setEmptyIndexRange ()  { _minIndex =  _numberOfDiscreteValues; _maxIndex = -2;  } 
    bool        isIndexRangeEmpty()    {return (_minIndex > _maxIndex); } 
	
	
	void        setDistanceWeight (qreal v) {_distanceWeight = v; _partOfDistanceFunction = (v!=0); }
	qreal       distanceWeight    ()        {return _distanceWeight;}
	bool        isPartOfDistanceFunction () {return _partOfDistanceFunction;}
	
private:

	QString  _name;

	long     _minIndex;
	long     _maxIndex;

	long     _numberOfDiscreteValues;

	long     _numberOfValuesInDist;
	long     _maxEntryInDist;
	long     _maxEntryExeptNaV;

	long     _numberOfValuesInDistS;
	long     _maxEntryInDistS;
	long     _maxEntryExeptNaVinDistS;

	qreal    _minNominalValue;
	qreal    _maxNominalValue;

	qreal    _distanceWeight;
	bool     _partOfDistanceFunction;

	QMap <long,  QString>	valueAsStringM;
	QMap <long,  qreal>     nominalValueM;
	QMap <long,  long>      distributionM;
	QMap <long,  long>      distributionSM;


	QHash <QString, long>  _keyOfAValue;
	
};

#endif
