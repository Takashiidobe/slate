#include <stdio.h>

/* typedef doc */
typedef struct Foo Foo;

/* struct doc */
struct Foo { int a; };

/* only typedef doc */
typedef struct Bar Bar;
struct Bar { int b; };

/* prototype doc */
int f(int x);
/* prototype only doc */
int h(int x);

/* definition doc */
int f(int x) { return x + 1; }
int h(int x) { return x * 2; }

extern int counter;
/* counter definition */
int counter = 4;

int main(void) {
    Foo foo = {1};
    Bar bar = {2};
    printf("%d %d %d\n", f(foo.a), h(bar.b), counter);
    return 0;
}
