#include <cstdio>
#include <iostream>

#include "Allocator.hpp"
#include "File.hpp"

namespace Sol {

const uint32_t *File::read_spv(size_t *byte_count, const char *file_name, Allocator *alloc) {
    FILE *file = fopen(file_name, "r");

    if (!file) {
        std::cerr << "FAILED TO READ FILE " << file_name << "!\n";
        fclose(file);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    *byte_count = ftell(file);
    fseek(file, 0, SEEK_SET);

    void *spv = mem_alloca(*byte_count, 4, alloc);
    fread(spv, *byte_count, 1, file);
    fclose(file);

    return reinterpret_cast<const uint32_t*>(spv);
}

void *File::read_char(size_t *byte_count, const char *file_name, Allocator *alloc) {
    FILE *file = fopen(file_name, "r");

    if (!file) {
        std::cerr << "FAILED TO READ FILE " << file_name << "!\n";
        fclose(file);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    *byte_count = ftell(file);
    fseek(file, 0, SEEK_SET);

    void *buffer = mem_alloca(*byte_count, 1, alloc);
    fread(buffer, *byte_count, 1, file);
    fclose(file);

    return buffer;
}

} // namespace Sol
