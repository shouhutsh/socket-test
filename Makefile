CFLAGS=-g -Wall

OBJECT=client_daytime server_daytime

all: $(OBJECT)

clean:
	-rm -rf $(OBJECT)