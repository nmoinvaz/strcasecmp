#include "test_strings.h"

#include <benchmark/benchmark.h>

/* Linux kernel way */

#define _U  0x01 /* upper */
#define _L  0x02 /* lower */
#define _D  0x04 /* digit */
#define _C  0x08 /* cntrl */
#define _P  0x10 /* punct */
#define _S  0x20 /* white space (space/lf/tab) */
#define _X  0x40 /* hex digit */
#define _SP 0x80 /* hard space (0x20) */

const unsigned char _ctype[] = {
    _C,       _C,      _C,      _C,      _C,      _C,      _C,      _C,                                  /* 0-7 */
    _C,       _C | _S, _C | _S, _C | _S, _C | _S, _C | _S, _C,      _C,                                  /* 8-15 */
    _C,       _C,      _C,      _C,      _C,      _C,      _C,      _C,                                  /* 16-23 */
    _C,       _C,      _C,      _C,      _C,      _C,      _C,      _C,                                  /* 24-31 */
    _S | _SP, _P,      _P,      _P,      _P,      _P,      _P,      _P,                                  /* 32-39 */
    _P,       _P,      _P,      _P,      _P,      _P,      _P,      _P,                                  /* 40-47 */
    _D,       _D,      _D,      _D,      _D,      _D,      _D,      _D,                                  /* 48-55 */
    _D,       _D,      _P,      _P,      _P,      _P,      _P,      _P,                                  /* 56-63 */
    _P,       _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U,                                  /* 64-71 */
    _U,       _U,      _U,      _U,      _U,      _U,      _U,      _U,                                  /* 72-79 */
    _U,       _U,      _U,      _U,      _U,      _U,      _U,      _U,                                  /* 80-87 */
    _U,       _U,      _U,      _P,      _P,      _P,      _P,      _P,                                  /* 88-95 */
    _P,       _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L,                                  /* 96-103 */
    _L,       _L,      _L,      _L,      _L,      _L,      _L,      _L,                                  /* 104-111 */
    _L,       _L,      _L,      _L,      _L,      _L,      _L,      _L,                                  /* 112-119 */
    _L,       _L,      _L,      _P,      _P,      _P,      _P,      _C,                                  /* 120-127 */
    0,        0,       0,       0,       0,       0,       0,       0,  0,  0,  0,  0,  0,  0,  0,  0,   /* 128-143 */
    0,        0,       0,       0,       0,       0,       0,       0,  0,  0,  0,  0,  0,  0,  0,  0,   /* 144-159 */
    _S | _SP, _P,      _P,      _P,      _P,      _P,      _P,      _P, _P, _P, _P, _P, _P, _P, _P, _P,  /* 160-175 */
    _P,       _P,      _P,      _P,      _P,      _P,      _P,      _P, _P, _P, _P, _P, _P, _P, _P, _P,  /* 176-191 */
    _U,       _U,      _U,      _U,      _U,      _U,      _U,      _U, _U, _U, _U, _U, _U, _U, _U, _U,  /* 192-207 */
    _U,       _U,      _U,      _U,      _U,      _U,      _U,      _P, _U, _U, _U, _U, _U, _U, _U, _L,  /* 208-223 */
    _L,       _L,      _L,      _L,      _L,      _L,      _L,      _L, _L, _L, _L, _L, _L, _L, _L, _L,  /* 224-239 */
    _L,       _L,      _L,      _L,      _L,      _L,      _L,      _P, _L, _L, _L, _L, _L, _L, _L, _L}; /* 240-255 */

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isupper(c)  ((__ismask(c) & (_U)) != 0)

static inline unsigned char __tolower(unsigned char c) {
    if (isupper(c))
        c -= 'A' - 'a';
    return c;
}

int strcasecmp_map_ctype(const char *s1, const char *s2) {
    unsigned char c1, c2;

    do {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2) {
            c1 = __tolower(c1);
            c2 = __tolower(c2);
            if (c1 != c2)
                return (int)c1 - (int)c2;
        }
    } while (c1 != 0);
    return 0;
}

