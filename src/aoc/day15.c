global Str8 day15_example = str8_literal(
    "##########\n"
    "#..O..O.O#\n"
    "#......O.#\n"
    "#.OO..O.O#\n"
    "#..O@..O.#\n"
    "#O#..O...#\n"
    "#O..O..O.#\n"
    "#.OO.O.OO#\n"
    "#....O...#\n"
    "##########\n"
    "\n"
    "<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^\n"
    "vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v\n"
    "><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<\n"
    "<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^\n"
    "^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><\n"
    "^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^\n"
    ">^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^\n"
    "<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>\n"
    "^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>\n"
    "v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^\n"
);

internal B32 day15_check(Str8 map, S64 width, S64 height, V2S64 position, V2S64 delta) {
    U8 tile = map.data[position.x + position.y * width];
    B32 result = true;
    if (tile == '.') {
        result = true;
    } else if (tile == '#') {
        result = false;
    } else {
        result = day15_check(map, width, height, v2s64_add(position, delta), delta);
        if (delta.y) {
            if (tile == '[') {
                result &= day15_check(map, width, height, v2s64_add(position, v2s64(1, delta.y)), delta);
            } else if (tile == ']') {
                result &= day15_check(map, width, height, v2s64_add(position, v2s64(-1, delta.y)), delta);
            }
        }
    }

    return result;
}

internal Void day15_move(Str8 map, S64 width, S64 height, V2S64 position, V2S64 delta, U8 new) {
    U8 tile = map.data[position.x + position.y * width];
    map.data[position.x + position.y * width] = new;
    if (tile == '.') {
        return;
    }

    day15_move(map, width, height, v2s64_add(position, delta), delta, tile);
    if (delta.y) {
        if (tile == '[') {
            day15_move(map, width, height, v2s64_add(position, v2s64(1, delta.y)), delta, ']');
            map.data[(position.x + 1) + position.y * width] = '.';
        } else if (tile == ']') {
            day15_move(map, width, height, v2s64_add(position, v2s64(-1, delta.y)), delta, '[');
            map.data[(position.x - 1) + position.y * width] = '.';
        }
    }
}

internal Void day15_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day15_example;
    os_file_read(arena, str8_literal("data/day15.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));

    Str8List map_lines = { 0 };
    Str8List instruction_lines = { 0 };
    for (Str8Node *line = lines.first; line; line = line->next) {
        if (line->string.size == 0) {
            for (Str8Node *instructions = line->next; instructions; instructions = instructions->next) {
                str8_list_push(arena, &instruction_lines, instructions->string);
            }
            break;
        } else {
            str8_list_push(arena, &map_lines, line->string);
        }
    }

    S64 width0 = (S64) map_lines.first->string.size;
    S64 height0 = (S64) map_lines.node_count;
    Str8 map0 = str8_join(arena, &map_lines);

    U64 robot_index = 0;
    str8_first_index_of(map0, '@', &robot_index);
    V2S64 position0 = v2s64((S64) robot_index % width0, (S64) robot_index / width0);

    Str8 map1 = { 0 };
    S64 width1 = 2 * width0;
    S64 height1 = height0;
    map1.data = arena_push_array_zero(arena, U8, 2 * map0.size);
    for (U64 i = 0; i < map0.size; ++i) {
        switch (map0.data[i]) {
            case '#': {
                map1.data[map1.size++] = '#';
                map1.data[map1.size++] = '#';
            } break;
            case 'O': {
                map1.data[map1.size++] = '[';
                map1.data[map1.size++] = ']';
            } break;
            case '.': {
                map1.data[map1.size++] = '.';
                map1.data[map1.size++] = '.';
            } break;
            case '@': {
                map1.data[map1.size++] = '@';
                map1.data[map1.size++] = '.';
            } break;
        }
    }
    V2S64 position1 = v2s64(2 * position0.x, position0.y);

    Str8 instructions = str8_join(arena, &instruction_lines);

    for (U64 i = 0; i < instructions.size; ++i) {
        V2S64 delta = { 0 };
        switch (instructions.data[i]) {
            case '^': delta = v2s64( 0, -1); break;
            case 'v': delta = v2s64( 0,  1); break;
            case '<': delta = v2s64(-1,  0); break;
            case '>': delta = v2s64( 1,  0); break;
        }

        if (day15_check(map0, width0, height0, v2s64_add(position0, delta), delta)) {
            day15_move(map0, width0, height0, v2s64_add(position0, delta), delta, '@');
            map0.data[position0.x + position0.y * width0] = '.';
            position0 = v2s64_add(position0, delta);
        }

        if (day15_check(map1, width1, height1, v2s64_add(position1, delta), delta)) {
            day15_move(map1, width1, height1, v2s64_add(position1, delta), delta, '@');
            map1.data[position1.x + position1.y * width1] = '.';
            position1 = v2s64_add(position1, delta);
        }
    }

    U64 sum0 = 0;
    for (U64 i = 0; i < map0.size; ++i) {
        if (map0.data[i] == 'O') {
            U64 x = i % (U64) width0;
            U64 y = i / (U64) width0;
            U64 gps = x + y * 100;
            sum0 += gps;
        }
    }
    U64 sum1 = 0;
    for (U64 i = 0; i < map1.size; ++i) {
        if (map1.data[i] == '[') {
            U64 x = i % (U64) width1;
            U64 y = i / (U64) width1;
            U64 gps = x + y * 100;
            sum1 += gps;
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", sum0));
    os_console_print(str8_format(arena, "Part 2: %lu\n", sum1));

    arena_destroy(arena);
}
