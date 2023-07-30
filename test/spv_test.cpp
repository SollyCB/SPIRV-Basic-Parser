#include "test.hpp"
#include "spv_test.hpp"
#include "Spv.hpp"
#include "File.hpp"
#include "Allocator.hpp"
#include <cstdint>
#include <cstddef>

#if SPV_DEBUG
#define DEBUG_TEST_CHECK_FILE_HASNT_CHANGED \
        uint32_t* ubo_name = (uint32_t*)"ubo"; \
        uint32_t* UBO_name = (uint32_t*)"UBO"; \
        uint32_t* in_pos_name = (uint32_t*)"in_pos"; \
        uint32_t* in_color_name = (uint32_t*)"in_color"; \
        uint32_t* tex_sampler_name = (uint32_t*)"tex_sampler"; \
        uint32_t* debug_UBO_name = (uint32_t*)spv.debug[3].name; \
        uint32_t* debug_ubo_name = (uint32_t*)spv.debug[4].name; \
        uint32_t* debug_in_pos_name = (uint32_t*)spv.debug[5].name; \
        uint32_t* debug_in_color_name = (uint32_t*)spv.debug[7].name; \
        uint32_t* debug_tex_sampler_name = (uint32_t*)spv.debug[8].name; \
 \
        TEST_EQ(*debug_UBO_name, *UBO_name, true, skip); \
        TEST_EQ(spv.debug[3].id, spv.types[1].id, true, skip); \
        TEST_EQ(*debug_ubo_name, *ubo_name, true, skip); \
        TEST_EQ(spv.debug[4].id, spv.types[2].id, true, skip); \
 \
        TEST_EQ(*debug_in_pos_name, *in_pos_name, true, skip); \
        TEST_EQ(spv.debug[5].id, spv.types[3].id, true, skip); \
        TEST_EQ(*debug_in_color_name, *in_color_name, true, skip); \
        TEST_EQ(spv.debug[7].id, spv.types[5].id, true, skip); \
 \
        TEST_EQ(*debug_tex_sampler_name, *tex_sampler_name, true, skip); \
        TEST_EQ(spv.debug[8].id, spv.types[6].id, true, skip);
#endif

namespace Sol::Test::SpvTest {
    static const bool SKIP = false;
    static Allocator *HEAP = &MemoryService::instance()->system_allocator;

    template<typename T>
    static T* get_T(Vec<Spv::Type> types, int ptr_id) {
        for(uint32_t i = 0; i < types.len; ++i) {
            if (types[i].id == ptr_id)
                return reinterpret_cast<T*>(types[i].data);
        }
        return nullptr;
    }
    static Spv::Type* get_parent(Vec<Spv::Type> types, int id) {
        for(uint32_t i = 0; i < types.len; ++i) {
            if (types[i].id == id)
                return &types[i];
        }
        return nullptr;
    }

