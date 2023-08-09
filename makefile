F = -g
CP = && cp triangle3.vert.spv build/

.PHONY: all dbg test just_test release run

all:
	cmake -S . -DCMAKE_BUILD_TYPE=Debug -DONLY_TEST=OFF -DBUILD_TESTS=OFF -DDEBUG=ON -B build && cmake --build build && ./build/exe $(CP)

.PHONY: dbg
dbg:
	cmake -S . -DCMAKE_BUILD_TYPE=Debug -DONLY_TEST=OFF -DBUILD_TESTS=ON -DDEBUG=ON -B build && cmake --build build $(CP)

.PHONY: test
test:
	cmake -S . -DCMAKE_BUILD_TYPE=Debug -DONLY_TEST=OFF -DBUILD_TESTS=ON -DDEBUG=ON -B build && cmake --build build $(CP) && ./build/test_all

release:
	cmake -S . -DONLY_TEST=OFF -DBUILD_TESTS=OFF -DDEBUG=OFF -B build && cmake --build build

run: all
	./build/exe

old:
	g++ $(F) -o bin File.cpp tlsf.cpp Allocator.cpp Spv.cpp main.cpp && ./bin


