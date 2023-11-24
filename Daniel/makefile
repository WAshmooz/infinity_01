gd=gcc -ansi -pedantic-errors -Wall -Wextra -g -D_DEBUG
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -D_DEBUG -pthread
INCLUDE= ./include -I 

./wes_pong: src/udp_pong.c
	@echo "will compile pong" 
	$(gd) -I include src/utils.c src/udp_pong.c -o wes_pong

./wes_ping: src/udp_ping.c 
	@echo "will compile ping"
	$(gd) -I include src/utils.c src/udp_ping.c -o wes_ping

