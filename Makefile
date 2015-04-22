CFLAGS=-g -Wall

OBJECT=client_hello server_hello client_daytime server_daytime

all: $(OBJECT)

clean:
	-rm -rf $(OBJECT)