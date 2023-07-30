#include <iostream>

#include "Spv.hpp"
#include "Assert.hpp"
#include "Array.hpp"

/*
    TODO:(Sol): 
    1. Forward referencing in spirv
*/

namespace Sol {

static const uint32_t magic = 0x07230203;
static LinearAllocator *scratch = &MemoryService::instance()->scratch_allocator;

template<typename T>
inline T* convert(void *ptr) {
    return reinterpret_cast<T*>(ptr);
}

Spv Spv::parse(size_t code_size, const uint32_t *spirv)
{
    Spv ret = {};
    if (spirv[0] != magic) {
        ret.stage = Stage::NONE;
        //ret.stage = VK_SHADER_STAGE_ALL;
        return ret;
    }

    ret.scratch_mark = scratch->get_mark();

#if SPV_DEBUG
    DebugInfo debug = {};
    ret.debug.init(16);
#endif

    ret.types.init(16);
    // Avoid multiple constr/destr calls
    Type type = {};
    Var var = {};

    
    uint16_t inc = 5;
    while (inc < code_size / 4) {
        const uint16_t *info = reinterpret_cast<const uint16_t*>(spirv + inc);
        const uint32_t *instr = spirv + inc;

        switch (info[0]) {
        // stage
			case 15: 
            {
				// TODO:(Sol): Support other shader stages
				const uint32_t *model = instr + 1;
				switch (*model) {
				case 0:
					//ret.stage = VK_SHADER_STAGE_VERTEX_BIT;
					ret.stage = Stage::VERT;
					break;
				case 4:
					//ret.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
					ret.stage = Stage::FRAG;
					break;
				default:
					break;
				}
				ret.p_name = reinterpret_cast<const char*>(model + 2);
				break;
			}
            // OpDecorate
            case 71:
            {
                Var* var_ref = ret.get_T<Var>(*(instr + 1), Name::VAR);
                Deco deco = static_cast<Deco>(*(instr + 2));
                uint32_t literal = *(instr + 3);

                switch(deco) {
                    case Deco::BLOCK:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::BLOCK);
                        break;
                    }
                    case Deco::ROW_MAJOR:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::ROW_MAJOR);
                        break;
                    }
                    case Deco::COL_MAJOR:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::COL_MAJOR);
                        break;
                    }
                    case Deco::ARRAY_STRIDE:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::ARRAY_STRIDE);
                        var_ref->deco_info.array_stride = literal;
                        break;
                    }
                    case Deco::MAT_STRIDE:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::MAT_STRIDE);
                        var_ref->deco_info.mat_stride = literal;
                        break;
                    }
                    case Deco::BUILTIN:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::BUILTIN);
                        break;
                    }
                    case Deco::CONSTANT:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::CONSTANT);
                        break;
                    }
                    case Deco::UNIFORM:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::UNIFORM);
                        break;
                    }
                    case Deco::LOCATION:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::LOCATION);
                        var_ref->deco_info.location = literal;
                        break;
                    }
                    case Deco::COMPONENT:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::COMPONENT);
                        var_ref->deco_info.component = literal;
                        break;
                    }
                    case Deco::BINDING:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::BINDING);
                        var_ref->deco_info.binding = literal;
                        break;
                    }
                    case Deco::DESC_SET: 
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::DESC_SET);
                        var_ref->deco_info.desc_set = literal;
                        break;
                    }
                    case Deco::OFFSET:
                    {
                        var_ref->deco_info.flags |= static_cast<uint32_t>(DecoFlagBits::OFFSET);
                        var_ref->deco_info.offset = literal;
                        break;
                    }
                    default:
                        break;
                }

                break;
            }
            // OpMemberDecorate
            case 72:
            {
                // TODO:(Sol): 
                break;
            }
            // OpVar
            case 59:
            {
                Var *var = ret.get_T<Var>(*(instr + 2), Name::VAR);
                var->ptr_id = *(instr + 1);

                break;
            }
            // OpTypePointer
            case 32:
            {
                Ptr *ptr = ret.get_T<Ptr>(*(instr + 1), Name::PTR);
                ptr->storage = static_cast<Storage>(*(instr + 2));
                ptr->type_id = *(instr + 3);

                break;
            }
            // OpTypeVoid 
            case 19:
            {
                ret.new_optype<int>(instr, nullptr, Name::VOID);
                break;
            }
            // OpTypeBool
            case 20:
            {
                ret.new_optype<int>(instr, nullptr, Name::BOOL);
                break;
            }
            // OpTypeInt
            case 21:
            {
                Int integer = {};
                integer.width = *(instr + 2);
                integer.sign = *(instr + 3);

                ret.new_optype<Int>(instr, &integer, Name::INT);
                break;
            }
            // OpTypeFloat
            case 22:
            {
                Float floating_point = {};
                floating_point.width = *(instr + 2);

                ret.new_optype<Float>(instr, &floating_point, Name::FLOAT);
                break;
            }
            // OpTypeVector
            case 23:
            {
                Vector vec = {};
                vec.type_id = *(instr + 2);
                vec.length = *(instr + 3);

                ret.new_optype<Vector>(instr, &vec, Name::VEC);
                break;
            }
            // OpTypeMatrix
            case 24:
            {
                Matrix mat = {};
                mat.type_id = *(instr + 2);
                mat.column_count = *(instr + 3);

                ret.new_optype<Matrix>(instr, &mat, Name::MATRIX);
                break;
            }


#if SPV_DEBUG
            // OpName
            case 5:
            {
                debug.id = *(instr + 1);
                debug.name = reinterpret_cast<const char*>(instr + 2);
                ret.debug.push(debug);

                break;
            }
#endif
        }

        inc += info[1];
    }
    return ret;
}


// TODO:(Sol): Loop through pointers and collect type info into final structs (not yet defined)
//  Collect all info first, then collect. Bouncing around will be ugly...

void Spv::kill() {
    scratch->cut_diff(scratch_mark);
    types.kill();

#if SPV_DEBUG
    debug.kill();
#endif
}

} // namespace Sol
