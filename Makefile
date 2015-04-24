CFLAGS=-g -Wall

OBJECT=client_daytime server_daytime \
	client_string server_string

all: $(OBJECT)

clean:
	-rm -rf $(OBJECT)