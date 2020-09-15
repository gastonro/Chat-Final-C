APP = client
CC = gcc
DEPS= ../include/header.h
CFLAGS = -I ../include
ODIR = ../obj
BDIR = ../bin

OBJ = ../obj/client.o

$(BDIR)/client:	$(OBJ)
	$(CC) -o $(APP) $(OBJ) -lpthread
	mv client ../bin

$(ODIR)/%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(ODIR)/*.o $(BDIR)/*





