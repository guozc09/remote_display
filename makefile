CXX = g++
CXXFLAG = -std=c++11
LIBS = -lpthread
INCLUDE = -I./ \
          -I./include
SOURCES += ./src/TransmissionClientNet.cpp \
           ./src/TransmissionServerNet.cpp \
           ./src/RemoteDisplay.cpp \
		   ./src/RemoteDisplayManager.cpp \
		   ./src/RemoteDisplayManagerTest.cpp
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
