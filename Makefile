MKDEP=$(CC) -MM
SRCS=click.c display.c display2D.c display3D.c human.c key.c load.c main.c rubik.c rubikRot.c
HDRS=click.h display.h display2D.h display3D.h human.h key.h load.h rubik.h rubikRot.h rubikStruct.h
OUTS=click.o display.o display2D.o display3D.o human.o key.o load.o main.o rubik.o rubikRot.o 

rubikRot: $(OUTS)
	$(CC) $(OUTS) -lSDL -lSDL_image -lm -L ~/lib -l3D -g -o rubikRot

depend: .depend
.depend: $(SRCS) $(HDRS)
	$(MKDEP) $(CFLAGS) $(SRCS) > .depend
-include .depend
