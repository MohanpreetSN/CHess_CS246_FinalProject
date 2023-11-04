CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -g -Werror=vla 
EXEC = chess
VPATH = ./pieces:./graphicsDisplay
OBJECTS = main.o \
controller.o \
exception.o \
textDisplay.o \
boardModel.o \
boardState.o \
position.o \
piece.o \
pawn.o \
pieces/bishop.o pieces/rook.o pieces/queen.o pieces/king.o pieces/knight.o \
subject.o \
graphicsDisplay/graphicsDisplay.o \
graphicsDisplay/window.o \
graphicsDisplay/graphicsException.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11 -lXpm

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
