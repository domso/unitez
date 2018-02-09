#include "unitez/unitez.h"
#include "demo.h"

void unit_test_list(unitez& tester) {
    tester.register_unit_test<test0>();
    tester.register_unit_test<test1>();
    tester.register_unit_test<test2>();
}

