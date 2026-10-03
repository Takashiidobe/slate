#include <stdint.h>
#include <stdio.h>

typedef struct client {
    int writes;
    int reads;
} client;

typedef void command_proc(client *c);

void eval_command(client *c) { c->writes += 1; }

void eval_ro_command(client *c) { eval_command(c); }

static void hash_command(client *c) { c->reads += 2; }

static void hash_ro_command(client *c) { hash_command(c); }

struct command {
    const char *name;
    command_proc *proc;
};

static struct command table[] = {
    {"eval", eval_command},
    {"eval_ro", eval_ro_command},
    {"hash", hash_command},
    {"hash_ro", hash_ro_command},
};

int main(void) {
    client c = {0, 0};
    for (unsigned i = 0; i < sizeof table / sizeof table[0]; i++) {
        struct command *cmd = &table[i];
        int ro = cmd->proc == eval_ro_command || cmd->proc == hash_ro_command;
        uintptr_t address = (uintptr_t)cmd->proc;
        int distinct = 1;
        for (unsigned j = 0; j < sizeof table / sizeof table[0]; j++)
            if (j != i && (uintptr_t)table[j].proc == address)
                distinct = 0;
        if (!ro)
            cmd->proc(&c);
        printf("%s ro=%d distinct=%d\n", cmd->name, ro, distinct);
    }
    printf("writes=%d reads=%d\n", c.writes, c.reads);
    return 0;
}
