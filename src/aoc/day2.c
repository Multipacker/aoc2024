global Str8 day2_example = str8_literal(
    "7 6 4 2 1\n"
    "1 2 7 8 9\n"
    "9 7 6 2 1\n"
    "1 3 2 4 5\n"
    "8 6 4 4 1\n"
    "1 3 6 7 9\n"
);

typedef struct S64Array S64Array;
struct S64Array {
    S64 *data;
    U64 size;
};

internal Void day2_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day2_example;
    os_file_read(arena, str8_literal("data/day2.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    S64Array *reports = arena_push_array(arena, S64Array, lines.node_count);
    U64 report_count = 0;

    // NOTE(simon): Parse
    for (Str8Node *node = lines.first; node; node = node->next, ++report_count) {
        Arena_Temporary scratch = arena_get_scratch(0, 0);
        Str8List level_nodes = str8_split_by_codepoints(scratch.arena, node->string, str8_literal(" "));

        S64Array levels = { 0 };
        levels.data = arena_push_array(arena, S64, level_nodes.node_count);
        for (Str8Node *level_node = level_nodes.first; level_node; level_node = level_node->next, ++levels.size) {
            levels.data[levels.size] = (S64) u64_from_str8(level_node->string).value;
        }
        reports[report_count] = levels;

        arena_end_temporary(scratch);
    }

    U64 safe_count = 0;
    U64 dampened_safe_count = 0;
    for (U64 report_index = 0; report_index < report_count; ++report_index) {
        S64Array levels = reports[report_index];

        S64 sign = levels.data[0] < levels.data[1] ? 1 : -1;

        U64 unsafe_count = 0;
        for (U64 level_index = 1; level_index < levels.size; ++level_index) {
            S64 previous = levels.data[level_index - 1];
            S64 current = levels.data[level_index];
            S64 difference = sign * (current - previous);
            if (!(1 <= difference && difference <= 3)) {
                ++unsafe_count;
            }
        }

        if (unsafe_count == 0) {
            ++safe_count;
        }

        if (unsafe_count <= 1) {
            ++dampened_safe_count;
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", safe_count));
    os_console_print(str8_format(arena, "Part 2: %lu\n", dampened_safe_count));

    arena_destroy(arena);
}
