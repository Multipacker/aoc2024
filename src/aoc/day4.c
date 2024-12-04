global Str8 day4_example = str8_literal(
    "MMMSXXMASM\n"
    "MSAMXMSMSA\n"
    "AMXSXMAAMM\n"
    "MSAMASMSMX\n"
    "XMASAMXAMM\n"
    "XXAMMXXAMA\n"
    "SMSMSASXSS\n"
    "SAXAMASAAA\n"
    "MAMMMXMMMM\n"
    "MXMXAXMASX\n"
);

internal U64 day4_count(Str8List lines) {
    U64 count = 0;
    for (Str8Node *node = lines.first; node; node = node->next) {
        U8 *ptr = node->string.data;
        U8 *opl = ptr + node->string.size;
        Str8 xmas = str8_literal("XMAS");
        Str8 samx = str8_literal("SAMX");

        while ((U64) (opl - ptr) >= xmas.size) {
            if (memory_equal(ptr, xmas.data, xmas.size)) {
                ++count;
            } else if (memory_equal(ptr, samx.data, samx.size)) {
                ++count;
            }

            ++ptr;
        }
    }

    return count;
}

internal Str8List day4_diagonalize(Arena *arena, Str8List rows) {
    Str8List diagonal = { 0 };

    U64 width = rows.first->string.size;
    U64 height = rows.node_count;

    S64 start = 0;
    for (U64 i = 0; i < width; ++i) {
        Str8 string = { 0 };
        string.data = arena_push_array(arena, U8, u64_min(width, height));

        Str8Node *node = rows.first;
        for (S64 j = start; j >= 0; --j, node = node->next) {
            string.data[string.size] = node->string.data[j];
            ++string.size;
        }

        str8_list_push(arena, &diagonal, string);
        ++start;
    }

    for (Str8Node *node = rows.first->next; node; node = node->next) {
        Str8 string = { 0 };
        string.data = arena_push_array(arena, U8, u64_min(width, height));

        U64 i = width - 1;
        for (Str8Node *iter = node; iter; iter = iter->next, --i) {
            string.data[string.size] = iter->string.data[i];
            ++string.size;
        }
        str8_list_push(arena, &diagonal, string);
    }

    return diagonal;
}

internal Void day4_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day4_example;
    os_file_read(arena, str8_literal("data/day4.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    U64 width = lines.first->string.size;
    U64 height = lines.node_count;

    Str8List rows = lines;
    Str8List columns = { 0 };
    for (U64 i = 0; i < width; ++i) {
        Str8 string = { 0 };
        string.data = arena_push_array(arena, U8, height);
        for (Str8Node *node = rows.first; node; node = node->next) {
            string.data[string.size] = node->string.data[i];
            ++string.size;
        }
        str8_list_push(arena, &columns, string);
    };
    Str8List diagonal0 = day4_diagonalize(arena, rows);
    Str8List rev_rows = { 0 };
    for (Str8Node *node = rows.last; node; node = node->previous) {
        str8_list_push(arena, &rev_rows, node->string);
    }
    Str8List diagonal1 = day4_diagonalize(arena, rev_rows);

    U64 count_xmas = 0;
    count_xmas += day4_count(lines);
    count_xmas += day4_count(columns);
    count_xmas += day4_count(diagonal0);
    count_xmas += day4_count(diagonal1);

    os_console_print(str8_format(arena, "Part 1: %lu\n", count_xmas));

    Str8 grid = str8_join(arena, &lines);
    U64 *a_positions = arena_push_array(arena, U64, grid.size);
    U64 a_count = 0;

    for (U64 i = 0; i < grid.size; ++i) {
        if (grid.data[i] == 'A') {
            a_positions[a_count] = i;
            ++a_count;
        }
    }

    U64 count_mas = 0;
    for (U64 i = 0; i < a_count; ++i) {
        U64 row    = a_positions[i] / width;
        U64 column = a_positions[i] % width;

        if (column == 0 || column == width - 1 || row == 0 || row == height - 1) {
            continue;
        }

        if (
            (
                (grid.data[a_positions[i] - width - 1] == 'M' && grid.data[a_positions[i] + width + 1] == 'S') ||
                (grid.data[a_positions[i] - width - 1] == 'S' && grid.data[a_positions[i] + width + 1] == 'M')
            ) && (
                (grid.data[a_positions[i] - width + 1] == 'M' && grid.data[a_positions[i] + width - 1] == 'S') ||
                (grid.data[a_positions[i] - width + 1] == 'S' && grid.data[a_positions[i] + width - 1] == 'M')
            )
        ) {
            ++count_mas;
        }
    }

    os_console_print(str8_format(arena, "Part 2: %lu\n", count_mas));

    arena_destroy(arena);
}
