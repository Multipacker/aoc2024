global Str8 day6_example = str8_literal(
    "....#.....\n"
    ".........#\n"
    "..........\n"
    "..#.......\n"
    ".......#..\n"
    "..........\n"
    ".#..^.....\n"
    "........#.\n"
    "#.........\n"
    "......#...\n"
);

internal V2S64 day6_update_position(V2S64 position, U8 direction) {
    V2S64 result = position;
    result.x += (direction & 0x01) != 0;
    result.y += (direction & 0x02) != 0;
    result.x -= (direction & 0x04) != 0;
    result.y -= (direction & 0x08) != 0;
    return result;
}

internal U8 day6_rotate_right(U8 direction) {
    U8 result = (U8) ((direction & 0x07) << 1 | (direction & 0x08) >> 3);
    return result;
}

internal Void day6_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day6_example;
    os_file_read(arena, str8_literal("data/day6.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));

    U64 width = lines.first->string.size;
    U64 height = lines.node_count;

    V2S64 guard_position = { 0 };
    U8 guard_direction = 0x08;

    Str8 map = str8_join(arena, &lines);
    for (U64 i = 0; i < map.size; ++i) {
        if (map.data[i] == '^') {
            guard_position = v2s64((S64) (i % width), (S64) (i / width));
        }
    }

    R2S64 bounds = r2s64(0, 0, (S64) width, (S64) height);

    U8 *directions = arena_push_array_zero(arena, U8, width * height);
    for (; r2s64_contains(bounds, guard_position); guard_position = day6_update_position(guard_position, guard_direction)) {
        rotate:
        directions[guard_position.x + guard_position.y * (S64) width] |= guard_direction;
        V2S64 new_position = day6_update_position(guard_position, guard_direction);
        if (r2s64_contains(bounds, new_position) && map.data[new_position.x + new_position.y * (S64) width] == '#') {
            guard_direction = day6_rotate_right(guard_direction);
            goto rotate;
        }

        if (r2s64_contains(bounds, new_position) && directions[new_position.x + new_position.y * (S64) width] == 0) {
            Arena_Temporary scratch = arena_get_scratch(0, 0);
            map.data[new_position.x + new_position.y * (S64) width] = '#';
            U8 *search_directions = arena_push_array(scratch.arena, U8, width * height);
            memory_copy(search_directions, directions, width * height);

            V2S64 search_guard_position = guard_position;
            U8 search_guard_direction = day6_rotate_right(guard_direction);
            for (; r2s64_contains(bounds, search_guard_position); search_guard_position = day6_update_position(search_guard_position, search_guard_direction)) {
                if (search_directions[search_guard_position.x + search_guard_position.y * (S64) width] & search_guard_direction) {
                    directions[new_position.x + new_position.y * (S64) width] |= 0x10;
                    break;
                }

                search_rotate:
                search_directions[search_guard_position.x + search_guard_position.y * (S64) width] |= search_guard_direction;
                V2S64 search_new_position = day6_update_position(search_guard_position, search_guard_direction);
                if (r2s64_contains(bounds, search_new_position) && map.data[search_new_position.x + search_new_position.y * (S64) width] == '#') {
                    search_guard_direction = day6_rotate_right(search_guard_direction);
                    goto search_rotate;
                }
            }

            map.data[new_position.x + new_position.y * (S64) width] = '.';
            arena_end_temporary(scratch);
        }
    }

    U64 visited_count = 0;
    U64 obstacle_count = 0;
    for (U64 i = 0; i < width * height; ++i) {
        visited_count  += (directions[i] & 0x0F) != 0;
        obstacle_count += (directions[i] & 0x10) != 0;
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", visited_count));
    os_console_print(str8_format(arena, "Part 2: %lu\n", obstacle_count));

    arena_destroy(arena);
}
