global Str8 day10_example = str8_literal(
    "89010123\n"
    "78121874\n"
    "87430965\n"
    "96549874\n"
    "45678903\n"
    "32019012\n"
    "01329801\n"
    "10456732\n"
);

internal V2U64 day10_find_9(B8 *visited, U8 *map, R2S64 bounds, V2S64 point, U8 wanted) {
    V2U64 result = v2u64(0, 0);

    S64 index = point.x + point.y * r2s64_size(bounds).width;
    if (map[index] == wanted) {
        if (map[index] == '9') {
            result = v2u64(!visited[index], 1);
            visited[index] = wanted == '9';
        } else {
            V2S64 candidates[] = {
                v2s64_add(point, v2s64( 1,  0)),
                v2s64_add(point, v2s64( 0,  1)),
                v2s64_add(point, v2s64(-1,  0)),
                v2s64_add(point, v2s64( 0, -1)),
            };

            for (U64 i = 0; i < array_count(candidates); ++i) {
                if (r2s64_contains(bounds, candidates[i])) {
                    result = v2u64_add(result, day10_find_9(visited, map, bounds, candidates[i], wanted + 1));
                }
            }
        }
    }

    return result;
}

internal Void day10_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day10_example;
    os_file_read(arena, str8_literal("data/day10.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    S64 width  = (S64) lines.first->string.size;
    S64 height = (S64) lines.node_count;
    Str8 map = str8_join(arena, &lines);

    V2U64 accumulator = v2u64(0, 0);
    for (U64 i = 0; i < map.size; ++i) {
        if (map.data[i] == '0') {
            Arena_Temporary scratch = arena_get_scratch(0, 0);
            B8 *visited = arena_push_array_zero(arena, B8, map.size);
            V2U64 stats = day10_find_9(visited, map.data, r2s64(0, 0, width, height), v2s64((S64) i % width, (S64) i / width), '0');
            accumulator = v2u64_add(accumulator, stats);
            arena_end_temporary(scratch);
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", accumulator.x));
    os_console_print(str8_format(arena, "Part 2: %lu\n", accumulator.y));

    arena_destroy(arena);
}
