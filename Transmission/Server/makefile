CXX = g++ #编译工具
INCLUDE = ./
LIBS =
SOURCES = *.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = app #可执行文件

$(TARGET):$(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) -I $(INCLUDE) $(LIBS)
$(OBJECTS):$(SOURCES)
	$(CXX) -c $(SOURCES)

# 编译规则 $@代表目标文件 $< 代表第一个依赖文件
%.o:%.cpp
	$(CXX) -I $(INCLUDE) -o $@ -c $<
clean:
	$(RM) $(OBJECTS) $(TARGET)
