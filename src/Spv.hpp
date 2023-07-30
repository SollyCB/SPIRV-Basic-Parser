#pragma once 

#include <cstdint>
#include <cstddef>
//#include <vulkan/vulkan.hpp>

#include "Vec.hpp"
#include "Allocator.hpp"

namespace Sol {
	
struct Spv {
    enum class Stage {
        NONE,
        VERT,
        FRAG,
    };
    enum class Storage {
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
    enum class Name {
        VOID = 19,
        BOOL = 20,
        INT = 21,
        FLOAT = 22,
        VEC = 23,
        MATRIX = 24,
        VAR = 59,
        PTR = 32,
    };
    struct Type {
        int id = -1;
        Name name;
        void *data = nullptr;
    };
    struct DecoInfo {
        uint32_t flags = 0x0;
        int array_stride = -1;
        int mat_stride = -1;
        int location = -1;
        int component = -1;
        int binding = -1;
        int desc_set = -1;
        int offset = -1;
    };
    struct Var {
        DecoInfo deco_info;
        int ptr_id = -1;
    };
    struct Ptr {
        Storage storage;
        int type_id = -1;
    };
    struct Int {
        int width = -1;
        bool sign = false;
    };
    struct Float {
        int width = -1;
    };
    struct Vector {
        int type_id = -1;
        int length = -1;
    };
    struct Matrix {
        int type_id = -1;
        int column_count = -1;
    };

    // Member objects
    // TODO:Sol: edit returned stage
    // VkShaderStageFlagBits stage = VK_SHADER_STAGE_ALL;
    Stage stage = Stage::NONE;
    const char* p_name = nullptr;
    Vec<Type> types;

    static Spv parse(size_t code_size, const uint32_t *spirv);
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

    template<typename T>
    void new_optype(const uint32_t *instr, T *t, Spv::Name name) {
        Spv::Type type = {};
        type.id = *(instr + 1);
        type.name = name;

        if (!t)
            return;

        type.data = lin_alloc(sizeof(T));
        mem_cpy(type.data, t, sizeof(T));
        types.push(type);
    }
    template<typename T>
    T* get_T(uint32_t id, Spv::Name name) {
        for(int i = 0; i < types.len; ++i) {
            if (id == types[i].id)
                return reinterpret_cast<T*>(types[i].data);
        }
        Spv::Type type = {};
        type.id = id;
        type.name = name;
        type.data = lin_alloc(sizeof(T));

        types.push(type);
        return reinterpret_cast<T*>(types.last()->data);
    }


#if SPV_DEBUG
public:
    struct DebugInfo {
        int id;
        const char *name;
    };
    Vec<DebugInfo> debug;
#endif
};

} // namespace Sol
