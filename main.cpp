#include <QApplication>

#include "window.h"
#include "GVsliderWindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#if 0
	qApp->setStyleSheet ( 
	"* { background:   rgb(70,70,70); }"
	"* { alternate-background-color:   rgb(75,75,75); }"
	"* { border-color:   red; }"
	"*:enabled     { color: rgb(164,164,164);  }"
	"*:disabled    { color: rgb(116,116,116); }"
	"QDockWidget   { border-width: 10px; border-style: solid; border-color: rgb(145,45,45); background: rgb(116,6,6); }"
	"QToolTip      { border-width: 1px;  border-style: solid; border-color: rgb(45,45,45); background:   rgb(75,75,75);}"
				   );
#endif
    
	QLocale::setDefault(QLocale::German);

	Q_INIT_RESOURCE(diseaseViewer);

    QTranslator translator;
    translator.load("dv_ge.qm");
    app.installTranslator(&translator);
    

#ifdef WIN32
   QApplication::setStyle ( "plastique" );
#endif
	
    Window                          window;
    GVsliderWindow sliderwindow;

   window.show();
//    sliderwindow.show();
    
    return app.exec();
}
