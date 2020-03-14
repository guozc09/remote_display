CURRENTPATH = $(shell /bin/pwd)
AM_CPPFLAGS = -I$(CURRENTPATH)/include
export AM_CPPFLAGS
noinst_PROGRAMS = remotedisplay
vpath %.h include
vpath %.cpp src
remotedisplay_SOURCES = TransmissionClientNet.cpp \
                        TransmissionServerNet.cpp \
                        RemoteDisplay.cpp \
                        RemoteDisplayManager.cpp \
                        RemoteDisplayManagerTest.cpp
remotedisplay_LDFLAGS = -D_GNU_SOURCE
DEFS += -D_GNU_SOURCE -std=c++11 -Wall
LIBS = -lpthread