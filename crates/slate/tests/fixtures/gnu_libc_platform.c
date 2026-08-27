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
