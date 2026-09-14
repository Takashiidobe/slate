#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <execinfo.h>
#include <fnmatch.h>
#include <glob.h>
#include <gnu/libc-version.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/random.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

static int gnu_environment_extensions(void) {
  char *directory;
  char *canonical;
  char  current[4096];
  int   total = 0;

  total += setenv("SLATE_GNU_LIBC_VALUE", "ready", 1) == 0;
  total += strcmp(secure_getenv("SLATE_GNU_LIBC_VALUE"), "ready") == 0;
  total += unsetenv("SLATE_GNU_LIBC_VALUE") == 0;

  directory  = get_current_dir_name();
  canonical  = canonicalize_file_name(".");
  total     += getcwd(current, sizeof(current)) != NULL;
  total     += directory != NULL && strcmp(directory, current) == 0;
  total     += canonical != NULL && strcmp(canonical, current) == 0;
  free(directory);
  free(canonical);

  total += strcmp(strdupa("slate"), "slate") == 0;
  total += strcmp(strndupa("slate-truncated", 5), "slate") == 0;
  return total;
}

static int gnu_time_extensions(void) {
  struct tm epoch = {};
  struct tm local = {};
  time_t    timestamp;
  int       total = 0;

  epoch.tm_year  = 70;
  epoch.tm_mon   = 0;
  epoch.tm_mday  = 1;
  timestamp      = timegm(&epoch);
  total         += timestamp == 0;

  local.tm_year  = 70;
  local.tm_mon   = 0;
  local.tm_mday  = 2;
  total         += timelocal(&local) != (time_t)-1;
  return total;
}

static int gnu_pattern_extensions(void) {
  regex_t     expression = {};
  glob_t      paths      = {};
  const char *error;
  int         total = 0;

  total += fnmatch("file-+(one|two).c", "file-two.c", FNM_EXTMATCH) == 0;
  re_set_syntax(RE_SYNTAX_POSIX_EXTENDED);
  error  = re_compile_pattern("sl(a|e)te", 9, &expression);
  total += error == NULL;
  total += re_match(&expression, "slate", 5, 0, NULL) == 5;
  regfree(&expression);

  total += glob("/dev/{null,zero}", GLOB_BRACE, NULL, &paths) == 0;
  total += paths.gl_pathc == 2;
  total += strcmp(paths.gl_pathv[0], "/dev/null") == 0;
  total += strcmp(paths.gl_pathv[1], "/dev/zero") == 0;
  globfree(&paths);
  return total;
}

static int gnu_runtime_extensions(void) {
  unsigned char random_bytes[8];
  void         *frames[8];
  Dl_info       information = {};
  long          page_size   = sysconf(_SC_PAGESIZE);
  int           total       = 0;

  total += getauxval(AT_PAGESZ) == (unsigned long)page_size;
  total += gettid() == (pid_t)syscall(SYS_gettid);
  total += getentropy(random_bytes, sizeof(random_bytes)) == 0;
  total += arc4random_uniform(1) == 0;
  total += get_nprocs() > 0;
  total += get_phys_pages() > 0;
  total += backtrace(frames, 8) > 0;
  total += dladdr((void *)&gnu_runtime_extensions, &information) != 0;
  total += information.dli_fname != NULL;
  total += gnu_get_libc_version()[0] != '\0';
  total += program_invocation_name != NULL;
  total += program_invocation_short_name != NULL;
  return total;
}

