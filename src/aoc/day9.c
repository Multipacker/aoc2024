global Str8 day9_example = str8_literal(
    "2333133121414131402"
);

typedef struct Day9Node Day9Node;
struct Day9Node {
    Day9Node *next;
    Day9Node *previous;
    U64 position;
    U64 value;
    U64 size;
};

typedef struct Day9List Day9List;
struct Day9List {
    Day9Node *first;
    Day9Node *last;
};

internal Void day9_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day9_example;
    os_file_read(arena, str8_literal("data/day9.txt"), &data);
    U32 *ids0 = arena_push_array_zero(arena, U32, data.size * 9);
    U32 id0_count = 0;

    U64 position = 0;
    Day9List filled = { 0 };
    Day9List empty = { 0 };

    // NOTE(simon): Decompress
    for (U64 i = 0; i < data.size && is_digit(data.data[i]); ++i) {
        U64 id = i % 2 == 0 ? 1 + i / 2 : 0;
        U64 count = data.data[i] - '0';
        for (U64 j = 0; j < count; ++j) {
            ids0[id0_count++] = (U32) id;
        }

        Day9Node *node = arena_push_struct_zero(arena, Day9Node);
        node->position = position;
        node->value = id;
        node->size = count;
        position += count;

        if (node->value) {
            dll_push_back(filled.first, filled.last, node);
        } else {
            dll_push_back(empty.first, empty.last, node);
        }
    }

    // NOTE(simon): Fragment
    U64 insert_index = 0;
    while (insert_index < id0_count) {
        while (insert_index < id0_count && ids0[insert_index]) {
            ++insert_index;
        }

        if (insert_index < id0_count) {
            ids0[insert_index] = ids0[id0_count - 1];
            --id0_count;
        }
    }

    // NOTE(simon): Defragment
    for (Day9Node *file = filled.last; file; file = file->previous) {
        Day9Node *selected = 0;
        for (Day9Node *free = empty.first; free && free->position < file->position; free = free->next) {
            if (free->size >= file->size) {
                selected = free;
                break;
            }
        }

        if (selected) {
            file->position = selected->position;
            selected->position += file->size;
            selected->size -= file->size;

            if (!selected->size) {
                dll_remove(empty.first, empty.last, selected);
            }
        }
    }

    // NOTE(simon): Checksum
    U64 checksum0 = 0;
    for (U64 i = 0; i < id0_count; ++i) {
        checksum0 += i * (ids0[i] - 1);
    }

    U64 checksum1 = 0;
    for (Day9Node *file = filled.first; file; file = file->next) {
        if (file->value) {
            U64 file_position_sum = file->size * (file->size - 1) / 2;
            U64 system_position_sum = file_position_sum + file->position * file->size;
            checksum1 += system_position_sum * (file->value - 1);
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", checksum0));
    os_console_print(str8_format(arena, "Part 2: %lu\n", checksum1));

    arena_destroy(arena);
}
