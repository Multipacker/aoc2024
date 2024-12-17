#include "src/base/base_include.h"

#include "src/base/base_include.c"

#include "src/aoc/day1.c"
#include "src/aoc/day2.c"
#include "src/aoc/day3.c"
#include "src/aoc/day4.c"
#include "src/aoc/day5.c"
#include "src/aoc/day6.c"
#include "src/aoc/day7.c"
#include "src/aoc/day8.c"
#include "src/aoc/day9.c"
#include "src/aoc/day10.c"
#include "src/aoc/day11.c"
#include "src/aoc/day12.c"
#include "src/aoc/day13.c"
#include "src/aoc/day14.c"
#include "src/aoc/day15.c"
#include "src/aoc/day16.c"

internal S32 os_run(Str8List arguments) {
    day1_solve();
    day2_solve();
    day3_solve();
    day4_solve();
    day5_solve();
    day6_solve();
    day7_solve();
    day8_solve();
    day9_solve();
    day10_solve();
    day11_solve();
    day12_solve();
    day13_solve();
    //day14_solve();
    day15_solve();
    day16_solve();
    return 0;
}
