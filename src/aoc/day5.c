global Str8 day5_example = str8_literal(
    "47|53\n"
    "97|13\n"
    "97|61\n"
    "97|47\n"
    "75|29\n"
    "61|13\n"
    "75|53\n"
    "29|13\n"
    "97|29\n"
    "53|29\n"
    "61|53\n"
    "97|53\n"
    "61|29\n"
    "47|13\n"
    "75|47\n"
    "97|75\n"
    "47|61\n"
    "75|61\n"
    "47|29\n"
    "75|13\n"
    "53|13\n"
    "\n"
    "75,47,61,53,29\n"
    "97,61,53,29,13\n"
    "75,29,13\n"
    "75,97,47,61,53\n"
    "61,13,29\n"
    "97,13,75,29,47\n"
);

typedef struct U64Array U64Array;
struct U64Array {
    U64 *data;
    U64 size;
};

typedef struct U64ArrayNode U64ArrayNode;
struct U64ArrayNode {
    U64ArrayNode *next;
    U64Array value;
};

typedef struct U64ArrayList U64ArrayList;
struct U64ArrayList {
    U64ArrayNode *first;
    U64ArrayNode *last;
};

typedef struct U64Tuple U64Tuple;
struct U64Tuple {
    U64 a;
    U64 b;
};

typedef struct U64TupleNode U64TupleNode;
struct U64TupleNode {
    U64TupleNode *next;
    U64Tuple value;
};

typedef struct U64TupleList U64TupleList;
struct U64TupleList {
    U64TupleNode *first;
    U64TupleNode *last;
};

internal Void day5_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day5_example;
    os_file_read(arena, str8_literal("data/day5.txt"), &data);

    // NOTE(simon): Parse
    U64TupleList rules = { 0 };
    U64ArrayList updates = { 0 };

    B32 is_updates = false;
    for (Str8Node *node = str8_split_by_codepoints(arena, data, str8_literal("\n")).first; node; node = node->next) {
        Arena_Temporary scratch = arena_get_scratch(0, 0);
        if (node->string.size == 0) {
            is_updates = true;
        } else if (is_updates) {
            Str8List page_nodes = str8_split_by_codepoints(scratch.arena, node->string, str8_literal(","));

            U64ArrayNode *pages = arena_push_struct_zero(arena, U64ArrayNode);
            pages->value.data = arena_push_array(arena, U64, page_nodes.node_count);
            for (Str8Node *page_node = page_nodes.first; page_node; page_node = page_node->next) {
                pages->value.data[pages->value.size++] = u64_from_str8(page_node->string).value;
            }

            sll_queue_push(updates.first, updates.last, pages);
        } else {
            U64TupleNode *rule = arena_push_struct_zero(arena, U64TupleNode);
            Str8List parts = str8_split_by_codepoints(scratch.arena, node->string, str8_literal("|"));
            rule->value.a = u64_from_str8(parts.first->string).value;
            rule->value.b = u64_from_str8(parts.last->string).value;
            sll_queue_push(rules.first, rules.last, rule);
        }
        arena_end_temporary(scratch);
    }

    U64 correct_middle_sum = 0;
    U64 incorrect_middle_sum = 0;
    for (U64ArrayNode *page_node = updates.first; page_node; page_node = page_node->next) {
        U64Array pages = page_node->value;

        B32 is_ordered = true;
        for (U64 i = 1; i < pages.size; ++i) {
            for (U64 j = i; j > 0; --j) {
                U64 a = pages.data[j - 1];
                U64 b = pages.data[j - 0];

                for (U64TupleNode *rule = rules.first; rule; rule = rule->next) {
                    if (rule->value.a == b && rule->value.b == a) {
                        swap(pages.data[j - 1], pages.data[j], U64);
                        is_ordered = false;
                    }
                }
            }
        }

        if (is_ordered) {
            correct_middle_sum += pages.data[pages.size / 2];
        } else {
            incorrect_middle_sum += pages.data[pages.size / 2];
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", correct_middle_sum));
    os_console_print(str8_format(arena, "Part 2: %lu\n", incorrect_middle_sum));

    arena_destroy(arena);
}
