CXX = g++
CXXFLAG = -std=c++11
LIBS = -lpthread
TRANSMISSION_DIR = ./Transmission
CLIENT_DIR = $(TRANSMISSION_DIR)/Client
SERVER_DIR = $(TRANSMISSION_DIR)/Server
REMOTEDISPLAY_DIR = ./RemoteDisplay
INCLUDE = -I./ \
          -I$(TRANSMISSION_DIR) \
          -I$(CLIENT_DIR) \
		  -I$(SERVER_DIR) \
		  -I$(REMOTEDISPLAY_DIR)
SOURCES += $(CLIENT_DIR)/TransmissionClient.cpp \
           $(SERVER_DIR)/TransmissionServer.cpp \
           $(REMOTEDISPLAY_DIR)/RemoteDisplay.cpp \
		   ./RemoteDisplayManager.cpp \
		   ./RemoteDisplayManagerTest.cpp
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))
TARGET = manager_test

$(TARGET):$(SOURCES)
	$(CXX) -o $(TARGET) $(INCLUDE) $(SOURCES) $(LIBS) $(CXXFLAG)
# $(OBJECTS):$(SOURCES)
# 	$(CXX) -c $(SOURCES) $(INCLUDE) $(CXXFLAG)

# 编译规则 $@代表目标文件 $< 代表第一个依赖文件
# %.o:%.cpp
# 	$(CXX) $(INCLUDE) -o $@ -c $<
clean:
	$(RM) $(OBJECTS) $(TARGET)
