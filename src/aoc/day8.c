global Str8 day8_example = str8_literal(
    "......#....#\n"
    "...#....0...\n"
    "....#0....#.\n"
    "..#....0....\n"
    "....0....#..\n"
    ".#....A.....\n"
    "...#........\n"
    "#......#....\n"
    "........A...\n"
    ".........A..\n"
    "..........#.\n"
    "..........#.\n"
);

typedef struct V2S64Node V2S64Node;
struct V2S64Node {
    V2S64Node *next;
    V2S64 value;
};

typedef struct V2S64List V2S64List;
struct V2S64List {
    V2S64Node *first;
    V2S64Node *last;
};

internal Void day8_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day8_example;
    os_file_read(arena, str8_literal("data/day8.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    S64 width = (S64) lines.first->string.size;
    S64 height = (S64) lines.node_count;
    Str8 map = str8_join(arena, &lines);

    U64 frequency_count = ('z' - 'a' + 1) + ('Z' - 'A' + 1) + ('9' - '0' + 1);
    V2S64List *frequencies = arena_push_array_zero(arena, V2S64List, frequency_count);
    for (U64 i = 0; i < map.size; ++i) {
        U8 cell = map.data[i];
        V2S64Node *antenna = arena_push_struct_zero(arena, V2S64Node);
        antenna->value.x = (S64) i % width; 
        antenna->value.y = (S64) i / width; 

        V2S64List *frequency = 0;
        if ('a' <= cell && cell <= 'z') {
            frequency = &frequencies[cell - 'a'];
        } else if ('A' <= cell && cell <= 'Z') {
            frequency = &frequencies[('z' - 'a' + 1) + (cell - 'A')];
        } else if ('0' <= cell && cell <= '9') {
            frequency = &frequencies[('z' - 'a' + 1) + ('Z' - 'A' + 1) + (cell - '0')];
        }

        if (frequency) {
            sll_queue_push(frequency->first, frequency->last, antenna);
        }
    }

    R2S64 bounds = r2s64(0, 0, width, height);
    B8 *antinodes = arena_push_array_zero(arena, B8, map.size);
    B8 *multi_antinodes = arena_push_array_zero(arena, B8, map.size);
    for (U64 i = 0; i < frequency_count; ++i) {
        for (V2S64Node *a = frequencies[i].first; a; a = a->next) {
            for (V2S64Node *b = a->next; b; b = b->next) {
                V2S64 dist = v2s64_subtract(b->value, a->value);

                V2S64 node0 = v2s64_subtract(a->value, dist);
                V2S64 node1 = v2s64_add(b->value, dist);

                if (r2s64_contains(bounds, node0)) {
                    antinodes[node0.x + node0.y * width] = true;
                }

                if (r2s64_contains(bounds, node1)) {
                    antinodes[node1.x + node1.y * width] = true;
                }

                V2S64 position = a->value;
                while (r2s64_contains(bounds, v2s64_subtract(position, dist))) {
                    position = v2s64_subtract(position, dist);
                }

                while (r2s64_contains(bounds, position)) {
                    multi_antinodes[position.x + position.y * width] = true;
                    position = v2s64_add(position, dist);
                }
            }
        }
    }

    U64 antinode_count = 0;
    U64 multi_antinode_count = 0;
    for (U64 i = 0; i < map.size; ++i) {
        antinode_count += antinodes[i];
        multi_antinode_count += multi_antinodes[i];
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", antinode_count));
    os_console_print(str8_format(arena, "Part 2: %lu\n", multi_antinode_count));

    arena_destroy(arena);
}
