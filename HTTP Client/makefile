all: http_client

http_client: dummy_http_client.c dummy_http.h
	gcc -o http_client dummy_http_client.c dummy_http.h

clean:
	rm -f http_client