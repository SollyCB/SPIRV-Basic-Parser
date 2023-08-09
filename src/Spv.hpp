#pragma once

#include <cstdint>
#include <cstddef>
#include <vulkan/vulkan.hpp>

#include "List.hpp"
#include "Allocator.hpp"
#include "Array.hpp"

namespace Sol {

struct Spv {
    enum class Stage : uint32_t {
        NONE,
        VERT,
        FRAG,
    };
    enum class Storage : uint32_t {
        UNIFORM_CONSTANT = 0,
        INPUT = 1,
        UNIFORM = 2,
        OUTPUT = 3,
        PUSH_CONSTANT = 9,
        IMAGE = 11,
        STORAGE_BUFFER = 12,
    };
    enum class DecoFlagBits : uint32_t {
        BLOCK = 0x0001,
        ROW_MAJOR = 0x0002,
        COL_MAJOR = 0x0004,
        ARRAY_STRIDE = 0x0008,
        MAT_STRIDE = 0x0010,
        BUILTIN = 0x0020,
        CONSTANT = 0x0040,
        UNIFORM = 0x0080,
        LOCATION = 0x0100,
        COMPONENT = 0x0200,
        BINDING = 0x0400,
        DESC_SET = 0x0800,
        OFFSET = 0x1000,
    };
    enum class Name : uint32_t {
        VOID = 19,
        BOOL = 20,
        INT = 21,
        FLOAT = 22,
        VEC = 23,
        MATRIX = 24,
        IMAGE = 25,
        SAMPLER = 26,
        SAMPLED_IMAGE = 27,
        ARRAY = 28,
        RUNTIME_ARRAY = 29,
        STRUCT = 30,
        VAR = 59,
        PTR = 32,
		FWD_PTR = 39,
        VAR_FINAL = 300,
        VEC_FINAL = 301,
        MAT_FINAL = 302,
    };
    struct DecoInfo {
        uint32_t flags = 0x0;
        union {
        uint32_t array_stride;
        uint32_t mat_stride;
        };
        uint32_t location;
        uint32_t component;
        uint32_t binding;
        uint32_t desc_set;
        uint32_t offset;
    };
    struct Var {
        DecoInfo deco_info;
        uint32_t ptr_id;
    };
    struct Ptr {
        Storage storage;
        uint32_t type_id;
    };
    struct Void {};
    struct Int {
        uint32_t width;
        bool sign;
    };
    struct Float {
        uint32_t width;
    };
    struct Vector {
        uint32_t type_id;
        uint32_t length;
    };
    struct Matrix {
        uint32_t type_id;
        uint32_t column_count;
    };
    struct Image {
        enum Dim : uint32_t {
            D1 = 0,
            D2 = 1,
            D3 = 2,
            CUBE = 3,
            RECT = 4,
            BUFFER = 5,
            SUBPASS_DATA = 6,
        };
        enum Depth : uint32_t {
            NO_DEPTH = 0,
            DEPTH = 1,
            NEITHER = 2,
        };
        enum Sampled : uint32_t {
            RUN_TIME = 0,
            SAMPLING = 1,
            READ_WRITE = 2,
        };
        enum Format : uint32_t {
            Unknown = 0,
            Rgba32f = 1,
            Rgba16f = 2,
            R32f = 3,
            Rgba8 = 4,
            Rgba8Snorm = 5,
            Rg32f = 6,
            Rg16f = 7,
            R11fG11fB10f = 8,
            R16f = 9,
            Rgba16 = 10,
            Rgb10A2 = 11,
            Rg16 = 12,
            Rg8 = 13,
            R16 = 14,
            R8 = 15,
            Rgba16Snorm = 16,
            Rg16Snorm = 17,
            Rg8Snorm = 18,
            R16Snorm = 19,
            R8Snorm = 20,
            Rgba32i = 21,
            Rgba16i = 22,
            Rgba8i = 23,
            R32i = 24,
            Rg32i = 25,
            Rg16i = 26,
            Rg8i = 27,
            R16i = 28,
            R8i = 29,
            Rgba32ui = 30,
            Rgba16ui = 31,
            Rgba8ui = 32,
            R32ui = 33,
            Rgb10a2ui = 34,
            Rg32ui = 35,
            Rg16ui = 36,
            Rg8ui = 37,
            R16ui = 38,
            R8ui = 39,
            R64ui = 40,
            R64i = 41,
        };
        uint32_t type_id;
        Dim dim;
        Depth depth;
        bool array;
        bool multisampled;
        Sampled sampled;
        Format format;
    };
    struct SampledImage {
        uint32_t type_id;
    };
    struct Arr {
        uint32_t type_id;
        uint32_t length;
    };
    struct RunTimeArray {
        uint32_t type_id;
    };
    struct Struct {
        Array<uint32_t> type_ids;
    };
    struct Type {
        uint32_t id;
        Name name;
        union {
            Var var;
            Ptr ptr;
            Void empty;
            Int integer;
            Float floating_point;
            Vector vector;
            Matrix matrix;
            Image image;
            SampledImage sampledimage;
            Arr arr;
            RunTimeArray runtimearray;
            Struct structure;
        };
    };
    VkShaderStageFlagBits stage;
    const char* p_name = nullptr;
    List<Type> types;

    static Spv parse(size_t code_size, const uint32_t *spirv, bool *ok);
	void kill();

private:
    enum class Deco : uint32_t {
        BLOCK = 2,
        ROW_MAJOR = 4,
        COL_MAJOR = 5,
        ARRAY_STRIDE = 6,
        MAT_STRIDE = 7,
        BUILTIN = 11,
        CONSTANT = 22,
        UNIFORM = 26,
        LOCATION = 30,
        COMPONENT = 31,
        BINDING = 33,
        DESC_SET = 34,
        OFFSET = 35,
    };
    size_t scratch_mark = 0;

    Type* get_type(uint32_t id, Spv::Name name); 
    inline void new_optype(const uint32_t *instr, Type *type, Spv::Name name) {
        type->id = *(instr + 1);
        type->name = name;
    }


#if DEBUG
public:
    struct DebugInfo {
        uint32_t id;
        const char *name;
    };
    List<DebugInfo> debug;
#endif
};

} // namespace Sol
