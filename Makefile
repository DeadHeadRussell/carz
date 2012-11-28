# Project: Carz
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++.exe
CC   = gcc.exe
WINDRES = windres.exe
RES  = Carz_private.res
OBJ  = main.o game.o car.o track.o keys.o font.o $(RES)
LINKOBJ  = main.o game.o car.o track.o keys.o font.o $(RES)
LIBS =  -L"lib" -mwindows -lopengl32  
INCS =  -I"include" 
CXXINCS =  -I"lib/gcc/mingw32/3.4.2/include"  -I"include/c++/3.4.2/backward"  -I"include/c++/3.4.2/mingw32"  -I"include/c++/3.4.2"  -I"include" 
BIN  = Carz.exe
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f
DEL = del

.PHONY: all all-before all-after clean clean-custom

all: all-before Carz.exe all-after


clean: clean-custom
	-${DEL} $(OBJ) $(BIN)
	-${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "Carz.exe" $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

game.o: game.cpp
	$(CPP) -c game.cpp -o game.o $(CXXFLAGS)

car.o: car.cpp
	$(CPP) -c car.cpp -o car.o $(CXXFLAGS)

track.o: track.cpp
	$(CPP) -c track.cpp -o track.o $(CXXFLAGS)

keys.o: keys.cpp
	$(CPP) -c keys.cpp -o keys.o $(CXXFLAGS)

font.o: font.cpp
	$(CPP) -c font.cpp -o font.o $(CXXFLAGS)

Carz_private.res: Carz_private.rc 
	$(WINDRES) -i Carz_private.rc --input-format=rc -o Carz_private.res -O coff 
