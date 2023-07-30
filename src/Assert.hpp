#define V_LAYERS 1

#define ASSERT(_arg1, _arg2) if (!(_arg1)) { \
    std::cout << "ASSERT FAILED IN " << __FILE__ << ", " << __LINE__ << ": " << #_arg1 << ", " << _arg2 << '\n'; \
    __builtin_trap(); \
}

#ifdef V_LAYERS
#define DEBUG_ASSERT(_arg1, _arg2) if (!(_arg1)) { \
    std::cout << "ASSERT FAILED IN " << __FILE__ << ", " << __LINE__ << ": " << #_arg1 << ", " << _arg2 << '\n'; \
    __builtin_trap(); \
}
#else

#define DEBUG_ASSERT(_arg1, _arg2)

#endif


