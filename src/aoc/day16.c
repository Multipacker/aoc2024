global Str8 day16_example = str8_literal(
    //"###############\n"
    //"#.......#....E#\n"
    //"#.#.###.#.###.#\n"
    //"#.....#.#...#.#\n"
    //"#.###.#####.#.#\n"
    //"#.#.#.......#.#\n"
    //"#.#.#####.###.#\n"
    //"#...........#.#\n"
    //"###.#.#####.#.#\n"
    //"#...#.....#.#.#\n"
    //"#.#.#.###.#.#.#\n"
    //"#.....#...#.#.#\n"
    //"#.###.#.#.#.#.#\n"
    //"#S..#.....#...#\n"
    //"###############\n"

    "#################\n"
    "#...#...#...#..E#\n"
    "#.#.#.#.#.#.#.#.#\n"
    "#.#.#.#...#...#.#\n"
    "#.#.#.#.###.#.#.#\n"
    "#...#.#.#.....#.#\n"
    "#.#.#.#.#.#####.#\n"
    "#.#...#.#.#.....#\n"
    "#.#.#####.#.###.#\n"
    "#.#.#.......#...#\n"
    "#.#.###.#####.###\n"
    "#.#.#...#.....#.#\n"
    "#.#.#.#####.###.#\n"
    "#.#.#.........#.#\n"
    "#.#.#.#########.#\n"
    "#S#.............#\n"
    "#################\n"
);

typedef struct Day16Entry Day16Entry;

typedef struct Day16Node Day16Node;
struct Day16Node {
    Day16Entry *entry;
    Day16Node *next;
};

typedef struct Day16List Day16List;
struct Day16List {
    Day16Node *first;
    Day16Node *last;
};

struct Day16Entry {
    Day16List path;
    V2S64 position;
    U32 direction;
    U64 cost_to_here;
};

internal V2S64 day16_direction_to_delta[] = {
    {{  1,  0, }},
    {{  0,  1, }},
    {{ -1,  0, }},
    {{  0, -1, }},
};

internal Void day16_fill(Str8 map, S64 width, S64 height, Day16Entry *entry) {
    if (!entry) {
        return;
    }

    S64 index = entry->position.x + entry->position.y * width;
    map.data[index] = 'O';

    for (Day16Node *node = entry->path.first; node; node = node->next) {
        day16_fill(map, width, height, node->entry);
    }
}

internal Void day16_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day16_example;
    os_file_read(arena, str8_literal("data/day16.txt"), &data);

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

    Day16Entry **visited = arena_push_array_zero(arena, Day16Entry *, (U64) (width * height * 4));
    Day16Entry **heap = arena_push_array_zero(arena, Day16Entry *, (U64) (width * height));
    U64 heap_size = 0;

    heap[0] = arena_push_struct_zero(arena, Day16Entry);
    heap[0]->position = start;
    ++heap_size;

    U64 best_cost = U64_MAX;
    while (heap_size && heap[0]->cost_to_here <= best_cost) {
        Day16Entry *entry = heap[0];

        // NOTE(simon): Shift down
        heap[0] = heap[--heap_size];
        for (U64 new_index = 0;;) {
            U64 left_child  = new_index * 2 + 1;
            U64 right_child = new_index * 2 + 2;

            U64 smaller = left_child;
            if (right_child < heap_size && heap[right_child]->cost_to_here < heap[smaller]->cost_to_here) {
                smaller = right_child;
            }

            if (smaller < heap_size && heap[smaller]->cost_to_here < heap[new_index]->cost_to_here) {
                swap(heap[smaller], heap[new_index], Day16Entry *);
                new_index = smaller;
            } else {
                break;
            }
        }

        S64 index = (entry->position.x + entry->position.y * width) * 4 + entry->direction;

        if (visited[index]) {
            if (visited[index]->cost_to_here == entry->cost_to_here) {
                Day16Node *path = arena_push_struct_zero(arena, Day16Node);
                path->entry = entry->path.first->entry;
                sll_queue_push(visited[index]->path.first, visited[index]->path.last, path);
            }
            continue;
        }
        visited[index] = entry;

        if (entry->position.x == end.x && entry->position.y == end.y) {
            best_cost = entry->cost_to_here;
        }

        for (U64 i = 0; i < 3; ++i) {
            Day16Entry *candidate = arena_push_struct_zero(arena, Day16Entry);
            Day16Node *path = arena_push_struct_zero(arena, Day16Node);
            path->entry = entry;
            sll_queue_push(candidate->path.first, candidate->path.last, path);
            candidate->direction = (entry->direction + i - 1) % 4;
            candidate->position = v2s64_add(entry->position, day16_direction_to_delta[candidate->direction]);
            candidate->cost_to_here = entry->cost_to_here + 1 + (entry->direction == candidate->direction ? 0 : 1000);

            S64 candidate_index = candidate->position.x + candidate->position.y * width;

            if (map.data[candidate_index] == '#') {
                continue;
            }

            heap[heap_size++] = candidate;
            for (U64 new_index = heap_size - 1; new_index;) {
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

    day16_fill(map, width, height, visited[(end.x + end.y * width) * 4 + 0]);
    day16_fill(map, width, height, visited[(end.x + end.y * width) * 4 + 1]);
    day16_fill(map, width, height, visited[(end.x + end.y * width) * 4 + 2]);
    day16_fill(map, width, height, visited[(end.x + end.y * width) * 4 + 3]);

    U64 seats = 0;
    for (U64 i = 0; i < map.size; ++i) {
        seats += map.data[i] == 'O';
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", best_cost));
    os_console_print(str8_format(arena, "Part 2: %lu\n", seats));

    arena_destroy(arena);
}
