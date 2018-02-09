#include "unitez/unitez.h"

struct test0 {
    constexpr static const auto name = "test-0";
    constexpr static const auto module = "mod0";
    constexpr static const auto flags = unitez::flags::singlethread;
    
    static bool check() {
        int* x = nullptr;
        *x = 5;
        return 1 != 1;
    }
};

struct test1 {
    constexpr static const auto name = "test-1";
    constexpr static const auto module = "mod0";
    constexpr static const auto flags = unitez::flags::no_options;
    
    static bool check() {
        while (true) {}
        return 1 == 1;
    }
};

struct test2 {
    constexpr static const auto name = "test-2";
    constexpr static const auto module = "mod1";
    constexpr static const auto flags = unitez::flags::no_options;
    
    static bool check() {
        return 1 == 1;
    }
};
