global Str8 day16_example = str8_literal(
    "###############\n"
    "#.......#....E#\n"
    "#.#.###.#.###.#\n"
    "#.....#.#...#.#\n"
    "#.###.#####.#.#\n"
    "#.#.#.......#.#\n"
    "#.#.#####.###.#\n"
    "#...........#.#\n"
    "###.#.#####.#.#\n"
    "#...#.....#.#.#\n"
    "#.#.#.###.#.#.#\n"
    "#.....#...#.#.#\n"
    "#.###.#.#.#.#.#\n"
    "#S..#.....#...#\n"
    "###############\n"

    //"#################\n"
    //"#...#...#...#..E#\n"
    //"#.#.#.#.#.#.#.#.#\n"
    //"#.#.#.#...#...#.#\n"
    //"#.#.#.#.###.#.#.#\n"
    //"#...#.#.#.....#.#\n"
    //"#.#.#.#.#.#####.#\n"
    //"#.#...#.#.#.....#\n"
    //"#.#.#####.#.###.#\n"
    //"#.#.#.......#...#\n"
    //"#.#.###.#####.###\n"
    //"#.#.#...#.....#.#\n"
    //"#.#.#.#####.###.#\n"
    //"#.#.#.........#.#\n"
    //"#.#.#.#########.#\n"
    //"#S#.............#\n"
    //"#################\n"
);

typedef struct Day16Entry Day16Entry;

typedef struct Day16List Day16List;
struct Day16List {
    Day16Entry *first;
    Day16Entry *last;
};

struct Day16Entry {
    Day16List path;
    Day16Entry *next;
    V2S64 position;
    U64 direction;
    U64 cost_to_here;
};

internal V2S64 day16_direction_to_delta[] = {
    {{  1,  0, }},
    {{  0,  1, }},
    {{ -1,  0, }},
    {{  0, -1, }},
};

internal Void day16_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day16_example;
    //os_file_read(arena, str8_literal("data/day16.txt"), &data);

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    S64 width = (S64) lines.first->string.size;
    S64 height = (S64) lines.node_count;
    Str8 map = str8_join(arena, &lines);
    V2S64 start = { 0 };
    V2S64 end = { 0 };
    for (S64 i = 0; i < (S64) map.size; ++i) {
        V2S64 location = v2s64(i % width, i / width);
        if (map.data[i] == 'S') {
            start = location;
        } else if (map.data[i] == 'E') {
            end = location;
        }
    }

    Day16Entry **visited = arena_push_array_zero(arena, Day16Entry *, (U64) (width * height));
    Day16Entry **heap = arena_push_array_zero(arena, Day16Entry *, (U64) (width * height));
    U64 heap_size = 0;

    Day16Entry *initial = arena_push_struct_zero(arena, Day16Entry);
    initial->position = start;

    heap[heap_size] = initial;
    ++heap_size;

    U64 best_cost = U64_MAX;

    while (heap_size) {
        Day16Entry *entry = heap[0];
        // NOTE(simon): Shift down
        {
            U64 new_index = 0;
            heap[new_index] = heap[--heap_size];
            while (new_index < heap_size) {
                U64 left_child = new_index * 2 + 1;
                U64 right_child = new_index * 2 + 2;

                if (right_child < heap_size) {
                    U64 smaller = left_child;
                    U64 larger = right_child;

                    if (heap[larger]->cost_to_here, heap[smaller]->cost_to_here) {
                        swap(smaller, larger, U64);
                    }

                    if (heap[smaller]->cost_to_here < heap[new_index]->cost_to_here) {
                        swap(heap[smaller], heap[new_index], Day16Entry *);
                        new_index = smaller;
                    } else {
                        break;
                    }
                } else if (left_child < heap_size && heap[left_child]->cost_to_here < heap[new_index]->cost_to_here) {
                    swap(heap[left_child], heap[new_index], Day16Entry *);
                    new_index = left_child;
                    break;
                } else {
                    break;
                }
            }
        }

        if (entry->cost_to_here > best_cost) {
            break;
        }

        S64 index = entry->position.x + entry->position.y * width;

        for (S64 y = 0; y < height; ++y) {
            for (S64 x = 0; x < width; ++x) {
                if (x == entry->position.x && y == entry->position.y) {
                    os_console_print(str8_literal("X"));
                } else if (visited[x + y * width]) {
                    os_console_print(str8_literal("O"));
                } else {
                    os_console_print(str8_substring(map, (U64) (x + y * width), 1));
                }
            }
            os_console_print(str8_literal("\n"));
        }

        if (visited[index]) {
            continue;
        }
        visited[index] = entry;

        if (entry->position.x == end.x && entry->position.y == end.y) {
            // NOTE(simon): Shortest path!
            best_cost = entry->cost_to_here;
        }

        for (U64 i = 0; i < 3; ++i) {
            Day16Entry *candidate = arena_push_struct_zero(arena, Day16Entry);
            sll_queue_push(candidate->path.first, candidate->path.last, entry);
            candidate->direction = (entry->direction + i - 1) % 4;
            candidate->position = v2s64_add(entry->position, day16_direction_to_delta[candidate->direction]);
            candidate->cost_to_here = entry->cost_to_here + 1 + (entry->direction == candidate->direction ? 0 : 1000);

            S64 candidate_index = candidate->position.x + candidate->position.y * width;

            if (map.data[candidate_index] == '#') {
                continue;
            }

            heap[heap_size++] = candidate;
            U64 new_index = heap_size - 1;
            while (new_index) {
                U64 parent_index = (new_index - 1) / 2;
                if (heap[new_index]->cost_to_here < heap[parent_index]->cost_to_here) {
                    swap(heap[new_index], heap[parent_index], Day16Entry *);
                    new_index = parent_index;
                } else {
                    break;
                }
            }
        }
    }

    U64 seats = 0;
    for (U64 i = 0; i < map.size; ++i) {
        if (map.data[i] == 'O') {
            ++seats;
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", best_cost));
    os_console_print(str8_format(arena, "Part 2: %lu\n", seats));

    for (S64 y = 0; y < height; ++y) {
        os_console_print(str8_substring(map, (U64) (y * width), (U64) width));
        os_console_print(str8_literal("\n"));
    }

    arena_destroy(arena);
}
