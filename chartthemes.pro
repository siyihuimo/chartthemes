QT += core gui charts

HEADERS += \
    themewidget.h \
    ChartView.h \
    SetChartPropertyWidget.h

SOURCES += \
    main.cpp \
    themewidget.cpp \
    ChartView.cpp \
    SetChartPropertyWidget.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/chartthemes
INSTALLS += target

RESOURCES += \
    res.qrc
