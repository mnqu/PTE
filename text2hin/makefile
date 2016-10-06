CC = g++
CFLAGS = -lm -pthread -Ofast -march=native -Wall -funroll-loops -Wno-unused-result -lgsl -lm -lgslcblas
INCLUDES = -I/usr/local/include -I../eigen-3.2.4
LIBS = -L/usr/local/lib


data2dl data2w: data2dl.cpp data2w.cpp
	$(CC) $(CFLAGS) data2dl.cpp -o data2dl $(INCLUDES) $(LIBS)
	$(CC) $(CFLAGS) data2w.cpp -o data2w $(INCLUDES) $(LIBS)

clean :
	rm -rf *.o data2dl data2w