    void test_triangle3(bool skip) {
        TEST_FUNC_BEGIN();

        size_t code_size = 0;
        const uint32_t *pcode = File::read_spv(&code_size, "triangle3.vert.spv", HEAP);
        Spv spv = Spv::parse(code_size, pcode);

        using Flags = Spv::DecoFlagBits;

#if SPV_DEBUG
        DEBUG_TEST_CHECK_FILE_HASNT_CHANGED;
#endif

        Spv::Type *types = spv.types.data;

        // VARs
        Spv::Var ubo = *(reinterpret_cast<Spv::Var*>(spv.types[2].data));
        uint32_t ubo_id = spv.types[2].id;
        uint32_t deco_flags_binding = static_cast<uint32_t>(Flags::BINDING);
        uint32_t deco_flags_desc_set = static_cast<uint32_t>(Flags::DESC_SET);
        TEST_EQ(ubo.deco_info.flags, deco_flags_binding | deco_flags_desc_set, true, skip);
        Spv::Type *ubo_parent = get_parent(spv.types, ubo_id);
        uint32_t ubo_parent_name = static_cast<uint32_t>(ubo_parent->name);
        TEST_EQ(ubo_parent_name, static_cast<uint32_t>(Spv::Name::VAR), true, skip);

        Spv::Ptr *ubo_ptr = get_T<Spv::Ptr>(spv.types, ubo.ptr_id);
        int ubo_ptr_id = spv.types[1].id;
        Spv::Type *ubo_ptr_parent = get_parent(spv.types, ubo.ptr_id);
        ASSERT(ubo_ptr_parent != nullptr, "ID not found");
        TEST_EQ(ubo.ptr_id, ubo_ptr_parent->id, true, skip);
        TEST_EQ(ubo_ptr->type_id, ubo_ptr_id, true, skip); 
        uint32_t ubo_ptr_parent_name = static_cast<uint32_t>(ubo_ptr_parent->name);
        TEST_EQ(ubo_ptr_parent_name, static_cast<uint32_t>(Spv::Name::PTR), true, skip);

        Spv::Var in_color = *(reinterpret_cast<Spv::Var*>(spv.types[5].data));
        uint32_t in_color_id = spv.types[5].id;
        uint32_t deco_flags_location = static_cast<uint32_t>(Flags::LOCATION);
        TEST_EQ(in_color.deco_info.flags, deco_flags_location, true, skip);
        TEST_EQ(in_color.deco_info.location, 1, true, skip);
        Spv::Type *in_color_parent = get_parent(spv.types, in_color_id);
        TEST_EQ(static_cast<uint32_t>(in_color_parent->name), static_cast<uint32_t>(Spv::Name::VAR), true, skip);

        Spv::Var tex_sampler = *(reinterpret_cast<Spv::Var*>(spv.types[6].data));
        TEST_EQ(tex_sampler.deco_info.flags, deco_flags_binding | deco_flags_desc_set, true, skip);
        TEST_EQ(tex_sampler.deco_info.binding, 1, true, skip);
        TEST_EQ(tex_sampler.deco_info.desc_set, 1, true, skip);
        Spv::Type *tex_sampler_parent = get_parent(spv.types, in_color_id);
        TEST_EQ(static_cast<uint32_t>(tex_sampler_parent->name), static_cast<uint32_t>(Spv::Name::VAR), true, skip);

        // INT FLOAT
        Spv::Float floating_point = *reinterpret_cast<Spv::Float*>(types[7].data);
        Spv::Int unsigned_int = *reinterpret_cast<Spv::Int*>(types[9].data);
        Spv::Int signed_int = *reinterpret_cast<Spv::Int*>(types[12].data);
        TEST_EQ(types[7].id, 6, true, skip);
        TEST_EQ(static_cast<uint32_t>(types[7].name), static_cast<uint32_t>(Spv::Name::FLOAT), true, skip);
        TEST_EQ(floating_point.width, 32, true, skip);
        TEST_EQ(types[9].id, 8, true, skip);
        TEST_EQ(static_cast<uint32_t>(types[9].name), static_cast<uint32_t>(Spv::Name::INT), true, skip);
        TEST_EQ(unsigned_int.sign, false, true, skip);
        TEST_EQ(unsigned_int.width, 32, true, skip);
        TEST_EQ(types[12].id, 14, true, skip);
        TEST_EQ(static_cast<uint32_t>(types[12].name), static_cast<uint32_t>(Spv::Name::INT), true, skip);
        TEST_EQ(signed_int.sign, true, true, skip);
        TEST_EQ(signed_int.width, 32, true, skip);

        // VEC
        Spv::Vector vec4_float = *reinterpret_cast<Spv::Vector*>(types[8].data);
        Spv::Vector vec2_float = *reinterpret_cast<Spv::Vector*>(types[16].data);
        Spv::Vector vec3_float = *reinterpret_cast<Spv::Vector*>(types[19].data);
        int float_id = types[7].id;
        TEST_EQ(vec4_float.type_id, float_id, true, skip);
        TEST_EQ(vec4_float.length, 4, true, skip);
        TEST_EQ(vec2_float.type_id, float_id, true, skip);
        TEST_EQ(vec2_float.length, 2, true, skip);
        TEST_EQ(vec3_float.type_id, float_id, true, skip);
        TEST_EQ(vec3_float.length, 3, true, skip);

        // MATRIX
        Spv::Matrix mat4_v4_float = *reinterpret_cast<Spv::Matrix*>(types[13].data);
        int vec4_float_id = types[8].id;
        TEST_EQ(mat4_v4_float.type_id, vec4_float_id, true, skip);
        TEST_EQ(mat4_v4_float.column_count, 4, true, skip);

        spv.kill();
        mem_free(pcode, HEAP);
        TEST_FUNC_END();
    }
    void run() {
        TEST_PRINT_BEGIN();
        test_triangle3(SKIP);
    }
} // namespace Sol::Test::SpvTest
