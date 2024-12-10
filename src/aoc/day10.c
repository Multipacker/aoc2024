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

internal V2U64 day10_find_9(U8 *map, R2S64 bounds, V2S64 point, U8 wanted) {
    S64 index = point.x + point.y * r2s64_size(bounds).width;

    if (wanted == '9' && map[index] == 'X') {
        return v2u64(0, 1);
    } else if (map[index] != wanted) {
        return v2u64(0, 0);
    } else if (map[index] == '9') {
        map[index] = 'X';
        return v2u64(1, 1);
    }

    V2S64 candidates[] = {
        v2s64_add(point, v2s64( 1,  0)),
        v2s64_add(point, v2s64( 0,  1)),
        v2s64_add(point, v2s64(-1,  0)),
        v2s64_add(point, v2s64( 0, -1)),
    };

    V2U64 result = v2u64(0, 0);;
    U8 new_wanted = wanted + 1;
    for (U64 i = 0; i < array_count(candidates); ++i) {
        if (r2s64_contains(bounds, candidates[i])) {
            result = v2u64_add(result, day10_find_9(map, bounds, candidates[i], new_wanted));
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

    V2S64List heads = { 0 };
    for (U64 i = 0; i < map.size; ++i) {
        if (map.data[i] == '0') {
            V2S64Node *node = arena_push_struct_zero(arena, V2S64Node);
            node->value.x = (S64) i % width;
            node->value.y = (S64) i / width;
            sll_queue_push(heads.first, heads.last, node);
        }
    }

    U64 score = 0;
    U64 rating = 0;
    for (V2S64Node *head = heads.first; head; head = head->next) {
        V2U64 stats = day10_find_9(map.data, r2s64(0, 0, width, height), head->value, '0');
        score += stats.x;
        rating += stats.y;
        for (U64 i = 0; i < map.size; ++i) {
            if (map.data[i] == 'X') {
                map.data[i] = '9';
            }
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", score));
    os_console_print(str8_format(arena, "Part 2: %lu\n", rating));

    arena_destroy(arena);
}
