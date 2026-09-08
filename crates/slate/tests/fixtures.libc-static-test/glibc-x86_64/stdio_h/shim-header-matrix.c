#include <stdio.h>

extern int slate_oracle___asprintf(char **restrict, const char *restrict, ...);
extern long slate_oracle___getdelim(char **restrict, unsigned long *restrict, int, struct _IO_FILE *restrict);
extern int slate_oracle___overflow(struct _IO_FILE *, int);
extern int slate_oracle___uflow(struct _IO_FILE *);
extern int slate_oracle_asprintf(char **restrict, const char *restrict, ...);
extern void slate_oracle_clearerr(struct _IO_FILE *);
extern void slate_oracle_clearerr_unlocked(struct _IO_FILE *);
extern char * slate_oracle_ctermid(char *);
extern char * slate_oracle_cuserid(char *);
extern int slate_oracle_dprintf(int, const char *restrict, ...);
extern int slate_oracle_fclose(struct _IO_FILE *);
extern int slate_oracle_fcloseall(void);
extern struct _IO_FILE * slate_oracle_fdopen(int, const char *);
extern int slate_oracle_feof(struct _IO_FILE *);
extern int slate_oracle_feof_unlocked(struct _IO_FILE *);
extern int slate_oracle_ferror(struct _IO_FILE *);
extern int slate_oracle_ferror_unlocked(struct _IO_FILE *);
extern int slate_oracle_fflush(struct _IO_FILE *);
extern int slate_oracle_fflush_unlocked(struct _IO_FILE *);
extern int slate_oracle_fgetc(struct _IO_FILE *);
extern int slate_oracle_fgetc_unlocked(struct _IO_FILE *);
extern int slate_oracle_fgetpos(struct _IO_FILE *restrict, struct _G_fpos_t *restrict);
extern int slate_oracle_fgetpos64(struct _IO_FILE *restrict, struct _G_fpos64_t *restrict);
extern char * slate_oracle_fgets(char *restrict, int, struct _IO_FILE *restrict);
extern char * slate_oracle_fgets_unlocked(char *restrict, int, struct _IO_FILE *restrict);
extern int slate_oracle_fileno(struct _IO_FILE *);
extern int slate_oracle_fileno_unlocked(struct _IO_FILE *);
extern void slate_oracle_flockfile(struct _IO_FILE *);
extern struct _IO_FILE * slate_oracle_fmemopen(void *, unsigned long, const char *);
extern struct _IO_FILE * slate_oracle_fopen(const char *, const char *);
extern struct _IO_FILE * slate_oracle_fopen64(const char *restrict, const char *restrict);
extern struct _IO_FILE * slate_oracle_fopencookie(void *restrict, const char *restrict, struct _IO_cookie_io_functions_t);
extern int slate_oracle_fprintf(struct _IO_FILE *restrict, const char *restrict, ...);
extern int slate_oracle_fputc(int, struct _IO_FILE *);
extern int slate_oracle_fputc_unlocked(int, struct _IO_FILE *);
extern int slate_oracle_fputs(const char *restrict, struct _IO_FILE *restrict);
extern int slate_oracle_fputs_unlocked(const char *restrict, struct _IO_FILE *restrict);
extern __size_t slate_oracle_fread(void *, __size_t, __size_t, struct _IO_FILE *);
extern unsigned long slate_oracle_fread_unlocked(void *restrict, unsigned long, unsigned long, struct _IO_FILE *restrict);
extern struct _IO_FILE * slate_oracle_freopen(const char *restrict, const char *restrict, struct _IO_FILE *restrict);
extern struct _IO_FILE * slate_oracle_freopen64(const char *restrict, const char *restrict, struct _IO_FILE *restrict);
extern int slate_oracle_fscanf(struct _IO_FILE *restrict, const char *restrict, ...);
extern int slate_oracle_fseek(struct _IO_FILE *, long, int);
extern int slate_oracle_fseeko(struct _IO_FILE *, long, int);
extern int slate_oracle_fseeko64(struct _IO_FILE *, long, int);
extern int slate_oracle_fsetpos(struct _IO_FILE *, const struct _G_fpos_t *);
extern int slate_oracle_fsetpos64(struct _IO_FILE *, const struct _G_fpos64_t *);
extern long slate_oracle_ftell(struct _IO_FILE *);
extern long slate_oracle_ftello(struct _IO_FILE *);
extern long slate_oracle_ftello64(struct _IO_FILE *);
extern int slate_oracle_ftrylockfile(struct _IO_FILE *);
extern void slate_oracle_funlockfile(struct _IO_FILE *);
extern __size_t slate_oracle_fwrite(const void *, __size_t, __size_t, struct _IO_FILE *);
extern unsigned long slate_oracle_fwrite_unlocked(const void *restrict, unsigned long, unsigned long, struct _IO_FILE *restrict);
extern int slate_oracle_getc(struct _IO_FILE *);
extern int slate_oracle_getc_unlocked(struct _IO_FILE *);
extern int slate_oracle_getchar(void);
extern int slate_oracle_getchar_unlocked(void);
extern long slate_oracle_getdelim(char **restrict, unsigned long *restrict, int, struct _IO_FILE *restrict);
extern long slate_oracle_getline(char **restrict, unsigned long *restrict, struct _IO_FILE *restrict);
extern int slate_oracle_getw(struct _IO_FILE *);
extern int slate_oracle_obstack_printf(struct obstack *restrict, const char *restrict, ...);
extern int slate_oracle_obstack_vprintf(struct obstack *restrict, const char *restrict, struct __va_list_tag *);
extern struct _IO_FILE * slate_oracle_open_memstream(char **, unsigned long *);
extern int slate_oracle_pclose(struct _IO_FILE *);
extern void slate_oracle_perror(const char *);
extern struct _IO_FILE * slate_oracle_popen(const char *, const char *);
extern int slate_oracle_printf(const char *, ...);
extern int slate_oracle_putc(int, struct _IO_FILE *);
extern int slate_oracle_putc_unlocked(int, struct _IO_FILE *);
extern int slate_oracle_putchar(int);
extern int slate_oracle_putchar_unlocked(int);
extern int slate_oracle_puts(const char *);
extern int slate_oracle_putw(int, struct _IO_FILE *);
extern int slate_oracle_remove(const char *);
extern int slate_oracle_rename(const char *, const char *);
extern int slate_oracle_renameat(int, const char *, int, const char *);
extern int slate_oracle_renameat2(int, const char *, int, const char *, unsigned int);
extern void slate_oracle_rewind(struct _IO_FILE *);
extern int slate_oracle_scanf(const char *restrict, ...);
extern void slate_oracle_setbuf(struct _IO_FILE *restrict, char *restrict);
extern void slate_oracle_setbuffer(struct _IO_FILE *restrict, char *restrict, unsigned long);
extern void slate_oracle_setlinebuf(struct _IO_FILE *);
extern int slate_oracle_setvbuf(struct _IO_FILE *restrict, char *restrict, int, unsigned long);
extern int slate_oracle_snprintf(char *restrict, __size_t, const char *restrict, ...);
extern int slate_oracle_sprintf(char *restrict, const char *restrict, ...);
extern int slate_oracle_sscanf(const char *restrict, const char *restrict, ...);
extern char * slate_oracle_tempnam(const char *, const char *);
extern struct _IO_FILE * slate_oracle_tmpfile(void);
extern struct _IO_FILE * slate_oracle_tmpfile64(void);
extern char * slate_oracle_tmpnam(char *);
extern char * slate_oracle_tmpnam_r(char *);
extern int slate_oracle_ungetc(int, struct _IO_FILE *);
extern int slate_oracle_vasprintf(char **restrict, const char *restrict, struct __va_list_tag *);
extern int slate_oracle_vdprintf(int, const char *restrict, struct __va_list_tag *);
extern int slate_oracle_vfprintf(struct _IO_FILE *restrict, const char *restrict, struct __va_list_tag *);
extern int slate_oracle_vfscanf(struct _IO_FILE *restrict, const char *restrict, struct __va_list_tag *);
extern int slate_oracle_vprintf(const char *restrict, struct __va_list_tag *);
extern int slate_oracle_vscanf(const char *restrict, struct __va_list_tag *);
extern int slate_oracle_vsnprintf(char *restrict, __size_t, const char *restrict, struct __va_list_tag *);
extern int slate_oracle_vsprintf(char *restrict, const char *restrict, struct __va_list_tag *);
extern int slate_oracle_vsscanf(const char *restrict, const char *restrict, struct __va_list_tag *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___asprintf), __typeof__(__asprintf)),
    "stdio.h:__asprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___getdelim), __typeof__(__getdelim)),
    "stdio.h:__getdelim declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___overflow), __typeof__(__overflow)),
    "stdio.h:__overflow declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___uflow), __typeof__(__uflow)),
    "stdio.h:__uflow declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asprintf), __typeof__(asprintf)),
    "stdio.h:asprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clearerr), __typeof__(clearerr)),
    "stdio.h:clearerr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clearerr_unlocked), __typeof__(clearerr_unlocked)),
    "stdio.h:clearerr_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ctermid), __typeof__(ctermid)),
    "stdio.h:ctermid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cuserid), __typeof__(cuserid)),
    "stdio.h:cuserid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_dprintf), __typeof__(dprintf)),
    "stdio.h:dprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fclose), __typeof__(fclose)),
    "stdio.h:fclose declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fcloseall), __typeof__(fcloseall)),
    "stdio.h:fcloseall declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdopen), __typeof__(fdopen)),
    "stdio.h:fdopen declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feof), __typeof__(feof)),
    "stdio.h:feof declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feof_unlocked), __typeof__(feof_unlocked)),
    "stdio.h:feof_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ferror), __typeof__(ferror)),
    "stdio.h:ferror declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ferror_unlocked), __typeof__(ferror_unlocked)),
    "stdio.h:ferror_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fflush), __typeof__(fflush)),
    "stdio.h:fflush declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fflush_unlocked), __typeof__(fflush_unlocked)),
    "stdio.h:fflush_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fgetc), __typeof__(fgetc)),
    "stdio.h:fgetc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fgetc_unlocked), __typeof__(fgetc_unlocked)),
    "stdio.h:fgetc_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fgetpos), __typeof__(fgetpos)),
    "stdio.h:fgetpos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fgetpos64), __typeof__(fgetpos64)),
    "stdio.h:fgetpos64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fgets), __typeof__(fgets)),
    "stdio.h:fgets declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fgets_unlocked), __typeof__(fgets_unlocked)),
    "stdio.h:fgets_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fileno), __typeof__(fileno)),
    "stdio.h:fileno declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fileno_unlocked), __typeof__(fileno_unlocked)),
    "stdio.h:fileno_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_flockfile), __typeof__(flockfile)),
    "stdio.h:flockfile declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmemopen), __typeof__(fmemopen)),
    "stdio.h:fmemopen declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fopen), __typeof__(fopen)),
    "stdio.h:fopen declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fopen64), __typeof__(fopen64)),
    "stdio.h:fopen64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fopencookie), __typeof__(fopencookie)),
    "stdio.h:fopencookie declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fprintf), __typeof__(fprintf)),
    "stdio.h:fprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fputc), __typeof__(fputc)),
    "stdio.h:fputc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fputc_unlocked), __typeof__(fputc_unlocked)),
    "stdio.h:fputc_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fputs), __typeof__(fputs)),
    "stdio.h:fputs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fputs_unlocked), __typeof__(fputs_unlocked)),
    "stdio.h:fputs_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fread), __typeof__(fread)),
    "stdio.h:fread declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fread_unlocked), __typeof__(fread_unlocked)),
    "stdio.h:fread_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_freopen), __typeof__(freopen)),
    "stdio.h:freopen declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_freopen64), __typeof__(freopen64)),
    "stdio.h:freopen64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fscanf), __typeof__(fscanf)),
    "stdio.h:fscanf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fseek), __typeof__(fseek)),
    "stdio.h:fseek declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fseeko), __typeof__(fseeko)),
    "stdio.h:fseeko declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fseeko64), __typeof__(fseeko64)),
    "stdio.h:fseeko64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fsetpos), __typeof__(fsetpos)),
    "stdio.h:fsetpos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fsetpos64), __typeof__(fsetpos64)),
    "stdio.h:fsetpos64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ftell), __typeof__(ftell)),
    "stdio.h:ftell declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ftello), __typeof__(ftello)),
    "stdio.h:ftello declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ftello64), __typeof__(ftello64)),
    "stdio.h:ftello64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ftrylockfile), __typeof__(ftrylockfile)),
    "stdio.h:ftrylockfile declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_funlockfile), __typeof__(funlockfile)),
    "stdio.h:funlockfile declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fwrite), __typeof__(fwrite)),
    "stdio.h:fwrite declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fwrite_unlocked), __typeof__(fwrite_unlocked)),
    "stdio.h:fwrite_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getc), __typeof__(getc)),
    "stdio.h:getc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getc_unlocked), __typeof__(getc_unlocked)),
    "stdio.h:getc_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getchar), __typeof__(getchar)),
    "stdio.h:getchar declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getchar_unlocked), __typeof__(getchar_unlocked)),
    "stdio.h:getchar_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getdelim), __typeof__(getdelim)),
    "stdio.h:getdelim declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getline), __typeof__(getline)),
    "stdio.h:getline declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getw), __typeof__(getw)),
    "stdio.h:getw declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_obstack_printf), __typeof__(obstack_printf)),
    "stdio.h:obstack_printf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_obstack_vprintf), __typeof__(obstack_vprintf)),
    "stdio.h:obstack_vprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_open_memstream), __typeof__(open_memstream)),
    "stdio.h:open_memstream declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pclose), __typeof__(pclose)),
    "stdio.h:pclose declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_perror), __typeof__(perror)),
    "stdio.h:perror declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_popen), __typeof__(popen)),
    "stdio.h:popen declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_printf), __typeof__(printf)),
    "stdio.h:printf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_putc), __typeof__(putc)),
    "stdio.h:putc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_putc_unlocked), __typeof__(putc_unlocked)),
    "stdio.h:putc_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_putchar), __typeof__(putchar)),
    "stdio.h:putchar declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_putchar_unlocked), __typeof__(putchar_unlocked)),
    "stdio.h:putchar_unlocked declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_puts), __typeof__(puts)),
    "stdio.h:puts declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_putw), __typeof__(putw)),
    "stdio.h:putw declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remove), __typeof__(remove)),
    "stdio.h:remove declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rename), __typeof__(rename)),
    "stdio.h:rename declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_renameat), __typeof__(renameat)),
    "stdio.h:renameat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_renameat2), __typeof__(renameat2)),
    "stdio.h:renameat2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rewind), __typeof__(rewind)),
    "stdio.h:rewind declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scanf), __typeof__(scanf)),
    "stdio.h:scanf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setbuf), __typeof__(setbuf)),
    "stdio.h:setbuf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setbuffer), __typeof__(setbuffer)),
    "stdio.h:setbuffer declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setlinebuf), __typeof__(setlinebuf)),
    "stdio.h:setlinebuf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setvbuf), __typeof__(setvbuf)),
    "stdio.h:setvbuf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_snprintf), __typeof__(snprintf)),
    "stdio.h:snprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sprintf), __typeof__(sprintf)),
    "stdio.h:sprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sscanf), __typeof__(sscanf)),
    "stdio.h:sscanf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tempnam), __typeof__(tempnam)),
    "stdio.h:tempnam declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tmpfile), __typeof__(tmpfile)),
    "stdio.h:tmpfile declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tmpfile64), __typeof__(tmpfile64)),
    "stdio.h:tmpfile64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tmpnam), __typeof__(tmpnam)),
    "stdio.h:tmpnam declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tmpnam_r), __typeof__(tmpnam_r)),
    "stdio.h:tmpnam_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ungetc), __typeof__(ungetc)),
    "stdio.h:ungetc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vasprintf), __typeof__(vasprintf)),
    "stdio.h:vasprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vdprintf), __typeof__(vdprintf)),
    "stdio.h:vdprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vfprintf), __typeof__(vfprintf)),
    "stdio.h:vfprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vfscanf), __typeof__(vfscanf)),
    "stdio.h:vfscanf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vprintf), __typeof__(vprintf)),
    "stdio.h:vprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vscanf), __typeof__(vscanf)),
    "stdio.h:vscanf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vsnprintf), __typeof__(vsnprintf)),
    "stdio.h:vsnprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vsprintf), __typeof__(vsprintf)),
    "stdio.h:vsprintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vsscanf), __typeof__(vsscanf)),
    "stdio.h:vsscanf declaration differs from oracle");

