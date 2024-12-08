#if COMPILER_CL
#  include <math.h>
#  include <stdlib.h>
#endif

internal U8 u8_min(U8 a, U8 b) {
    U8 result = (a < b ? a : b);
    return result;
}

internal U8 u8_max(U8 a, U8 b) {
    U8 result = (a > b ? a : b);
    return result;
}

internal U8 u8_clamp(U8 x, U8 min, U8 max) {
    U8 result = u8_min(u8_max(min, x), max);
    return result;
}

internal U8 u8_round_down_to_power_of_2(U8 value, U8 power) {
    U8 result = value & ~(power - 1);
    return result;
}

internal U8 u8_round_up_to_power_of_2(U8 value, U8 power) {
    U8 result = (value + power - 1) & ~(power - 1);
    return result;
}

internal U8 u8_floor_to_power_of_2(U8 value) {
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    return value ^ (value >> 1);
}

internal U8 u8_rotate_right(U8 x, U8 amount) {
    return (U8) ((x >> amount) | (x << (8 - amount)));
}

internal U8 u8_rotate_left(U8 x, U8 amount) {
    return (U8) ((x >> (8 - amount)) | (x << amount));
}

internal U8 u8_ceil_to_power_of_2(U8 x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    ++x;
    return x;
}

internal U8 u8_reverse(U8 x) {
    x = (U8) (((x >> 1) & 0x55) | ((x & 0x55) << 1));
    x = (U8) (((x >> 2) & 0x33) | ((x & 0x33) << 2));
    x = (U8) (((x >> 4) & 0x0F) | ((x & 0x0F) << 4));
    return x;
}



internal U16 u16_min(U16 a, U16 b) {
    U16 result = (a < b ? a : b);
    return result;
}

internal U16 u16_max(U16 a, U16 b) {
    U16 result = (a > b ? a : b);
    return result;
}

internal U16 u16_clamp(U16 x, U16 min, U16 max) {
    U16 result = u16_min(u16_max(min, x), max);
    return result;
}

internal U16 u16_round_down_to_power_of_2(U16 value, U16 power) {
    U16 result = value & ~(power - 1);
    return result;
}

internal U16 u16_round_up_to_power_of_2(U16 value, U16 power) {
    U16 result = (value + power - 1) & ~(power - 1);
    return result;
}

internal U16 u16_floor_to_power_of_2(U16 value) {
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    return value ^ (value >> 1);
}

internal U16 u16_rotate_right(U16 x, U16 amount) {
    return (U16) ((x >> amount) | (x << (16 - amount)));
}

internal U16 u16_rotate_left(U16 x, U16 amount) {
    return (U16) ((x >> (16 - amount)) | (x << amount));
}

internal U16 u16_ceil_to_power_of_2(U16 x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    ++x;
    return x;
}

internal U16 u16_reverse(U16 x) {
    x = (U16) (((x >> 1) & 0x5555) | ((x & 0x5555) << 1));
    x = (U16) (((x >> 2) & 0x3333) | ((x & 0x3333) << 2));
    x = (U16) (((x >> 4) & 0x0F0F) | ((x & 0x0F0F) << 4));
    x = (U16) (((x >> 8) & 0x00FF) | ((x & 0x00FF) << 8));
    return x;
}

internal U16 u16_big_to_local_endian(U16 x) {
#if COMPILER_CL
	return _byteswap_ushort(x);
#elif COMPILER_CLANG || COMPILER_GCC
    return __builtin_bswap16(x);
#else
# error Your compiler does not have an implementation of u16_big_to_local_endian
#endif
}



internal U32 u32_min(U32 a, U32 b) {
    U32 result = (a < b ? a : b);
    return result;
}

internal U32 u32_max(U32 a, U32 b) {
    U32 result = (a > b ? a : b);
    return result;
}

internal U32 u32_clamp(U32 x, U32 min, U32 max) {
    U32 result = u32_min(u32_max(min, x), max);
    return result;
}

internal U32 u32_round_down_to_power_of_2(U32 value, U32 power) {
    U32 result = value & ~(power - 1);
    return result;
}

internal U32 u32_round_up_to_power_of_2(U32 value, U32 power) {
    U32 result = (value + power - 1) & ~(power - 1);
    return result;
}

internal U32 u32_floor_to_power_of_2(U32 value) {
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value ^ (value >> 1);
}

internal U32 u32_rotate_right(U32 x, U32 amount) {
    return (x >> amount) | (x << (32 - amount));
}

internal U32 u32_rotate_left(U32 x, U32 amount) {
    return (x >> (32 - amount)) | (x << amount);
}

