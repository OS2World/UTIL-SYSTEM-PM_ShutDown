#
#  PM Shutdown Makefile for use with IBM C Set++ and related tools.
#  (c) 1994 by Carsten Wimmer
#

OBJ     = shutdown.obj

CC      = icc
CCFLAGS = /Q /C /Gm+ /G3 /Gh- /O+ /Gr- /Ti- /Tm- /Tx- /Fi+ /Si+ /Ss+
CL      = link386
CLFLAGS = /nologo /bat /map /BASE:65536 /ALIGN:16 /EXEPACK /pmtype:pm $(OBJ),shutdown.exe,shutdown.map,,shutdown.def

all: shutdown.exe

shutdown.EXE: $(OBJ) shutdown.res Makefile shutdown.def
              $(CL) $(CLFLAGS)
              RC shutdown.res shutdown.exe

shutdown.RES: shutdown.rc Makefile
              RC -r shutdown.rc

shutdown.OBJ: shutdown.h Makefile
              $(CC) $(CCFLAGS) shutdown.c

