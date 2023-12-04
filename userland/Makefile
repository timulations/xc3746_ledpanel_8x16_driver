CXX = g++
CXXFLAGS = -Wall -O3 -std=c++17
TARGET = ledpanel_write
SOURCES = ledpanel_user.cpp ledpanel_write.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)

ledpanel_user.o: ledpanel_user.cpp ledpanel_user.h
ledpanel_write.o: ledpanel_write.cpp ledpanel_user.h ledpanel_bitmaps.h
