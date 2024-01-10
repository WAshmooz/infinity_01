CC = g++
CPPFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -pthread -fPIC -rdynamic -ldl
INCLUDES = -Iinclude -I../concrete/include
LIB = -shared
DBG = -g
RELEASE = -DNDEBUG -O3
SRC = $(wildcard *.cpp)

# # List all source directories
# SRC_DIRS = src framework/src

# # Use wildcard to get all .cpp files recursively from the specified directories
# SRC_FILES = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))

# SRC_TARGET = $(SRC_FILES) test/re_fw_test.cpp

SRC_TARGET = \
../concrete/src/i_driver_comuunicator.cpp \
../concrete/src/nbd_driver_communicator.cpp \
../concrete/src/ram_storage.cpp \
src/i_task.cpp \
src/thread_pool.cpp \
src/pnp.cpp \
test/re_fw_test.cpp


OBJ = $(SRC:.cpp=.o)
OBJ_TARGET = $(SRC_TARGET:.cpp=.o)

TARGET = a.out


all: $(OBJ_TARGET) 

%.o: %.cpp 
	$(CC) -c $< $(INCLUDES) $(CPPFLAGS) -o $@

$(TARGET): $(OBJ_TARGET)
	$(CC) $(INCLUDES) $(CPPFLAGS) $^ -o $@


debug: CPPFLAGS += $(DBG)
debug: $(TARGET)


clean:
	rm -f $(OBJ_TARGET) $(OBJ_LIBRARY) $(TARGET)
	

release: CPPFLAGS += $(RELEASE)
release: $(TARGET)
