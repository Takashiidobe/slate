// SLATE-FILECHECK-DEFINES NAMES
// SLATE-FILECHECK-ARGS --dump-ir-names

typedef int word;
struct Node { int value; };
enum Choice { FIRST, SECOND = FIRST + 1 };

int global;
int Node;

int resolve(word value) {
    struct Node *node = 0;
    int global = value;
    {
        int value = global;
        global = value;
    }
target:
    if (global)
        goto target;
    return FIRST + global + Node;
}

// SLATE-FILECHECK-BEGIN NAMES
// NAMES: bind typedef word = %0
// NAMES-NEXT: bind tag Node = %1
// NAMES-NEXT: bind tag Choice = %2
// NAMES-NEXT: bind enumerator FIRST = %3
// NAMES-NEXT: bind enumerator SECOND = %4
// NAMES-NEXT: bind object global = %5
// NAMES-NEXT: bind object Node#1 = %6
// NAMES-NEXT: bind function resolve = %7
// NAMES-NEXT: bind label target = %8
// NAMES-NEXT: bind parameter value = %9
// NAMES-NEXT: bind object node = %10
// NAMES-NEXT: bind object global#1 = %11
// NAMES-NEXT: bind object value#1 = %12
// NAMES-NEXT: ref enumerator FIRST -> %3
// NAMES-NEXT: ref typedef word -> %0
// NAMES-NEXT: ref tag Node -> %1
// NAMES-NEXT: ref parameter value -> %9
// NAMES-NEXT: ref object global#1 -> %11
// NAMES-NEXT: ref object global#1 -> %11
// NAMES-NEXT: ref object value#1 -> %12
// NAMES-NEXT: ref object global#1 -> %11
// NAMES-NEXT: ref label target -> %8
// NAMES-NEXT: ref enumerator FIRST -> %3
// NAMES-NEXT: ref object global#1 -> %11
// NAMES-NEXT: ref object Node#1 -> %6
// SLATE-FILECHECK-END NAMES
