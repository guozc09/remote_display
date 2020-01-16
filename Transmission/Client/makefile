RM = rm -f
CXX = g++
INCLUDE = ./
LIBS =
SOURCES = *.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = client_test

$(TARGET):$(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) -I $(INCLUDE) $(LIBS)
$(OBJECTS):$(SOURCES)
	$(CXX) -c $(SOURCES)

# 编译规则 $@代表目标文件 $< 代表第一个依赖文件
%.o:%.cpp
	$(CXX) -I $(INCLUDE) -o $@ -c $<
clean:
	$(RM) $(OBJECTS) $(TARGET)
