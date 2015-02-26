/*
 *  GV__simpleObjectFindings.cpp
 *  sampleDBobjectViewer
 *
 *  Created by mue on 17.10.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GV__simpleObjectFindings.h"


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

GV__simpleObjectFindings::GV__simpleObjectFindings ( GVfindings   *findings, QGLContext *cx) {

    
    this->findings = findings;
    this->cx       = cx;
    gq = gluNewQuadric();
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

GLuint GV__simpleObjectFindings::makeBase () {

//    cx->makeCurrent();
    
    GLuint dltemp = glGenLists(1);
    glNewList(dltemp, GL_COMPILE);
    generateBase (0.08, nodataT, false);
    glEndList();
    return dltemp;
    
}





//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
GLuint GV__simpleObjectFindings::generateObject    ( int elementIndex, int level) {
    
  //    qDebug ("hello in GV__simpleObjectSampleDB::generateObject  -> RENDERING ELEMENT %d", elementIndex);
    //    qDebug ("sampleDB->el.count() =  %d", sampleDB->el.count());
    
    // first generate eh pBuffers
    
//    cx->makeCurrent();
    
    GLint slices = 15;
    GLint stack = 15;
    
    float transparency = 0.8;
    float sockel_h     = 0.15;    
    float marginb      = 0.02;


    float box          = 0.90;  
    
    float boxh       = 0.5; 
    float boxdelta   = 0.05; 

    float height; 

    float black[4]={0,0,0,0};
	
    int     n   = 2;
	float   nv  = 2;
	float   sv;



    int MstagingIndex = 2;
    int NstagingIndex = 2;
    int GIndex        = 2;


    int yearsSurvived;
    

    int survivalIndex = 1; 
 
    if (yearsSurvived < 5)  survivalIndex = 1;
    if (yearsSurvived >=5)  survivalIndex = 2;
    if (yearsSurvived >=10) survivalIndex = 3;
    if (yearsSurvived >=15) survivalIndex = 4;
   
    survivalIndex = 3;
    
 
    GLuint dltemp = glGenLists(1);
    glNewList(dltemp, GL_COMPILE);	
    
    glEnable(GL_BLEND);		// Turn Blending On
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    
    GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );   
    
    
    QString mainColorAttribute = GVfindingsObject::visu2attribute["MAIN-COL-DISC"];
    int mainColorIndex = findings->el[elementIndex]->value[mainColorAttribute];

    GLuint texture = 0;
    
    
    generateBase (sockel_h, 0, true);

    float boxwidthf;
    
    boxwidthf = 1.0;
    


    box = 0.6;
    float boxw;

    float boxStart;
    float boxEnd;

    boxwidthf = 1.0;

    boxw = box *boxwidthf;

#ifdef LEVEL3XX
    boxStart = 0;
    boxEnd   = boxStart + boxw;

#else
    boxStart = (1 - boxw)  / 2;
    boxEnd   = boxStart + boxw;
#endif

    float zStart   = sockel_h;
    float zEnd     = sockel_h +  box/2;
   
    QColor glyphColor = QColor (200,200,200);

    nv = 0;

    if (mainColorIndex==0)  {
        glColor4f     ( 0.8, 0.8, 0.8, 1.0);  
		glyphColor  = QColor (200,200,200);
    }
    
    if (mainColorIndex==1)  {
        glColor4f     ( 0.0, 0.4+nv, 0.0, 1.0);    
		glyphColor  = QColor (0,100,00);

    }
    
    if (mainColorIndex == 2) {
        glColor4f     ( 0.3, 0.4+nv, 0.4+nv, 1.0);    
		glyphColor  = QColor (70,100,100);

    }             

    if (mainColorIndex == 3) {
        glColor4f     ( 0.4+nv, 0.4+nv, 0.0, 1.0);    
		glyphColor  = QColor (100,100,0);
    }            
    
    if (mainColorIndex == 4) {
        glColor4f     ( 0.4+nv, 0.0, 0.0, 1.0);    
		glyphColor  = QColor (100,0,0);

    }
    
    zEnd   = zStart +  box/2 * survivalIndex ;

    glColor4f     ( glyphColor.redF(), glyphColor.greenF(), glyphColor.blueF(), 1.0);
    generateBOX   ( boxStart, boxStart,  zStart,    boxEnd,  boxEnd,  zEnd, glyphColor);

    zStart = zEnd;
    zEnd   = zStart +  0.1 ;
	QColor topC = glyphColor;
	topC = glyphColor.darker(200);
	
    generateBOX   ( boxStart+0.1, boxStart+0.1,  zStart,    boxEnd-0.1,  boxEnd-0.1,  zEnd, topC);

     zStart   = sockel_h;
     zEnd   = zStart +  0.6 ;

//grading    
#if 0    
    
   

    float smallBoxW = 0.3;    
    float borderMINW = 0.1; 


    if (GIndex > 1)  
        zEnd   = zStart +  box/2 * survivalIndex * 0.8;
    else
        zEnd   = zStart +  0.2 ;
    
    
    glColor4f(0.0, 0.0, 0.0,  1.0);
    if (GIndex == 2) glColor4f(0.0, 0.4, 0.0,  1.0);
    if (GIndex == 3) glColor4f(0.3, 0.4, 0.4,  1.0);
    if (GIndex == 4) glColor4f(0.4, 0.4, 0.0,  1.0);
    if (GIndex == 5) glColor4f(0.4, 0.0, 0.0,  1.0);
 
  
    generateBOX   (borderMINW,  borderMINW,  zStart,    smallBoxW,  smallBoxW,  zEnd, false);
  
     zStart = zEnd;
     zEnd   = zStart +  0.05 ;
    generateBOX   (borderMINW+0.05,  borderMINW+0.05,  zStart,    smallBoxW-0.05,  smallBoxW-0.05,  zEnd, false);


    zStart = sockel_h;;
#endif

// N STAGGING
#if 0

    if (NstagingIndex > 1)  
       zEnd   = zStart +  box/2 * survivalIndex * 0.8;
    else
        zEnd   = zStart +  0.2 ;
   

     glColor4f(0.3, 0.3, 0.3,  1.0);
    if (NstagingIndex == 2) glColor4f(0.0, 0.4, 0.0,  1.0);
    if (NstagingIndex == 3) glColor4f(0.3, 0.4, 0.4,  1.0);
    if (NstagingIndex == 4) glColor4f(0.4, 0.4, 0.0,  1.0);
    if (NstagingIndex == 5) glColor4f(0.4, 0.0, 0.0,  1.0);
   
    generateBOX   (1-smallBoxW,  borderMINW,  zStart,    1-borderMINW,  smallBoxW,  zEnd, false);
     zStart = zEnd+0.01;
     zEnd   = zStart +  0.03 ;
     generateBOX   (1-smallBoxW,  borderMINW,  zStart,    1-borderMINW,  smallBoxW,  zEnd, false);

    zStart = sockel_h;;

   if (MstagingIndex > 1)  
        zEnd   = zStart +  box/2 * survivalIndex * 0.8;
    else
        zEnd   = zStart +  0.2 ;
   
    glColor4f(0.3, 0.3, 0.3,  1.0);
    if (MstagingIndex == 2) glColor4f(0.0, 0.4, 0.0,  1.0);
    if (MstagingIndex == 3) glColor4f(0.3, 0.4, 0.4,  1.0);
    if (MstagingIndex == 4) glColor4f(0.4, 0.4, 0.0,  1.0);
    if (MstagingIndex == 5) glColor4f(0.4, 0.0, 0.0,  1.0);
     generateBOX   (borderMINW,  1-smallBoxW,  zStart,    smallBoxW,  1-borderMINW,  zEnd, false);
#endif

    glDisable     ( GL_TEXTURE_2D);
    
      bool yearInd = false;
//    if (findings->el.value(elementIndex)->year %2)        yearInd = true;
    
    glEndList();
    return dltemp;
}



//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void  GV__simpleObjectFindings::generateBase (float height, GLuint textureID, bool borderHighlight) 
{
    
//    cx->makeCurrent();
   
    
#if 1
    float xmin = 0.0; float ymin = 0.0; float zmin = 0.0;
    float xmax = 1.0; float ymax = 1.0; float zmax = height;

    
    glBegin(GL_QUADS);
    glNormal3i (0, -1, 0);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmin, ymin, zmax);
    
    glEnd( );
    
    
    glBegin(GL_QUADS);
    glNormal3i (-1, 0, 0);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymin, zmax);
    
    glEnd( );
    
    if (textureID) {
        
        glEnable       ( GL_TEXTURE_2D);
        glBindTexture  ( GL_TEXTURE_2D, textureID);
        glEnable       ( GL_BLEND);
        glBlendFunc    ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST  );   
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );    
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }   
    
    glBegin(GL_QUADS);
    glNormal3i (0, 0, 1);
    glTexCoord2d (0,  0);   glVertex3f(xmin, ymin, zmax);
    glTexCoord2d (1,  0);   glVertex3f(xmax, ymin, zmax);
    glTexCoord2d (1,  1);   glVertex3f(xmax, ymax, zmax);
    glTexCoord2d (0,  1);   glVertex3f(xmin, ymax, zmax);
    glEnd( );
    glDisable     ( GL_TEXTURE_2D);
#endif
    
  

}



//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void  GV__simpleObjectFindings::generateBOX (float xmin, float ymin, float zmin, float xmax, float ymax, float zmax, QColor topcolor)
{ 
//    cx->makeCurrent();
    // der Boden
#if 0
    glBegin(GL_QUADS); 
    glNormal3i (0, 0, -1);
    glTexCoord2d (0,  0);   glVertex3f(xmin, ymax, zmin);    
    glTexCoord2d (1,  0);   glVertex3f(xmax, ymax, zmin);
    glTexCoord2d (1,  1);    glVertex3f(xmax, ymin, zmin);    
    glTexCoord2d (0,  1);   glVertex3f(xmin, ymin, zmin);
    glEnd( );
#endif    

#if 0
    glBegin(GL_QUADS);
    glNormal3i (0, 1, 0);
    glTexCoord2d (0,  0);     glVertex3f(xmin, ymax, zmin);
    glTexCoord2d (1,  0);     glVertex3f(xmin, ymax, zmax);
    glTexCoord2d (1,  0.5);   glVertex3f(xmax, ymax, zmax);
    glTexCoord2d (0,  0.5);   glVertex3f(xmax, ymax, zmin);
    glEnd( );
#endif 
    glBegin(GL_QUADS);
    glNormal3i (1, 0, 0);    
    glTexCoord2d (0,  0);   glVertex3f(xmax, ymin, zmax);
    glTexCoord2d (1,  0);   glVertex3f(xmax, ymin, zmin);
    glTexCoord2d (1,  1);   glVertex3f(xmax, ymax, zmin);    
    glTexCoord2d (0,  1);   glVertex3f(xmax, ymax, zmax);
    glEnd( );
    
    // rechts vorne

    glBegin(GL_QUADS);
    glNormal3i (0, -1, 0);
    glTexCoord2d (0,  0);   glVertex3f(xmin, ymin, zmin);
    glTexCoord2d (1,  0);   glVertex3f(xmax, ymin, zmin);
    glTexCoord2d (1,  0.5);   glVertex3f(xmax, ymin, zmax);    
    glTexCoord2d (0,  0.5);   glVertex3f(xmin, ymin, zmax);
    glEnd( );


    // links vorne
    glBegin(GL_QUADS);
    glNormal3i (-1, 0, 0);
    glTexCoord2d (0,    1);   glVertex3f(xmin, ymax, zmax);
    glTexCoord2d (0.5,  1);   glVertex3f(xmin, ymax, zmin);
    glTexCoord2d (0.5,  0);   glVertex3f(xmin, ymin, zmin);
    glTexCoord2d (0,    0);   glVertex3f(xmin, ymin, zmax);
    glEnd( );
    
    glColor4f     ( topcolor.redF(),  topcolor.greenF(),topcolor.blueF(),1);   

    glBegin(GL_QUADS);
    glNormal3i (0, 0, 1);
    glTexCoord2d (0,  0);   glVertex3f(xmin, ymin, zmax);
    glTexCoord2d (1,  0);   glVertex3f(xmax, ymin, zmax);
    glTexCoord2d (1,  1);   glVertex3f(xmax, ymax, zmax);
    glTexCoord2d (0,  1);   glVertex3f(xmin, ymax, zmax);
    glEnd( );
}    
