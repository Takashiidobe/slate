#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char **argv, char **envp) {
    printf("envp %d argc %d\n", envp == environ, argc);
    size_t len = strlen(argv[0]);
    memset(argv[0], 0, len);
    memcpy(argv[0], "renamed", len < 7 ? len : 7);
    char buf[64] = {0};
    int fd = open("/proc/self/cmdline", O_RDONLY);
    ssize_t n = read(fd, buf, sizeof buf - 1);
    close(fd);
    printf("%d %s\n", n > 0, buf);
    printf("environ after argv %d\n", environ[0] == NULL || environ[0] > argv[argc - 1]);
    return 0;
}
