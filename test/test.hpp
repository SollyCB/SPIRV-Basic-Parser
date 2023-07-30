#pragma once
#include <iostream>

#define GLOBAL_TEST_KILL false
#define GLOBAL_TEST_NO_KILL true

static const char* RED = "\033[0;31m";
static const char* NC = "\033[0m";
static const char* GREEN = "\033[0;33m";
static const char* YELLOW = "\033[1;33m";

#define TEST_MSG() \
    std::cout << "Test Func: " << __FUNCTION__ << ", Result: \n"
#define TEST_FUNC_BEGIN() \
    TEST_MSG(); \
    size_t test_index__ = 0; \
    bool test_pass_check__ = true;

#define TEST_PRINT_BEGIN() \
    std::cout << "\nBeginning Test Suite " << __FILE__ << ":\n"
#define TEST_PRINT_PASS() \
    std::cout << GREEN << "OK\n" << NC;
#define TEST_PRINT_SKIP() \
    std::cout << YELLOW << "SKIPPED\n" << NC;

#define TEST_PRINT_FAIL(print_arg1__, print_arg2__, op_arg__) \
    std::cout << RED << "FAIL " << NC << "[Test Index: " << test_index__ << "] (" << #print_arg1__ << ' ' << #op_arg__ << ' ' << #print_arg2__ << ")\n";

#define TEST_PRINT_FAIL_INFO(print_arg1__, print_arg2__, op_arg__) \
    std::cout << RED << "FAIL " << NC << "[Test Index: " << test_index__ << "] (" << #print_arg1__ \
    << ' ' << #op_arg__ << ' ' << #print_arg2__ << ", " << #print_arg1__ << " = " << print_arg1__ \
    << ", " << #print_arg2__ << " = " << (print_arg2__) << ")\n";

#define TEST_FUNC_END() \
    if (test_pass_check__) { \
        TEST_PRINT_PASS(); \
    } \
    test_index__ = 0;


namespace Sol::Test {

template<typename T>
bool match_eq(T arg1, T arg2, bool skip) {
    if (skip) {
        TEST_PRINT_SKIP();
        return true;
    }
    if (arg1 != arg2) {
        return false;
    }
    return true;
}
template<typename T>
bool byte_match_eq(T arg1, T arg2, bool skip) {
    if (skip) {
        TEST_PRINT_SKIP();
        return true;
    }
    size_t size = sizeof(T);
    char* char_arg1 = reinterpret_cast<char*>(&arg1);
    char* char_arg2 = reinterpret_cast<char*>(&arg2);
    for(size_t i = 0; i < size; ++i) {
        if (char_arg1[i] != char_arg2[i]) {
            return false;
        }
    }
    return true;
}
} // namespace Sol::Test

#define KILL_TEST(mod_kill__) \
    if (GLOBAL_TEST_KILL) { \
        std::cout << "\n    TEST HALT EXECUTION ENABLED (GLOBAL), CRASHING PROGRAM \n"; \
        __builtin_trap(); \
    } \
    else if (!GLOBAL_TEST_NO_KILL && mod_kill__) { \
        std::cout << "\n    TEST HALT EXECUTION ENABLED (TEST LOCAL), CRASHING PROGRAM \n"; \
        __builtin_trap(); \
    }

#define TEST_INC() \
    ++test_index__

#define MAKE_RESULT_NAME(test_index, arg1, arg2) \
    #test_index#arg1#arg2

#define TEST_EQ(eq_arg1__, eq_arg2__, mod_kill__, skip__) \
    TEST_INC(); \
    if (!Test::match_eq(eq_arg1__, eq_arg2__, skip__)) { \
        test_pass_check__ = false; \
        TEST_PRINT_FAIL_INFO(eq_arg1__, eq_arg2__, !=); \
        KILL_TEST(mod_kill__) \
    } 

#define TEST_BYTE_EQ(eq_arg1__, eq_arg2__, mod_kill__, skip__) \
    TEST_INC(); \
    if (!Test::byte_match_eq(eq_arg1__, eq_arg2__, skip__)) { \
        test_pass_check__ = false; \
        TEST_PRINT_FAIL(eq_arg1__, eq_arg2__, !=); \
        KILL_TEST(mod_kill__) \
    } 
