# strcasecmp

Benchmark for `strcasecmp` to determine whether or not there is a better implementation.

Most implementations will convert each character to `tolower` without checking to see if the characters are first the same. If the characters are the same then no lowercase conversion is needed.

This code uses Google Benchmark to perform tests against several 256 character strings of varying similarity.

## Popular implementations

**glibc**
```c
int
__strcasecmp (const char *s1, const char *s2 LOCALE_PARAM)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    int result;

    if (p1 == p2)
        return 0;

    while ((result = TOLOWER (*p1) - TOLOWER (*p2++)) == 0)
        if (*p1++ == '\0')
        break;

    return result;
}
```
https://github.com/bminor/glibc/blob/b92a49359f33a461db080a33940d73f47c756126/string/strcasecmp.c

The check for `p1 == p2` can add an additional benefit for user-mode applications that are compiled with string pooling enabled.

**Apple**

```c
int
strcasecmp(s1, s2)
	const char *s1, *s2;
{
    const u_char
            *us1 = (const u_char *)s1,
            *us2 = (const u_char *)s2;

    while (tolower(*us1) == tolower(*us2++))
        if (*us1++ == '\0')
            return (0);
    return (tolower(*us1) - tolower(*--us2));
}

https://opensource.apple.com/source/Libc/Libc-583/string/FreeBSD/strcasecmp.c.auto.html

```

**OpenSSL**

```c
int OPENSSL_strncasecmp(const char *s1, const char *s2, size_t n)
{
    int t;
    size_t i;

    for (i = 0; i < n; i++)
        if ((t = ossl_tolower(*s1) - ossl_tolower(*s2++)) != 0)
            return t;
        else if (*s1++ == '\0')
            return 0;
    return 0;
}
```

https://github.com/openssl/openssl/blob/79c8dcf3985a7b75eac8e53eb8652728af6c5d3d/crypto/o_str.c

**Linux Kernel**

```c
int strcasecmp(const char *s1, const char *s2)
{
    int c1, c2;

    do {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
    } while (c1 == c2 && c1 != 0);
    return c1 - c2;
}
```

https://github.com/torvalds/linux/blob/master/lib/string.c

Interestingly enough the kernel's implementations for `strcasecmp` and `strncasecmp` will return different negative values for `string1 = "", string2 = "A"` because the quick return in `strncasecmp` when `!c1 || !c2`. In this case `strcasecmp = -'a'` and `strncasecmp = '-A'`. But perhaps it is left up to the implementation what negative value to return.

## Our implementation

Partially derived from Linux kernel implementations for `strcasecmp` and `strncasecmp`.
```c
int strcasecmp_new(const char *s1, const char *s2)
{
    unsigned char c1, c2;

    do {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2) {
            c1 = tolower(c1);
            c2 = tolower(c2);
            if (c1 != c2)
                return (int)c1 - (int)c2;
        }
    } while (c1 != 0);
    return 0;
}
```

## Build

CMake instructions:
```
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
## Running

The application allows most Google Benchmark arguments including `--benchmark_repetitions=#` which can specify the number of repetitions to perform.

## Results

```
2022-10-25T08:48:13-07:00
Running 20 repetitions
Run on (12 X 3202.02 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 256 KiB (x6)
  L3 Unified 12288 KiB (x1)
```

