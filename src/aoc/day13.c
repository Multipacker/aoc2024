global Str8 day13_example = str8_literal(
    "Button A: X+94, Y+34\n"
    "Button B: X+22, Y+67\n"
    "Prize: X=8400, Y=5400\n"
    "\n"
    "Button A: X+26, Y+66\n"
    "Button B: X+67, Y+21\n"
    "Prize: X=12748, Y=12176\n"
    "\n"
    "Button A: X+17, Y+86\n"
    "Button B: X+84, Y+37\n"
    "Prize: X=7870, Y=6450\n"
    "\n"
    "Button A: X+69, Y+23\n"
    "Button B: X+27, Y+71\n"
    "Prize: X=18641, Y=10279\n"
);

internal Void day13_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day13_example;
    os_file_read(arena, str8_literal("data/day13.txt"), &data);

    S64 tokens0 = 0;
    S64 tokens1 = 0;
    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    for (Str8Node *line = lines.first, *next = 0; line; line = next) {
        Str8List a_numbers = str8_split_by_codepoints(arena, line->string, str8_literal("+"));
        Str8List b_numbers = str8_split_by_codepoints(arena, line->next->string, str8_literal("+"));
        Str8List prize_numbers = str8_split_by_codepoints(arena, line->next->next->string, str8_literal("="));

        V2S64 a = v2s64(
            (S64) u64_from_str8(a_numbers.first->next->string).value,
            (S64) u64_from_str8(a_numbers.first->next->next->string).value
        );
        V2S64 b = v2s64(
            (S64) u64_from_str8(b_numbers.first->next->string).value,
            (S64) u64_from_str8(b_numbers.first->next->next->string).value
        );
        V2S64 c = v2s64(
            (S64) u64_from_str8(prize_numbers.first->next->string).value,
            (S64) u64_from_str8(prize_numbers.first->next->next->string).value
        );

        if ((c.x * a.y - c.y * a.x) % (b.x * a.y - b.y * a.x) == 0) {
            S64 B = (c.x * a.y - c.y * a.x) / (b.x * a.y - b.y * a.x);
            if ((c.y - B * b.y) % a.y == 0) {
                S64 A = (c.y - B * b.y) / a.y;
                tokens0 += 3 * A + B;
            }
        }

        c.x += 10000000000000;
        c.y += 10000000000000;

        if ((c.x * a.y - c.y * a.x) % (b.x * a.y - b.y * a.x) == 0) {
            S64 B = (c.x * a.y - c.y * a.x) / (b.x * a.y - b.y * a.x);
            if ((c.y - B * b.y) % a.y == 0) {
                S64 A = (c.y - B * b.y) / a.y;
                tokens1 += 3 * A + B;
            }
        }

        next = line->next->next->next;
        if (next && next->string.size == 0) {
            next = next->next;
        }
    }

    os_console_print(str8_format(arena, "Part 1: %ld\n", tokens0));
    os_console_print(str8_format(arena, "Part 2: %ld\n", tokens1));

    arena_destroy(arena);
}