static __typeof__(__asprintf) *const slate_reference___asprintf = &__asprintf;
static __typeof__(__getdelim) *const slate_reference___getdelim = &__getdelim;
static __typeof__(__overflow) *const slate_reference___overflow = &__overflow;
static __typeof__(__uflow) *const slate_reference___uflow = &__uflow;
static __typeof__(asprintf) *const slate_reference_asprintf = &asprintf;
static __typeof__(clearerr) *const slate_reference_clearerr = &clearerr;
static __typeof__(clearerr_unlocked) *const slate_reference_clearerr_unlocked = &clearerr_unlocked;
static __typeof__(ctermid) *const slate_reference_ctermid = &ctermid;
static __typeof__(cuserid) *const slate_reference_cuserid = &cuserid;
static __typeof__(dprintf) *const slate_reference_dprintf = &dprintf;
static __typeof__(fclose) *const slate_reference_fclose = &fclose;
static __typeof__(fcloseall) *const slate_reference_fcloseall = &fcloseall;
static __typeof__(fdopen) *const slate_reference_fdopen = &fdopen;
static __typeof__(feof) *const slate_reference_feof = &feof;
static __typeof__(feof_unlocked) *const slate_reference_feof_unlocked = &feof_unlocked;
static __typeof__(ferror) *const slate_reference_ferror = &ferror;
static __typeof__(ferror_unlocked) *const slate_reference_ferror_unlocked = &ferror_unlocked;
static __typeof__(fflush) *const slate_reference_fflush = &fflush;
static __typeof__(fflush_unlocked) *const slate_reference_fflush_unlocked = &fflush_unlocked;
static __typeof__(fgetc) *const slate_reference_fgetc = &fgetc;
static __typeof__(fgetc_unlocked) *const slate_reference_fgetc_unlocked = &fgetc_unlocked;
static __typeof__(fgetpos) *const slate_reference_fgetpos = &fgetpos;
static __typeof__(fgetpos64) *const slate_reference_fgetpos64 = &fgetpos64;
static __typeof__(fgets) *const slate_reference_fgets = &fgets;
static __typeof__(fgets_unlocked) *const slate_reference_fgets_unlocked = &fgets_unlocked;
static __typeof__(fileno) *const slate_reference_fileno = &fileno;
static __typeof__(fileno_unlocked) *const slate_reference_fileno_unlocked = &fileno_unlocked;
static __typeof__(flockfile) *const slate_reference_flockfile = &flockfile;
static __typeof__(fmemopen) *const slate_reference_fmemopen = &fmemopen;
static __typeof__(fopen) *const slate_reference_fopen = &fopen;
static __typeof__(fopen64) *const slate_reference_fopen64 = &fopen64;
static __typeof__(fopencookie) *const slate_reference_fopencookie = &fopencookie;
static __typeof__(fprintf) *const slate_reference_fprintf = &fprintf;
static __typeof__(fputc) *const slate_reference_fputc = &fputc;
static __typeof__(fputc_unlocked) *const slate_reference_fputc_unlocked = &fputc_unlocked;
static __typeof__(fputs) *const slate_reference_fputs = &fputs;
static __typeof__(fputs_unlocked) *const slate_reference_fputs_unlocked = &fputs_unlocked;
static __typeof__(fread) *const slate_reference_fread = &fread;
static __typeof__(fread_unlocked) *const slate_reference_fread_unlocked = &fread_unlocked;
static __typeof__(freopen) *const slate_reference_freopen = &freopen;
static __typeof__(freopen64) *const slate_reference_freopen64 = &freopen64;
static __typeof__(fscanf) *const slate_reference_fscanf = &fscanf;
static __typeof__(fseek) *const slate_reference_fseek = &fseek;
static __typeof__(fseeko) *const slate_reference_fseeko = &fseeko;
static __typeof__(fseeko64) *const slate_reference_fseeko64 = &fseeko64;
static __typeof__(fsetpos) *const slate_reference_fsetpos = &fsetpos;
static __typeof__(fsetpos64) *const slate_reference_fsetpos64 = &fsetpos64;
static __typeof__(ftell) *const slate_reference_ftell = &ftell;
static __typeof__(ftello) *const slate_reference_ftello = &ftello;
static __typeof__(ftello64) *const slate_reference_ftello64 = &ftello64;
static __typeof__(ftrylockfile) *const slate_reference_ftrylockfile = &ftrylockfile;
static __typeof__(funlockfile) *const slate_reference_funlockfile = &funlockfile;
static __typeof__(fwrite) *const slate_reference_fwrite = &fwrite;
static __typeof__(fwrite_unlocked) *const slate_reference_fwrite_unlocked = &fwrite_unlocked;
static __typeof__(getc) *const slate_reference_getc = &getc;
static __typeof__(getc_unlocked) *const slate_reference_getc_unlocked = &getc_unlocked;
static __typeof__(getchar) *const slate_reference_getchar = &getchar;
static __typeof__(getchar_unlocked) *const slate_reference_getchar_unlocked = &getchar_unlocked;
static __typeof__(getdelim) *const slate_reference_getdelim = &getdelim;
static __typeof__(getline) *const slate_reference_getline = &getline;
static __typeof__(getw) *const slate_reference_getw = &getw;
static __typeof__(obstack_printf) *const slate_reference_obstack_printf = &obstack_printf;
static __typeof__(obstack_vprintf) *const slate_reference_obstack_vprintf = &obstack_vprintf;
static __typeof__(open_memstream) *const slate_reference_open_memstream = &open_memstream;
static __typeof__(pclose) *const slate_reference_pclose = &pclose;
static __typeof__(perror) *const slate_reference_perror = &perror;
static __typeof__(popen) *const slate_reference_popen = &popen;
static __typeof__(printf) *const slate_reference_printf = &printf;
static __typeof__(putc) *const slate_reference_putc = &putc;
static __typeof__(putc_unlocked) *const slate_reference_putc_unlocked = &putc_unlocked;
static __typeof__(putchar) *const slate_reference_putchar = &putchar;
static __typeof__(putchar_unlocked) *const slate_reference_putchar_unlocked = &putchar_unlocked;
static __typeof__(puts) *const slate_reference_puts = &puts;
static __typeof__(putw) *const slate_reference_putw = &putw;
static __typeof__(remove) *const slate_reference_remove = &remove;
static __typeof__(rename) *const slate_reference_rename = &rename;
static __typeof__(renameat) *const slate_reference_renameat = &renameat;
static __typeof__(renameat2) *const slate_reference_renameat2 = &renameat2;
static __typeof__(rewind) *const slate_reference_rewind = &rewind;
static __typeof__(scanf) *const slate_reference_scanf = &scanf;
static __typeof__(setbuf) *const slate_reference_setbuf = &setbuf;
static __typeof__(setbuffer) *const slate_reference_setbuffer = &setbuffer;
static __typeof__(setlinebuf) *const slate_reference_setlinebuf = &setlinebuf;
static __typeof__(setvbuf) *const slate_reference_setvbuf = &setvbuf;
static __typeof__(snprintf) *const slate_reference_snprintf = &snprintf;
static __typeof__(sprintf) *const slate_reference_sprintf = &sprintf;
static __typeof__(sscanf) *const slate_reference_sscanf = &sscanf;
static __typeof__(tempnam) *const slate_reference_tempnam = &tempnam;
static __typeof__(tmpfile) *const slate_reference_tmpfile = &tmpfile;
static __typeof__(tmpfile64) *const slate_reference_tmpfile64 = &tmpfile64;
static __typeof__(tmpnam) *const slate_reference_tmpnam = &tmpnam;
static __typeof__(tmpnam_r) *const slate_reference_tmpnam_r = &tmpnam_r;
static __typeof__(ungetc) *const slate_reference_ungetc = &ungetc;
static __typeof__(vasprintf) *const slate_reference_vasprintf = &vasprintf;
static __typeof__(vdprintf) *const slate_reference_vdprintf = &vdprintf;
static __typeof__(vfprintf) *const slate_reference_vfprintf = &vfprintf;
static __typeof__(vfscanf) *const slate_reference_vfscanf = &vfscanf;
static __typeof__(vprintf) *const slate_reference_vprintf = &vprintf;
static __typeof__(vscanf) *const slate_reference_vscanf = &vscanf;
static __typeof__(vsnprintf) *const slate_reference_vsnprintf = &vsnprintf;
static __typeof__(vsprintf) *const slate_reference_vsprintf = &vsprintf;
static __typeof__(vsscanf) *const slate_reference_vsscanf = &vsscanf;

