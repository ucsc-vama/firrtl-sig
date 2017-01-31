CXX ?= g++
CFLAGS = -O3 -std=c++11

uplay: uplay.cc uint.h
	$(CXX) $(CFLAGS) uplay.cc -o uplay

splay: splay.cc sint.h uint.h
	$(CXX) $(CFLAGS) splay.cc -o splay

tester: tester.cc uint.h sint.h
	$(CXX) $(CFLAGS) -Ilest/include/lest tester.cc -o tester

.PHONY: play
play: uplay
	./uplay

.PHONY: test
test: tester
	./tester

.PHONY: clean
clean:
	rm -rf tester uplay
