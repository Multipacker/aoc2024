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

typedef struct U64Tuple U64Tuple;
struct U64Tuple {
    U64 a;
    U64 b;
};

internal Void day5_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day5_example;
    os_file_read(arena, str8_literal("data/day5.txt"), &data);

    // NOTE(simon): Parse
    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));

    Str8List rule_nodes = { 0 };
    Str8List update_nodes = { 0 };

    B32 is_updates = false;
    for (Str8Node *node = lines.first; node; node = node->next) {
        if (node->string.size == 0) {
            is_updates = true;
        } else if (is_updates) {
            str8_list_push(arena, &update_nodes, node->string);
        } else {
            str8_list_push(arena, &rule_nodes, node->string);
        }
    }

    U64Tuple *rules = arena_push_array(arena, U64Tuple, rule_nodes.node_count);
    U64 rule_count = 0;
    for (Str8Node *node = rule_nodes.first; node; node = node->next) {
        Str8 rule = node->string;
        U64Parse a = u64_from_str8(rule);
        U64Parse b = u64_from_str8(str8_skip(rule, a.size + 1));
        rules[rule_count].a = a.value;
        rules[rule_count].b = b.value;
        ++rule_count;
    }

    U64Array *updates = arena_push_array(arena, U64Array, update_nodes.node_count);
    U64 update_count = 0;
    for (Str8Node *update = update_nodes.first; update; update = update->next) {
        Arena_Temporary scratch = arena_get_scratch(0, 0);
        Str8List page_nodes = str8_split_by_codepoints(scratch.arena, update->string, str8_literal(","));

        U64Array pages = { 0 };
        pages.data = arena_push_array(arena, U64, page_nodes.node_count);
        for (Str8Node *page_node = page_nodes.first; page_node; page_node = page_node->next) {
            U64Parse page = u64_from_str8(page_node->string);
            pages.data[pages.size] = page.value;
            ++pages.size;
        }

        updates[update_count] = pages;
        ++update_count;

        arena_end_temporary(scratch);
    }


    // NOTE(simon): Part 1
    U64 correct_middle_sum = 0;
    U64 incorrect_middle_sum = 0;
    for (U64 update_index = 0; update_index < update_count; ++update_index) {
        Arena_Temporary scratch = arena_get_scratch(0, 0);

        U64Array pages = updates[update_index];

        B32 is_ordered = true;
        for (U64 i = 1; i < pages.size; ++i) {
            for (U64 j = i; j > 0; --j) {
                U64 a = pages.data[j - 1];
                U64 b = pages.data[j - 0];

                for (U64 k = 0; k < rule_count; ++k) {
                    U64Tuple rule = rules[k];
                    if (rule.a == a && rule.b == b) {
                        break;
                    } else if (rule.a == b && rule.b == a) {
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

        arena_end_temporary(scratch);
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", correct_middle_sum));
    os_console_print(str8_format(arena, "Part 2: %lu\n", incorrect_middle_sum));

    arena_destroy(arena);
}