internal U32 u32_ceil_to_power_of_2(U32 x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    ++x;
    return x;
}

internal U32 u32_reverse(U32 x) {
    x = ((x >>  1) & 0x55555555) | ((x & 0x55555555) <<  1);
    x = ((x >>  2) & 0x33333333) | ((x & 0x33333333) <<  2);
    x = ((x >>  4) & 0x0F0F0F0F) | ((x & 0x0F0F0F0F) <<  4);
    x = ((x >>  8) & 0x00FF00FF) | ((x & 0x00FF00FF) <<  8);
    x = ((x >> 16) & 0x0000FFFF) | ((x & 0x0000FFFF) << 16);
    return x;
}

internal U32 u32_big_to_local_endian(U32 x) {
#if COMPILER_CL
    return _byteswap_ulong(x);
#elif COMPILER_CLANG || COMPILER_GCC
    return __builtin_bswap32(x);
#else
# error Your compiler does not have an implementation of u32_big_to_local_endian.
#endif
}



internal U64 u64_min(U64 a, U64 b) {
    U64 result = (a < b ? a : b);
    return result;
}

internal U64 u64_max(U64 a, U64 b) {
    U64 result = (a > b ? a : b);
    return result;
}

internal U64 u64_clamp(U64 x, U64 min, U64 max) {
    U64 result = u64_min(u64_max(min, x), max);
    return result;
}

internal U64 u64_round_down_to_power_of_2(U64 value, U64 power) {
    U64 result = value & ~(power - 1);
    return result;
}

internal U64 u64_round_up_to_power_of_2(U64 value, U64 power) {
    U64 result = (value + power - 1) & ~(power - 1);
    return result;
}

internal U64 u64_floor_to_power_of_2(U64 value) {
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return value ^ (value >> 1);
}

internal U64 u64_rotate_right(U64 x, U64 amount) {
    return (x >> amount) | (x << (64 - amount));
}

internal U64 u64_rotate_left(U64 x, U64 amount) {
    return (x >> (64 - amount)) | (x << amount);
}

internal U64 u64_ceil_to_power_of_2(U64 x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    ++x;
    return x;
}

internal U64 u64_reverse(U64 x) {
    x = ((x >>  1) & 0x5555555555555555) | ((x & 0x5555555555555555) <<  1);
    x = ((x >>  2) & 0x3333333333333333) | ((x & 0x3333333333333333) <<  2);
    x = ((x >>  4) & 0x0F0F0F0F0F0F0F0F) | ((x & 0x0F0F0F0F0F0F0F0F) <<  4);
    x = ((x >>  8) & 0x00FF00FF00FF00FF) | ((x & 0x00FF00FF00FF00FF) <<  8);
    x = ((x >> 16) & 0x0000FFFF0000FFFF) | ((x & 0x0000FFFF0000FFFF) << 16);
    x = ((x >> 32) & 0x00000000FFFFFFFF) | ((x & 0x00000000FFFFFFFF) << 32);
    return x;
}

internal U64 u64_big_to_local_endian(U64 x) {
#if COMPILER_CL
    return _byteswap_uint64(x);
#elif COMPILER_CLANG || COMPILER_GCC
    return __builtin_bswap64(x);
#else
# error Your compiler does not have an implementation of u64_big_to_local_endian.
#endif
}



internal S8 s8_min(S8 a, S8 b) {
    S8 result = (a < b ? a : b);
    return result;
}

internal S8 s8_max(S8 a, S8 b) {
    S8 result = (a > b ? a : b);
    return result;
}

internal S8 s8_clamp(S8 x, S8 min, S8 max) {
    S8 result = s8_min(s8_max(min, x), max);
    return result;
}

internal S8 s8_abs(S8 x) {
    S8 result = (x < 0 ? -x : x);
    return result;
}



internal S16 s16_min(S16 a, S16 b) {
    S16 result = (a < b ? a : b);
    return result;
}

internal S16 s16_max(S16 a, S16 b) {
    S16 result = (a > b ? a : b);
    return result;
}

internal S16 s16_clamp(S16 x, S16 min, S16 max) {
    S16 result = s16_min(s16_max(min, x), max);
    return result;
}

internal S16 s16_abs(S16 x) {
    S16 result = (x < 0 ? -x : x);
    return result;
}

