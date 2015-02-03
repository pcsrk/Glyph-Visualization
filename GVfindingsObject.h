/*
 *  GVfindingsObject.h
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GVfindingsObject_H
#define GVfindingsObject_H

#include "GVattributeType.h"

#include <QStringList>
#include <QHash>
#include <QDate>



class GVfindingsObject 
{
    
public:
    
    GVfindingsObject();
	
    int		updateSelectionState ();
    void	updateAttributeTypeIndex();
   
	bool    isSelected;
    QMap <QString, int> value;

    static QMap <QString, QString>  visu2attribute;
    static QList<GVattributeType*>  GVattributes;
    static QList<signed char>       GVIndex;

};

#endif
