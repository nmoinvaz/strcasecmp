
const char string1[256] =
    "ilcpsklryvmcpjnbpbwllsrehfmxrkecwitrsglrexvtjmxypunbqfgxmuvgfajclfvenhyuhuorjosamibdnjdbeyhkbsombltouujdrbwcrrcgbf"
    "lqpottpegrwvgajcrgwdlpgitydvhedtusippyvxsuvbvfenodqasajoyomgsqcpjlhbmdahyviuemkssdsldebesnnngpesdntrrvysuipywatpfo"
    "elthrowhfexlwdysvspwlkfblfd";
const char string2[9][256] = {
    /* 100% same */
    "ilcpsklryvmcpjnbpbwllsrehfmxrkecwitrsglrexvtjmxypunbqfgxmuvgfajclfvenhyuhuorjosamibdnjdbeyhkbsombltouujdrbwcrrcgbf"
    "lqpottpegrwvgajcrgwdlpgitydvhedtusippyvxsuvbvfenodqasajoyomgsqcpjlhbmdahyviuemkssdsldebesnnngpesdntrrvysuipywatpfo"
    "elthrowhfexlwdysvspwlkfblfd",
    /* 83% same */
    "IlcpsKlryvMcpjnBpbwlLsrehFmxrkEcwitRsglrExvtjMxypuNbqfgXmuvgFajclFvenhYuhuoRjosaMibdnJdbeyHkbsoMbltoUujdrBwcrrCgbf"
    "lQpottPegrwVgajcRgwdlPgityDvhedTusipPyvxsUvbvfEnodqAsajoYomgsQcpjlHbmdaHyviuEmkssDsldeBesnnNgpesDntrrVysuiPywatPfo"
    "elThrowHfexlWdysvSpwlkFblfd",
    /* 75% same */
    "IlcpSklrYvmcPjnbPbwlLsreHfmxRkecWitrSglrExvtJmxyPunbQfgxMuvgFajcLfveNhyuHuorJosaMibdNjdbEyhkBsomBltoUujdRbwcRrcgBf"
    "lqPottPegrWvgaJcrgWdlpGityDvheDtusIppyVxsuVbvfEnodQasaJoyoMgsqCpjlHbmdAhyvIuemKssdSldeBesnNngpEsdnTrrvYsuiPywaTpfo"
    "ElthRowhFexlWdysVspwLkfbLfd",
    /* 66% same */
    "IlcPskLryVmcPjnBpbWllSreHfmXrkEcwItrSglRexVtjMxyPunBqfGxmUvgFajClfVenHyuHuoRjoSamIbdNjdBeyHkbSomBltOuuJdrBwcRrcGbf"
    "LqpOttPegRwvGajCrgWdlPgiTydVheDtuSipPyvXsuVbvFenOdqAsaJoyOmgSqcPjlHbmDahYviUemKssDslDebEsnNngPesDntRrvYsuIpyWatPfo"
    "EltHroWhfExlWdySvsPwlKfbLfd",
    /* 50% same*/
    "iLcPsKlRyVmCpJnBpBwLlSrEhFmXrKeCwItRsGlReXvTjMxYpUnBqFgXmUvGfAjClFvEnHyUhUoRjOsAmIbDnJdBeYhKbSoMbLtOuUjDrBwCrRcGbF"
    "lQpOtTpEgRwVgAjCrGwDlPgItYdVhEdTuSiPpYvXsUvBvFeNoDqAsAjOyOmGsQcPjLhBmDaHyViUeMkSsDsLdEbEsNnNgPeSdNtRrVySuIpYwAtPfO"
    "eLtHrOwHfExLwDySvSpWlKfBlFd",
    /* 33% same */
    "iLCpSKlRYvMCpJNbPBwLLsREhFMxRKeCWiTRsGLrEXvTJmXYpUNbQFgXMuVGfAJcLFvENhYUhUOrJOsAMiBDnJDbEYhKBsOMbLToUUjDRbWCrRCgBF"
    "lQPoTTpEGrWVgAJcRGwDLpGItYDvHEdTUsIPpYVxSUvBVfENoDQaSAjOYoMGsQCpJLhBMdAHyVIuEMkSSdSLdEBeSNnNGpESdNTrRVySUiPYwATpFO"
    "eLThROwHFeXLwDYsVSpWLkFBlFD",
    /* 25% same */
    "iLCPsKLRyVMCpJNBpBWLlSREhFMXrKECwITRsGLReXVTjMXYpUNBqFGXmUVGfAJClFVEnHYUhUORjOSAmIBDnJDBeYHKbSOMbLTOuUJDrBWCrRCGbF"
    "LQpOTTpEGRwVGAjCRGwDLPgITYdVHEdTUSiPPYvXSUvBVFeNODqASAjOYOmGSQcPJLhBMDaHYViUEMkSSDsLDEbESNnNGPeSDNtRRVySUIpYWAtPFO"
    "eLTHrOWHfEXLwDYSvSPWlKFBlFD",
    /* 15% same */
    "iLCPSKLrYVMCPJnBPBWLLsREHFMXrKECWITrSGLREXvTJMXYPuNBQFGXmUVGFAJcLFVENHyUHUORJoSAMIBDnJDBEYHkBSOMBLtOUUJDRbWCRRCGbF"
    "LQPOTtPEGRWVgAJCRGWdLPGITYdVHEDTUsIPPYVXsUVBVFEnODQASAjOYOMGSqCPJLHBmDAHYVIuEMKSSDsLDEBESnNNGPESdNTRRVYsUIPYWAtPFO"
    "ELThROWHFExLWDYSVsPWLKFBlFD",
    /* 0% same */
    "ILCPSKLRYVMCPJNBPBWLLSREHFMXRKECWITRSGLREXVTJMXYPUNBQFGXMUVGFAJCLFVENHYUHUORJOSAMIBDNJDBEYHKBSOMBLTOUUJDRBWCRRCGBF"
    "LQPOTTPEGRWVGAJCRGWDLPGITYDVHEDTUSIPPYVXSUVBVFENODQASAJOYOMGSQCPJLHBMDAHYVIUEMKSSDSLDEBESNNNGPESDNTRRVYSUIPYWATPFO"
    "ELTHROWHFEXLWDYSVSPWLKFBLFD"};
