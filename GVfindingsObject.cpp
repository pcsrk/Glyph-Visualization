/*
 *  GVfindingsObject.cpp
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GVfindingsObject.h"

GVfindingsObject::GVfindingsObject()
{
	isSelected     = false;
}


int GVfindingsObject::updateSelectionState (){
	isSelected = true;
    foreach(GVattributeType *attributeType, GVfindingsObject::GVattributes) {
        int index = value [attributeType->name()];
        if (index < attributeType->minIndex()) goto end;
        if (index > attributeType->maxIndex()) goto end;
    }
    return 1;
	end:
    isSelected = false;
	return 0;
}


void GVfindingsObject::updateAttributeTypeIndex  (){
      foreach(GVattributeType *attributeType, GVfindingsObject::GVattributes) {
          int index = value [attributeType->name()];
          if (index < attributeType->minIndex())  attributeType->setMinIndex(index);
          if (index > attributeType->maxIndex())  attributeType->setMaxIndex(index);
      }
}


QMap <QString, QString>  GVfindingsObject::visu2attribute;
QList<GVattributeType*>  GVfindingsObject::GVattributes;
QList<signed char>       GVfindingsObject::GVIndex;


