QT += charts

HEADERS += \
    themewidget.h \
    ChartView.h

SOURCES += \
    main.cpp \
    themewidget.cpp \
    ChartView.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/chartthemes
INSTALLS += target
