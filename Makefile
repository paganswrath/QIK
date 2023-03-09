
  UOS=$(shell uname)
  ifeq ($(OS),Windows_NT)
        PLATFORM = Windows
    else
		ifeq ($(UOS),Linux)
			PLATFORM = Linux
		endif
	endif

  ifeq ($(PLATFORM),Windows)
    CC = wine C:/raylib/mingw/bin/g++.exe
    exename ?= QIK.exe
    CFLAGS  = -s -Os -Wall -Iexternal -DPLATFORM_DESKTOP -lopengl32 -lraylib -lgdi32 -lwinmm -Wl,--subsystem,windows
  endif

  ifeq ($(PLATFORM),Linux)
    CC = g++
    exename ?= QIK
    CFLAGS  = -Wall -Wno-missing-braces -s  -lraylib -lGL -lm -ldl -lrt -lX11
  endif

  all: $(TARGET)
	  $(CC) Source/Main.cpp -o $(exename) $(CFLAGS)
