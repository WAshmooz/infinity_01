CC=gcc
CFLAGS += -I ./include -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -pthread -fPIC

scheduler.so: src/* include/*
		$(CC) $(CFLAGS) -shared -o scheduler.so src/*.c

