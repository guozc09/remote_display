bin_PROGRAMS = remote_display
remote_display_SOURCES = src/TransmissionClientNet.cpp \
                        src/TransmissionServerNet.cpp \
                        src/RemoteDisplay.cpp \
                        src/RemoteDisplayManager.cpp \
                        src/RemoteDisplayManagerTest.cpp
remote_display_CXXFLAGS = -I./include
remote_display_CXXFLAGS += -std=c++11 -Wall
LIBS = -D_GNU_SOURCE -lpthread

outdir = ./out
out_DATA = $(bin_PROGRAMS)

rmout:
	$(shell rm -f ./out/*)