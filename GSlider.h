/*
 *  GSlider.h
 *  isoProjTest
 *
 *  Created by mue on 26.01.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <QGLWidget>
#include <QObject>
#include <QImage>

class GSlider : public QGLWidget 
{
    
    Q_OBJECT
    
public:
    
    GSlider  (QWidget                 *parent = 0, 
              QGLWidget               *shareWidget=0, 
              Qt::Orientation orientation=Qt::Horizontal);
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setBorder      (double borderInPerzent);
    void setBGtexture   (QImage bgImage); 
    
public slots:
        
    void setMainSliderPos (double v);
    void setMinSliderPos  (double v);
    void setMaxSliderPos  (double v);
    
    void repaint();
    
    

signals:

    void valueChanged  (double v);

        
protected:
        
    void initializeGL();
    
    void paintGL();
    void paintSlider(double position);
    
    void doResize();
    void resizeGL(int width, int height);
    void mousePressEvent    (QMouseEvent *event);
    void mouseReleaseEvent  (QMouseEvent *event);
    void mouseMoveEvent     (QMouseEvent *event);
    
private:        
        
    Qt::Orientation orientation;   
   
    QImage *buttonImage;
    QImage *valueImage;
    
    double canvasWidth;
    double canvasHeight;

    double leftClippingPlane;
    double rightClippingPlane;
    double topClippingPlane;
    double bottomClippingPlane;
    
    double buttonScale;
    
    double sreenDxCorr;
    double sreenDyCorr;
    double sreenToGlX;
    
    double border;
    
    QPoint lastEventPos;
    
    bool startMovement;
    
    GLuint haarlineDL;
    GLuint buttonDL;
    GLuint bgDL;
    GLuint crossDL;

    
    GLuint valueDL;

    GLuint valueTexture;     
    GLuint buttonTexture;
    GLuint bgTexture;
    
    double msv;     // mainslidervalue;
    
    double       scaleCorrectionX;
    double       scaleCorrectionY;
    double       scaleCorrectionDx;
    double       scaleCorrectionDy;
    
};