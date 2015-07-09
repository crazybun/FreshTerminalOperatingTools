#-------------------------------------------------
#
# Project created by QtCreator 2014-12-09T12:13:34
#
#-------------------------------------------------

QT       += core gui
QT	    += serialport network
QT       += axcontainer					#Operate the Excel Should add this
QT	    += xml
QT	    += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -lws2_32
TARGET = FreshTerminalOperatingTools
TEMPLATE = app


SOURCES += main.cpp\
        enterwidget.cpp \
    mainoperatewindow.cpp \
    comparasetting.cpp \
    terparasetting.cpp \
    xlmasterwin.cpp \
    upgradewidget.cpp \
    proprasewidget.cpp \
    netprotocol.cpp \
    commprocess.cpp \
    teraddrdialog.cpp \
    excelproc.cpp \
    threadproc.cpp \
    protocolbase.cpp \
    xmlproc.cpp \
    GB376_Protocol/gb376_1.cpp \
    maintain_protocol/wsdmaintain.cpp \
    statusMachine/someprotocoltask.cpp \
    logofupgrade.cpp \
    GB376_Protocol/gb376_1_afn09.cpp \
    gsoap/stdsoap2.c \
    gsoap/soapClient.c \
    gsoap/soapC.c \
    gsoap/httpsoap.cpp \
    CSG_Protocol/csg2015.cpp \
    CSG_Protocol/csg2015_afn00.cpp \
    CSG_Protocol/csg2015_afn0f.cpp \
    CSG_Protocol/csg2015_afn0a.cpp \
    CSG_Protocol/csg2015_afn02.cpp \
    CSG_Protocol/csg2015_afn04.cpp \
    GB376_Protocol/gb376_1_afn0f.cpp \
    GB376_Protocol/gb376_1_afn00_01_02.cpp \
    BaseType.cpp \
    GB376_Protocol/gb376_1_afn04.cpp

HEADERS  += enterwidget.h \
    BaseType.h \
    mainoperatewindow.h \
    comparasetting.h \
    terparasetting.h \
    xlmasterwin.h \
    upgradewidget.h \
    proprasewidget.h \
    netprotocol.h \
    commprocess.h \
    teraddrdialog.h \
    excelproc.h \
    threadproc.h \
    protocolbase.h \
    xmlproc.h \
    soapcpp2.h \
    GB376_Protocol/gb376_1.h \
    maintain_protocol/wsdmaintain.h \
    statusMachine/someprotocoltask.h \
    logofupgrade.h \
    gsoap/stdsoap2.h \
    gsoap/soapStub.h \
    gsoap/soapH.h \
    gsoap/httpsoap.h \
    CSG_Protocol/csg2015.h

FORMS    += enterwidget.ui \
    mainoperatewindow.ui \
    comparasetting.ui \
    terparasetting.ui \
    xlmasterwin.ui \
    upgradewidget.ui \
    proprasewidget.ui \
    teraddrdialog.ui

RESOURCES += \
    TRES.qrc

RC_FILE += FRc.rc

OTHER_FILES += \
    maintain_protocol/维护软件说明文档.txt \
    statusMachine/状态运行表说明文档.txt

