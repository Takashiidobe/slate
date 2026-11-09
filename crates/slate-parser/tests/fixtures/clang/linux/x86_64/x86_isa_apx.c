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
// SLATE-FILECHECK-PREFIX-ARGS APXF -mapxf -mvzeroupper
// SLATE-FILECHECK-PP APXF
// SLATE-FILECHECK-DEFINES NOAPXF
// SLATE-FILECHECK-PREFIX-ARGS NOAPXF -march=diamondrapids -mno-apxf
// SLATE-FILECHECK-PP NOAPXF
// SLATE-FILECHECK-DEFINES DMR
// SLATE-FILECHECK-PREFIX-ARGS DMR -march=diamondrapids -mno-apx-features=ndd -mapx-features=cf
// SLATE-FILECHECK-PP DMR
// SLATE-FILECHECK-DEFINES FEATURES
// SLATE-FILECHECK-PREFIX-ARGS FEATURES -mapx-features=egpr,,ndd -mapx-inline-asm-use-gpr32 -mno-vzeroupper
// SLATE-FILECHECK-PP FEATURES
// SLATE-FILECHECK-DEFINES NOEGPR
// SLATE-FILECHECK-PREFIX-ARGS NOEGPR -mapx-features=egpr -mno-apxf -mapx-inline-asm-use-gpr32
// SLATE-FILECHECK-PP NOEGPR

// SLATE-FILECHECK-BEGIN APXF
// APXF: present__APX_F__
// APXF-NEXT: present__EGPR__
// APXF-NEXT: present__PUSH2POP2__
// APXF-NEXT: present__PPX__
// APXF-NEXT: present__NDD__
// APXF-NEXT: present__CCMP__
// APXF-NEXT: present__NF__
// APXF-NEXT: present__ZU__
// APXF-NEXT: present__JMPABS__
// SLATE-FILECHECK-END APXF
// SLATE-FILECHECK-BEGIN NOAPXF
// SLATE-FILECHECK-END NOAPXF
// SLATE-FILECHECK-BEGIN DMR
// DMR: present__EGPR__
// DMR-NEXT: present__PUSH2POP2__
// DMR-NEXT: present__PPX__
// DMR-NEXT: present__CCMP__
// DMR-NEXT: present__NF__
// DMR-NEXT: present__CF__
// DMR-NEXT: present__ZU__
// DMR-NEXT: present__JMPABS__
// SLATE-FILECHECK-END DMR
// SLATE-FILECHECK-BEGIN FEATURES
// FEATURES: present__APX_INLINE_ASM_USE_GPR32__
// FEATURES-NEXT: present__EGPR__
// FEATURES-NEXT: present__NDD__
// SLATE-FILECHECK-END FEATURES
// SLATE-FILECHECK-BEGIN NOEGPR
// SLATE-FILECHECK-END NOEGPR