internal S16 s16_big_to_local_endian(S16 x) {
#if COMPILER_CL
    U16 swapped = _byteswap_ushort(*(U16 *) &x);
#elif COMPILER_CLANG || COMPILER_GCC
    U16 swapped = __builtin_bswap16(*(U16 *) &x);
#else
# error Your compiler does not have an implementation of s16_big_to_local_endian.
#endif
    return *(S16 *) &swapped;
}



internal S32 s32_min(S32 a, S32 b) {
    S32 result = (a < b ? a : b);
    return result;
}

internal S32 s32_max(S32 a, S32 b) {
    S32 result = (a > b ? a : b);
    return result;
}

internal S32 s32_clamp(S32 x, S32 min, S32 max) {
    S32 result = s32_min(s32_max(min, x), max);
    return result;
}

internal S32 s32_abs(S32 x) {
    S32 result = (x < 0 ? -x : x);
    return result;
}

internal S32 s32_big_to_local_endian(S32 x) {
#if COMPILER_CL
    U32 swapped = _byteswap_ulong(*(U32 *) &x);
#elif COMPILER_CLANG || COMPILER_GCC
    U32 swapped = __builtin_bswap32(*(U32 *) &x);
#else
# error Your compiler does not have an implementation of s32_big_to_local_endian.
#endif
    return *(S32 *) &swapped;
}



internal S64 s64_min(S64 a, S64 b) {
    S64 result = (a < b ? a : b);
    return result;
}

internal S64 s64_max(S64 a, S64 b) {
    S64 result = (a > b ? a : b);
    return result;
}

internal S64 s64_clamp(S64 x, S64 min, S64 max) {
    S64 result = s64_min(s64_max(min, x), max);
    return result;
}

internal S64 s64_abs(S64 x) {
    S64 result = (x < 0 ? -x : x);
    return result;
}

internal S64 s64_big_to_local_endian(S64 x) {
#if COMPILER_CL
    U64 swapped = _byteswap_uint64(*(U64 *) &x);
#elif COMPILER_CLANG || COMPILER_GCC
    U64 swapped = __builtin_bswap64(*(U64 *) &x);
#else
# error Your compiler does not have an implementation of s64_big_to_local_endian.
#endif

    return *(S64 *) &swapped;
}



internal F32 f32_infinity(Void) {
    union {F32 f; U32 u; } result;
    result.u = 0x7F800000;
    return result.f;
}

internal F32 f32_negative_infinity(Void) {
    union {F32 f; U32 u; } result;
    result.u = 0xFF800000;
    return result.f;
}

internal F64 f64_infinity(Void) {
    union {F64 f; U64 u; } result;
    result.u = 0x7F80000000000000;
    return result.f;
}

internal F64 f64_negative_infinity(Void) {
    union {F64 f; U64 u; } result;
    result.u = 0xFF80000000000000;
    return result.f;
}

internal F32 f32_min(F32 a, F32 b) {
    F32 result = (a < b ? a : b);
    return result;
}

internal F32 f32_max(F32 a, F32 b) {
    F32 result = (a > b ? a : b);
    return result;
}

internal F32 f32_clamp(F32 x, F32 min, F32 max) {
    F32 result = f32_min(f32_max(min, x), max);
    return result;
}

internal F32 f32_sign(F32 x) {
    union {F32 f; U32 u; } result;
    result.f = x;
    result.u &= 0x80000000;
    result.u |= 0x3F800000; // Binary representation of 1.0
    return result.f;
}

internal F32 f32_abs(F32 x) {
    union {F32 f; U32 u; } result;
    result.f = x;
    result.u &= 0x7FFFFFFF;
    return result.f;
}

internal F32 f32_sqrt(F32 x) {
#if COMPILER_CL
    return sqrtf(x);
#else
    return __builtin_sqrtf(x);
#endif
}

internal F32 f32_cbrt(F32 x) {
#if COMPILER_CL
    return cbrtf(x);
#else
    return __builtin_cbrtf(x);
#endif
}

internal F32 f32_sin(F32 x) {
#if COMPILER_CL
    return sinf(x);
#else
    return __builtin_sinf(x);
#endif
}

internal F32 f32_cos(F32 x) {
#if COMPILER_CL
    return cosf(x);
#else
    return __builtin_cosf(x);
#endif
}

internal F32 f32_tan(F32 x) {
#if COMPILER_CL
    return tanf(x);
#else
    return __builtin_tanf(x);
#endif
}

internal F32 f32_arctan(F32 x) {
#if COMPILER_CL
    return atanf(x);
#else
    return __builtin_atanf(x);
#endif
}

internal F32 f32_arctan2(F32 y, F32 x) {
#if COMPILER_CL
    return atan2f(y, x);
#else
    return __builtin_atan2f(y, x);
#endif
}

