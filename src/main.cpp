#include <iostream>

#include "Allocator.hpp"
#include "File.hpp"
#include "Spv.hpp"

using namespace Sol;

int main() {

    MemoryConfig config = {};
    MemoryService::instance()->init(&config);
    Allocator *heap = &MemoryService::instance()->system_allocator;

    size_t code_size = 0;
    const uint32_t *pcode = File::read_spv(&code_size, "triangle3.vert.spv", heap);
    Spv spv = Spv::parse(code_size, pcode);

	spv.kill();

    mem_free(pcode, heap);
    MemoryService::instance()->shutdown();

    return 0;
}
