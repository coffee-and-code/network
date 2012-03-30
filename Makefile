CC = gcc
CFLAGS = -Wall -g
SRC = ./src
MAKE = make
SERVER_BIN = server
CLIENT_BIN = client

all: server client

server:
	$(CC) $(CFLAGS) $(SRC)/server.c -o $(SERVER_BIN)
	
client:
	$(CC) $(CFLAGS) $(SRC)/client.c -o $(CLIENT_BIN)

clean:
	rm $(SERVER_BIN) $(CLIENT_BIN)
