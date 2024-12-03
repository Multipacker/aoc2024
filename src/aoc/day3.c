global Str8 day3_example = str8_literal(
    "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"
);

internal Void day3_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day3_example;
    os_file_read(arena, str8_literal("data/day3.txt"), &data);

    U64 sum = 0;
    U64 control_sum = 0;
    B32 enabled = true;
    while (data.size) {
        Str8 mul_str = str8_literal("mul(");
        Str8 do_str = str8_literal("do()");
        Str8 dont_str = str8_literal("don't()");
        if (data.size >= mul_str.size && memory_equal(data.data, mul_str.data, mul_str.size)) {
            data = str8_skip(data, mul_str.size);

            B32 valid = true;

            U64Parse left = u64_from_str8(data);
            data = str8_skip(data, left.size);
            valid &= (left.size != 0);

            valid &= (data.size >= 1 && data.data[0] == ',');
            data = str8_skip(data, valid);

            U64Parse right = u64_from_str8(data);
            data = str8_skip(data, right.size);
            valid &= (right.size != 0);

            valid &= (data.size >= 1 && data.data[0] == ')');
            data = str8_skip(data, valid);

            if (valid) {
                sum += left.value * right.value;

                if (enabled) {
                    control_sum += left.value * right.value;
                }
            }
        } else if (data.size >= do_str.size && memory_equal(data.data, do_str.data, do_str.size)) {
            data = str8_skip(data, do_str.size);
            enabled = true;
        } else if (data.size >= dont_str.size && memory_equal(data.data, dont_str.data, dont_str.size)) {
            data = str8_skip(data, dont_str.size);
            enabled = false;
        } else {
            data = str8_skip(data, 1);
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", sum));
    os_console_print(str8_format(arena, "Part 2: %lu\n", control_sum));

    arena_destroy(arena);
}
