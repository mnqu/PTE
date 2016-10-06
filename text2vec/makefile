CC = g++
CFLAGS = -lm -pthread -Ofast -march=native -Wall -funroll-loops -Wno-unused-result -lgsl -lm -lgslcblas
INCLUDES = -I/usr/local/include -I../eigen-3.2.4
LIBS = -L/usr/local/lib


infer : infer.cpp
	$(CC) $(CFLAGS) infer.cpp -o infer $(INCLUDES) $(LIBS)

clean :
	rm -rf *.o infer
