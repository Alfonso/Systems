all: client server

client: client.c
	gcc -Wall -Werror -fsanitize=address client.c -o bankingClient

server: server.c
	gcc -Wall -Werror -fsanitize=address server.c -o bankingServer

clean:
	rm -rf bankingServer bankingClient
