#OBJS specifies which files to compile as part of the project
OBJS = main.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -g add debug info
COMPILER_FLAGS = -g

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = forg

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