int main(void) {
  printf("%d %d %d\n", gnu_environment_extensions(), gnu_time_extensions(),
         gnu_pattern_extensions() + gnu_runtime_extensions());
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[0]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           0,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "dli_fname",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 44,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "dli_fbase",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 45,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "dli_sname",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 46,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "dli_saddr",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 47,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   4,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 43,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[15]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           15,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "size_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "gl_pathc",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 19,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "gl_pathv",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 20,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "size_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "gl_offs",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 21,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__reserved1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 22,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "__reserved2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           5,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               11,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 23,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   11,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               11,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 18,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[16]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           16,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "re_pattern_buffer",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "__buffer",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               14,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 49,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   14,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__allocated",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               14,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 50,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   14,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__used",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               14,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 51,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   14,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__syntax",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               14,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 52,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   14,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "__fastmap",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               14,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 53,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   14,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: Some(
// DEFAULT-NEXT:                                       false,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "__translate",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               14,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 54,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   14,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "size_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "re_nsub",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               14,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 55,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   14,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "__regex_flags",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               14,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 56,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   14,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               14,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 48,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[74]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           74,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Enum,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Enum(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_read",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 5,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_write",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           1,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 6,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_open",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           2,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 7,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_close",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 8,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_stat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           4,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 9,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fstat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           5,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 10,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lstat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           6,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 11,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_poll",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           7,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 12,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lseek",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           8,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 13,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mmap",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           9,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 14,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mprotect",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           10,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 15,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_munmap",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           11,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 16,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_brk",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           12,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 17,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rt_sigaction",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           13,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 18,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rt_sigprocmask",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           14,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 19,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rt_sigreturn",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           15,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 20,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_ioctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           16,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 21,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pread64",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           17,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 22,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pwrite64",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           18,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 23,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_readv",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           19,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 24,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_writev",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           20,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 25,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_access",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           21,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 26,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pipe",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           22,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 27,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_select",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           23,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 28,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_yield",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           24,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 29,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mremap",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           25,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 30,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_msync",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           26,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 31,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mincore",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           27,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 32,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_madvise",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           28,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 33,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_shmget",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           29,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 34,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_shmat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           30,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 35,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_shmctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           31,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 36,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_dup",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           32,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 37,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_dup2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           33,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 38,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pause",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           34,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 39,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_nanosleep",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           35,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 40,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getitimer",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           36,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 41,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_alarm",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           37,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 42,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setitimer",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           38,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 43,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getpid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 44,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sendfile",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           40,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 45,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_socket",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           41,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 46,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_connect",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           42,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 47,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_accept",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           43,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 48,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sendto",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           44,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 49,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_recvfrom",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           45,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 50,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sendmsg",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           46,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 51,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_recvmsg",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           47,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 52,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_shutdown",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           48,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 53,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_bind",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           49,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 54,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_listen",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           50,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 55,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getsockname",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           51,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 56,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getpeername",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           52,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 57,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_socketpair",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           53,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 58,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setsockopt",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           54,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 59,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getsockopt",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           55,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 60,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_clone",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           56,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 61,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fork",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           57,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 62,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_vfork",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           58,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 63,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_execve",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           59,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 64,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_exit",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           60,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 65,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_wait4",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           61,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 66,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_kill",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           62,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 67,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_uname",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           63,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 68,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_semget",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           64,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 69,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_semop",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           65,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 70,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_semctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           66,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 71,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_shmdt",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           67,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 72,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_msgget",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           68,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 73,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_msgsnd",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           69,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 74,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_msgrcv",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           70,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 75,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_msgctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           71,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 76,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fcntl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           72,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 77,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_flock",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           73,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 78,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fsync",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           74,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 79,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fdatasync",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           75,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 80,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_truncate",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           76,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 81,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_ftruncate",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           77,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 82,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getdents",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           78,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 83,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getcwd",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           79,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 84,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_chdir",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           80,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 85,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fchdir",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           81,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 86,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rename",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           82,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 87,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mkdir",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           83,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 88,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rmdir",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           84,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 89,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_creat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           85,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 90,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_link",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           86,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 91,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_unlink",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           87,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 92,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_symlink",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           88,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 93,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_readlink",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           89,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 94,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_chmod",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           90,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 95,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fchmod",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           91,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 96,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_chown",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           92,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 97,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fchown",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           93,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 98,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lchown",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           94,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 99,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_umask",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           95,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 100,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_gettimeofday",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           96,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 101,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getrlimit",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           97,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 102,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getrusage",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           98,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 103,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sysinfo",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           99,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 104,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_times",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           100,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 105,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_ptrace",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           101,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 106,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getuid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           102,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 107,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_syslog",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           103,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 108,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getgid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           104,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 109,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setuid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           105,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 110,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setgid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           106,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 111,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_geteuid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           107,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 112,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getegid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           108,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 113,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setpgid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           109,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 114,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getppid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           110,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 115,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getpgrp",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           111,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 116,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setsid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           112,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 117,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setreuid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           113,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 118,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setregid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           114,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 119,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getgroups",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           115,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 120,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setgroups",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           116,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 121,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setresuid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           117,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 122,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getresuid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           118,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 123,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setresgid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           119,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 124,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getresgid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           120,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 125,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getpgid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           121,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 126,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setfsuid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           122,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 127,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setfsgid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           123,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 128,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getsid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           124,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 129,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_capget",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           125,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 130,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_capset",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           126,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 131,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rt_sigpending",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           127,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 132,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rt_sigtimedwait",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           128,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 133,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rt_sigqueueinfo",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           129,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 134,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rt_sigsuspend",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           130,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 135,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sigaltstack",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           131,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 136,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_utime",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           132,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 137,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mknod",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           133,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 138,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_uselib",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           134,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 139,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_personality",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           135,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 140,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_ustat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           136,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 141,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_statfs",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           137,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 142,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fstatfs",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           138,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 143,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sysfs",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           139,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 144,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getpriority",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           140,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 145,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setpriority",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           141,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 146,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_setparam",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           142,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 147,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_getparam",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           143,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 148,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_setscheduler",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           144,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 149,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_getscheduler",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           145,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 150,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_get_priority_max",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           146,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 151,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_get_priority_min",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           147,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 152,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_rr_get_interval",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           148,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 153,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mlock",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           149,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 154,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_munlock",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           150,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 155,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mlockall",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           151,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 156,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_munlockall",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           152,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 157,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_vhangup",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           153,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 158,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_modify_ldt",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           154,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 159,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pivot_root",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           155,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 160,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR__sysctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           156,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 161,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_prctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           157,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 162,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_arch_prctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           158,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 163,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_adjtimex",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           159,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 164,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setrlimit",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           160,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 165,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_chroot",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           161,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 166,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sync",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           162,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 167,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_acct",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           163,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 168,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_settimeofday",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           164,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 169,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mount",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           165,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 170,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_umount2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           166,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 171,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_swapon",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           167,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 172,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_swapoff",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           168,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 173,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_reboot",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           169,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 174,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sethostname",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           170,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 175,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setdomainname",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           171,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 176,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_iopl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           172,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 177,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_ioperm",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           173,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 178,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_create_module",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           174,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 179,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_init_module",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           175,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 180,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_delete_module",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           176,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 181,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_get_kernel_syms",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           177,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 182,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_query_module",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           178,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 183,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_quotactl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           179,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 184,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_nfsservctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           180,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 185,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getpmsg",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           181,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 186,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_putpmsg",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           182,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 187,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_afs_syscall",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           183,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 188,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_tuxcall",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           184,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 189,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_security",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           185,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 190,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_gettid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           186,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 191,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_readahead",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           187,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 192,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           188,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 193,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lsetxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           189,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 194,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fsetxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           190,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 195,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           191,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 196,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lgetxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           192,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 197,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fgetxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           193,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 198,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_listxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           194,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 199,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_llistxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           195,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 200,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_flistxattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           196,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 201,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_removexattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           197,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 202,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lremovexattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           198,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 203,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fremovexattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           199,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 204,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_tkill",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           200,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 205,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_time",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           201,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 206,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_futex",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           202,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 207,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_setaffinity",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           203,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 208,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_getaffinity",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           204,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 209,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_set_thread_area",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           205,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 210,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_setup",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           206,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 211,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_destroy",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           207,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 212,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_getevents",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           208,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 213,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_submit",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           209,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 214,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_cancel",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           210,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 215,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_get_thread_area",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           211,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 216,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lookup_dcookie",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           212,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 217,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_epoll_create",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           213,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 218,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_epoll_ctl_old",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           214,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 219,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_epoll_wait_old",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           215,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 220,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_remap_file_pages",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           216,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 221,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getdents64",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           217,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 222,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_set_tid_address",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           218,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 223,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_restart_syscall",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           219,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 224,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_semtimedop",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           220,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 225,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fadvise64",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           221,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 226,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_timer_create",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           222,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 227,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_timer_settime",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           223,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 228,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_timer_gettime",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           224,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 229,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_timer_getoverrun",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           225,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 230,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_timer_delete",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           226,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 231,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_clock_settime",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           227,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 232,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_clock_gettime",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           228,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 233,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_clock_getres",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           229,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 234,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_clock_nanosleep",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           230,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 235,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_exit_group",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           231,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 236,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_epoll_wait",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           232,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 237,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_epoll_ctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           233,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 238,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_tgkill",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           234,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 239,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_utimes",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           235,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 240,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_vserver",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           236,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 241,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mbind",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           237,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 242,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_set_mempolicy",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           238,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 243,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_get_mempolicy",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           239,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 244,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mq_open",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           240,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 245,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mq_unlink",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           241,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 246,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mq_timedsend",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           242,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 247,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mq_timedreceive",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           243,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 248,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mq_notify",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           244,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 249,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mq_getsetattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           245,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 250,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_kexec_load",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           246,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 251,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_waitid",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           247,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 252,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_add_key",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           248,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 253,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_request_key",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           249,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 254,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_keyctl",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           250,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 255,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_ioprio_set",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           251,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 256,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_ioprio_get",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           252,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 257,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_inotify_init",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           253,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 258,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_inotify_add_watch",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           254,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 259,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_inotify_rm_watch",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           255,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 260,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_migrate_pages",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           256,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 261,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_openat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           257,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 262,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mkdirat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           258,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 263,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mknodat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           259,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 264,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fchownat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           260,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 265,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_futimesat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           261,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 266,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_newfstatat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           262,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 267,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_unlinkat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           263,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 268,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_renameat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           264,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 269,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_linkat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           265,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 270,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_symlinkat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           266,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 271,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_readlinkat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           267,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 272,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fchmodat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           268,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 273,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_faccessat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           269,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 274,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pselect6",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           270,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 275,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_ppoll",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           271,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 276,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_unshare",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           272,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 277,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_set_robust_list",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           273,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 278,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_get_robust_list",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           274,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 279,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_splice",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           275,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 280,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_tee",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           276,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 281,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sync_file_range",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           277,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 282,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_vmsplice",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           278,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 283,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_move_pages",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           279,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 284,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_utimensat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           280,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 285,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_epoll_pwait",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           281,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 286,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_signalfd",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           282,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 287,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_timerfd_create",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           283,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 288,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_eventfd",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           284,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 289,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fallocate",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           285,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 290,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_timerfd_settime",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           286,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 291,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_timerfd_gettime",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           287,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 292,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_accept4",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           288,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 293,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_signalfd4",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           289,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 294,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_eventfd2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           290,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 295,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_epoll_create1",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           291,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 296,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_dup3",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           292,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 297,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pipe2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           293,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 298,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_inotify_init1",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           294,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 299,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_preadv",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           295,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 300,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pwritev",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           296,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 301,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rt_tgsigqueueinfo",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           297,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 302,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_perf_event_open",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           298,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 303,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_recvmmsg",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           299,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 304,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fanotify_init",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           300,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 305,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fanotify_mark",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           301,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 306,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_prlimit64",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           302,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 307,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_name_to_handle_at",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           303,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 308,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_open_by_handle_at",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           304,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 309,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_clock_adjtime",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           305,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 310,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_syncfs",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           306,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 311,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sendmmsg",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           307,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 312,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setns",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           308,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 313,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getcpu",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           309,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 314,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_process_vm_readv",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           310,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 315,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_process_vm_writev",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           311,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 316,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_kcmp",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           312,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 317,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_finit_module",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           313,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 318,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_setattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           314,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 319,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_sched_getattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           315,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 320,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_renameat2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           316,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 321,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_seccomp",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           317,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 322,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getrandom",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           318,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 323,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_memfd_create",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           319,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 324,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_kexec_file_load",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           320,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 325,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_bpf",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           321,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 326,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_execveat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           322,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 327,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_userfaultfd",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           323,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 328,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_membarrier",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           324,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 329,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mlock2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           325,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 330,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_copy_file_range",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           326,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 331,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_preadv2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           327,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 332,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pwritev2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           328,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 333,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pkey_mprotect",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           329,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 334,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pkey_alloc",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           330,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 335,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pkey_free",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           331,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 336,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_statx",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           332,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 337,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_pgetevents",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           333,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 338,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_rseq",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           334,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 339,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_uretprobe",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           335,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 340,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_uprobe",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           336,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 341,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pidfd_send_signal",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           424,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 342,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_uring_setup",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           425,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 343,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_uring_enter",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           426,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 344,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_io_uring_register",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           427,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 345,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_open_tree",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           428,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 346,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_move_mount",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           429,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 347,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fsopen",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           430,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 348,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fsconfig",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           431,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 349,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fsmount",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           432,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 350,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fspick",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           433,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 351,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pidfd_open",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           434,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 352,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_clone3",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           435,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 353,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_close_range",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           436,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 354,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_openat2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           437,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 355,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_pidfd_getfd",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           438,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 356,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_faccessat2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           439,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 357,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_process_madvise",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           440,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 358,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_epoll_pwait2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           441,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 359,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mount_setattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           442,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 360,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_quotactl_fd",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           443,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 361,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_landlock_create_ruleset",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           444,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 362,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_landlock_add_rule",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           445,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 363,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_landlock_restrict_self",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           446,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 364,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_memfd_secret",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           447,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 365,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_process_mrelease",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           448,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 366,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_futex_waitv",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           449,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 367,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_set_mempolicy_home_node",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           450,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 368,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_cachestat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           451,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 369,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_fchmodat2",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           452,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 370,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_map_shadow_stack",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           453,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 371,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_futex_wake",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           454,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 372,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_futex_wait",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           455,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 373,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_futex_requeue",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           456,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 374,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_statmount",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           457,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 375,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_listmount",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           458,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 376,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lsm_get_self_attr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           459,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 377,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lsm_set_self_attr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           460,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 378,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_lsm_list_modules",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           461,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 379,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_mseal",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           462,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 380,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_setxattrat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           463,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 381,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_getxattrat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           464,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 382,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_listxattrat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           465,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 383,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_removexattrat",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           466,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 384,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_open_tree_attr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           467,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 385,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_file_getattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           468,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 386,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_file_setattr",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           469,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 387,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "__NR_listns",
// DEFAULT-NEXT:                   value: Some(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           470,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   provenance: Provenance {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           39,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       kind: System,
// DEFAULT-NEXT:                       line: 388,
// DEFAULT-NEXT:                       header: Some(
// DEFAULT-NEXT:                           FileId(
// DEFAULT-NEXT:                               37,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               39,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   37,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[77]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           77,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "tm",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_sec",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 50,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_min",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 51,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_hour",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 52,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_mday",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 53,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_mon",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 54,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_year",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 55,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_wday",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 56,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_yday",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 57,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_isdst",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 58,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "tm_gmtoff",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 60,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "tm_zone",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               41,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 64,
// DEFAULT-NEXT:                           header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   41,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               41,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 49,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   41,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "Dl_info",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 43,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "dladdr",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "Dl_info",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 49,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Pointer {
// DEFAULT-NEXT:                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "program_invocation_name",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   8,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Pointer {
// DEFAULT-NEXT:                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "program_invocation_short_name",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               8,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 171,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   8,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "backtrace",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "__array",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Name(
// DEFAULT-NEXT:                                       "__size",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               9,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   9,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "fnmatch",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               10,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 28,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   10,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "__pid_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 54,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "__regoff_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 78,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "__size_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 258,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[9]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "__time_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 262,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "__size_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "size_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 795,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[11]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           15,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "glob_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               11,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 18,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[12]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "glob",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Function {
// DEFAULT-NEXT:                                       inner: Grouped(
// DEFAULT-NEXT:                                           Pointer {
// DEFAULT-NEXT:                                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                                               inner: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       parameters: [
// DEFAULT-NEXT:                                           Parameter {
// DEFAULT-NEXT:                                               ty: Qualified {
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_const: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Char {
// DEFAULT-NEXT:                                                           signed: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Some(
// DEFAULT-NEXT:                                                   Pointer {
// DEFAULT-NEXT:                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                       inner: Abstract,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           Parameter {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Int,
// DEFAULT-NEXT:                                                       signed: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "glob_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               11,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 26,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[13]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "globfree",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "glob_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               11,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 28,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   11,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[14]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "gnu_get_libc_version",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               13,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   13,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[15]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "__pid_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "pid_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 834,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[16]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "__time_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "time_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 852,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[17]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "__regoff_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "regoff_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               12,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 703,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[18]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           16,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "regex_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               14,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 48,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[19]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "regfree",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "regex_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               14,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 182,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[20]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "reg_syntax_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               14,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 187,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[21]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "reg_syntax_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "re_set_syntax",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "reg_syntax_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               14,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 232,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[22]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "re_compile_pattern",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "size_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "regex_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               14,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 233,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[23]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "regoff_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "re_match",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "regex_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "size_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "regoff_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               14,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 235,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   14,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[24]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       variadic: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               16,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   16,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[25]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "free",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "ptr",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               17,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 94,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   17,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[26]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "setenv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "name",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "value",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Name(
// DEFAULT-NEXT:                                       "overwrite",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               17,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 167,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   17,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[27]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "unsetenv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "name",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               17,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 168,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   17,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[28]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "secure_getenv",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "name",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               17,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 233,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   17,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[29]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "canonicalize_file_name",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "name",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               17,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 234,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   17,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[30]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "arc4random_uniform",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Name(
// DEFAULT-NEXT:                                       "upper_bound",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               17,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 259,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   17,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[31]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "size_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               24,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 17,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   19,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[32]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "memcpy",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "size_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 14,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   19,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[33]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "strcpy",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 29,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   19,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[34]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "strcmp",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 35,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   19,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[35]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "size_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "strlen",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 70,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   19,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[36]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "size_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "strnlen",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "size_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               19,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 93,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   19,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[37]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Pointer {
// DEFAULT-NEXT:               pointee: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           name: "__slate_strndupa_finish",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Void,
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "buf",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Qualified {
// DEFAULT-NEXT:                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                           is_const: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Char {
// DEFAULT-NEXT:                               signed: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "s",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "size_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "len",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "out",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Cast {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Char {
// DEFAULT-NEXT:                                                   signed: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Pointer {
// DEFAULT-NEXT:                                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                                               inner: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           value: Identifier(
// DEFAULT-NEXT:                                               "buf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "memcpy",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "out",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "s",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "len",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Index {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "out",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           index: Identifier(
// DEFAULT-NEXT:                               "len",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       value: Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "out",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   19,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: System,
// DEFAULT-NEXT:               line: 129,
// DEFAULT-NEXT:               header: Some(
// DEFAULT-NEXT:                   FileId(
// DEFAULT-NEXT:                       19,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:           is_inline: true,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[38]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "getauxval",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               32,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   32,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[39]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "getentropy",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "size_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               36,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 17,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   36,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[40]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           74,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               39,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   37,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[41]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "get_nprocs",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               40,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 27,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   40,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[42]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "get_phys_pages",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               40,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 28,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   40,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[43]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           77,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               41,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 49,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   41,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[44]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "time_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "timelocal",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Tag(
// DEFAULT-NEXT:                                   Reference {
// DEFAULT-NEXT:                                       kind: Struct,
// DEFAULT-NEXT:                                       name: "tm",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               41,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 168,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   41,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[45]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "time_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "timegm",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Tag(
// DEFAULT-NEXT:                                   Reference {
// DEFAULT-NEXT:                                       kind: Struct,
// DEFAULT-NEXT:                                       name: "tm",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               41,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 178,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   41,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[46]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "getcwd",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "size_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               43,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 89,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   43,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[47]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "sysconf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               43,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 140,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   43,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[48]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Long,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "syscall",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       variadic: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               43,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 187,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   43,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[49]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "getentropy",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "size_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               43,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 190,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   43,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[50]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "get_current_dir_name",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               43,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 202,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   43,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[51]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "pid_t",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "gettid",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               43,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 207,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   43,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[52]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "gnu_environment_extensions",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "directory",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "canonical",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "current",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           4096,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "setenv",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "SLATE_GNU_LIBC_VALUE",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "ready",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "strcmp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "secure_getenv",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLit(
// DEFAULT-NEXT:                                               "SLATE_GNU_LIBC_VALUE",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "ready",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "unsetenv",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "SLATE_GNU_LIBC_VALUE",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "directory",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "get_current_dir_name",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "canonical",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "canonicalize_file_name",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   ".",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "getcwd",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "current",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   SizeOfExpr(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "current",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Paren(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: And,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "directory",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Paren(
// DEFAULT-NEXT:                                   Cast {
// DEFAULT-NEXT:                                       ty: Void,
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Abstract,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strcmp",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "directory",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "current",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: And,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: NotEqual,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "canonical",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Paren(
// DEFAULT-NEXT:                                   Cast {
// DEFAULT-NEXT:                                       ty: Void,
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Abstract,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: Equal,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "strcmp",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "canonical",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "current",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "free",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "directory",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "free",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "canonical",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "strcmp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "strcpy",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Cast {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Char {
// DEFAULT-NEXT:                                                       signed: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Pointer {
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                   inner: Abstract,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               value: Call {
// DEFAULT-NEXT:                                                   callee: Identifier(
// DEFAULT-NEXT:                                                       "__builtin_alloca",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   arguments: [
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Call {
// DEFAULT-NEXT:                                                               callee: Identifier(
// DEFAULT-NEXT:                                                                   "strlen",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               arguments: [
// DEFAULT-NEXT:                                                                   StringLit(
// DEFAULT-NEXT:                                                                       "slate",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           Paren(
// DEFAULT-NEXT:                                               StringLit(
// DEFAULT-NEXT:                                                   "slate",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "slate",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "strcmp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "__slate_strndupa_finish",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "__builtin_alloca",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: Call {
// DEFAULT-NEXT:                                                           callee: Identifier(
// DEFAULT-NEXT:                                                               "strnlen",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           arguments: [
// DEFAULT-NEXT:                                                               Paren(
// DEFAULT-NEXT:                                                                   StringLit(
// DEFAULT-NEXT:                                                                       "slate-truncated",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               Paren(
// DEFAULT-NEXT:                                                                   Integer(
// DEFAULT-NEXT:                                                                       5,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           Paren(
// DEFAULT-NEXT:                                               StringLit(
// DEFAULT-NEXT:                                                   "slate-truncated",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "strnlen",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Paren(
// DEFAULT-NEXT:                                                       StringLit(
// DEFAULT-NEXT:                                                           "slate-truncated",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   Paren(
// DEFAULT-NEXT:                                                       Integer(
// DEFAULT-NEXT:                                                           5,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "slate",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "total",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 18,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[53]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "gnu_time_extensions",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "tm",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "epoch",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "tm",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "local",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "time_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "timestamp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Member {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "epoch",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           field: "tm_year",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       value: Integer(
// DEFAULT-NEXT:                           70,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Member {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "epoch",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           field: "tm_mon",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       value: Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Member {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "epoch",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           field: "tm_mday",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       value: Integer(
// DEFAULT-NEXT:                           1,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "timestamp",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "timegm",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Unary {
// DEFAULT-NEXT:                                   op: AddrOf,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "epoch",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "timestamp",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Member {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "local",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           field: "tm_year",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       value: Integer(
// DEFAULT-NEXT:                           70,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Member {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "local",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           field: "tm_mon",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       value: Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Member {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "local",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           field: "tm_mday",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       value: Integer(
// DEFAULT-NEXT:                           2,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "timelocal",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Identifier(
// DEFAULT-NEXT:                                           "local",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Cast {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "time_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   operand: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "total",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 41,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[54]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "gnu_pattern_extensions",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "regex_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "expression",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "glob_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "paths",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "error",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "fnmatch",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "file-+(one|two).c",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "file-two.c",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       32,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "re_set_syntax",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: BitOr,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: BitOr,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: BitOr,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: BitOr,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: BitOr,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: BitOr,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: BitOr,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: BitOr,
// DEFAULT-NEXT:                                                                   left: Binary {
// DEFAULT-NEXT:                                                                       op: BitOr,
// DEFAULT-NEXT:                                                                       left: Binary {
// DEFAULT-NEXT:                                                                           op: BitOr,
// DEFAULT-NEXT:                                                                           left: Binary {
// DEFAULT-NEXT:                                                                               op: BitOr,
// DEFAULT-NEXT:                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                   Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                           Binary {
// DEFAULT-NEXT:                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                               left: Integer(
// DEFAULT-NEXT:                                                                                                   1,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                   1,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                           1,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Paren(
// DEFAULT-NEXT:                                                                                   Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                           Binary {
// DEFAULT-NEXT:                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                               left: Integer(
// DEFAULT-NEXT:                                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                           1,
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                           1,
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                   1,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                           1,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           right: Paren(
// DEFAULT-NEXT:                                                                               Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                   left: Integer(
// DEFAULT-NEXT:                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                               1,
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                       1,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                               1,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                   Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                           Binary {
// DEFAULT-NEXT:                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                               left: Integer(
// DEFAULT-NEXT:                                                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                           1,
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                           1,
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                           1,
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                   1,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                           1,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                   1,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                           left: Paren(
// DEFAULT-NEXT:                                                                               Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                                                                   left: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                               1,
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                       1,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                               1,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Integer(
// DEFAULT-NEXT:                                                                               1,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                   left: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                           left: Paren(
// DEFAULT-NEXT:                                                                               Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Integer(
// DEFAULT-NEXT:                                                                               1,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Integer(
// DEFAULT-NEXT:                                                                       1,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Paren(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                               left: Paren(
// DEFAULT-NEXT:                                                                   Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                   Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Integer(
// DEFAULT-NEXT:                                                                                           1,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                           1,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                   1,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Integer(
// DEFAULT-NEXT:                                                                           1,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                   left: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                           left: Paren(
// DEFAULT-NEXT:                                                                               Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                   left: Integer(
// DEFAULT-NEXT:                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                               1,
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                       1,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                               1,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Integer(
// DEFAULT-NEXT:                                                                               1,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Integer(
// DEFAULT-NEXT:                                                                       1,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                               left: Paren(
// DEFAULT-NEXT:                                                                   Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                   Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                           Binary {
// DEFAULT-NEXT:                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                       left: Integer(
// DEFAULT-NEXT:                                                                                                                                                           1,
// DEFAULT-NEXT:                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                                           1,
// DEFAULT-NEXT:                                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                                           1,
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                                           1,
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                                   1,
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                                           1,
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                                   1,
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Integer(
// DEFAULT-NEXT:                                                                                           1,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                   1,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Integer(
// DEFAULT-NEXT:                                                                           1,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                   left: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                           left: Paren(
// DEFAULT-NEXT:                                                                               Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                                   left: Integer(
// DEFAULT-NEXT:                                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                               1,
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                       1,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                               1,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Integer(
// DEFAULT-NEXT:                                                                               1,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Integer(
// DEFAULT-NEXT:                                                                       1,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Paren(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: ShiftLeft,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                       left: Paren(
// DEFAULT-NEXT:                                                           Binary {
// DEFAULT-NEXT:                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                               left: Paren(
// DEFAULT-NEXT:                                                                   Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                               left: Integer(
// DEFAULT-NEXT:                                                                                   1,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Integer(
// DEFAULT-NEXT:                                                                                   1,
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Integer(
// DEFAULT-NEXT:                                                                           1,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: ShiftLeft,
// DEFAULT-NEXT:                                           left: Paren(
// DEFAULT-NEXT:                                               Binary {
// DEFAULT-NEXT:                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                   left: Paren(
// DEFAULT-NEXT:                                                       Binary {
// DEFAULT-NEXT:                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                   left: Paren(
// DEFAULT-NEXT:                                                                       Binary {
// DEFAULT-NEXT:                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                           left: Paren(
// DEFAULT-NEXT:                                                                               Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                                                                                           left: Integer(
// DEFAULT-NEXT:                                                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                                               1,
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                                       1,
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                                               1,
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                                       1,
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Integer(
// DEFAULT-NEXT:                                                                                               1,
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Integer(
// DEFAULT-NEXT:                                                                                       1,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Integer(
// DEFAULT-NEXT:                                                                               1,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Integer(
// DEFAULT-NEXT:                                                                       1,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: Assign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "error",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "re_compile_pattern",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               StringLit(
// DEFAULT-NEXT:                                   "sl(a|e)te",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   9,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Unary {
// DEFAULT-NEXT:                                   op: AddrOf,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "expression",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "error",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Paren(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "re_match",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Identifier(
// DEFAULT-NEXT:                                           "expression",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "slate",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       5,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Paren(
// DEFAULT-NEXT:                                       Cast {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                           declarator: Pointer {
// DEFAULT-NEXT:                                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                                               inner: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               5,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "regfree",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: AddrOf,
// DEFAULT-NEXT:                               operand: Identifier(
// DEFAULT-NEXT:                                   "expression",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "glob",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "/dev/{null,zero}",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       1024,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Paren(
// DEFAULT-NEXT:                                       Cast {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                           declarator: Pointer {
// DEFAULT-NEXT:                                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                                               inner: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Identifier(
// DEFAULT-NEXT:                                           "paths",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "paths",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "gl_pathc",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               2,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "strcmp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Index {
// DEFAULT-NEXT:                                       base: Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "paths",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "gl_pathv",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "/dev/null",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "strcmp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Index {
// DEFAULT-NEXT:                                       base: Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "paths",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "gl_pathv",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   StringLit(
// DEFAULT-NEXT:                                       "/dev/zero",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "globfree",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: AddrOf,
// DEFAULT-NEXT:                               operand: Identifier(
// DEFAULT-NEXT:                                   "paths",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "total",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 60,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[55]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "gnu_runtime_extensions",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: Some(
// DEFAULT-NEXT:                                       false,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "random_bytes",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "frames",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           8,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "Dl_info",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "information",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Long,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "page_size",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "sysconf",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   30,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "total",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "getauxval",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       6,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Cast {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Identifier(
// DEFAULT-NEXT:                                   "page_size",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "gettid",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Cast {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "pid_t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               value: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "syscall",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "__NR_gettid",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "getentropy",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "random_bytes",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   SizeOfExpr(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "random_bytes",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "arc4random_uniform",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Greater,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "get_nprocs",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Greater,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "get_phys_pages",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: Greater,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "backtrace",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "frames",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       8,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "dladdr",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Cast {
// DEFAULT-NEXT:                                       ty: Void,
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Abstract,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       value: Unary {
// DEFAULT-NEXT:                                           op: AddrOf,
// DEFAULT-NEXT:                                           operand: Identifier(
// DEFAULT-NEXT:                                               "gnu_runtime_extensions",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: AddrOf,
// DEFAULT-NEXT:                                       operand: Identifier(
// DEFAULT-NEXT:                                           "information",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "information",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "dli_fname",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Paren(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Index {
// DEFAULT-NEXT:                               base: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "gnu_get_libc_version",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "program_invocation_name",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Paren(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Assign {
// DEFAULT-NEXT:                       op: AddAssign,
// DEFAULT-NEXT:                       target: Identifier(
// DEFAULT-NEXT:                           "total",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       value: Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "program_invocation_short_name",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Paren(
// DEFAULT-NEXT:                               Cast {
// DEFAULT-NEXT:                                   ty: Void,
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "total",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 81,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           storage: Static,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[56]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLit(
// DEFAULT-NEXT:                               "%d %d %d\\n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "gnu_environment_extensions",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "gnu_time_extensions",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "gnu_pattern_extensions",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "gnu_runtime_extensions",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Integer(
// DEFAULT-NEXT:                       0,
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 103,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
