#pragma once
#include <cstdint>
#include <cstddef>
#include <utility>
#include "Allocator.hpp"
#include "Assert.hpp"

namespace Sol {

template <typename T>
struct Array {
  T* mem = nullptr;
  size_t cap = 0;
  Allocator *alloc = &MemoryService::instance()->scratch_allocator;

static Array<T> get(size_t size, size_t alignment) {
    Array a;
    a.init(size, alignment);
    return a;
}
static Array<T> get(size_t size, size_t alignment, Allocator *alloc_) {
    Array a;
    a.init(size, alignment, alloc_);
    return a;
}
void init(size_t size, size_t alignment, Allocator *alloc_) {
  cap = size;
  alloc = alloc_;
  mem = reinterpret_cast<T*>(mem_alloca(size * sizeof(T), alignment, alloc));
}
void init(size_t size, size_t alignment) {
  cap = size;
  alloc = SCRATCH;
  mem = reinterpret_cast<T*>(mem_alloca(size * sizeof(T), alignment, alloc));
}
void kill() {
    mem_free(mem, alloc);
}
void copy_here(T* data, size_t count, size_t offset) {
  mem_cpy(mem + offset, data, count * sizeof(T));
}
T& operator[](size_t i) {
    ASSERT(i < cap, "OUT OF BOUND ACCESS ON ARRAY");
  return mem[i];
}
};

} // namespace Sol
