global Str8 day2_example = str8_literal(
    "7 6 4 2 1\n"
    "1 2 7 8 9\n"
    "9 7 6 2 1\n"
    "1 3 2 4 5\n"
    "8 6 4 4 1\n"
    "1 3 6 7 9\n"
);

internal Void day2_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day2_example;
    os_file_read(arena, str8_literal("data/day2.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    S64 *reports = arena_push_array(arena, S64, lines.node_count * lines.first->string.size);
    U64 report_count = lines.node_count;
    U64 level_count = 0;

    // NOTE(simon): Parse
    {
        U64 report_index = 0;
        for (Str8Node *node = lines.first; node; node = node->next, ++report_index) {
            level_count = 0;
            Str8 report = node->string;
            while (report.size) {
                U64Parse level = u64_from_str8(report);
                reports[report_index * report_count + level_count] = (S64) level.value;
                ++level_count;
                report = str8_skip_whitespace(str8_skip(report, level.size));
            }
        }
    }

    U64 safe_count = 0;
    for (U64 report_index = 0; report_index < report_count; ++report_index) {
        S64 previous_value = reports[report_index * report_count + 0];
        S64 current_value  = reports[report_index * report_count + 1];
        S64 sign = previous_value < current_value ? 1 : -1;
        B32 safe = true;

        for (U64 level_index = 1; level_index < level_count; ++level_index) {
            current_value  = reports[report_index * report_count + level_index];
            S64 difference = sign * (current_value - previous_value);
            safe &= (1 <= difference && difference <= 3);
            previous_value = current_value;
        }

        if (safe) {
            ++safe_count;
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", safe_count));

    arena_destroy(arena);
}
