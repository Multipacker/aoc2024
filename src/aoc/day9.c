global Str8 day9_example = str8_literal(
    "2333133121414131402"
);

typedef struct V2U64Node V2U64Node;
struct V2U64Node {
    V2U64Node *next;
    V2U64Node *previous;
    V2U64 value;
};

typedef struct V2U64List V2U64List;
struct V2U64List {
    V2U64Node *first;
    V2U64Node *last;
};

internal Void day9_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day9_example;
    os_file_read(arena, str8_literal("data/day9.txt"), &data);
    U32 *ids0 = arena_push_array_zero(arena, U32, data.size * 9);
    U32 id0_count = 0;
    V2U64List blocks = { 0 };

    // NOTE(simon): Decompress
    for (U64 i = 0; i < data.size && is_digit(data.data[i]); ++i) {
        U64 id = i % 2 == 0 ? 1 + i / 2 : 0;
        U64 count = data.data[i] - '0';
        for (U64 j = 0; j < count; ++j) {
            ids0[id0_count++] = (U32) id;
        }

        V2U64Node *node = arena_push_struct_zero(arena, V2U64Node);
        node->value.x = id;
        node->value.y = count;
        dll_push_back(blocks.first, blocks.last, node);
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
    for (V2U64Node *file = blocks.last; file; file = file->previous) {
        if (!file->value.x) {
            continue;
        }

        V2U64Node *selected = 0;
        for (V2U64Node *free = blocks.first; free != file; free = free->next) {
            if (!free->value.x && free->value.y >= file->value.y) {
                selected = free;
                break;
            }
        }

        if (selected) {
            if (selected->value.y == file->value.y) {
                selected->value.x = file->value.x;
            } else {
                V2U64Node *new = arena_push_struct_zero(arena, V2U64Node);
                new->value = file->value;
                selected->value.y -= new->value.y;
                dll_insert_before(blocks.first, blocks.last, selected, new);
            }
            file->value.x = 0;
        }
    }

    // NOTE(simon): Checksum
    U64 checksum0 = 0;
    for (U64 i = 0; i < id0_count; ++i) {
        checksum0 += i * (ids0[i] - 1);
    }

    U64 checksum1 = 0;
    U64 position = 0;
    for (V2U64Node *file = blocks.first; file; file = file->next) {
        if (file->value.x) {
            for (U64 i = 0; i < file->value.y; ++i) {
                checksum1 += (position + i) * (file->value.x - 1);
            }
        }

        position += file->value.y;
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", checksum0));
    os_console_print(str8_format(arena, "Part 2: %lu\n", checksum1));

    arena_destroy(arena);
}
