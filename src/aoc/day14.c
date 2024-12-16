global Str8 day14_example = str8_literal(
    "p=0,4 v=3,-3\n"
    "p=6,3 v=-1,-3\n"
    "p=10,3 v=-1,2\n"
    "p=2,0 v=2,-1\n"
    "p=0,0 v=1,3\n"
    "p=3,0 v=-2,-2\n"
    "p=7,6 v=-1,-3\n"
    "p=3,0 v=-1,-2\n"
    "p=9,3 v=2,3\n"
    "p=7,3 v=-1,2\n"
    "p=2,4 v=2,-3\n"
    "p=9,5 v=-3,-3\n"
);

typedef struct S64Parse S64Parse;
struct S64Parse {
    S64 value;
    U64 size;
};

internal S64Parse s64_from_str8(Str8 string) {
    U8 *ptr = string.data;
    U8 *opl = string.data + string.size;

    S64 value = 0;
    if (ptr + 1 < opl && ptr[0] == '-' && is_digit(ptr[1])) {
        ++ptr;
        while (ptr < opl && is_digit(*ptr)) {
            value = 10 * value - (*ptr - '0');
            ++ptr;
        }
    } else {
        while (ptr < opl && is_digit(*ptr)) {
            value = 10 * value + (*ptr - '0');
            ++ptr;
        }
    }

    S64Parse result = { 0 };
    result.value    = value;
    result.size     = (U64) (ptr - string.data);

    return result;
}

internal Void day14_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day14_example;
    os_file_read(arena, str8_literal("data/day14.txt"), &data);

    V2S64 size = v2s64(101, 103);
    S64 time = 100;

    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));
    V2S64 *positions = arena_push_array_zero(arena, V2S64, lines.node_count);
    V2S64 *velocities = arena_push_array_zero(arena, V2S64, lines.node_count);
    U64 count = 0;

    for (Str8Node *line = lines.first; line; line = line->next) {
        Arena_Temporary scratch = arena_get_scratch(0, 0);
        Str8List parts = str8_split_by_codepoints(scratch.arena, line->string, str8_literal("=,"));
        positions[count] = v2s64(
            s64_from_str8(parts.first->next->string).value,
            s64_from_str8(parts.first->next->next->string).value
        );
        velocities[count] = v2s64(
            s64_from_str8(parts.first->next->next->next->string).value,
            s64_from_str8(parts.first->next->next->next->next->string).value
        );

        ++count;
        arena_end_temporary(scratch);
    }

    U64 q0 = 0;
    U64 q1 = 0;
    U64 q2 = 0;
    U64 q3 = 0;

    for (U64 i = 0; i < count; ++i) {
        V2S64 final_position = v2s64_add(positions[i], v2s64_scale(velocities[i], time));
        V2S64 wrapped_position = v2s64(
            (final_position.x % size.width + size.width) % size.width,
            (final_position.y % size.height + size.height) % size.height
        );

        if (wrapped_position.x < size.width / 2) {
            if (wrapped_position.y < size.height / 2) {
                ++q1;
            } else if (wrapped_position.y > size.height / 2) {
                ++q2;
            }
        } else if (wrapped_position.x > size.width / 2) {
            if (wrapped_position.y < size.height / 2) {
                ++q0;
            } else if (wrapped_position.y > size.height / 2) {
                ++q3;
            }
        }
    }

    os_console_print(str8_format(arena, "%lu\n", q0 * q1 * q2 * q3));

    // NOTE(simon): These look visually interesting and follow a predictable
    // pattern, manually inspect them from here
    // 53, 156, 259, 362, 465, 568, 671, 774, 877, 980

    for (S64 t = 53; t < 10000; t += 103) {
        Arena_Temporary scratch = arena_get_scratch(0, 0);
        os_console_print(str8_format(scratch.arena, "Time: %lu\n", t));
        U64 *counts = arena_push_array_zero(scratch.arena, U64, (U64) (size.width * size.height));

        for (U64 i = 0; i < count; ++i) {
            V2S64 final_position = v2s64_add(positions[i], v2s64_scale(velocities[i], t));
            V2S64 wrapped_position = v2s64(
                (final_position.x % size.width + size.width) % size.width,
                (final_position.y % size.height + size.height) % size.height
            );

            ++counts[wrapped_position.x + wrapped_position.y * size.width];
        }

        for (U64 y = 0; y < (U64) size.height; ++y) {
            for (U64 x = 0; x < (U64) size.width; ++x) {
                if (counts[x + y * (U64) size.width]) {
                    os_console_print(str8_format(scratch.arena, "%lu", counts[x + y * (U64) size.width]));
                } else {
                    os_console_print(str8_literal("."));
                }
            }
            os_console_print(str8_literal("\n"));
        }
        arena_end_temporary(scratch);
    }

    arena_destroy(arena);
}
