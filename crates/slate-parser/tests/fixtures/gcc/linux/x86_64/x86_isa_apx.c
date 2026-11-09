#ifdef __APX_F__
present__APX_F__
#endif
#ifdef __APX_INLINE_ASM_USE_GPR32__
present__APX_INLINE_ASM_USE_GPR32__
#endif
#ifdef __EGPR__
present__EGPR__
#endif
#ifdef __PUSH2POP2__
present__PUSH2POP2__
#endif
#ifdef __PPX__
present__PPX__
#endif
#ifdef __NDD__
present__NDD__
#endif
#ifdef __CCMP__
present__CCMP__
#endif
#ifdef __NF__
present__NF__
#endif
#ifdef __CF__
present__CF__
#endif
#ifdef __ZU__
present__ZU__
#endif
#ifdef __JMPABS__
present__JMPABS__
#endif

// SLATE-FILECHECK-DEFINES APXF
// SLATE-FILECHECK-PREFIX-ARGS APXF -mapxf -mapx-features=none -mvzeroupper
// SLATE-FILECHECK-PP APXF
// SLATE-FILECHECK-DEFINES NOAPXF
// SLATE-FILECHECK-PREFIX-ARGS NOAPXF -mapxf -mno-apxf -mno-vzeroupper
// SLATE-FILECHECK-PP NOAPXF
// SLATE-FILECHECK-DEFINES FEATURES
// SLATE-FILECHECK-PREFIX-ARGS FEATURES -mapx-features=egpr,ndd -mapx-inline-asm-use-gpr32
// SLATE-FILECHECK-PP FEATURES
// SLATE-FILECHECK-DEFINES GPR32OFF
// SLATE-FILECHECK-PREFIX-ARGS GPR32OFF -mapx-inline-asm-use-gpr32 -mno-apx-inline-asm-use-gpr32
// SLATE-FILECHECK-PP GPR32OFF

// SLATE-FILECHECK-BEGIN APXF
// APXF: present__APX_F__
// SLATE-FILECHECK-END APXF
// SLATE-FILECHECK-BEGIN NOAPXF
// SLATE-FILECHECK-END NOAPXF
// SLATE-FILECHECK-BEGIN FEATURES
// FEATURES: present__APX_INLINE_ASM_USE_GPR32__
// SLATE-FILECHECK-END FEATURES
// SLATE-FILECHECK-BEGIN GPR32OFF
// SLATE-FILECHECK-END GPR32OFF
