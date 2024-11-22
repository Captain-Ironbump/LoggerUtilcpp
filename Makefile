CXX = g++
CXXFLAGS = -g -Wall -std=c++11
TARGET = main
STATIC_LIBRARY = libloggercpp.a
MAIN_OBJ = main.o
LOGGER_OBJ = logger.o
OBJ = $(MAIN_OBJ) $(LOGGER_OBJ) 

all: $(TARGET) $(STATIC_LIBRARY)

$(TARGET): $(OBJ) 
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

$(STATIC_LIBRARY): $(LOGGER_OBJ)
	ar rcs $(STATIC_LIBRARY) $(LOGGER_OBJ)

$(MAIN_OBJ): main.cpp logger.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

$(LOGGER_OBJ): logger.cpp logger.hpp
	$(CXX) $(CXXFLAGS) -c logger.cpp

clean:
	rm -f $(OBJ) $(TARGET) $(STATIC_LIBRARY)

clean-o:
	rm -f $(OBJ)
