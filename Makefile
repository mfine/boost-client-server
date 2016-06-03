CC        = g++
LDFLAGS   = -I/usr/local/Cellar/boost/1.58.0/include
LLIBFLAGS = -L/usr/local/Cellar/boost/1.58.0/
LINKFLAGS = -lboost_system
FLAGS     = $(LLIBFLAGS) $(LDFLAGS) $(LINKFLAGS)
EXECS     = client.exe server.exe

all: $(EXECS)

%.exe: %.cpp
	$(CC) $(FLAGS) $< message.cpp -o $@

clean:
	rm $(EXECS)
