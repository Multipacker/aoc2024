#include <stdio.h>

internal Str8 str8(U8 *data, U64 size) {
    Str8 result;
    result.data = data;
    result.size = size;

    return result;
}

internal Str8 str8_range(U8 *start, U8 *opl) {
    Str8 result;
    result.data = start;
    result.size = (U64) (opl - start);

    return result;
}

internal Str8 str8_copy(Arena *arena, Str8 string) {
    U8 *data = arena_push_array(arena, U8, string.size);
    memory_copy(data, string.data, string.size);

    Str8 result;
    result.data = data;
    result.size = string.size;
    return result;
}

internal Str8 str8_cstr(CStr data) {
    Str8 result;
    result.data = (U8 *) data;
    result.size = 0;

    while (result.data[result.size]) {
        ++result.size;
    }

    return result;
}

internal Str8 str8_copy_cstr(Arena *arena, U8 *data) {
    Str8 result;
    result.size = 0;

    while (data[result.size]) {
        ++result.size;
    }

    result.data = arena_push_array(arena, U8, result.size);
    memory_copy(result.data, data, result.size);

    return result;
}

internal Str16 str16_cstr16(CStr16 data) {
    Str16 result;
    result.data = (U16 *) data;
    result.size = 0;

    while (result.data[result.size]) {
        ++result.size;
    }

    return result;
}

internal Str8 str8_prefix(Str8 string, U64 size) {
    U64 clamped_size = u64_min(size, string.size);

    Str8 result;
    result.data = string.data;
    result.size = clamped_size;

    return result;
}

internal Str8 str8_postfix(Str8 string, U64 size) {
    U64 clamped_size = u64_min(size, string.size);

    Str8 result;
    result.data = string.data + string.size - clamped_size;
    result.size = clamped_size;

    return result;
}

internal Str8 str8_skip(Str8 string, U64 size) {
    U64 clamped_size = u64_min(size, string.size);

    Str8 result;
    result.data = string.data + clamped_size;
    result.size = string.size - clamped_size;

    return result;
}

internal Str8 str8_chop(Str8 string, U64 size) {
    U64 clamped_size = u64_min(size, string.size);

    Str8 result;
    result.data = string.data;
    result.size = string.size - clamped_size;

    return result;
}

internal Str8 str8_substring(Str8 string, U64 start, U64 size) {
    U64 clamped_start = u64_min(start, string.size);
    U64 clamped_size  = u64_min(size, string.size - clamped_start);

    Str8 result;
    result.data = string.data + clamped_start;
    result.size = clamped_size;

    return result;
}

// TODO: Unicode implementation
internal B32 str8_equal(Str8 a, Str8 b) {
    if (a.size != b.size) {
        return false;
    }

    U8 *a_ptr = a.data;
    U8 *b_ptr = b.data;
    U8 *a_opl = a.data + a.size;
    U8 *b_opl = b.data + b.size;
    while (a_ptr < a_opl && b_ptr < b_opl) {
        StringDecode a_decode = string_decode_utf8(a_ptr, (U64) (a_opl - a_ptr));
        StringDecode b_decode = string_decode_utf8(b_ptr, (U64) (b_opl - b_ptr));
        a_ptr += a_decode.size;
        b_ptr += b_decode.size;

        if (a_decode.codepoint != b_decode.codepoint) {
            return false;
        }
    }

    return true;
}

internal B32 str8_first_index_of(Str8 string, U32 codepoint, U64 *result_index) {
    B32 found  = false;
    
    U8 *ptr = string.data;
    U8 *opl = string.data + string.size;

    while (ptr < opl) {
        StringDecode decode = string_decode_utf8(ptr, (U64) (ptr - opl));
        if (decode.codepoint == codepoint) {
            found = true;
            *result_index = (U64) (ptr - string.data);
            break;
        }
        ptr += decode.size;
    }

    return found;
}

internal B32 str8_last_index_of(Str8 string, U32 codepoint, U64 *result_index) {
    B32 found  = false;
    U64 last_index = 0;
    
    U8 *ptr = string.data;
    U8 *opl = string.data + string.size;

    while (ptr < opl) {
        StringDecode decode = string_decode_utf8(ptr, (U64) (ptr - opl));
        if (decode.codepoint == codepoint) {
            found = true;
            last_index = (U64) (ptr - string.data);
        }
        ptr += decode.size;
    }

    if (found) {
        *result_index = last_index;
    }

    return found;
}

