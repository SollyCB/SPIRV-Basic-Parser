#include "test.hpp"
#include "spv_test.hpp"
#include "Allocator.hpp"

using namespace Sol;

int main() {

    MemoryConfig config = {};
    MemoryService::instance()->init(&config);
    Allocator *heap = &MemoryService::instance()->system_allocator;

    Sol::Test::SpvTest::run();

    MemoryService::instance()->shutdown();

    return 0;
}