static void strcasecmp_map_ctype_bench(benchmark::State &state) {
    int r = state.range(0);
    while (state.KeepRunning()) {
        int len = strcasecmp_map_ctype(string1, string2[r]);
        benchmark::DoNotOptimize(len);
    }
}

BENCHMARK(strcasecmp_map_ctype_bench)->Arg(0)->ArgName("100% same");
BENCHMARK(strcasecmp_map_ctype_bench)->Arg(2)->ArgName("75% same");
BENCHMARK(strcasecmp_map_ctype_bench)->Arg(3)->ArgName("66% same");
BENCHMARK(strcasecmp_map_ctype_bench)->Arg(4)->ArgName("50% same");
BENCHMARK(strcasecmp_map_ctype_bench)->Arg(6)->ArgName("25% same");
BENCHMARK(strcasecmp_map_ctype_bench)->Arg(7)->ArgName("15% same");
BENCHMARK(strcasecmp_map_ctype_bench)->Arg(8)->ArgName("0% same");

//// Android/CURL & other way

/* Mapping table to go from uppercase to lowercase for plain ASCII.*/
static const unsigned char tolowermap[256] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95,
96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

int strcasecmp_map_tolower_if(const char *s1, const char *s2) {
    unsigned char c1, c2;

    do {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2) {
            c1 = tolowermap[c1];
            c2 = tolowermap[c2];
            if (c1 != c2)
                return (int)c1 - (int)c2;
        }
    } while (c1 != 0);
    return 0;
}

static void strcasecmp_map_tolower_if_bench(benchmark::State &state) {
    int r = state.range(0);
    while (state.KeepRunning()) {
        int len = strcasecmp_map_tolower_if(string1, string2[r]);
        benchmark::DoNotOptimize(len);
    }
}

BENCHMARK(strcasecmp_map_tolower_if_bench)->Arg(0)->ArgName("100% same");
BENCHMARK(strcasecmp_map_tolower_if_bench)->Arg(2)->ArgName("75% same");
BENCHMARK(strcasecmp_map_tolower_if_bench)->Arg(3)->ArgName("66% same");
BENCHMARK(strcasecmp_map_tolower_if_bench)->Arg(4)->ArgName("50% same");
BENCHMARK(strcasecmp_map_tolower_if_bench)->Arg(6)->ArgName("25% same");
BENCHMARK(strcasecmp_map_tolower_if_bench)->Arg(7)->ArgName("15% same");
BENCHMARK(strcasecmp_map_tolower_if_bench)->Arg(8)->ArgName("0% same");

int strcasecmp_map_tolower_no_if(const char *s1, const char *s2) {
    unsigned char c1, c2;

    do {
        c1 = tolowermap[*s1++];
        c2 = tolowermap[*s2++];
    } while (c1 == c2 && c1 != 0);
    return (int)c1 - (int)c2;
}

static void strcasecmp_map_tolower_no_if_bench(benchmark::State &state) {
    int r = state.range(0);
    while (state.KeepRunning()) {
        int len = strcasecmp_map_tolower_no_if(string1, string2[r]);
        benchmark::DoNotOptimize(len);
    }
}

BENCHMARK(strcasecmp_map_tolower_no_if_bench)->Arg(0)->ArgName("100% same");
BENCHMARK(strcasecmp_map_tolower_no_if_bench)->Arg(2)->ArgName("75% same");
BENCHMARK(strcasecmp_map_tolower_no_if_bench)->Arg(3)->ArgName("66% same");
BENCHMARK(strcasecmp_map_tolower_no_if_bench)->Arg(4)->ArgName("50% same");
BENCHMARK(strcasecmp_map_tolower_no_if_bench)->Arg(6)->ArgName("25% same");
BENCHMARK(strcasecmp_map_tolower_no_if_bench)->Arg(7)->ArgName("15% same");
BENCHMARK(strcasecmp_map_tolower_no_if_bench)->Arg(8)->ArgName("0% same");