extern struct _IO_FILE * slate_oracle_stderr;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_stderr), __typeof__(stderr)), "stderr object type differs from oracle");

static __typeof__(stderr) *const slate_reference_stderr = &stderr;

extern struct _IO_FILE * slate_oracle_stdin;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_stdin), __typeof__(stdin)), "stdin object type differs from oracle");

static __typeof__(stdin) *const slate_reference_stdin = &stdin;

extern struct _IO_FILE * slate_oracle_stdout;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_stdout), __typeof__(stdout)), "stdout object type differs from oracle");

static __typeof__(stdout) *const slate_reference_stdout = &stdout;

typedef struct _IO_FILE slate_oracle_typedef_FILE;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_FILE, FILE), "typedef FILE differs from oracle");

typedef __float128 slate_oracle_typedef__Float128;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float128, _Float128), "typedef _Float128 differs from oracle");

typedef float slate_oracle_typedef__Float32;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float32, _Float32), "typedef _Float32 differs from oracle");

typedef double slate_oracle_typedef__Float32x;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float32x, _Float32x), "typedef _Float32x differs from oracle");

typedef double slate_oracle_typedef__Float64;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float64, _Float64), "typedef _Float64 differs from oracle");

typedef long double slate_oracle_typedef__Float64x;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float64x, _Float64x), "typedef _Float64x differs from oracle");

