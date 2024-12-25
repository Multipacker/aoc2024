global Str8 day18_example = str8_literal(
    "5,4\n"
    "4,2\n"
    "4,5\n"
    "3,0\n"
    "2,1\n"
    "6,3\n"
    "2,4\n"
    "1,5\n"
    "0,6\n"
    "3,3\n"
    "2,6\n"
    "5,1\n"
    "1,2\n"
    "5,5\n"
    "2,5\n"
    "6,5\n"
    "1,4\n"
    "0,4\n"
    "6,4\n"
    "1,1\n"
    "6,1\n"
    "1,0\n"
    "0,5\n"
    "1,6\n"
    "2,0\n"
);

typedef struct Day18Entry Day18Entry;
struct Day18Entry {
    Day18Entry *previous;
    V2S64 position;
    U64 cost_to_here;
};

internal Day18Entry *day18_search(Arena *arena, B8 *map, V2S64 size, V2S64 start, V2S64 end) {
    Day18Entry *result = 0;

    B8 *visited = arena_push_array_zero(arena, B8, (U64) (size.width * size.height));

    Day18Entry **heap = arena_push_array_zero(arena, Day18Entry *, (U64) (size.width * size.height));
    U64 heap_size = 0;

    heap[0] = arena_push_struct_zero(arena, Day18Entry);
    heap[0]->position = start;
    ++heap_size;

    while (heap_size) {
        Day18Entry *entry = heap[0];

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
                swap(heap[smaller], heap[new_index], Day18Entry *);
                new_index = smaller;
            } else {
                break;
            }
        }

        S64 index = entry->position.x + entry->position.y * size.width;
        if (visited[index]) {
            continue;
        }
        visited[index] = true;

        if (entry->position.x == end.x && entry->position.y == end.y) {
            result = entry;
            break;
        }

        for (U64 i = 0; i < 4; ++i) {
            Day18Entry *candidate = arena_push_struct_zero(arena, Day18Entry);
            candidate->previous = entry;
            V2S64 deltas[] = {
                {{ -1,  0, }},
                {{  0, -1, }},
                {{  1,  0, }},
                {{  0,  1, }},
            };
            candidate->position = v2s64_add(entry->position, deltas[i]);
            candidate->cost_to_here = entry->cost_to_here + 1;

            S64 candidate_index = candidate->position.x + candidate->position.y * size.width;
            if (!r2s64_contains(r2s64_from_position_size(v2s64(0, 0), size), candidate->position) || map[candidate_index]) {
                continue;
            }

            heap[heap_size++] = candidate;
            for (U64 new_index = heap_size - 1; new_index;) {
                U64 parent_index = (new_index - 1) / 2;
                if (heap[new_index]->cost_to_here < heap[parent_index]->cost_to_here) {
                    swap(heap[new_index], heap[parent_index], Day18Entry *);
                    new_index = parent_index;
                } else {
                    break;
                }
            }
        }
    }

    return result;
}

internal Void day18_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day18_example;
    os_file_read(arena, str8_literal("data/day18.txt"), &data);

    V2S64 size = v2s64(7, 7);
    size = v2s64(71, 71);

    U64 simulated = 12;
    simulated = 1024;

    V2S64 start = v2s64(0, 0);
    V2S64 end = v2s64_subtract(size, v2s64(1, 1));

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    V2S64 *coordinates = arena_push_array_zero(arena, V2S64, lines.node_count);
    U64 coordinate_count = 0;
    for (Str8Node *line = lines.first; line; line = line->next) {
        Str8List numbers = str8_split_by_codepoints(arena, line->string, str8_literal(","));
        coordinates[coordinate_count].x = s64_from_str8(numbers.first->string).value;
        coordinates[coordinate_count].y = s64_from_str8(numbers.last->string).value;
        ++coordinate_count;
    }

    B8 *map = arena_push_array_zero(arena, B8, (U64) (size.width * size.height));
    for (U64 i = 0; i < simulated; ++i) {
        V2S64 coordinate = coordinates[i];
        map[coordinate.x + coordinate.y * size.width] = true;
    }

    {
        Arena_Temporary scratch = arena_get_scratch(0, 0);
        Day18Entry *path = day18_search(scratch.arena, map, size, start, end);
        U64 length = 0;
        for (Day18Entry *step = path; step; step = step->previous) {
            ++length;
        }
        os_console_print(str8_format(scratch.arena, "Part 1: %lu\n", length - 1));
        arena_end_temporary(scratch);
    }

    U64 low = simulated;
    U64 high = coordinate_count - 1;
    while (high - low > 1) {
        Arena_Temporary scratch = arena_get_scratch(0, 0);
        U64 middle = (low + high) / 2;

        for (U64 i = simulated; i <= middle; ++i) {
            V2S64 coordinate = coordinates[i];
            map[coordinate.x + coordinate.y * size.width] = true;
        }

        Day18Entry *path = day18_search(scratch.arena, map, size, start, end);

        for (U64 i = simulated; i <= middle; ++i) {
            V2S64 coordinate = coordinates[i];
            map[coordinate.x + coordinate.y * size.width] = false;
        }

        if (path) {
            low = middle;
        } else {
            high = middle;
        }

        arena_end_temporary(scratch);
    }

    os_console_print(str8_format(arena, "Part 2: %lu,%lu\n", coordinates[high].x, coordinates[high].y));

    arena_destroy(arena);
}