internal Void str8_list_append(Arena *arena, Str8List *list, Str8List others) {
    for (Str8Node *node = others.first; node; node = node->next) {
        str8_list_push(arena, list, node->string);
    }
}

internal Void str8_list_push_explicit(Str8List *list, Str8 string, Str8Node *node) {
    node->string = string;
    dll_push_back(list->first, list->last, node);
    ++list->node_count;
    list->total_size += string.size;
}

internal Void str8_list_push(Arena *arena, Str8List *list, Str8 string) {
    Str8Node *node = arena_push_struct(arena, Str8Node);
    str8_list_push_explicit(list, string, node);
}

internal Str8 str8_join(Arena *arena, Str8List *list) {
    U64 size = list->total_size;
    U8 *data = arena_push_array(arena, U8, size);

    U8 *ptr = data;
    for (Str8Node *node = list->first; node; node = node->next) {
        memory_copy(ptr, node->string.data, node->string.size);
        ptr += node->string.size;
    }

    return str8(data, size);
}

internal Str8 str8_format(Arena *arena, CStr format, ...) {
    va_list arguments;
    va_start(arguments, format);
    Str8 result = str8_format_list(arena, format, arguments);
    va_end(arguments);
    return result;
}

internal Str8 str8_format_list(Arena *arena, CStr format, va_list arguments) {
    Str8 result = { 0 };

    va_list format_arguments;
    va_copy(format_arguments, arguments);

    U64 needed_size = (U64) vsnprintf(0, 0, format, arguments);

    result.data = arena_push_array(arena, U8, needed_size + 1);
    result.size = needed_size;

    vsnprintf((CStr) result.data, needed_size + 1, format, format_arguments);

    va_end(format_arguments);
    return result;
}

internal Str8List str8_split_by_codepoints(Arena *arena, Str8 string, Str8 codepoints) {
    Str8List result = { 0 };

    U8 *last_split_point = string.data;
    U8 *string_ptr       = string.data;
    U8 *string_opl       = string.data + string.size;

    while (string_ptr < string_opl) {
        StringDecode string_decode = string_decode_utf8(string_ptr, (U64) (string_ptr - string_opl));

        U8 *codepoint_ptr       = codepoints.data;
        U8 *codepoint_opl       = codepoints.data + codepoints.size;
        while (codepoint_ptr < codepoint_opl) {
            StringDecode codepoint_decode = string_decode_utf8(codepoint_ptr, (U64) (codepoint_opl - codepoint_ptr));

            if (string_decode.codepoint == codepoint_decode.codepoint) {
                str8_list_push(arena, &result, str8_range(last_split_point, string_ptr));
                last_split_point = string_ptr + string_decode.size;
                break;
            }

            codepoint_ptr += codepoint_decode.size;
        }

        string_ptr += string_decode.size;
    }

    if (last_split_point != string_ptr) {
        str8_list_push(arena, &result, str8_range(last_split_point, string_ptr));
    }

    return result;
}

internal StringDecode string_decode_utf8(U8 *string, U64 size) {
    // 0:  // 0 bytes needed
    // 4:  // Invalid
    // 1:  // 1 byte needed
    // 2:  // 2 bytes needed
    // 3:  // 3 bytes needed
    // 4:  // Invalid
    local U8 lengths[32] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xxxxxxx: 0 bytes needed
        4, 4, 4, 4, 4, 4, 4, 4,                         // 10xxxxxx: Invalid
        1, 1, 1, 1,                                     // 110xxxxx: 1 byte needed
        2, 2,                                           // 1110xxxx: 2 bytes needed
        3,                                              // 11110xxx: 3 bytes needed
        4,                                              // 11111xxx: Invalid
    };
    local U8 masks[4]            = { 0x7F, 0x1F, 0x0F, 0x07, };

    // NOTE(simon): We prefill with 0xFF as it is an invalid UTF-8 byte and it
    // will ensure we handle end of buffer properly.
    U8 buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF, };
    memory_copy(buffer, string, u64_min(size, 4));
    U8 *ptr = buffer;

    StringDecode result;
    result.codepoint = 0xFFFD;
    result.size      = 0;

    if (size == 0) {
        return result;
    }

    U8 byte = *ptr++;
    ++result.size;

    U8 bytes_needed = lengths[byte >> 3];
    if (bytes_needed == 4) {
        return result;
    }

    U8  lower_boundary = (byte == 0xE0 ? 0xA0 : byte == 0xF0 ? 0x90 : 0x80);
    U8  upper_boundary = (byte == 0xED ? 0x9F : byte == 0xF4 ? 0x8F : 0xBF);
    U32 codepoint      = byte & masks[bytes_needed];

    while (bytes_needed != 0) {
        byte = *ptr++;

        if (!(lower_boundary <= byte && byte <= upper_boundary)) {
            return result;
        } else {
            lower_boundary = 0x80;
            upper_boundary = 0xBF;
            codepoint = (codepoint << 6) | (byte & 0x3F);
            --bytes_needed;
            ++result.size;
        }
    }

    result.codepoint = codepoint;
    return result;
}

