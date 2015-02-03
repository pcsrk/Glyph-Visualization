HEADERS       = sqlite3.h \
			    SQLiteLayer.h \
				GVglobals.h \
				GVattributeType.h \
				GVattributeSelectionWidget.h \
				GVselectionMaster.h \
				GVmainToolbar.h \
				GVanimationMaster.h \
				GVinfoWidget.h \
				GVfindings.h \
                GVfindingsObject.h \
				GV__simpleObjectFindings.h \
                GVsimpleObjectFindings.h \
                GVsimpleObjectFindingsglDisplay.h \
                GVelementViewFindings.h \
                GVmainGlVisuPlane.h \
                window.h \
                GLmaster.h \
                Gnumber.h \                
                G_number.h \        
                G__texture.h \
                GVbasicElement.h \
                GVelemetBox.h \
                GVelementThreeStateDistribution.h \                
                GVisoGrid.h \
                GVarrayQuantCont.h \
                GVsliderWindow.h \
                GSlider.h \    
				GVsettings.h \            
				GVoptionsDialog.h             
             
                
SOURCES     =   sqlite3.c \
				SQLiteLayer.cpp \
				GVattributeType.cpp \
				GVattributeSelectionWidget.cpp \
				GVselectionMaster.cpp \
				GVmainToolbar.cpp \
				GVanimationMaster.cpp \
				GVinfoWidget.cpp \
				GVfindings.cpp \
                GVfindingsObject.cpp \
                GVsimpleObjectFindings.cpp \
                GV__simpleObjectFindings.cpp \
                GVsimpleObjectFindingsglDisplay.cpp \
                GVelementViewFindings.cpp \
                GVmainGlVisuPlane.cpp \
                Gnumber.cpp \                
                G_number.cpp \
                G__texture.cpp \
                GLmaster.cpp \
                main.cpp \
                window.cpp \
                GVbasicElement.cpp \
                GVelementThreeStateDistribution.cpp \
                GVelemetBox.cpp \
                GVisoGrid.cpp \
                GVarrayQuantCont.cpp \
                GVsliderWindow.cpp \
                GSlider.cpp \
				GVsettings.cpp \            
				GVoptionsDialog.cpp             
    
            				
QT           += opengl
QT           += sql

TRANSLATIONS    = dv_ge.ts 

CONFIG       += thread sql 

HEADERS        += ogl/OglExt.h   ogl/glext.h ogl\Macros.h ogl/RenderingContext.hpp ogl/RCHashArray.hpp
SOURCES        += ogl/OglExt.cpp ogl/RenderingContext.cpp

RESOURCES    = diseaseViewer.qrc

TARGET = dv

include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)

macosx {
    QMAKE_LFLAGS    += -L/System/Library/Frameworks/
    QMAKE_MAC_SDK   =  /Applications/XCode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk
}