typedef void slate_oracle_typedef__IO_lock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__IO_lock_t, _IO_lock_t), "typedef _IO_lock_t differs from oracle");

typedef struct _IO_cookie_io_functions_t slate_oracle_typedef_cookie_io_functions_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_cookie_io_functions_t, cookie_io_functions_t), "typedef cookie_io_functions_t differs from oracle");

typedef struct _G_fpos64_t slate_oracle_typedef_fpos64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fpos64_t, fpos64_t), "typedef fpos64_t differs from oracle");

typedef struct _G_fpos_t slate_oracle_typedef_fpos_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fpos_t, fpos_t), "typedef fpos_t differs from oracle");

typedef long slate_oracle_typedef_off64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off64_t, off64_t), "typedef off64_t differs from oracle");

typedef long slate_oracle_typedef_off_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off_t, off_t), "typedef off_t differs from oracle");

typedef unsigned long slate_oracle_typedef_size_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_size_t, size_t), "typedef size_t differs from oracle");

typedef long slate_oracle_typedef_ssize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ssize_t, ssize_t), "typedef ssize_t differs from oracle");

_Static_assert(__builtin_offsetof(struct _G_fpos64_t, __pos) == 0, "struct _G_fpos64_t.__pos offset differs from oracle");

typedef long slate_oracle_struct__G_fpos64_t___pos;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _G_fpos64_t *)0)->__pos), slate_oracle_struct__G_fpos64_t___pos), "struct _G_fpos64_t.__pos field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _G_fpos64_t, __state) == 8, "struct _G_fpos64_t.__state offset differs from oracle");

