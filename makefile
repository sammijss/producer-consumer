# define compiler
CPPCOMPILER=g++

# define compiler flags
CPPFLAGS=-std=c++11 -g -Wall

# define executable target
EXECUTABLEFILE=producer-consumer

all: $(EXECUTABLEFILE)

$(EXECUTABLEFILE):
	$(CPPCOMPILER) $(CPPFLAGS) -o $(EXECUTABLEFILE) producer-consumer.cpp -lpthread

clean:
	rm -f $(EXECUTABLEFILE) *.o
