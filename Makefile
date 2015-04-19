CFLAGS=-g -Wall

OBJECT=client_hello server_hello

all: $(OBJECT)

clean:
	-rm -rf $(OBJECT)