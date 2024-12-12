// TODO(simon): Try using hash tries for this solution. They could be a really
// good tool for AoC

global Str8 day11_example = str8_literal(
    "125 17"
);

typedef struct U64Node U64Node;
struct U64Node {
    U64Node *next;
    U64Node *previous;
    U64 value;
};

typedef struct U64List U64List;
struct U64List {
    U64Node *first;
    U64Node *last;
};

typedef struct Day11Entry Day11Entry;
struct Day11Entry {
    Day11Entry *next;
    U64 value;
    U64 depth;
    U64 count;
};

typedef struct Day11EntryList Day11EntryList;
struct Day11EntryList {
    Day11Entry *first;
    Day11Entry *last;
};

typedef struct Day11Table Day11Table;
struct Day11Table {
    Day11EntryList *entries;
    U64 size;
};

internal U64 day11_apply(Arena *arena, Day11Table *table, U64 value, U64 depth) {
    if (depth == 0) {
        return 1;
    }

    U64 index = hash_combine(u64_hash(value), u64_hash(depth)) & (table->size - 1);
    Day11EntryList *bucket = &table->entries[index];
    Day11Entry *entry = bucket->first;
    while (entry && (entry->value != value || entry->depth != depth)) {
        entry = entry->next;
    }

    if (!entry) {
        entry = arena_push_struct_zero(arena, Day11Entry);
        sll_queue_push(bucket->first, bucket->last, entry);
        entry->value = value;
        entry->depth = depth;

        if (value == 0) {
            entry->count = day11_apply(arena, table, 1, depth - 1);
        } else {
            U64 digits = 0;
            for (U64 i = value; i; i /= 10) {
                ++digits;
            }
            if (digits % 2 == 0) {
                U64 factor = 1;
                while (digits) {
                    factor *= 10;
                    digits -= 2;
                }
                U64 a = value / factor;
                U64 b = value % factor;
                U64 a_count = day11_apply(arena, table, a, depth - 1);
                U64 b_count = day11_apply(arena, table, b, depth - 1);
                entry->count = a_count + b_count;
            } else {
                entry->count = day11_apply(arena, table, value * 2024, depth - 1);
            }
        }
    }

    return entry->count;
}

internal Void day11_solve(Void) {
    Arena *arena = arena_create_reserve(gigabytes(4));

    Str8 data = day11_example;
    os_file_read(arena, str8_literal("data/day11.txt"), &data);

    Day11Table table = { 0 };
    table.size = 1 << 12;
    table.entries = arena_push_array_zero(arena, Day11EntryList, table.size);

    U64 count0 = 0;
    U64 count1 = 0;
    for (Str8Node *node = str8_split_by_codepoints(arena, data, str8_literal(" ")).first; node; node = node->next) {
        U64 value = u64_from_str8(node->string).value;
        count0 += day11_apply(arena, &table, value, 25);
        count1 += day11_apply(arena, &table, value, 75);
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", count0));
    os_console_print(str8_format(arena, "Part 2: %lu\n", count1));

    arena_destroy(arena);
}
