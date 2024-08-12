.PHONY:all
all:shm_server shm_client

shm_server:shm_server.cc
	g++ -o $@ $^ -std=c++11
shm_client:shm_client.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -rf shm_client shm_server