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

internal Void day7_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day7_example;
    os_file_read(arena, str8_literal("data/day7.txt"), &data);

    U64ArrayList equations = { 0 };

    for (Str8Node *line = str8_split_by_codepoints(arena, data, str8_literal("\n")).first; line; line = line->next) {
        Arena_Temporary scratch = arena_get_scratch(0, 0);
        Str8 equation_node = line->string;

        U64Parse wanted = u64_from_str8(equation_node);
        Str8List operand_nodes = str8_split_by_codepoints(scratch.arena, str8_skip(equation_node, wanted.size + 2), str8_literal(" "));

        U64Array equation = { 0 };
        equation.data = arena_push_array(arena, U64, 1 + operand_nodes.node_count);
        equation.data[equation.size] = wanted.value;
        ++equation.size;

        for (Str8Node *operand = operand_nodes.first; operand; operand = operand->next) {
            equation.data[equation.size] = u64_from_str8(operand->string).value;
            ++equation.size;
        }

        U64ArrayNode *node = arena_push_struct_zero(arena, U64ArrayNode);
        node->value = equation;
        sll_queue_push(equations.first, equations.last, node);

        arena_end_temporary(scratch);
    }

    U64 sum0 = 0;
    U64 sum1 = 0;
    for (U64ArrayNode *equation_node = equations.first; equation_node; equation_node = equation_node->next) {
        U64Array equation = equation_node->value;

        U64 correct0 = 0;
        U64 correct1 = 0;
        for (U64 i = 0; i < 1 << 2 * (equation.size - 2) && (correct0 == 0 || correct1 == 0); ++i) {
            U64 result0 = equation.data[1];
            U64 result1 = equation.data[1];

            for (U64 j = 2; j < equation.size; ++j) {
                if (((i >> 2 * (j - 2)) & 0x03) == 0) {
                    result0 += equation.data[j];
                    result1 += equation.data[j];
                } else if (((i >> 2 * (j - 2)) & 0x03) == 1) {
                    result0 *= equation.data[j];
                    result1 *= equation.data[j];
                } else {
                    for (U64 k = equation.data[j]; k; k /= 10) {
                        result1 *= 10;
                    }
                    result1 += equation.data[j];
                }
            }

            if (result0 == equation.data[0]) {
                correct0 = result0;
            }

            if (result1 == equation.data[0]) {
                correct1 = result1;
            }
        }

        sum0 += correct0;
        sum1 += correct1;
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", sum0));
    os_console_print(str8_format(arena, "Part 2: %lu\n", sum1));

    arena_destroy(arena);
}
