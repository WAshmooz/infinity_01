#COMPILER
CC	= gcc
CFLAGS	+= -I ./ -L. -Wl,-rpath=. -ansi -pedantic-errors -Wall -Wextra -lrt -pthread -fPIC
LDFLAGS:= -shared

# Source files for USER_UP and WD_PROCESS
SRCS_USER_UP = user_app.c watchdog.c  common.c aux_funcs.c
SRCS_WHACHDOG_MAIN = watchdog_main.c  common.c aux_funcs.c

# Object files for USER_UP and WD_PROCESS
OBJS_RUN1 = $(SRCS_USER_UP:.c=.o) 
OBJS_RUN2 = $(SRCS_WHACHDOG_MAIN:.c=.o)

debug: CFLAGS+=-g
debug: all

release: CFLAGS+=-DNDEBUG
release: all

all: user_app.out watchdog_main.out

user_app.out: $(OBJS_RUN1) scheduler.so common.so
	$(CC) $(CFLAGS) -o $@ $^ 

watchdog_main.out: $(OBJS_RUN2) scheduler.so 
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

