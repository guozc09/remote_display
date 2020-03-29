CURRENTPATH = $(shell /bin/pwd)
AM_CPPFLAGS = -I$(CURRENTPATH)/include
export AM_CPPFLAGS
noinst_PROGRAMS = remotedisplay
remotedisplay_SOURCES = src/TransmissionClientNet.cpp \
                        src/TransmissionServerNet.cpp \
                        src/RemoteDisplay.cpp \
                        src/RemoteDisplayManager.cpp \
                        src/RemoteDisplayManagerTest.cpp
remotedisplay_LDFLAGS = -D_GNU_SOURCE
DEFS += -D_GNU_SOURCE -std=c++11 -Wall
LIBS = -lpthread

all-am: cpout $(noinst_PROGRAMS)
cpout:
	$(shell mv -f $(noinst_PROGRAMS) $(CURRENTPATH)/out)
rmout:
	$(shell rm -f $(CURRENTPATH)/out/*)