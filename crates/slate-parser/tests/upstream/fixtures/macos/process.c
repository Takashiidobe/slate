#include <dlfcn.h>
#include <fnmatch.h>
#include <poll.h>
#include <pwd.h>
#include <spawn.h>
#include <stddef.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

_Static_assert(sizeof(struct termios) == 72, "termios size");
_Static_assert(sizeof(struct passwd) == 72, "passwd size");
_Static_assert(sizeof(posix_spawnattr_t) == 8, "posix_spawnattr_t size");
_Static_assert(P_ALL == 0, "P_ALL");
_Static_assert(P_PID == 1, "P_PID");
_Static_assert(FNM_NOESCAPE == 0x01, "FNM_NOESCAPE");
_Static_assert(_SC_PAGESIZE == 29, "_SC_PAGESIZE");
_Static_assert(_SC_NPROCESSORS_ONLN == 58, "_SC_NPROCESSORS_ONLN");

int slate_spawn_and_wait(pid_t *child, const char *path, char *const argv[],
                         char *const envp[]) {
  int status = 0;
  int result = posix_spawn(child, path, NULL, NULL, argv, envp);
  if (result != 0)
    return result;
  return waitpid(*child, &status, 0) < 0 ? -1 : status;
}

long slate_terminal_page_size(int fd, struct termios *state) {
  if (tcgetattr(fd, state) != 0)
    return -1;
  return sysconf(_SC_PAGESIZE);
}

int slate_lookup_user_shell(uid_t uid, char *buffer, size_t size) {
  struct passwd *entry = getpwuid(uid);
  if (entry == NULL)
    return -1;
  size_t length = 0;
  while (entry->pw_shell[length] != '\0' && length + 1 < size)
    length++;
  __builtin_memcpy(buffer, entry->pw_shell, length);
  buffer[length] = '\0';
  return 0;
}