typedef struct struct __mbstate_t slate_oracle_struct__G_fpos64_t___state;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _G_fpos64_t *)0)->__state), slate_oracle_struct__G_fpos64_t___state), "struct _G_fpos64_t.__state field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _G_fpos_t, __pos) == 0, "struct _G_fpos_t.__pos offset differs from oracle");

typedef long slate_oracle_struct__G_fpos_t___pos;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _G_fpos_t *)0)->__pos), slate_oracle_struct__G_fpos_t___pos), "struct _G_fpos_t.__pos field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _G_fpos_t, __state) == 8, "struct _G_fpos_t.__state offset differs from oracle");

typedef struct struct __mbstate_t slate_oracle_struct__G_fpos_t___state;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _G_fpos_t *)0)->__state), slate_oracle_struct__G_fpos_t___state), "struct _G_fpos_t.__state field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _flags) == 0, "struct _IO_FILE._flags offset differs from oracle");

typedef int slate_oracle_struct__IO_FILE__flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_flags), slate_oracle_struct__IO_FILE__flags), "struct _IO_FILE._flags field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_read_ptr) == 8, "struct _IO_FILE._IO_read_ptr offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_read_ptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_read_ptr), slate_oracle_struct__IO_FILE__IO_read_ptr), "struct _IO_FILE._IO_read_ptr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_read_end) == 16, "struct _IO_FILE._IO_read_end offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_read_end;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_read_end), slate_oracle_struct__IO_FILE__IO_read_end), "struct _IO_FILE._IO_read_end field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_read_base) == 24, "struct _IO_FILE._IO_read_base offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_read_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_read_base), slate_oracle_struct__IO_FILE__IO_read_base), "struct _IO_FILE._IO_read_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_write_base) == 32, "struct _IO_FILE._IO_write_base offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_write_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_write_base), slate_oracle_struct__IO_FILE__IO_write_base), "struct _IO_FILE._IO_write_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_write_ptr) == 40, "struct _IO_FILE._IO_write_ptr offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_write_ptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_write_ptr), slate_oracle_struct__IO_FILE__IO_write_ptr), "struct _IO_FILE._IO_write_ptr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_write_end) == 48, "struct _IO_FILE._IO_write_end offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_write_end;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_write_end), slate_oracle_struct__IO_FILE__IO_write_end), "struct _IO_FILE._IO_write_end field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_buf_base) == 56, "struct _IO_FILE._IO_buf_base offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_buf_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_buf_base), slate_oracle_struct__IO_FILE__IO_buf_base), "struct _IO_FILE._IO_buf_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_buf_end) == 64, "struct _IO_FILE._IO_buf_end offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_buf_end;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_buf_end), slate_oracle_struct__IO_FILE__IO_buf_end), "struct _IO_FILE._IO_buf_end field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_save_base) == 72, "struct _IO_FILE._IO_save_base offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_save_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_save_base), slate_oracle_struct__IO_FILE__IO_save_base), "struct _IO_FILE._IO_save_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_backup_base) == 80, "struct _IO_FILE._IO_backup_base offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_backup_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_backup_base), slate_oracle_struct__IO_FILE__IO_backup_base), "struct _IO_FILE._IO_backup_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_FILE, _IO_save_end) == 88, "struct _IO_FILE._IO_save_end offset differs from oracle");