two strings 100% same case = ~48% improvement
```
strcasecmp_bench/100% same:0_mean              300 ns          164 ns           20
strcasecmp_bench/100% same:0_median            291 ns          229 ns           20
strcasecmp_bench/100% same:0_stddev           23.5 ns          120 ns           20
strcasecmp_bench/100% same:0_cv               7.84 %         73.54 %            20

strcasecmp_new_bench/100% same:0_mean          158 ns         89.8 ns           20
strcasecmp_new_bench/100% same:0_median        159 ns          102 ns           20
strcasecmp_new_bench/100% same:0_stddev       2.66 ns         53.5 ns           20
strcasecmp_new_bench/100% same:0_cv           1.69 %         59.58 %            20
```
two strings 83% same case = ~32% improvement
```
strcasecmp_bench/83% same:1_mean               304 ns          173 ns           20
strcasecmp_bench/83% same:1_median             304 ns          220 ns           20
strcasecmp_bench/83% same:1_stddev            6.58 ns          123 ns           20
strcasecmp_bench/83% same:1_cv                2.16 %         70.95 %            20

strcasecmp_new_bench/83% same:1_mean           204 ns          121 ns           20
strcasecmp_new_bench/83% same:1_median         203 ns          120 ns           20
strcasecmp_new_bench/83% same:1_stddev        3.48 ns         73.3 ns           20
strcasecmp_new_bench/83% same:1_cv            1.71 %         60.41 %            20
```
two strings 75% same case = ~36% improvement
```
strcasecmp_bench/75% same:2_mean               321 ns          183 ns           20
strcasecmp_bench/75% same:2_median             322 ns          237 ns           20
strcasecmp_bench/75% same:2_stddev            5.37 ns          129 ns           20
strcasecmp_bench/75% same:2_cv                1.67 %         70.76 %            20

strcasecmp_new_bench/75% same:2_mean           205 ns          127 ns           20
strcasecmp_new_bench/75% same:2_median         205 ns          173 ns           20
strcasecmp_new_bench/75% same:2_stddev        4.52 ns         81.0 ns           20
strcasecmp_new_bench/75% same:2_cv            2.21 %         63.68 %            20
```
two strings 66% same case = ~32% improvement
```
strcasecmp_bench/66% same:3_mean               351 ns          197 ns           20
strcasecmp_bench/66% same:3_median             348 ns          199 ns           20
strcasecmp_bench/66% same:3_stddev            22.8 ns         37.4 ns           20
strcasecmp_bench/66% same:3_cv                6.50 %         18.92 %            20

strcasecmp_new_bench/66% same:3_mean           236 ns          138 ns           20
strcasecmp_new_bench/66% same:3_median         232 ns          141 ns           20
strcasecmp_new_bench/66% same:3_stddev        10.9 ns         57.4 ns           20
strcasecmp_new_bench/66% same:3_cv            4.60 %         41.62 %            20
```
two strings 50% same case = ~32% improvement
```
strcasecmp_bench/50% same:4_mean               384 ns          217 ns           20
strcasecmp_bench/50% same:4_median             371 ns          220 ns           20
strcasecmp_bench/50% same:4_stddev            27.9 ns         21.2 ns           20
strcasecmp_bench/50% same:4_cv                7.26 %          9.77 %            20

strcasecmp_new_bench/50% same:4_mean           259 ns          147 ns           20
strcasecmp_new_bench/50% same:4_median         259 ns          175 ns           20
strcasecmp_new_bench/50% same:4_stddev        4.08 ns          102 ns           20
strcasecmp_new_bench/50% same:4_cv            1.57 %         69.35 %            20
```
two strings 33% same case = ~13% improvement
```
strcasecmp_bench/33% same:5_mean               303 ns          169 ns           20
strcasecmp_bench/33% same:5_median             299 ns          159 ns           20
strcasecmp_bench/33% same:5_stddev            15.7 ns         54.2 ns           20
strcasecmp_bench/33% same:5_cv                5.17 %         32.01 %            20

strcasecmp_new_bench/33% same:5_mean           263 ns          156 ns           20
strcasecmp_new_bench/33% same:5_median         264 ns          166 ns           20
strcasecmp_new_bench/33% same:5_stddev        14.7 ns         64.0 ns           20
strcasecmp_new_bench/33% same:5_cv            5.57 %         41.05 %            20
```
two strings 25% same case = ~5% improvement
```
strcasecmp_bench/25% same:6_mean               287 ns          163 ns           20
strcasecmp_bench/25% same:6_median             287 ns          161 ns           20
strcasecmp_bench/25% same:6_stddev            2.99 ns         59.9 ns           20
strcasecmp_bench/25% same:6_cv                1.04 %         36.73 %            20

strcasecmp_new_bench/25% same:6_mean           269 ns          159 ns           20
strcasecmp_new_bench/25% same:6_median         268 ns          158 ns           20
strcasecmp_new_bench/25% same:6_stddev        3.46 ns         8.69 ns           20
strcasecmp_new_bench/25% same:6_cv            1.29 %          5.47 %            20
```
two strings 15% same case = ~2% decrease
```
strcasecmp_bench/15% same:7_mean               292 ns          168 ns           20
strcasecmp_bench/15% same:7_median             291 ns          169 ns           20
strcasecmp_bench/15% same:7_stddev            2.76 ns         14.9 ns           20
strcasecmp_bench/15% same:7_cv                0.95 %          8.83 %            20

strcasecmp_new_bench/15% same:7_mean           299 ns          161 ns           20
strcasecmp_new_bench/15% same:7_median         300 ns          139 ns           20
strcasecmp_new_bench/15% same:7_stddev        6.81 ns          122 ns           20
strcasecmp_new_bench/15% same:7_cv            2.28 %         75.59 %            20
```
two strings 0% same case = ~11% decrease
```
strcasecmp_bench/0% same:8_mean                297 ns          155 ns           20
strcasecmp_bench/0% same:8_median              293 ns          193 ns           20
strcasecmp_bench/0% same:8_stddev             14.1 ns          118 ns           20
strcasecmp_bench/0% same:8_cv                 4.75 %         76.46 %            20

strcasecmp_new_bench/0% same:8_mean            332 ns          209 ns           20
strcasecmp_new_bench/0% same:8_median          331 ns          242 ns           20
strcasecmp_new_bench/0% same:8_stddev         4.96 ns          111 ns           20
strcasecmp_new_bench/0% same:8_cv             1.50 %         52.89 %            20
```

## Conclusion

For strings that are over 25% the same case our implementation provides up to 48% performance benefits over other implementations. However if the two strings are less than 25% there can be a similar up to 11% performance decrease.

We surmise that perhaps CPU caching does not play a significant role in string case comparison, but only branches and perdictions. `tolower` has its own branch to determine if the character is uppercase or lowercase which results in a lookup to a hash map. If we can reduce the total number of branches, by skipping an extra branch when characters match, we can speed up the function.
