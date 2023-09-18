#COMPILER
CC	= gcc
CFLAGS	+= -I ./ -L. -Wl,-rpath=. -ansi -pedantic-errors -Wall -Wextra -lrt -pthread -fPIC
LDFLAGS:= -shared

# Source files for USER_UP and WD_PROCESS
SRCS_USER_UP = DA_USER_AP.c DA_WATCHDOG.c aux_funcs.c
SRCS_WD_PROCESS = watchdog_process.c aux_funcs.c

# Object files for USER_UP and WD_PROCESS
OBJS_RUN1 = $(SRCS_USER_UP:.c=.o)
OBJS_RUN2 = $(SRCS_WD_PROCESS:.c=.o)

debug: CFLAGS+=-g
debug: all

release: CFLAGS+=-DNDEBUG
release: all

all: user_app.out watchdog.out

user_app.out: $(OBJS_RUN1) scheduler.so 
	$(CC) $(CFLAGS) -o $@ $^ 

watchdog.out: $(OBJS_RUN2) scheduler.so 
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -c $<