typedef char * slate_oracle_struct__IO_FILE__IO_save_end;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_IO_save_end), slate_oracle_struct__IO_FILE__IO_save_end), "struct _IO_FILE._IO_save_end field type differs from oracle");

typedef struct _IO_marker * slate_oracle_struct__IO_FILE__markers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_markers), slate_oracle_struct__IO_FILE__markers), "struct _IO_FILE._markers field type differs from oracle");

typedef struct _IO_FILE * slate_oracle_struct__IO_FILE__chain;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_chain), slate_oracle_struct__IO_FILE__chain), "struct _IO_FILE._chain field type differs from oracle");

typedef int slate_oracle_struct__IO_FILE__fileno;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_fileno), slate_oracle_struct__IO_FILE__fileno), "struct _IO_FILE._fileno field type differs from oracle");

typedef long slate_oracle_struct__IO_FILE__old_offset;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_old_offset), slate_oracle_struct__IO_FILE__old_offset), "struct _IO_FILE._old_offset field type differs from oracle");

typedef unsigned short slate_oracle_struct__IO_FILE__cur_column;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_cur_column), slate_oracle_struct__IO_FILE__cur_column), "struct _IO_FILE._cur_column field type differs from oracle");

typedef signed char slate_oracle_struct__IO_FILE__vtable_offset;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_vtable_offset), slate_oracle_struct__IO_FILE__vtable_offset), "struct _IO_FILE._vtable_offset field type differs from oracle");

