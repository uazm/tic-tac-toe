CC = clang
CXX = clang++
CFLAGS = -ggdb
CXXFLAGS = $(CFLAGS)

BINS = main-cpp main-c

all: $(BINS)

main-c: main.c
	${CC} $(CFLAGS) $^ -o $@

main-cpp: main.cpp
	${CXX} $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(BINS)

.PHONY: all  clean main-c main-cpp
