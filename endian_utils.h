#ifndef ENDIAN_UTILS_H
#define ENDIAN_UTILS_H

// compile-time detection of target endianness
constexpr bool is_little_endian = (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);
// why: the algorithm assumes little-endian data layout
static_assert(is_little_endian, "Kangaroo requires little-endian architecture.");

#endif // ENDIAN_UTILS_H
