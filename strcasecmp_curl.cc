#include <benchmark/benchmark.h>

#include "test_strings.h"

/* Mapping table to go from lowercase to uppercase for plain ASCII.*/
static const unsigned char touppermap[256] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 65,
66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
85, 86, 87, 88, 89, 90, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

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

/* Portable, consistent toupper. Do not use toupper() because its behavior is
   altered by the current locale. */
char Curl_raw_toupper(char in)
{
  return touppermap[(unsigned char) in];
}

/* Portable, consistent tolower. Do not use tolower() because its behavior is
   altered by the current locale. */
static char raw_tolower(char in)
{
  return tolowermap[(unsigned char) in];
}

int Curl_strcasecompare(const char *first, const char *second)
{
  while(*first && *second) {
    if(Curl_raw_toupper(*first) != Curl_raw_toupper(*second))
      /* get out of the loop as soon as they don't match */
      return 0;
    first++;
    second++;
  }
  /* If we're here either the strings are the same or the length is different.
     We can just test if the "current" character is non-zero for one and zero
     for the other. Note that the characters may not be exactly the same even
     if they match, we only want to compare zero-ness. */
  return !*first == !*second;
}

static void Curl_strcasecompare_bench(benchmark::State &state) {
    int r = state.range(0);
    while (state.KeepRunning()) {
        int len = Curl_strcasecompare(string1, string2[r]);
        // EXPECT_EQ(len, sizeof(string1) - 1);
        benchmark::DoNotOptimize(len);
    }
}

BENCHMARK(Curl_strcasecompare_bench)->Arg(0)->ArgName("100% same");
BENCHMARK(Curl_strcasecompare_bench)->Arg(1)->ArgName("83% same");
BENCHMARK(Curl_strcasecompare_bench)->Arg(2)->ArgName("75% same");
BENCHMARK(Curl_strcasecompare_bench)->Arg(3)->ArgName("66% same");
BENCHMARK(Curl_strcasecompare_bench)->Arg(4)->ArgName("50% same");
BENCHMARK(Curl_strcasecompare_bench)->Arg(5)->ArgName("33% same");
BENCHMARK(Curl_strcasecompare_bench)->Arg(6)->ArgName("25% same");
BENCHMARK(Curl_strcasecompare_bench)->Arg(7)->ArgName("15% same");
BENCHMARK(Curl_strcasecompare_bench)->Arg(8)->ArgName("0% same");

int Curl_strcasecompare_new(const char *first, const char *second)
{
    while(*first && *second && *first==*second) {
        first++, second++;
    }
        
  while(*first && *second) {
    if(*first != *second && Curl_raw_toupper(*first) != Curl_raw_toupper(*second))
      /* get out of the loop as soon as they don't match */
      return 0;
    first++;
    second++;
  }
  /* If we're here either the strings are the same or the length is different.
     We can just test if the "current" character is non-zero for one and zero
     for the other. Note that the characters may not be exactly the same even
     if they match, we only want to compare zero-ness. */
  return !*first == !*second;
}


static void Curl_strcasecompare_new_bench(benchmark::State &state) {
    int r = state.range(0);
    while (state.KeepRunning()) {
        int len = Curl_strcasecompare_new(string1, string2[r]);
        // EXPECT_EQ(len, sizeof(string1) - 1);
        benchmark::DoNotOptimize(len);
    }
}

BENCHMARK(Curl_strcasecompare_new_bench)->Arg(0)->ArgName("100% same");
BENCHMARK(Curl_strcasecompare_new_bench)->Arg(1)->ArgName("83% same");
BENCHMARK(Curl_strcasecompare_new_bench)->Arg(2)->ArgName("75% same");
BENCHMARK(Curl_strcasecompare_new_bench)->Arg(3)->ArgName("66% same");
BENCHMARK(Curl_strcasecompare_new_bench)->Arg(4)->ArgName("50% same");
BENCHMARK(Curl_strcasecompare_new_bench)->Arg(5)->ArgName("33% same");
BENCHMARK(Curl_strcasecompare_new_bench)->Arg(6)->ArgName("25% same");
BENCHMARK(Curl_strcasecompare_new_bench)->Arg(7)->ArgName("15% same");
BENCHMARK(Curl_strcasecompare_new_bench)->Arg(8)->ArgName("0% same");
