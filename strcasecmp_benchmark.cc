#include <benchmark/benchmark.h>

/* From linux kernel: tools/include/linux/ctype.h */

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

const unsigned char _ctype[] = {
_C,_C,_C,_C,_C,_C,_C,_C,				/* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,			/* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,				/* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,				/* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,				/* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,				/* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,				/* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,				/* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,		/* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,				/* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,				/* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,				/* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,		/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,				/* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,				/* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,				/* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			/* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			/* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,	/* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,	/* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,	/* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,	/* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,	/* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};	/* 240-255 */

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isupper(c)	((__ismask(c)&(_U)) != 0)

static inline unsigned char __tolower(unsigned char c)
{
	if (isupper(c))
		c -= 'A'-'a';
	return c;
}

#define tolower(c) __tolower(c)

const char string1[256] =
    "ilcpsklryvmcpjnbpbwllsrehfmxrkecwitrsglrexvtjmxypunbqfgxmuvgfajclfvenhyuhuorjosamibdnjdbeyhkbsombltouujdrbwcrrcgbflqpottpegrwvgajcrgwdlpgitydvhedtusippyvxsuvbvfenodqasajoyomgsqcpjlhbmdahyviuemkssdsldebesnnngpesdntrrvysuipywatpfoelthrowhfexlwdysvspwlkfblfd";
const char string2[9][256] = {
    /* 100% same */
    "ilcpsklryvmcpjnbpbwllsrehfmxrkecwitrsglrexvtjmxypunbqfgxmuvgfajclfvenhyuhuorjosamibdnjdbeyhkbsombltouujdrbwcrrcgbflqpottpegrwvgajcrgwdlpgitydvhedtusippyvxsuvbvfenodqasajoyomgsqcpjlhbmdahyviuemkssdsldebesnnngpesdntrrvysuipywatpfoelthrowhfexlwdysvspwlkfblfd",
    /* 83% same */
    "IlcpsKlryvMcpjnBpbwlLsrehFmxrkEcwitRsglrExvtjMxypuNbqfgXmuvgFajclFvenhYuhuoRjosaMibdnJdbeyHkbsoMbltoUujdrBwcrrCgbflQpottPegrwVgajcRgwdlPgityDvhedTusipPyvxsUvbvfEnodqAsajoYomgsQcpjlHbmdaHyviuEmkssDsldeBesnnNgpesDntrrVysuiPywatPfoelThrowHfexlWdysvSpwlkFblfd",
    /* 75% same */
    "IlcpSklrYvmcPjnbPbwlLsreHfmxRkecWitrSglrExvtJmxyPunbQfgxMuvgFajcLfveNhyuHuorJosaMibdNjdbEyhkBsomBltoUujdRbwcRrcgBflqPottPegrWvgaJcrgWdlpGityDvheDtusIppyVxsuVbvfEnodQasaJoyoMgsqCpjlHbmdAhyvIuemKssdSldeBesnNngpEsdnTrrvYsuiPywaTpfoElthRowhFexlWdysVspwLkfbLfd",
    /* 66% same */
    "IlcPskLryVmcPjnBpbWllSreHfmXrkEcwItrSglRexVtjMxyPunBqfGxmUvgFajClfVenHyuHuoRjoSamIbdNjdBeyHkbSomBltOuuJdrBwcRrcGbfLqpOttPegRwvGajCrgWdlPgiTydVheDtuSipPyvXsuVbvFenOdqAsaJoyOmgSqcPjlHbmDahYviUemKssDslDebEsnNngPesDntRrvYsuIpyWatPfoEltHroWhfExlWdySvsPwlKfbLfd",
    /* 50% same*/
    "iLcPsKlRyVmCpJnBpBwLlSrEhFmXrKeCwItRsGlReXvTjMxYpUnBqFgXmUvGfAjClFvEnHyUhUoRjOsAmIbDnJdBeYhKbSoMbLtOuUjDrBwCrRcGbFlQpOtTpEgRwVgAjCrGwDlPgItYdVhEdTuSiPpYvXsUvBvFeNoDqAsAjOyOmGsQcPjLhBmDaHyViUeMkSsDsLdEbEsNnNgPeSdNtRrVySuIpYwAtPfOeLtHrOwHfExLwDySvSpWlKfBlFd",
    /* 33% same */
    "iLCpSKlRYvMCpJNbPBwLLsREhFMxRKeCWiTRsGLrEXvTJmXYpUNbQFgXMuVGfAJcLFvENhYUhUOrJOsAMiBDnJDbEYhKBsOMbLToUUjDRbWCrRCgBFlQPoTTpEGrWVgAJcRGwDLpGItYDvHEdTUsIPpYVxSUvBVfENoDQaSAjOYoMGsQCpJLhBMdAHyVIuEMkSSdSLdEBeSNnNGpESdNTrRVySUiPYwATpFOeLThROwHFeXLwDYsVSpWLkFBlFD",
    /* 25% same */
    "iLCPsKLRyVMCpJNBpBWLlSREhFMXrKECwITRsGLReXVTjMXYpUNBqFGXmUVGfAJClFVEnHYUhUORjOSAmIBDnJDBeYHKbSOMbLTOuUJDrBWCrRCGbFLQpOTTpEGRwVGAjCRGwDLPgITYdVHEdTUSiPPYvXSUvBVFeNODqASAjOYOmGSQcPJLhBMDaHYViUEMkSSDsLDEbESNnNGPeSDNtRRVySUIpYWAtPFOeLTHrOWHfEXLwDYSvSPWlKFBlFD",
    /* 15% same */
    "iLCPSKLrYVMCPJnBPBWLLsREHFMXrKECWITrSGLREXvTJMXYPuNBQFGXmUVGFAJcLFVENHyUHUORJoSAMIBDnJDBEYHkBSOMBLtOUUJDRbWCRRCGbFLQPOTtPEGRWVgAJCRGWdLPGITYdVHEDTUsIPPYVXsUVBVFEnODQASAjOYOMGSqCPJLHBmDAHYVIuEMKSSDsLDEBESnNNGPESdNTRRVYsUIPYWAtPFOELThROWHFExLWDYSVsPWLKFBlFD",
    /* 0% same */
    "ILCPSKLRYVMCPJNBPBWLLSREHFMXRKECWITRSGLREXVTJMXYPUNBQFGXMUVGFAJCLFVENHYUHUORJOSAMIBDNJDBEYHKBSOMBLTOUUJDRBWCRRCGBFLQPOTTPEGRWVGAJCRGWDLPGITYDVHEDTUSIPPYVXSUVBVFENODQASAJOYOMGSQCPJLHBMDAHYVIUEMKSSDSLDEBESNNNGPESDNTRRVYSUIPYWATPFOELTHROWHFEXLWDYSVSPWLKFBLFD"
};

/* From linux kernel: lib/string.c */

int strcasecmp(const char *s1, const char *s2)
{
	int c1, c2;

	do {
		c1 = tolower(*s1++);
		c2 = tolower(*s2++);
	} while (c1 == c2 && c1 != 0);
	return c1 - c2;
}

static void strcasecmp_bench(benchmark::State& state) {
    int r = state.range(0);
    while (state.KeepRunning()) {
        int len = strcasecmp(string1, string2[r]);
        //EXPECT_EQ(len, sizeof(string1) - 1);
        benchmark::DoNotOptimize(len);
    }
}

BENCHMARK(strcasecmp_bench)->Arg(0)->ArgName("100% same");
BENCHMARK(strcasecmp_bench)->Arg(1)->ArgName("83% same");
BENCHMARK(strcasecmp_bench)->Arg(2)->ArgName("75% same");
BENCHMARK(strcasecmp_bench)->Arg(3)->ArgName("66% same");
BENCHMARK(strcasecmp_bench)->Arg(4)->ArgName("50% same");
BENCHMARK(strcasecmp_bench)->Arg(5)->ArgName("33% same");
BENCHMARK(strcasecmp_bench)->Arg(6)->ArgName("25% same");
BENCHMARK(strcasecmp_bench)->Arg(7)->ArgName("15% same");
BENCHMARK(strcasecmp_bench)->Arg(8)->ArgName("0% same");

/* New proposal for strcasecmp */

int strcasecmp_new(const char *s1, const char *s2)
{
	/* Yes, Virginia, it had better be unsigned */
	unsigned char c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;
		if (c1 == c2)
			continue;
        c1 = tolower(c1);
        c2 = tolower(c2);
        if (c1 != c2)
            return (int)c1 - (int)c2;
	} while (c1 != 0);
	return 0;
}

static void strcasecmp_new_bench(benchmark::State& state) {
    int r = state.range(0);
    while (state.KeepRunning()) {
        int len = strcasecmp_new(string1, string2[r]);
        //EXPECT_EQ(len, sizeof(string1) - 1);
        benchmark::DoNotOptimize(len);
    }
}

BENCHMARK(strcasecmp_new_bench)->Arg(0)->ArgName("100% same");
BENCHMARK(strcasecmp_new_bench)->Arg(1)->ArgName("83% same");
BENCHMARK(strcasecmp_new_bench)->Arg(2)->ArgName("75% same");
BENCHMARK(strcasecmp_new_bench)->Arg(3)->ArgName("66% same");
BENCHMARK(strcasecmp_new_bench)->Arg(4)->ArgName("50% same");
BENCHMARK(strcasecmp_new_bench)->Arg(5)->ArgName("33% same");
BENCHMARK(strcasecmp_new_bench)->Arg(6)->ArgName("25% same");
BENCHMARK(strcasecmp_new_bench)->Arg(7)->ArgName("15% same");
BENCHMARK(strcasecmp_new_bench)->Arg(8)->ArgName("0% same");


/* Google benchmark entry point */

int main(int argc, char** argv)
{
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}