all: client server

client: client.c
	gcc -Wall -Werror -fsanitize=address -lpthread bank.h client.c -o bankingClient

server: server.c
	gcc -Wall -Werror -fsanitize=address -lpthread bank.h server.c -o bankingServer

clean:
	rm -rf bankingServer bankingClient
