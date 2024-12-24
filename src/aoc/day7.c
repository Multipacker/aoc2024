global Str8 day7_example = str8_literal(
    "190: 10 19\n"
    "3267: 81 40 27\n"
    "83: 17 5\n"
    "156: 15 6\n"
    "7290: 6 8 6 15\n"
    "161011: 16 10 13\n"
    "192: 17 8 14\n"
    "21037: 9 7 18 13\n"
    "292: 11 6 16 20\n"
);

internal U64 day7_iterate0(U64 accumulator, U64Array equation, U64 index) {
    if (index == equation.size) {
        return accumulator == equation.data[0];
    }

    return day7_iterate0(accumulator + equation.data[index], equation, index + 1) ||
        day7_iterate0(accumulator * equation.data[index], equation, index + 1);
}

internal U64 day7_iterate1(U64 accumulator, U64Array equation, U64 index) {
    if (index == equation.size) {
        return accumulator == equation.data[0];
    } else if (accumulator > equation.data[0]) {
        return false;
    }

    U64 factor = 1;
    while (factor <= equation.data[index]) {
        factor *= 10;
    }

    return day7_iterate1(accumulator + equation.data[index], equation, index + 1) ||
        day7_iterate1(accumulator * equation.data[index], equation, index + 1) ||
        day7_iterate1(accumulator * factor + equation.data[index], equation, index + 1);
}

internal Void day7_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day7_example;
    os_file_read(arena, str8_literal("data/day7.txt"), &data);

    U64 sum0 = 0;
    U64 sum1 = 0;
    for (Str8Node *line = str8_split_by_codepoints(arena, data, str8_literal("\n")).first; line; line = line->next) {
        Str8List operand_nodes = str8_split_by_codepoints(arena, line->string, str8_literal(" "));

        U64Array equation = { 0 };
        equation.data = arena_push_array(arena, U64, operand_nodes.node_count);

        for (Str8Node *operand = operand_nodes.first; operand; operand = operand->next) {
            equation.data[equation.size] = u64_from_str8(operand->string).value;
            ++equation.size;
        }

        if (day7_iterate0(equation.data[1], equation, 2)) {
            sum0 += equation.data[0];
        }

        if (day7_iterate1(equation.data[1], equation, 2)) {
            sum1 += equation.data[0];
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", sum0));
    os_console_print(str8_format(arena, "Part 2: %lu\n", sum1));

    arena_destroy(arena);
}
