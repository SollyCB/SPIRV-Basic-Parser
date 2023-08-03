F = -g

.PHONY: all dbg test just_test release run

all:
	cmake -S . -DONLY_TEST=OFF -DBUILD_TESTS=OFF -DDEBUG=ON -B build && cmake --build build && ./build/exe

dbg:
	cmake -S . -DONLY_TEST=OFF -DBUILD_TESTS=OFF -DDEBUG=ON -B build && cmake --build build

release:
	cmake -S . -DONLY_TEST=OFF -DBUILD_TESTS=OFF -DDEBUG=OFF -B build && cmake --build build

test:
	cmake -S . -DONLY_TEST=OFF -DBUILD_TESTS=ON -B build && cmake --build build && ./build/test_all

just_test:
	cmake -S . -DONLY_TEST=ON -DBUILD_TESTS=ON -B build && cmake --build build && ./build/test_all


run:
	./build/exe

old:
	g++ $(F) -o bin File.cpp tlsf.cpp Allocator.cpp Spv.cpp main.cpp && ./bin


