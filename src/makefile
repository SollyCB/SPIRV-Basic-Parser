all:
	cmake . -B build -DBUILD_TESTS=ON -DBUILD_DEBUG=ON && cmake --build build -- -j4

test: all
	./build/SlugVk-lib-common-TestAll
