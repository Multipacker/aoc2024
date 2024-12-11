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

internal Void day11_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day11_example;
    //os_file_read(arena, str8_literal("data/day11.txt"), &data);

    Str8List raw_stones = str8_split_by_codepoints(arena, data, str8_literal(" "));
    U64List stones = { 0 };
    for (Str8Node *node = raw_stones.first; node; node = node->next) {
        U64Node *stone = arena_push_struct_zero(arena, U64Node);
        stone->value = u64_from_str8(node->string).value;
        dll_push_back(stones.first, stones.last, stone);
    }

    for (U64Node *node = stones.first, *next = 0; node; node = next) {
        next = node->next;

        if (node->value == 0) {
            node->value = 1;
        } else {
        }

        os_console_print(str8_format(arena, "%lu\n", node->value));
    }

    arena_destroy(arena);
}