typedef void * slate_oracle_struct__IO_FILE__lock;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_lock), slate_oracle_struct__IO_FILE__lock), "struct _IO_FILE._lock field type differs from oracle");

typedef long slate_oracle_struct__IO_FILE__offset;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_offset), slate_oracle_struct__IO_FILE__offset), "struct _IO_FILE._offset field type differs from oracle");

typedef struct _IO_codecvt * slate_oracle_struct__IO_FILE__codecvt;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_codecvt), slate_oracle_struct__IO_FILE__codecvt), "struct _IO_FILE._codecvt field type differs from oracle");

typedef struct _IO_wide_data * slate_oracle_struct__IO_FILE__wide_data;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_wide_data), slate_oracle_struct__IO_FILE__wide_data), "struct _IO_FILE._wide_data field type differs from oracle");

typedef struct _IO_FILE * slate_oracle_struct__IO_FILE__freeres_list;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_freeres_list), slate_oracle_struct__IO_FILE__freeres_list), "struct _IO_FILE._freeres_list field type differs from oracle");

typedef void * slate_oracle_struct__IO_FILE__freeres_buf;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_freeres_buf), slate_oracle_struct__IO_FILE__freeres_buf), "struct _IO_FILE._freeres_buf field type differs from oracle");

typedef struct _IO_FILE ** slate_oracle_struct__IO_FILE__prevchain;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_prevchain), slate_oracle_struct__IO_FILE__prevchain), "struct _IO_FILE._prevchain field type differs from oracle");

typedef int slate_oracle_struct__IO_FILE__mode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_mode), slate_oracle_struct__IO_FILE__mode), "struct _IO_FILE._mode field type differs from oracle");

typedef int slate_oracle_struct__IO_FILE__unused3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_unused3), slate_oracle_struct__IO_FILE__unused3), "struct _IO_FILE._unused3 field type differs from oracle");

typedef unsigned long slate_oracle_struct__IO_FILE__total_written;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct _IO_FILE *)0)->_total_written), slate_oracle_struct__IO_FILE__total_written), "struct _IO_FILE._total_written field type differs from oracle");

_Static_assert(sizeof(struct _IO_cookie_io_functions_t) == 32, "struct _IO_cookie_io_functions_t size differs from oracle");

_Static_assert(_Alignof(struct _IO_cookie_io_functions_t) == 8, "struct _IO_cookie_io_functions_t alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_cookie_io_functions_t, read) == 0, "struct _IO_cookie_io_functions_t.read offset differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_cookie_io_functions_t, write) == 8, "struct _IO_cookie_io_functions_t.write offset differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_cookie_io_functions_t, seek) == 16, "struct _IO_cookie_io_functions_t.seek offset differs from oracle");

