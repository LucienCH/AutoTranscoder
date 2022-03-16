OBJS	= AutoTranscoder.o Fonctions.o
SOURCE	= AutoTranscoder.c Fonctions.c
HEADER	= Fonctions.h
OUT	= AutoTranscoder
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

AutoTranscoder.o: AutoTranscoder.c
	$(CC) $(FLAGS) AutoTranscoder.c 

Fonctions.o: Fonctions.c
	$(CC) $(FLAGS) Fonctions.c 


clean:
	rm -f $(OBJS) $(OUT)
