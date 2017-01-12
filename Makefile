CXX ?= g++
CFLAGS = -O3 -std=c++11

uplay: uplay.cc uint.h
	$(CXX) $(CFLAGS) uplay.cc -o uplay

.PHONY: play
play: uplay
	./uplay
