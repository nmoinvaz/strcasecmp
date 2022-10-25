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
## Our implementation

Partially derived from Linux kernel implementation.
```c
int strcasecmp_new(const char *s1, const char *s2)
{
    unsigned char c1, c2;

    do {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 == c2)
            continue;
        c1 = tolower(c1);
        c2 = tolower(c2);
        if (c1 == c2)
            return (int)c1 - (int)c2;
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

two strings 100% same case = ~50% improvement
```
strcasecmp_bench/100% same:0_mean              349 ns          132 ns           20
strcasecmp_bench/100% same:0_median            315 ns          132 ns           20
strcasecmp_bench/100% same:0_stddev           49.8 ns         50.2 ns           20
strcasecmp_bench/100% same:0_cv              14.26 %         38.03 %            20

strcasecmp_new_bench/100% same:0_mean          180 ns         75.2 ns           20
strcasecmp_new_bench/100% same:0_median        174 ns         71.9 ns           20
strcasecmp_new_bench/100% same:0_stddev       15.1 ns         42.1 ns           20
strcasecmp_new_bench/100% same:0_cv           8.39 %         55.98 %            20
```
two strings 83% same case = ~33% improvement
```
strcasecmp_bench/83% same:1_mean               340 ns          154 ns           20
strcasecmp_bench/83% same:1_median             332 ns          212 ns           20
strcasecmp_bench/83% same:1_stddev            22.5 ns          115 ns           20
strcasecmp_bench/83% same:1_cv                6.62 %         74.68 %            20

strcasecmp_new_bench/83% same:1_mean           226 ns         95.5 ns           20
strcasecmp_new_bench/83% same:1_median         221 ns          123 ns           20
strcasecmp_new_bench/83% same:1_stddev        10.7 ns         75.9 ns           20
strcasecmp_new_bench/83% same:1_cv            4.75 %         79.41 %            20
```
two strings 75% same case = ~35% improvement
```
strcasecmp_bench/75% same:2_mean               366 ns          144 ns           20
strcasecmp_bench/75% same:2_median             360 ns          132 ns           20
strcasecmp_bench/75% same:2_stddev            29.3 ns          109 ns           20
strcasecmp_bench/75% same:2_cv                7.99 %         75.73 %            20

strcasecmp_new_bench/75% same:2_mean           235 ns          104 ns           20
strcasecmp_new_bench/75% same:2_median         228 ns          104 ns           20
strcasecmp_new_bench/75% same:2_stddev        16.7 ns         59.3 ns           20
strcasecmp_new_bench/75% same:2_cv            7.11 %         56.97 %            20
```
two strings 66% same case = ~25% improvement
```
strcasecmp_bench/66% same:3_mean               350 ns          166 ns           20
strcasecmp_bench/66% same:3_median             348 ns          210 ns           20
strcasecmp_bench/66% same:3_stddev            5.78 ns          109 ns           20
strcasecmp_bench/66% same:3_cv                1.65 %         65.48 %            20

strcasecmp_new_bench/66% same:3_mean           261 ns          112 ns           20
strcasecmp_new_bench/66% same:3_median         256 ns          110 ns           20
strcasecmp_new_bench/66% same:3_stddev        10.0 ns         7.57 ns           20
strcasecmp_new_bench/66% same:3_cv            3.84 %          6.76 %            20
```
two strings 50% same case = ~29% improvement
```
strcasecmp_bench/50% same:4_mean               400 ns          171 ns           20
strcasecmp_bench/50% same:4_median             394 ns          166 ns           20
strcasecmp_bench/50% same:4_stddev            15.5 ns         34.4 ns           20
strcasecmp_bench/50% same:4_cv                3.88 %         20.13 %            20

strcasecmp_new_bench/50% same:4_mean           281 ns          133 ns           20
strcasecmp_new_bench/50% same:4_median         280 ns          184 ns           20
strcasecmp_new_bench/50% same:4_stddev        7.61 ns          106 ns           20
strcasecmp_new_bench/50% same:4_cv            2.71 %         79.55 %            20
```
two strings 33% same case = ~7% improvement
```
strcasecmp_bench/33% same:5_mean               329 ns          138 ns           20
strcasecmp_bench/33% same:5_median             324 ns          109 ns           20
strcasecmp_bench/33% same:5_stddev            16.3 ns         50.2 ns           20
strcasecmp_bench/33% same:5_cv                4.96 %         36.48 %            20

strcasecmp_new_bench/33% same:5_mean           306 ns          137 ns           20
strcasecmp_new_bench/33% same:5_median         296 ns          131 ns           20
strcasecmp_new_bench/33% same:5_stddev        24.1 ns         93.4 ns           20
strcasecmp_new_bench/33% same:5_cv            7.87 %         68.32 %            20
```
two strings 25% same case = ~3% decrease
```
strcasecmp_bench/25% same:6_mean               319 ns          141 ns           20
strcasecmp_bench/25% same:6_median             316 ns          184 ns           20
strcasecmp_bench/25% same:6_stddev            19.0 ns          103 ns           20
strcasecmp_bench/25% same:6_cv                5.97 %         72.65 %            20

strcasecmp_new_bench/25% same:6_mean           329 ns          140 ns           20
strcasecmp_new_bench/25% same:6_median         325 ns          137 ns           20
strcasecmp_new_bench/25% same:6_stddev        18.8 ns         17.6 ns           20
strcasecmp_new_bench/25% same:6_cv            5.71 %         12.64 %            20
```
two strings 15% same case = ~6% decrease
```
strcasecmp_bench/15% same:7_mean               331 ns          139 ns           20
strcasecmp_bench/15% same:7_median             323 ns          138 ns           20
strcasecmp_bench/15% same:7_stddev            21.6 ns         45.9 ns           20
strcasecmp_bench/15% same:7_cv                6.54 %         33.03 %            20

strcasecmp_new_bench/15% same:7_mean           351 ns          140 ns           20
strcasecmp_new_bench/15% same:7_median         342 ns          146 ns           20
strcasecmp_new_bench/15% same:7_stddev        18.5 ns         35.3 ns           20
strcasecmp_new_bench/15% same:7_cv            5.28 %         25.14 %            20
```
two strings 0% same case = ~25% decrease
```
strcasecmp_bench/0% same:8_mean                319 ns          136 ns           20
strcasecmp_bench/0% same:8_median              318 ns          140 ns           20
strcasecmp_bench/0% same:8_stddev             6.90 ns         99.0 ns           20
strcasecmp_bench/0% same:8_cv                 2.16 %         72.78 %            20

strcasecmp_new_bench/0% same:8_mean            397 ns          154 ns           20
strcasecmp_new_bench/0% same:8_median          381 ns          157 ns           20
strcasecmp_new_bench/0% same:8_stddev         27.0 ns         31.8 ns           20
strcasecmp_new_bench/0% same:8_cv             6.79 %         20.71 %            20
```

## Conclusion

For strings that are over 30% the same case our implementation provides up to 50% performance benefits over other implementations. However if the two strings are less than 30% there can be a similar up to 25% performance decrement.