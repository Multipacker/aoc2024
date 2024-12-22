global Str8 day17_example = str8_literal(
    //"Register A: 729\n"
    //"Register B: 0\n"
    //"Register C: 0\n"
    //"\n"
    //"Program: 0,1,5,4,3,0\n"

    "Register A: 2024\n"
    "Register B: 0\n"
    "Register C: 0\n"
    "\n"
    "Program: 0,3,5,4,3,0\n"

);

internal U64List day17_run(Arena *arena, U64 a, U64 b, U64 c, U8 *program, U64 program_size) {
    U64List output = { 0 };

    for (U64 ip = 0; ip < program_size; ) {
        U8 opcode = program[ip++];
        U64 value = program[ip++];

        B8 is_combo[] = { 1, 0, 1, 0, 0, 1, 1, 1, };
        U64 operand[2][8] = {
            { 0, 1, 2, 3, 4, 5, 6, 7, },
            { 0, 1, 2, 3, a, b, c, 7, },
        };
        value = operand[is_combo[opcode]][value];

        switch (opcode) {
            case 0: { a  = a >> value;     } break;
            case 1: { b ^= value;          } break;
            case 2: { b  = value % 8;      } break;
            case 3: { ip = a ? value : ip; } break;
            case 4: { b ^= c;              } break;
            case 5: {
                U64Node *node = arena_push_struct_zero(arena, U64Node);
                node->value = value % 8;
                dll_push_back(output.first, output.last, node);
            } break;
            case 6: { b  = a >> value;     } break;
            case 7: { c  = a >> value;     } break;
        }
    }

    return output;
}

internal Void day17_solve(Void) {
    Arena *arena = arena_create();

    Str8 data = day17_example;
    os_file_read(arena, str8_literal("data/day17.txt"), &data);

    Str8List parts = str8_split_by_codepoints(arena, data, str8_literal(" "));
    U64 a = u64_from_str8(parts.first->next->next->string).value;
    U64 b = u64_from_str8(parts.first->next->next->next->next->string).value;
    U64 c = u64_from_str8(parts.first->next->next->next->next->next->next->string).value;
    Str8List instructions = str8_split_by_codepoints(arena, parts.last->string, str8_literal(","));
    U8 *program = arena_push_array_zero(arena, U8, instructions.node_count);
    U64 program_size = 0;
    for (Str8Node *node = instructions.first; node; node = node->next) {
        program[program_size++] = (U8) u64_from_str8(node->string).value;
    }

    U64List output = day17_run(arena, a, b, c, program, program_size);

    os_console_print(str8_literal("Part 1: "));
    for (U64Node *node = output.first; node; node = node->next) {
        os_console_print(str8_format(arena, "%lu", node->value));
        if (node->next) {
            os_console_print(str8_literal(","));
        }
    }
    os_console_print(str8_literal("\n"));

    arena_destroy(arena);
}
