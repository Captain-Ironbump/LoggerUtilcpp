CXX = g++
CXXFLAGS = -g -Wall -std=c++17
TARGET = main
STATIC_LIBRARY = libloggercpp.a
MAIN_OBJ = main.o
LOGGER_OBJ = logger.o
OBJ = $(MAIN_OBJ) $(LOGGER_OBJ) 

ifeq ($(OS),Windows_NT)
	TARGET = main.exe
	CLEAN_CMD = del /f
else
	CLEAN_CMD = rm -f
endif

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
	$(CLEAN_CMD) $(OBJ) $(TARGET) $(STATIC_LIBRARY)

clean-o:
	$(CLEAN_CMD) $(OBJ)
