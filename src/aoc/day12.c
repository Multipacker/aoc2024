global Str8 day12_example = str8_literal(
    //"AAAA\n"
    //"BBCD\n"
    //"BBCC\n"
    //"EEEC\n"

    //"EEEEE\n"
    //"EXXXX\n"
    //"EEEEE\n"
    //"EXXXX\n"
    //"EEEEE\n"

    "AAAAAA\n"
    "AAABBA\n"
    "AAABBA\n"
    "ABBAAA\n"
    "ABBAAA\n"
    "AAAAAA\n"

    //"RRRRIICCFF\n"
    //"RRRRIICCCF\n"
    //"VVRRRCCFFF\n"
    //"VVRCCCJFFF\n"
    //"VVVVCJJCFE\n"
    //"VVIVCCJJEE\n"
    //"VVIIICJJEE\n"
    //"MIIIIIJJEE\n"
    //"MIIISIJEEE\n"
    //"MMMISSJEEE\n"
);

internal V2S64 day12_left(V2S64 direction) {
    V2S64 result = { 0 };
    result.x =  direction.y;
    result.y = -direction.x;
    return result;
}

internal V2S64 day12_right(V2S64 direction) {
    V2S64 result = { 0 };
    result.x = -direction.y;
    result.y =  direction.x;
    return result;
}

internal B32 day12_contains(U8 *map, R2S64 bounds, V2S64 position, U8 wanted) {
    S64 index = position.x + position.y * r2s64_size(bounds).width;
    return r2s64_contains(bounds, position) && map[index] == wanted;
}

internal V2U64 day12_fence0(U8 *map, B8 *visited, R2S64 bounds, V2S64 position, U8 wanted) {
    S64 index = position.x + position.y * r2s64_size(bounds).width;
    if (!r2s64_contains(bounds, position) || map[index] != wanted) {
        return v2u64(0, 1);
    }

    if (visited[index]) {
        return v2u64(0, 0);
    }

    visited[index] = true;

    V2U64 result = v2u64(1, 0);
    V2S64 offsets[] = {
        v2s64( 1,  0),
        v2s64( 0,  1),
        v2s64(-1,  0),
        v2s64( 0, -1),
    };
    for (U64 i = 0; i < array_count(offsets); ++i) {
        result = v2u64_add(result, day12_fence0(map, visited, bounds, v2s64_add(position, offsets[i]), wanted));
    }

    return result;
}

internal U64 day12_fence1(U8 *map, R2S64 bounds, V2S64 position, U8 wanted) {
    // NOTE(simon): Find edge
    while (day12_contains(map, bounds, v2s64_add(position, v2s64(0, -1)), wanted)) {
        position = v2s64_add(position, v2s64(0, -1));
    }

    // NOTE(simon): Find corner
    while (day12_contains(map, bounds, v2s64_add(position, v2s64(-1, 0)), wanted)) {
        position = v2s64_add(position, v2s64(-1, 0));
    }

    U64 edge_count = 0;
    V2S64 direction = v2s64(1, 0);
    V2S64 start_position = position;
    do {
        for (;;) {
            V2S64 new_position = v2s64_add(position, direction);
            V2S64 adjacent = v2s64_add(new_position, day12_left(direction));

            if (day12_contains(map, bounds, new_position, wanted) && !day12_contains(map, bounds, adjacent, wanted)) {
                position = new_position;
            } else {
                if (day12_contains(map, bounds, adjacent, wanted)) {
                    direction = day12_left(direction);
                    position = new_position;
                } else {
                    direction = day12_right(direction);
                }
                break;
            }
        }

        ++edge_count;
    } while (position.x != start_position.x || position.y != start_position.y || direction.x != 1 || direction.y != 0);

    return edge_count;
}

internal Void day12_solve(Void) {
    Arena *arena = arena_create_reserve(gigabytes(4));

    Str8 data = day12_example;
    //os_file_read(arena, str8_literal("data/day12.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    S64 width = (S64) lines.first->string.size;
    S64 height = (S64) lines.node_count;
    Str8 map = str8_join(arena, &lines);
    B8 *visited = arena_push_array_zero(arena, B8, map.size);

    U64 sum0 = 0;
    U64 sum1 = 0;
    for (U64 i = 0; i < map.size; ++i) {
        if (visited[i]) {
            continue;
        }

        V2S64 position = v2s64((S64) i % width, (S64) i / width);
        V2U64 fence = day12_fence0(map.data, visited, r2s64(0, 0, width, height), position, map.data[i]);
        U64 faces = day12_fence1(map.data, r2s64(0, 0, width, height), position, map.data[i]);
        sum0 += fence.x * fence.y;
        sum1 += fence.x * faces;
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", sum0));
    os_console_print(str8_format(arena, "Part 2: %lu\n", sum1));

    arena_destroy(arena);
}
