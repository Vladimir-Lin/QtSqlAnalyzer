NAME         = SqlAnalyzer
TARGET       = $${NAME}

QT           = core
QT          -= gui
CONFIG(static,static|shared) {
# static version does not support Qt Script now
QT          -= script
} else {
QT          += script
}
QT          += network
QT          += sql
QT          += Essentials

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/qtsqlanalyzer.h

SOURCES     += $${PWD}/nSqlCondition.cpp
SOURCES     += $${PWD}/nSqlConstraint.cpp
SOURCES     += $${PWD}/nSqlConditions.cpp
SOURCES     += $${PWD}/nSqlConstraints.cpp
SOURCES     += $${PWD}/nSqlTemplates.cpp
SOURCES     += $${PWD}/nSqlParser.cpp
SOURCES     += $${PWD}/nSqlClone.cpp
SOURCES     += $${PWD}/nNameItem.cpp
SOURCES     += $${PWD}/nNameItems.cpp
SOURCES     += $${PWD}/nNameMapItem.cpp
SOURCES     += $${PWD}/nNameMapItems.cpp
SOURCES     += $${PWD}/nNameCollection.cpp

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)
