/*
 *  GVattribute.cpp
 *  diseaseViewer
 *
 *  Created by mue on 21.09.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVattributeType.h"

//------------------------------------------------------------------------------
GVattributeType::GVattributeType(QString attributeName) {
	_name = attributeName;
	_numberOfDiscreteValues = 0;
	
	// we always have a element denoting NaV = Not a Value, index = -1
	valueAsStringM[-1]    = "NaV";
	nominalValueM [-1]    = 0.0; // put the float code for NaV
	_keyOfAValue  ["NaV"] = -1;
	
	// todo - this has to be changed to min/max double values
	_maxNominalValue = -999999999999999.0;
	_minNominalValue = +999999999999999.0;

	_distanceWeight = 0;
}


//------------------------------------------------------------------------------
void GVattributeType::setNominalValue (QString valueS, qreal valueR) {
	if ( _keyOfAValue.contains (valueS) ) {
		 long index = _keyOfAValue[valueS];
		 nominalValueM [index] = valueR;
		 if (_maxNominalValue < valueR) _maxNominalValue = valueR;
		 if (_minNominalValue > valueR) _minNominalValue = valueR;

	}
}


//------------------------------------------------------------------------------
void GVattributeType::appendValue     (QString valueS, qreal valueR) {
	valueAsStringM[_numberOfDiscreteValues] = valueS;
	nominalValueM [_numberOfDiscreteValues] = valueR;
	_keyOfAValue [valueS] = _numberOfDiscreteValues;
	_numberOfDiscreteValues++;
	 if (_maxNominalValue < valueR) _maxNominalValue = valueR;
	 if (_minNominalValue > valueR) _minNominalValue = valueR;
}


//------------------------------------------------------------------------------
long GVattributeType::indexOf  (QString valueS) {
	if (_keyOfAValue.contains (valueS) ) {
	   return _keyOfAValue[valueS];
	}
	else
	{
		return -1;
	}
}

//------------------------------------------------------------------------------
void        GVattributeType::incDistribution   (long index) {
	distributionM[index]++;  
    if (_maxEntryInDist   < distributionM[index]) _maxEntryInDist   = distributionM[index];
	if (index >=0) if (_maxEntryExeptNaV < distributionM[index]) _maxEntryExeptNaV = distributionM[index];
	_numberOfValuesInDist++; 	
}



//------------------------------------------------------------------------------
void GVattributeType::resetDistribution () {
	for (long i = -1; i<_numberOfDiscreteValues; i++) {
		distributionM[i] = 0;
	}
    _numberOfValuesInDist = 0;
	_maxEntryInDist       = 0;
	_maxEntryExeptNaV     = 0;
}



//------------------------------------------------------------------------------
void        GVattributeType::incDistributionS   (long index) {
	distributionSM[index]++;  
	               if (_maxEntryInDistS          < distributionSM[index]) _maxEntryInDistS         = distributionSM[index];
	if (index >=0) if (_maxEntryExeptNaVinDistS  < distributionSM[index]) _maxEntryExeptNaVinDistS = distributionSM[index];
	_numberOfValuesInDistS++; 	
}



//------------------------------------------------------------------------------
void GVattributeType::resetDistributionS () {
	for (long i = -1; i<_numberOfDiscreteValues; i++) {
		distributionSM[i] = 0;
	}
    _numberOfValuesInDistS       = 0;
	_maxEntryInDistS             = 0;
	_maxEntryExeptNaVinDistS     = 0;
}


