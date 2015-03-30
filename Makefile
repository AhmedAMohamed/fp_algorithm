#Get list with names of all cpp file
CPP_DIR = src lib
CPP_FILES := $(shell find $(CPP_DIR) -type f -name '*.cpp')	

#Make a list of object files from source files and with obj/ as directory
OBJ_FILES := $(addprefix .obj/, $(CPP_FILES:.cpp=.o))

INC = -Ilib/
CPPFLAGS := -Wall -g -std=c++11 $(INC)

all:  start_msg_make_file main

start_msg_make_file:
	@echo "Building...\n"

main:  $(OBJ_FILES)
	@mkdir -p build
	g++ -o build/$@ $^ 
	@echo "\nBuild Completed."


.obj/%.o: %.cpp
	@mkdir -p $(@D)
	g++ $(CPPFLAGS) -c -o $@ $< 



# Automatic dependency graph generation
CPPFLAGS += -MMD
-include $(OBJFILES:.o=.d)

clean:
	-rm -f output/* obj/*
	@echo "Cleaned build"



.PHONY: clean
