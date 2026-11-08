#include <windows.h>
#include <stdio.h>
#include "MinHook.h"

typedef DWORD (WINAPI *GETTICKCOUNT)(void);
typedef DWORD (WINAPI *GETCURRENTPROCESSID)(void);

static volatile int sink;
static int data_target[16];

__declspec(noinline) int target(int x) {
    int i, acc = x;
    for (i = 0; i < 3; i++) { acc = acc * 3 + 1; sink = acc; }
    return acc;
}

__declspec(noinline) int second(int x, int y) {
    int i, acc;
    sink = x;
    sink = y;
    acc = sink + x;
    for (i = 0; i < y; i++) { acc ^= acc << 1; sink = acc; }
    return acc;
}

static int (*original_target)(int);
static int detour_target(int x) { return original_target(x) + 1000; }

static int (*original_second)(int, int);
static int detour_second(int x, int y) { return -original_second(x, y); }

static GETTICKCOUNT original_tick;
static DWORD WINAPI detour_tick(void) { return 42; }

static GETCURRENTPROCESSID original_pid;
static DWORD WINAPI detour_pid(void) { return original_pid() + 7; }

#define SHOW(label, call) printf("%-24s %s\n", label, MH_StatusToString(call))

int main(void) {
    int before = target(1);
    int second_before = second(5, 4);
    DWORD real_pid = GetCurrentProcessId();
    LPVOID scratch = NULL;
    setvbuf(stdout, NULL, _IONBF, 0);

    SHOW("create before init", MH_CreateHook(&target, &detour_target, NULL));
    SHOW("enable before init", MH_EnableHook(MH_ALL_HOOKS));
    SHOW("uninit before init", MH_Uninitialize());
    SHOW("init", MH_Initialize());
    SHOW("init again", MH_Initialize());

    SHOW("create data target", MH_CreateHook(data_target, &detour_target, NULL));
    SHOW("create data detour", MH_CreateHook(&target, data_target, NULL));
    SHOW("enable missing", MH_EnableHook(&target));
    SHOW("remove missing", MH_RemoveHook(&target));
    SHOW("create", MH_CreateHook(&target, &detour_target, (LPVOID *)&original_target));
    SHOW("create dup", MH_CreateHook(&target, &detour_target, NULL));
    SHOW("disable disabled", MH_DisableHook(&target));
    SHOW("enable", MH_EnableHook(&target));
    SHOW("enable enabled", MH_EnableHook(&target));
    printf("hooked %d %d\n", before, target(1));
    printf("original %d\n", original_target(1));
    SHOW("disable", MH_DisableHook(&target));
    printf("unhooked %d\n", target(1));

    SHOW("create second", MH_CreateHook(&second, &detour_second, (LPVOID *)&original_second));
    SHOW("queue enable second", MH_QueueEnableHook(&second));
    printf("queued %d\n", second(5, 4) == second_before);
    SHOW("apply", MH_ApplyQueued());
    printf("second %d %d\n", second_before, second(5, 4));
    SHOW("queue disable missing", MH_QueueDisableHook(data_target));
    SHOW("queue disable second", MH_QueueDisableHook(&second));
    SHOW("apply", MH_ApplyQueued());
    printf("second restored %d\n", second(5, 4) == second_before);

    SHOW("api no module", MH_CreateHookApi(L"slate_no_such_module", "GetTickCount", &detour_tick, NULL));
    SHOW("api no function", MH_CreateHookApi(L"kernel32", "SlateNoSuchFunction", &detour_tick, NULL));
    SHOW("api tick", MH_CreateHookApi(L"kernel32", "GetTickCount", &detour_tick, (LPVOID *)&original_tick));
    SHOW("api pid", MH_CreateHookApiEx(L"kernel32", "GetCurrentProcessId", &detour_pid,
                                       (LPVOID *)&original_pid, &scratch));
    printf("api target found %d\n", scratch == (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32"), "GetCurrentProcessId"));
    SHOW("queue all", MH_QueueEnableHook(MH_ALL_HOOKS));
    SHOW("apply all", MH_ApplyQueued());
    printf("tick %lu\n", GetTickCount());
    printf("pid shifted %d\n", GetCurrentProcessId() == real_pid + 7);
    printf("all hooked %d %d\n", target(1), second(5, 4));
    SHOW("disable all", MH_DisableHook(MH_ALL_HOOKS));
    printf("tick restored %d\n", GetTickCount() != 42);
    printf("pid restored %d\n", GetCurrentProcessId() == real_pid);
    printf("all restored %d %d\n", target(1), second(5, 4));
    SHOW("enable all", MH_EnableHook(MH_ALL_HOOKS));
    printf("rehooked %d\n", target(1));

    SHOW("remove enabled", MH_RemoveHook(&target));
    printf("removed %d\n", target(1));
    SHOW("remove again", MH_RemoveHook(&target));
    SHOW("uninit", MH_Uninitialize());
    printf("uninit restored %d %d %d\n", second(5, 4) == second_before, GetTickCount() != 42,
           GetCurrentProcessId() == real_pid);
    SHOW("uninit again", MH_Uninitialize());
    printf("status unknown %s\n", MH_StatusToString((MH_STATUS)1234));
    return 0;
}