internal F32 f32_ln(F32 x) {
#if COMPILER_CL
    return logf(x);
#else
    return __builtin_logf(x);
#endif
}

internal F32 f32_log2(F32 x) {
#if COMPILER_CL
    return log2f(x);
#else
    return __builtin_log2f(x);
#endif
}

internal F32 f32_log(F32 x) {
#if COMPILER_CL
    return log10f(x);
#else
    return __builtin_log10f(x);
#endif
}

internal F32 f32_lerp(F32 a, F32 b, F32 t) {
    F32 x = a + (b - a) * t;
    return x;
}

internal F32 f32_unlerp(F32 a, F32 b, F32 x) {
    F32 t = 0.0f;
    if (a != b) {
        t = (x - a) / (b - a);
    }
    return t;
}

internal F32 f32_pow(F32 a, F32 b) {
#if COMPILER_CL
    return powf(a, b);
#else
    return __builtin_powf(a, b);
#endif
}

internal F32 f32_floor(F32 x) {
#if COMPILER_CL
    return floorf(x);
#else
    return __builtin_floorf(x);
#endif
}

internal F32 f32_ceil(F32 x) {
#if COMPILER_CL
    return ceilf(x);
#else
    return __builtin_ceilf(x);
#endif
}

internal F32 f32_mod(F32 x, F32 y) {
    F32 integral = 0.0f;
#if COMPILER_CL
    F32 fraction = modff(x / y, &integral);
#else
    F32 fraction = __builtin_modff(x / y, &integral);
#endif
    F32 result = fraction * y;
    return result;
}

internal F32 f32_round(F32 x) {
#if COMPILER_CL
    return roundf(x);
#else
    return __builtin_roundf(x);
#endif
}

internal U32 f32_round_to_u32(F32 x) {
#if COMPILER_CL
    return (U32) roundf(x);
#else
    return (U32) __builtin_roundf(x);
#endif
}

internal S32 f32_round_to_s32(F32 x) {
#if COMPILER_CL
    return (S32) roundf(x);
#else
    return (S32) __builtin_roundf(x);
#endif
}

internal U32 f32_solve_cubic(F32 a, F32 b, F32 c, F32 d, F32 *result_xs) {
    F32 cos120 = -0.5f;
    F32 sin120 = 0.866025404f;
    U32 n = 0;

    if (f32_abs(d) < F32_EPSILON) {
        // first solution is x = 0
        result_xs[n++] = 0.0f;
        // divide all terms by x, converting to quadratic equation
        d = c;
        c = b;
        b = a;
        a = 0.0f;
    }

    if (f32_abs(a) < F32_EPSILON) {
        if (f32_abs(b) < F32_EPSILON) {
            // linear equation
            if (f32_abs(c) > F32_EPSILON) {
                result_xs[n++] = -d / c;
            }
        } else {
            // quadratic equation
            F32 yy = c * c - 4.0f * b * d;
            if (yy >= 0) {
                F32 inv2b = 1.0f / (2.0f * b);
                F32 y = f32_sqrt(yy);
                result_xs[n++] = (-c + y) * inv2b;
                result_xs[n++] = (-c - y) * inv2b;
            }
        }
    } else {
        // cubic equation
        F32 inva = 1.0f / a;
        F32 invaa = inva * inva;
        F32 bb = b * b;
        F32 bover3a = b * (1.0f / 3.0f) * inva;
        F32 p = (3.0f * a * c - bb) * (1.0f / 3.0f) * invaa;
        F32 halfq = (2.0f * bb * b - 9.0f * a * b * c + 27.0f * a * a  * d) * (0.5f / 27.0f) * invaa * inva;
        F32 yy = p * p * p / 27.0f + halfq * halfq;
        if (yy > F32_EPSILON) {
            // sqrt is positive: one real solution
            F32 y = f32_sqrt(yy);
            F32 uuu = -halfq + y;
            F32 vvv = -halfq - y;
            F32 www = f32_abs(uuu) > f32_abs(vvv) ? uuu : vvv;
            F32 w = (www < 0.0f) ? -f32_cbrt(-www) : f32_cbrt(www);
            result_xs[n++] = w - p / (3.0f * w) - bover3a;
        } else if (yy < -F32_EPSILON) {
            // sqrt is negative: three real solutions
            F32 x = -halfq;
            F32 y = f32_sqrt(-yy);

            // convert to polar form
            F32 theta = f32_arctan2(y, x);
            F32 r = f32_sqrt(x * x - yy);

            // calc cube root
            theta /= 3.0f;
            r = f32_cbrt(r);

            // convert to complex coordinate
            F32 ux  = f32_cos(theta) * r;
            F32 uyi = f32_sin(theta) * r;

            // first solution
            result_xs[n++] = ux + ux - bover3a;
            // second solution, rotate +120 degrees
            result_xs[n++] = 2 * (ux * cos120 - uyi * sin120) - bover3a;
            // third solution, rotate -120 degrees
            result_xs[n++] = 2 * (ux * cos120 + uyi * sin120) - bover3a;
        } else {
            // sqrt is zero: two real solutions
            F32 www = -halfq;
            F32 w = (www < 0) ? -f32_cbrt(-www) : f32_cbrt(www);
            // first solution
            result_xs[n++] = w + w - bover3a;
            // second solution, rotate +120 degrees
            result_xs[n++] = 2 * w * cos120 - bover3a;
        }
    }
    return n;
}



