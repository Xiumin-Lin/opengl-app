CC = g++
SRC = main.cpp $(wildcard ./include/*.cpp) $(wildcard ./common/*.cpp) $(wildcard ./vendor/*.cc) $(wildcard ./vendor/stb/*.cpp)

# MacOS makefile for OpenGL application
BIN = opengl_app
FLAGS = -std=c++14 -Wall -pedantic -arch arm64 -fmessage-length=0
INCLUDE = -I/opt/homebrew/opt/glew/include -I/opt/homebrew/opt/glfw/include -Ivendor
LOC_LIB = -L/opt/homebrew/opt/glew/lib -L/opt/homebrew/opt/glfw/lib -lGLEW -lglfw
FRAMEWORKS = -framework Cocoa -framework OpenGL -framework IOKit

all:
	${CC} ${FLAGS} ${FRAMEWORKS} -o ${BIN} ${SRC} ${INCLUDE} ${LOC_LIB}

clean:
	rm -f ${BIN}

# MingWIN makefile for OpenGL application
# BIN = opengl_app.exe
# FLAGS = -std=c++14 -Wall -pedantic -fmessage-length=0
# INCLUDE = -I../libs/GLFW/include -I../libs/GLEW/include -Ivendor
# LOC_LIB = -L../libs/GLFW/lib/lib-mingw-w64 -L../libs/GLEW/lib/Release/x64 -lglfw3 -lglew32 -lopengl32

# all:
# 	${CC} ${FLAGS} -o ${BIN} ${SRC} ${INCLUDE} ${LOC_LIB}

# clean:
# 	del /F /Q ${BIN}