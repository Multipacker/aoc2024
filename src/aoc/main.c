#include "src/base/base_include.h"

#include "src/base/base_include.c"
#include "src/aoc/day1.c"
#include "src/aoc/day2.c"

internal S32 os_run(Str8List arguments) {
    day1_solve();
    day2_solve();
    return 0;
}