internal F64 f64_min(F64 a, F64 b) {
    F64 result = (a < b ? a : b);
    return result;
}

internal F64 f64_max(F64 a, F64 b) {
    F64 result = (a > b ? a : b);
    return result;
}

internal F64 f64_clamp(F64 x, F64 min, F64 max) {
    F64 result = f64_min(f64_max(min, x), max);
    return result;
}

internal F64 f64_abs(F64 x) {
    union {F64 f; U64 u; } result;
    result.f = x;
    result.u &= 0x7FFFFFFFFFFFFFFF;
    return result.f;
}

internal F64 f64_sqrt(F64 x) {
#if COMPILER_CL
    return sqrt(x);
#else
    return __builtin_sqrt(x);
#endif
}

internal F64 f64_sin(F64 x) {
#if COMPILER_CL
    return sin(x);
#else
    return __builtin_sin(x);
#endif
}

internal F64 f64_cos(F64 x) {
#if COMPILER_CL
    return cos(x);
#else
    return __builtin_cos(x);
#endif
}

internal F64 f64_tan(F64 x) {
#if COMPILER_CL
    return tan(x);
#else
    return __builtin_tan(x);
#endif
}

internal F64 f64_ln(F64 x) {
#if COMPILER_CL
    return log(x);
#else
    return __builtin_log(x);
#endif
}

internal F64 f64_lg(F64 x) {
#if COMPILER_CL
    return log10(x);
#else
    return __builtin_log10(x);
#endif
}

internal F64 f64_lerp(F64 a, F64 b, F64 t) {
    F64 x = a + (b - a) * t;
    return x;
}

internal F64 f64_unlerp(F64 a, F64 b, F64 x) {
    F64 t = 0.0;
    if (a != b) {
        t = (x - a) / (b - a);
    }
    return t;
}

internal F64 f64_pow(F64 a, F64 b) {
#if COMPILER_CL
    return pow(a, b);
#else
    return __builtin_pow(a, b);
#endif
}

internal F64 f64_floor(F64 x) {
#if COMPILER_CL
    return floor(x);
#else
    return __builtin_floor(x);
#endif
}

internal F64 f64_ceil(F64 x) {
#if COMPILER_CL
    return ceil(x);
#else
    return __builtin_ceil(x);
#endif
}

internal F64 f64_mod(F64 x, F64 y) {
    F64 integral = 0.0;
#if COMPILER_CL
    F64 fraction = modf(x / y, &integral);
#else
    F64 fraction = __builtin_modf(x / y, &integral);
#endif
    F64 result = fraction * y;
    return result;
}



internal DenseTime dense_time_from_date_time(DateTime *date_time) {
    DenseTime result = 0;
    result += (U32) ((S32) date_time->year + 0x8000);
    result *= 12;
    result += date_time->month;
    result *= 31;
    result += date_time->day;
    result *= 24;
    result += date_time->hour;
    result *= 60;
    result += date_time->minute;
    result *= 61;
    result += date_time->second;
    result *= 1000;
    result += date_time->millisecond;
    return result;
}

internal DateTime date_time_from_dense_time(DenseTime dense_time) {
    DateTime result;
    result.millisecond = dense_time % 1000;
    dense_time /= 1000;
    result.second = dense_time % 61;
    dense_time /= 61;
    result.minute = dense_time % 60;
    dense_time /= 60;
    result.hour = dense_time % 24;
    dense_time /= 24;
    result.day = dense_time % 31;
    dense_time /= 31;
    result.month = dense_time % 12;
    dense_time /= 12;
    result.year = (S16) ((S32) dense_time - 0x8000);
    return result;
}