internal U64 string_encode_utf8(U8 *destination, U32 codepoint) {
    U64 size = 0;

    if (codepoint <= 0x7F) {
        destination[0] = (U8) codepoint;
        size = 1;
    } else if (codepoint <= 0x07FF) {
        destination[0] = (U8) (0xC0 | ((codepoint >> 6) & 0x1F));
        destination[1] = (U8) (0x80 | ((codepoint >> 0) & 0x3F));
        size = 2;
    } else if (codepoint <= 0xFFFF) {
        destination[0] = (U8) (0xE0 | ((codepoint >> 12) & 0x0F));
        destination[1] = (U8) (0x80 | ((codepoint >>  6) & 0x3F));
        destination[2] = (U8) (0x80 | ((codepoint >>  0) & 0x3F));
        size = 3;
    } else if (codepoint <= 0x10FFFF) {
        destination[0] = (U8) (0xF0 | ((codepoint >> 18) & 0x07));
        destination[1] = (U8) (0x80 | ((codepoint >> 12) & 0x3F));
        destination[2] = (U8) (0x80 | ((codepoint >>  6) & 0x3F));
        destination[3] = (U8) (0x80 | ((codepoint >>  0) & 0x3F));
        size = 4;
    } else {
        U32 missing_codepoint = 0xFFFD;
        destination[0] = (U8) (0xE0 | ((missing_codepoint >> 12) & 0x0F));
        destination[1] = (U8) (0x80 | ((missing_codepoint >>  6) & 0x3F));
        destination[2] = (U8) (0x80 | ((missing_codepoint >>  0) & 0x3F));
        size = 3;
    }

    return size;
}

internal StringDecode string_decode_utf16(U16 *string, U64 size) {
    StringDecode result;
    result.codepoint = 0xFFFD;
    result.size = 0;

    if (size == 0) {
        return result;
    }

    U16 code_unit = *string++;
    ++result.size;

    if (code_unit < 0xD800 || 0xDFFF < code_unit) {
        result.codepoint = code_unit;
    } else if (size >= 2) {
        U16 lead_surrogate = code_unit;
        code_unit = *string++;

        if (0xD800 <= lead_surrogate && lead_surrogate <= 0xDBFF && 0xDC00 <= code_unit && code_unit <= 0xDFFF) {
            result.codepoint = (U32) (0x10000 + ((lead_surrogate - 0xD800) << 10) + (code_unit - 0xDC00));
            ++result.size;
        }
    }

    return result;
}

internal U64 string_encode_utf16(U16 *destination, U32 codepoint) {
    U64 size = 0;

    if (codepoint < 0x10000) {
        destination[0] = (U16) codepoint;
        size = 1;
    } else {
        U32 adjusted_codepoint = codepoint - 0x10000;
        destination[0] = (U16) (0xD800 + (adjusted_codepoint >> 10));
        destination[1] = (U16) (0xDC00 + (adjusted_codepoint & 0x03FF));
        size = 2;
    }

    return size;
}

