CC=g++
CFLAGS=-Iinclude -std=c++11
LDFLAGS=-ljack -lsoundtouch

dsp: src/main.cpp
\t$(CC) $(CFLAGS) -o dsp src/main.cpp $(LDFLAGS)

clean:
\trm dsp
