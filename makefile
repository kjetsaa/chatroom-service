all: server client

client: client.cpp 
	g++ -g -w -std=c++14 -o client client.cpp

server: server.cpp 
	g++ -g -w -std=c++14 -o server server.cpp
	
clean:
	rm -f *.o server client