_Static_assert(__builtin_offsetof(struct _IO_cookie_io_functions_t, close) == 24, "struct _IO_cookie_io_functions_t.close offset differs from oracle");

#ifndef AT_RENAME_EXCHANGE
#error "stdio.h:AT_RENAME_EXCHANGE macro is missing from libc-shim"
#endif

#ifndef AT_RENAME_NOREPLACE
#error "stdio.h:AT_RENAME_NOREPLACE macro is missing from libc-shim"
#endif

#ifndef AT_RENAME_WHITEOUT
#error "stdio.h:AT_RENAME_WHITEOUT macro is missing from libc-shim"
#endif

#ifndef BUFSIZ
#error "stdio.h:BUFSIZ macro is missing from libc-shim"
#endif

#ifndef EOF
#error "stdio.h:EOF macro is missing from libc-shim"
#endif

#ifndef FILENAME_MAX
#error "stdio.h:FILENAME_MAX macro is missing from libc-shim"
#endif

#ifndef FOPEN_MAX
#error "stdio.h:FOPEN_MAX macro is missing from libc-shim"
#endif

#ifndef L_ctermid
#error "stdio.h:L_ctermid macro is missing from libc-shim"
#endif

#ifndef L_cuserid
#error "stdio.h:L_cuserid macro is missing from libc-shim"
#endif

#ifndef L_tmpnam
#error "stdio.h:L_tmpnam macro is missing from libc-shim"
#endif

#ifndef NULL
#error "stdio.h:NULL macro is missing from libc-shim"
#endif

#ifndef P_tmpdir
#error "stdio.h:P_tmpdir macro is missing from libc-shim"
#endif

#ifndef RENAME_EXCHANGE
#error "stdio.h:RENAME_EXCHANGE macro is missing from libc-shim"
#endif

#ifndef RENAME_NOREPLACE
#error "stdio.h:RENAME_NOREPLACE macro is missing from libc-shim"
#endif

#ifndef RENAME_WHITEOUT
#error "stdio.h:RENAME_WHITEOUT macro is missing from libc-shim"
#endif

#ifndef SEEK_CUR
#error "stdio.h:SEEK_CUR macro is missing from libc-shim"
#endif

#ifndef SEEK_DATA
#error "stdio.h:SEEK_DATA macro is missing from libc-shim"
#endif

#ifndef SEEK_END
#error "stdio.h:SEEK_END macro is missing from libc-shim"
#endif

#ifndef SEEK_HOLE
#error "stdio.h:SEEK_HOLE macro is missing from libc-shim"
#endif

#ifndef SEEK_SET
#error "stdio.h:SEEK_SET macro is missing from libc-shim"
#endif

#ifndef TMP_MAX
#error "stdio.h:TMP_MAX macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "stdio.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _BITS_FLOATN_COMMON_H
#error "stdio.h:_BITS_FLOATN_COMMON_H macro is missing from libc-shim"
#endif

#ifndef _BITS_FLOATN_H
#error "stdio.h:_BITS_FLOATN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_STDIO_LIM_H
#error "stdio.h:_BITS_STDIO_LIM_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TIME64_H
#error "stdio.h:_BITS_TIME64_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPESIZES_H
#error "stdio.h:_BITS_TYPESIZES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES_H
#error "stdio.h:_BITS_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "stdio.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "stdio.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "stdio.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _IOFBF
#error "stdio.h:_IOFBF macro is missing from libc-shim"
#endif

#ifndef _IOLBF
#error "stdio.h:_IOLBF macro is missing from libc-shim"
#endif

#ifndef _IONBF
#error "stdio.h:_IONBF macro is missing from libc-shim"
#endif

#ifndef _IO_EOF_SEEN
#error "stdio.h:_IO_EOF_SEEN macro is missing from libc-shim"
#endif

#ifndef _IO_ERR_SEEN
#error "stdio.h:_IO_ERR_SEEN macro is missing from libc-shim"
#endif

#ifndef _IO_USER_LOCK
#error "stdio.h:_IO_USER_LOCK macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "stdio.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "stdio.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "stdio.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "stdio.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "stdio.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "stdio.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "stdio.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "stdio.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "stdio.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _PRINTF_NAN_LEN_MAX
#error "stdio.h:_PRINTF_NAN_LEN_MAX macro is missing from libc-shim"
#endif

#ifndef _SIZE_T
#error "stdio.h:_SIZE_T macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "stdio.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _STDIO_H
#error "stdio.h:_STDIO_H macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "stdio.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _VA_LIST_DEFINED
#error "stdio.h:_VA_LIST_DEFINED macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "stdio.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "stdio.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

#ifndef stderr
#error "stdio.h:stderr macro is missing from libc-shim"
#endif

#ifndef stdin
#error "stdio.h:stdin macro is missing from libc-shim"
#endif

#ifndef stdout
#error "stdio.h:stdout macro is missing from libc-shim"
#endif

int main(void) { return 0; }
