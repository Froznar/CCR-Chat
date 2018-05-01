TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lallegro
LIBS += -lpthread
unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_ttf



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_primitives



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_physfs



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_memfile



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_main



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_image


unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_font



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_dialog



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_color



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_audio



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -lallegro_acodec


SOURCES += main.cpp

HEADERS += \
    server.h \
    extra.h \
    server_functions.h \
    client.h \
    client_functions.h \
    protocols.h \
    game_protocols.h \
    file.h
