// SLATE-FILECHECK-DEFINES NAMES
// SLATE-FILECHECK-ARGS --dump-ir-names

int prototype_type(void) {
    goto prototype;
    int (*callback)(int values[({ prototype: 1; })]);
    return 0;
}

int expression(void) {
    return ({ goto done; done: 1; });
}

int initializer(void) {
    void *target = &&done;
    int value = ({ goto done; done: 2; });
    return value;
}

int type_expression(void) {
    goto done;
    __typeof__(({ done: 1; })) value = 0;
    return value;
}

int array_type(void) {
    void *target = &&bound;
    int values[({ bound: 2; })];
    return sizeof(values);
}

int type_name(void) {
    void *target = &&sized;
    return sizeof(int[({ sized: 2; })]);
}

int aggregate_initializer(void) {
    void *target = &&element;
    int values[] = { ({ element: 2; }) };
    return values[0];
}

int condition(int value) {
    void *target = &&checked;
    if (({ checked: value; })) return 1;
    return 0;
}

int loop_expressions(void) {
    void *targets[] = { &&initial, &&condition, &&increment, &&while_test, &&do_test };
    for (int value = ({ initial: 0; }); ({ condition: value < 1; }); ({ increment: ++value; })) ;
    while (({ while_test: 0; })) ;
    do ; while (({ do_test: 0; }));
    return 0;
}

int operands(void) {
    void *target = &&operand;
    __asm__("" : : "r" (({ operand: 1; })));
    return 0;
}

int statement_locals(void) {
    int first = ({ __label__ done; goto done; done: 1; });
    int second = ({ __label__ done; goto done; done: 2; });
    return first + second;
}

int outward(void) {
    void *target = &&done;
    return ({ done: 1; });
}

// SLATE-FILECHECK-BEGIN NAMES
// NAMES: bind function prototype_type = %0
// NAMES-NEXT: bind label prototype = %1
// NAMES-NEXT: bind object callback = %2
// NAMES-NEXT: bind function expression = %3
// NAMES-NEXT: bind label done = %4
// NAMES-NEXT: bind function initializer = %5
// NAMES-NEXT: bind label done#1 = %6
// NAMES-NEXT: bind object target = %7
// NAMES-NEXT: bind object value = %8
// NAMES-NEXT: bind function type_expression = %9
// NAMES-NEXT: bind label done#2 = %10
// NAMES-NEXT: bind object value#1 = %11
// NAMES-NEXT: bind function array_type = %12
// NAMES-NEXT: bind label bound = %13
// NAMES-NEXT: bind object target#1 = %14
// NAMES-NEXT: bind object values = %15
// NAMES-NEXT: bind function type_name = %16
// NAMES-NEXT: bind label sized = %17
// NAMES-NEXT: bind object target#2 = %18
// NAMES-NEXT: bind function aggregate_initializer = %19
// NAMES-NEXT: bind label element = %20
// NAMES-NEXT: bind object target#3 = %21
// NAMES-NEXT: bind object values#1 = %22
// NAMES-NEXT: bind function condition = %23
// NAMES-NEXT: bind label checked = %24
// NAMES-NEXT: bind parameter value#2 = %25
// NAMES-NEXT: bind object target#4 = %26
// NAMES-NEXT: bind function loop_expressions = %27
// NAMES-NEXT: bind label initial = %28
// NAMES-NEXT: bind label condition#1 = %29
// NAMES-NEXT: bind label increment = %30
// NAMES-NEXT: bind label while_test = %31
// NAMES-NEXT: bind label do_test = %32
// NAMES-NEXT: bind object targets = %33
// NAMES-NEXT: bind object value#3 = %34
// NAMES-NEXT: bind function operands = %35
// NAMES-NEXT: bind label operand = %36
// NAMES-NEXT: bind object target#5 = %37
// NAMES-NEXT: bind function statement_locals = %38
// NAMES-NEXT: bind label done#3 = %39
// NAMES-NEXT: bind label done#4 = %40
// NAMES-NEXT: bind object first = %41
// NAMES-NEXT: bind object second = %42
// NAMES-NEXT: bind function outward = %43
// NAMES-NEXT: bind label done#5 = %44
// NAMES-NEXT: bind object target#6 = %45
// NAMES-NEXT: ref label prototype -> %1
// NAMES-NEXT: ref label done -> %4
// NAMES-NEXT: ref label done#1 -> %6
// NAMES-NEXT: ref label done#1 -> %6
// NAMES-NEXT: ref object value -> %8
// NAMES-NEXT: ref label done#2 -> %10
// NAMES-NEXT: ref object value#1 -> %11
// NAMES-NEXT: ref label bound -> %13
// NAMES-NEXT: ref object values -> %15
// NAMES-NEXT: ref label sized -> %17
// NAMES-NEXT: ref label element -> %20
// NAMES-NEXT: ref object values#1 -> %22
// NAMES-NEXT: ref label checked -> %24
// NAMES-NEXT: ref parameter value#2 -> %25
// NAMES-NEXT: ref label initial -> %28
// NAMES-NEXT: ref label condition#1 -> %29
// NAMES-NEXT: ref label increment -> %30
// NAMES-NEXT: ref label while_test -> %31
// NAMES-NEXT: ref label do_test -> %32
// NAMES-NEXT: ref object value#3 -> %34
// NAMES-NEXT: ref object value#3 -> %34
// NAMES-NEXT: ref label operand -> %36
// NAMES-NEXT: ref label done#3 -> %39
// NAMES-NEXT: ref label done#4 -> %40
// NAMES-NEXT: ref object first -> %41
// NAMES-NEXT: ref object second -> %42
// NAMES-NEXT: ref label done#5 -> %44
// SLATE-FILECHECK-END NAMES
