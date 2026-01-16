all: chat_server chat_client

chat_server: chatServer.c
	gcc -o chatServer chatServer.c

chat_client: chatClient.c
	gcc -o chatClient chatClient.c

clean:
	rm -f chatServer chatClient
