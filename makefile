all: server client

server: server.cpp
	g++ -o server server.cpp -lws2_32 -leasyx
client: client.cpp
	g++ -o client client.cpp -lws2_32 -leasyx
	.\server