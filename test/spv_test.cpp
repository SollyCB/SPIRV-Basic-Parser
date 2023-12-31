#include "test.hpp"
#include "Spv.hpp"
#include "spv_test.hpp"
#include "File.hpp"
#include "Allocator.hpp"
#include <cstdint>
#include <cstddef>
#include <iostream>

namespace Sol {
    static const bool NO_SKIP = false;
    static const bool SKIP = true;

    static Spv::Type* get_type(List<Spv::Type> types, int ptr_id) {
        for(uint32_t i = 0; i < types.len; ++i) {
            if (types[i].id == ptr_id)
                return &types[i];
        }
        return nullptr;
    }
    static Spv::Type* get_parent(List<Spv::Type> types, int id) {
        for(uint32_t i = 0; i < types.len; ++i) {
            if (types[i].id == id)
                return &types[i];
        }
        return nullptr;
    }

    void test_triangle3(bool skip) {
        size_t code_size = 0;
        const uint32_t *pcode = File::read_spv(&code_size, "triangle3.vert.spv", HEAP);
        bool ok;
        Spv spv = Spv::parse(code_size, pcode, &ok);
        TEST_EQ("SpvParse_check_arg", ok, true, skip);

        using Flags = Spv::DecoFlagBits;

#if DEBUG
// DEBUG_TEST_CHECK_FILE_HASNT_CHANGED 
        std::cout << "RUNNING DEBUG FILE CHECKS...\n";
        const char* ubo_name = "ubo"; \
        const char* UBO_name = "UBO"; \
        const char* in_pos_name = "in_pos"; \
        const char* in_color_name = "in_color"; \
        const char* tex_sampler_name = "tex_sampler"; \
        const char* debug_UBO_name = spv.debug[3].name; \
        const char* debug_ubo_name = spv.debug[4].name; \
        const char* debug_in_pos_name = spv.debug[5].name; \
        const char* debug_in_color_name = spv.debug[7].name; \
        const char* debug_tex_sampler_name = spv.debug[8].name; \
 \
        TEST_STR_EQ("Debug_Name_UBO", debug_UBO_name, UBO_name, skip); \
        TEST_EQ("Debug_ID_UBO", spv.debug[3].id, spv.types[1].id, skip); \
        TEST_STR_EQ("Debug_Name_ubo", debug_ubo_name, ubo_name, skip); \
        TEST_EQ("Debug_ID_ubo", spv.debug[4].id, spv.types[2].id, skip); \
 \
        TEST_STR_EQ("Debug_Name_inpos", debug_in_pos_name, in_pos_name, skip); \
        TEST_EQ("Debug_ID_inpos", spv.debug[5].id, spv.types[3].id, skip); \
        TEST_STR_EQ("Debug_Name_incolor", debug_in_color_name, in_color_name, skip); \
        TEST_EQ("Debug_ID_incolor", spv.debug[7].id, spv.types[5].id, skip); \
 \
        TEST_STR_EQ("Debug_Name_texsampler", debug_tex_sampler_name, tex_sampler_name, skip); \
        TEST_EQ("Debug_ID_texsampler", spv.debug[8].id, spv.types[6].id, skip);
#endif

        Spv::Type *types = spv.types.data;

        // VARs
        Spv::Var ubo = spv.types[2].var;
        uint32_t ubo_id = spv.types[2].id;
        uint32_t deco_flags_binding = static_cast<uint32_t>(Flags::BINDING);
        uint32_t deco_flags_desc_set = static_cast<uint32_t>(Flags::DESC_SET);
        TEST_EQ("ubo_deco_flags", ubo.deco_info.flags, deco_flags_binding | deco_flags_desc_set, skip);
        Spv::Type *ubo_parent = get_parent(spv.types, ubo_id);
        uint32_t ubo_parent_name = static_cast<uint32_t>(ubo_parent->name);
        TEST_EQ("ubo_type", ubo_parent_name, static_cast<uint32_t>(Spv::Name::VAR), skip);

        Spv::Ptr ubo_ptr = get_type(spv.types, ubo.ptr_id)->ptr;
        uint32_t ubo_ptr_id = spv.types[1].id;
        Spv::Type *ubo_ptr_parent = get_parent(spv.types, ubo.ptr_id);
        ASSERT(ubo_ptr_parent != nullptr, "ID not found");
        TEST_EQ("ubo_ptr_id", ubo.ptr_id, ubo_ptr_parent->id, skip);
        TEST_EQ("ubo_ptr_type_id", ubo_ptr.type_id, ubo_ptr_id, skip);
        uint32_t ubo_ptr_parent_name = static_cast<uint32_t>(ubo_ptr_parent->name);
        TEST_EQ("ubo_ptr_type", ubo_ptr_parent_name, static_cast<uint32_t>(Spv::Name::PTR), skip);

        Spv::Var in_color = spv.types[5].var;
        uint32_t in_color_id = spv.types[5].id;
        uint32_t deco_flags_location = static_cast<uint32_t>(Flags::LOCATION);
        TEST_EQ("in_color_flags", in_color.deco_info.flags, deco_flags_location, skip);
        TEST_EQ("in_color_location", in_color.deco_info.location, static_cast<uint32_t>(1), skip);
        Spv::Type *in_color_parent = get_parent(spv.types, in_color_id);
        TEST_EQ("in_color_type", static_cast<uint32_t>(in_color_parent->name), static_cast<uint32_t>(Spv::Name::VAR), skip);

        Spv::Var tex_sampler = spv.types[6].var;
        TEST_EQ("tex_sampler_flags", tex_sampler.deco_info.flags, deco_flags_binding | deco_flags_desc_set, skip);
        TEST_EQ("tex_sampler_binding", tex_sampler.deco_info.binding, static_cast<uint32_t>(1), skip);
        TEST_EQ("tex_sampler_desc_set", tex_sampler.deco_info.desc_set, static_cast<uint32_t>(1), skip);
        Spv::Type *tex_sampler_parent = get_parent(spv.types, in_color_id);
        TEST_EQ("tex_sampler_type", static_cast<uint32_t>(tex_sampler_parent->name), static_cast<uint32_t>(Spv::Name::VAR), skip);

        // INT FLOAT
        Spv::Float floating_point = types[8].floating_point;
        Spv::Int unsigned_int = types[10].integer;
        Spv::Int signed_int = types[15].integer;
        TEST_EQ("floating_point_id", types[8].id, static_cast<uint32_t>(6), skip);
        TEST_EQ("floating_point_type", static_cast<uint32_t>(types[8].name), static_cast<uint32_t>(Spv::Name::FLOAT), skip);
        TEST_EQ("floating_point_width", floating_point.width, static_cast<uint32_t>(32), skip);
        TEST_EQ("unsigned_int_id", types[10].id, static_cast<uint32_t>(8), skip);
        TEST_EQ("unsigned_int_type", static_cast<uint32_t>(types[10].name), static_cast<uint32_t>(Spv::Name::INT), skip);
        TEST_EQ("unsigned_int_sign", unsigned_int.sign, false, skip);
        TEST_EQ("signed_int_width", unsigned_int.width, static_cast<uint32_t>(32), skip);
        TEST_EQ("signed_int_id", types[15].id, static_cast<uint32_t>(14), skip);
        TEST_EQ("signed_int_type", static_cast<uint32_t>(types[15].name), static_cast<uint32_t>(Spv::Name::INT), skip);
        TEST_EQ("signed_int_sign", signed_int.sign, true, skip);
        TEST_EQ("signed_int_width", signed_int.width, static_cast<uint32_t>(32), skip);

        // VEC
        Spv::Vector vec4_float = types[9].vector;
        Spv::Vector vec2_float = types[20].vector;
        Spv::Vector vec3_float = types[23].vector;
        uint32_t float_id = types[8].id;
        TEST_EQ("vec4_type_id", vec4_float.type_id, float_id, skip);
        TEST_EQ("vec4_type_length", vec4_float.length, static_cast<uint32_t>(4), skip);
        TEST_EQ("vec2_type_id", vec2_float.type_id, float_id, skip);
        TEST_EQ("vec2_type_length", vec2_float.length, static_cast<uint32_t>(2), skip);
        TEST_EQ("vec3_type_id", vec3_float.type_id, float_id, skip);
        TEST_EQ("vec3_type_length", vec3_float.length, static_cast<uint32_t>(3), skip);

        // MATRIX
        Spv::Matrix mat4_v4_float = types[16].matrix;
        uint32_t vec4_float_id = types[9].id;
        TEST_EQ("mat4_type_id", mat4_v4_float.type_id, vec4_float_id, skip);
        TEST_EQ("mat4_col_count", mat4_v4_float.column_count, static_cast<uint32_t>(4), skip);

        // STRUCT
        Spv::Struct structure = types[17].structure;
        uint32_t mat4_v4_id = types[16].id;
        TEST_EQ("UBO_structure_member_types[0]", structure.type_ids[0], mat4_v4_id, skip);
        TEST_EQ("UBO_structure_member_types[1]", structure.type_ids[1], mat4_v4_id, skip);
        TEST_EQ("UBO_structure_member_types[2]", structure.type_ids[2], mat4_v4_id, skip);

        spv.kill();
        mem_free(pcode, HEAP);
    }
    void Test::Spv::run() {
        TEST_MODULE_BEGIN("SpvTest_1", true, false);

        test_triangle3(NO_SKIP);

        TEST_MODULE_END();
    }
} // namespace Sol::Test::SpvTest