internal Str32 str32_from_str8(Arena *arena, Str8 string) {
    U64 allocated_size = string.size;
    U32 *memory = arena_push_array(arena, U32, allocated_size);

    U32 *destination_ptr = memory;
    U8 *ptr = string.data;
    U8 *opl = string.data + string.size;

    while (ptr < opl) {
        StringDecode decode = string_decode_utf8(ptr, (U64) (opl - ptr));
        *destination_ptr++ = decode.codepoint;
        ptr += decode.size;
    }

    U64 string_size = (U64) (destination_ptr - memory);
    U64 unused_size = allocated_size - string_size;
    arena_pop_amount(arena, unused_size * sizeof(*memory));

    Str32 result;
    result.data = memory;
    result.size = string_size;
    return result;
}

internal Str8 str8_from_str32(Arena *arena, Str32 string) {
    U64 allocated_size = 4 * string.size;
    U8 *memory = arena_push_array(arena, U8, allocated_size);

    U8 *destination_ptr = memory;
    U32 *ptr = string.data;
    U32 *opl = string.data + string.size;

    while (ptr < opl) {
        U32 codepoint = *ptr++;
        U64 size = string_encode_utf8(destination_ptr, codepoint);
        destination_ptr += size;
    }

    U64 string_size = (U64) (destination_ptr - memory);
    U64 unused_size = allocated_size - string_size;
    arena_pop_amount(arena, unused_size * sizeof(*memory));

    Str8 result;
    result.data = memory;
    result.size = string_size;
    return result;
}

internal Str16 str16_from_str8(Arena *arena, Str8 string) {
    // TODO: Is this atually the upper bound for memory consumption?
    U64 allocated_size = string.size;
    U16 *memory = arena_push_array(arena, U16, allocated_size);

    U16 *destination_ptr = memory;
    U8 *ptr = string.data;
    U8 *opl = string.data + string.size;

    while (ptr < opl) {
        StringDecode decode = string_decode_utf8(ptr, (U64) (opl - ptr));
        U64 encode_size = string_encode_utf16(destination_ptr, decode.codepoint);
        destination_ptr += encode_size;
        ptr += decode.size;
    }

    U64 string_size = (U64) (destination_ptr - memory);
    U64 unused_size = allocated_size - string_size;
    arena_pop_amount(arena, unused_size * sizeof(*memory));

    Str16 result;
    result.data = memory;
    result.size = string_size;
    return result;
}

internal Str8  str8_from_str16(Arena *arena, Str16 string) {
    U64 allocated_size = 3 * string.size;
    U8 *memory = arena_push_array(arena, U8, allocated_size);

    U8 *destination_ptr = memory;
    U16 *ptr = string.data;
    U16 *opl = string.data + string.size;

    while (ptr < opl) {
        StringDecode decode = string_decode_utf16(ptr, (U64) (opl - ptr));
        U64 size = string_encode_utf8(destination_ptr, decode.codepoint);
        ptr += decode.size;
        destination_ptr += size;
    }

    U64 string_size = (U64) (destination_ptr - memory);
    U64 unused_size = allocated_size - string_size;
    arena_pop_amount(arena, unused_size * sizeof(*memory));

    Str8 result;
    result.data = memory;
    result.size = string_size;
    return result;
}

internal CStr cstr_from_str8(Arena *arena, Str8 string) {
    U64 allocated_size = string.size + 1;
    U8 *memory = arena_push_array(arena, U8, allocated_size);

    memory_copy(memory, string.data, string.size);
    memory[string.size] = 0;

    return (CStr) memory;
}

internal CStr16 cstr16_from_str8(Arena *arena, Str8 string) {
    // TODO: Is this atually the upper bound for memory consumption?
    U64 allocated_size = string.size + 1;
    U16 *memory = arena_push_array(arena, U16, allocated_size);

    U16 *destination_ptr = memory;
    U8 *ptr = string.data;
    U8 *opl = string.data + string.size;

    while (ptr < opl) {
        StringDecode decode = string_decode_utf8(ptr, (U64) (opl - ptr));
        U64 encode_size = string_encode_utf16(destination_ptr, decode.codepoint);
        destination_ptr += encode_size;
        ptr += decode.size;
    }
    *destination_ptr++ = 0;

    U64 string_size = (U64) (destination_ptr - memory);
    U64 unused_size = allocated_size - string_size;
    arena_pop_amount(arena, unused_size * sizeof(*memory));

    return memory;
}
