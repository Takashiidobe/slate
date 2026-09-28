// SLATE-FILECHECK-DEFINES NAMES
// SLATE-FILECHECK-ARGS --dump-ir-names

int unwrapped(void) {
    __label__ done;
    goto done;
done:
    return 0;
}

int siblings(void) {
    { __label__ done; goto done; done: ; }
    { __label__ done; goto done; done: ; }
    return 0;
}

int shadowing(void) {
    __label__ done;
    { __label__ done; goto done; done: ; }
    goto done;
done:
    return 0;
}

int nested_blocks(void) {
    __label__ done;
    {
        goto done;
done:
        ;
    }
    return 0;
}

int function_shadow(void) {
    goto done;
    {
        __label__ done;
        void *address = &&done;
        __asm__ goto ("" : : : : done);
        goto done;
        done: ;
    }
    goto done;
    done: return 0;
}

int attributed(void) {
    __label__ done;
    [[likely]] if (1) [[likely]] { goto done; }
    [[likely]] done: return 0;
}

int nested_functions(void) {
    goto done;
    int nested(void) { goto done; done: return 1; }
    goto done;
    done: return nested();
}

void inner(void) {
    __label__ first;
    goto first;
first:
    return;
}

void later(void) {
    __label__ second;
    goto second;
second:
    return;
}

// SLATE-FILECHECK-BEGIN NAMES
// NAMES: bind function unwrapped = %0
// NAMES-NEXT: bind label done = %1
// NAMES-NEXT: bind function siblings = %2
// NAMES-NEXT: bind label done#1 = %3
// NAMES-NEXT: bind label done#2 = %4
// NAMES-NEXT: bind function shadowing = %5
// NAMES-NEXT: bind label done#3 = %6
// NAMES-NEXT: bind label done#4 = %7
// NAMES-NEXT: bind function nested_blocks = %8
// NAMES-NEXT: bind label done#5 = %9
// NAMES-NEXT: bind function function_shadow = %10
// NAMES-NEXT: bind label done#6 = %11
// NAMES-NEXT: bind label done#7 = %12
// NAMES-NEXT: bind object address = %13
// NAMES-NEXT: bind function attributed = %14
// NAMES-NEXT: bind label done#8 = %15
// NAMES-NEXT: bind function nested_functions = %16
// NAMES-NEXT: bind label done#9 = %17
// NAMES-NEXT: bind function nested = %18
// NAMES-NEXT: bind label done#10 = %19
// NAMES-NEXT: bind function inner = %20
// NAMES-NEXT: bind label first = %21
// NAMES-NEXT: bind function later = %22
// NAMES-NEXT: bind label second = %23
// NAMES-NEXT: ref label done -> %1
// NAMES-NEXT: ref label done#1 -> %3
// NAMES-NEXT: ref label done#2 -> %4
// NAMES-NEXT: ref label done#4 -> %7
// NAMES-NEXT: ref label done#3 -> %6
// NAMES-NEXT: ref label done#5 -> %9
// NAMES-NEXT: ref label done#7 -> %12
// NAMES-NEXT: ref label done#6 -> %11
// NAMES-NEXT: ref label done#6 -> %11
// NAMES-NEXT: ref label done#6 -> %11
// NAMES-NEXT: ref label done#7 -> %12
// NAMES-NEXT: ref label done#8 -> %15
// NAMES-NEXT: ref label done#9 -> %17
// NAMES-NEXT: ref label done#10 -> %19
// NAMES-NEXT: ref label done#9 -> %17
// NAMES-NEXT: ref function nested -> %18
// NAMES-NEXT: ref label first -> %21
// NAMES-NEXT: ref label second -> %23
// SLATE-FILECHECK-END NAMES
