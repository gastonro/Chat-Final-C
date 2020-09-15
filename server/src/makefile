APP = server
CC = gcc
DEPS= ../include/header.h
CFLAGS = -I ../include
ODIR = ../obj
BDIR = ../bin

OBJ = ../obj/server.o

$(BDIR)/server:	$(OBJ)
	$(CC) -o $(APP) $(OBJ) -lpthread
	mv server ../bin

$(ODIR)/%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm $(ODIR)/*.o $(BDIR)/*





