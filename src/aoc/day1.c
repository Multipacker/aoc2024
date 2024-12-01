global Str8 day1_example = str8_literal(
    "3   4\n"
    "4   3\n"
    "2   5\n"
    "1   3\n"
    "3   9\n"
    "3   3\n"
);

typedef struct U64Parse U64Parse;
struct U64Parse {
    U64 value;
    U64 size;
};

internal B32 is_digit(U32 codepoint) {
    B32 result = ('0' <= codepoint && codepoint <= '9');
    return result;
}

internal B32 is_whitespace(U32 codepoint) {
    B32 result = (codepoint == ' ' || codepoint == '\t');
    return result;
}

internal U64Parse u64_from_str8(Str8 string) {
    U8 *ptr = string.data;
    U8 *opl = string.data + string.size;
    U64 value = 0;

    while (ptr < opl && is_digit(*ptr)) {
        value = 10 * value + (*ptr - '0');
        ++ptr;
    }

    U64Parse result = { 0 };
    result.value    = value;
    result.size     = (U64) (ptr - string.data);

    return result;
}

internal Str8 str8_skip_whitespace(Str8 string) {
    U8 *ptr = string.data;
    U8 *opl = string.data + string.size;

    while (ptr < opl && is_whitespace(*ptr)) {
        ++ptr;
    }

    return str8_range(ptr, opl);
}

internal Void quicksort(U64 *ptr, U64 *opl) {
    U64 size = (U64) (opl - ptr);
    if (size <= 1) {
        return;
    }

    // NOTE(simon): Find median of 3
    U64 first_index  = 0;
    U64 middle_index = size / 2;
    U64 last_index   = size - 1;

    if (ptr[first_index] > ptr[middle_index]) {
        swap(first_index, middle_index, U64);
    }
    if (ptr[middle_index] > ptr[last_index]) {
        swap(middle_index, last_index, U64);
    }
    if (ptr[first_index] > ptr[middle_index]) {
        swap(first_index, middle_index, U64);
    }

    U64 pivot_index = middle_index;
    U64 pivot = ptr[pivot_index];

    // NOTE(simon): Partition
    swap(ptr[0], ptr[pivot_index], U64);

    U64 *start = ptr + 1;
    U64 *end   = opl - 1;
    while (start <= end) {
        while (start <= end && *start <= pivot) {
            ++start;
        }

        while (start <= end && pivot < *end) {
            --end;
        }

        if (start <= end) {
            swap(*start, *end, U64);
        }
    }

    swap(ptr[0], *end, U64);

    quicksort(ptr, end);
    quicksort(end + 1, opl);
}

internal Void day1_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day1_example;
    os_file_read(arena, str8_literal("data/day1.txt"), &data);

    // NOTE(simon): Parse
    Str8List lines = str8_split_by_codepoints(arena, data, str8_literal("\n"));

    U64 *left  = arena_push_array_zero(arena, U64, lines.node_count);
    U64 *right = arena_push_array_zero(arena, U64, lines.node_count);
    U64 count  = 0;

    for (Str8Node *node = lines.first; node; node = node->next) {
        Str8 string = node->string;
        U64Parse left_parse = u64_from_str8(string);
        string = str8_skip_whitespace(str8_skip(string, left_parse.size));
        U64Parse right_parse = u64_from_str8(string);

        left[count]  = left_parse.value;
        right[count] = right_parse.value;
        ++count;
    }

    // NOTE(simon): Pre-process
    quicksort(left,  left  + count);
    quicksort(right, right + count);

    // NOTE(simon): Part 1
    U64 part1_sum = 0;
    for (U64 i = 0; i < count; ++i) {
        if (left[i] < right[i]) {
            part1_sum += right[i] - left[i];
        } else {
            part1_sum += left[i] - right[i];
        }
    }

    os_console_print(str8_format(arena, "Part 1: %lu\n", part1_sum));

    // NOTE(simon): Part 2
    U64 part2_sum   = 0;
    U64 left_index  = 0;
    U64 right_index = 0;
    while (left_index < count) {
        U64 number = left[left_index];
        U64 left_frequency  = 0;
        U64 right_frequency = 0;

        while (left_index < count && left[left_index] == number) {
            ++left_frequency;
            ++left_index;
        }

        // NOTE(simon): Skip numbers in the right list that don't exist in the left list
        while (right_index < count && right[right_index] < number) {
            ++right_index;
        }

        while (right_index < count && right[right_index] == number) {
            ++right_frequency;
            ++right_index;
        }

        part2_sum += number * left_frequency * right_frequency;
    }

    os_console_print(str8_format(arena, "Part 2: %lu\n", part2_sum));

    arena_destroy(arena);
}
