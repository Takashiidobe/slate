struct Point {
  int x;
  int y;
};

union Value {
  int i;
  char c;
};

struct {
  int z;
};

enum Color {
  RED,
  GREEN = 3,
  BLUE,
};

struct Forward;
struct Point point;
enum Color color;
typedef struct Point PointAlias;
PointAlias alias;

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: struct name=Point
// DEFAULT-NEXT:   field: type=int declarator=name=x
// DEFAULT-NEXT:   field: type=int declarator=name=y
// DEFAULT-NEXT: decl[1]: union name=Value
// DEFAULT-NEXT:   field: type=int declarator=name=i
// DEFAULT-NEXT:   field: type=char declarator=name=c
// DEFAULT-NEXT: decl[2]: struct name=<anonymous>
// DEFAULT-NEXT:   field: type=int declarator=name=z
// DEFAULT-NEXT: decl[3]: enum name=Color
// DEFAULT-NEXT:   enumerator: name=RED value=implicit
// DEFAULT-NEXT:   enumerator: name=GREEN value=3
// DEFAULT-NEXT:   enumerator: name=BLUE value=implicit
// DEFAULT-NEXT: decl[4]: declaration type=struct Forward declarator=_
// DEFAULT-NEXT: decl[5]: declaration type=struct Point declarator=name=point
// DEFAULT-NEXT: decl[6]: declaration type=enum Color declarator=name=color
// DEFAULT-NEXT: decl[7]: typedef name=PointAlias type=struct Point
// DEFAULT-NEXT: decl[8]: declaration type=PointAlias declarator=name=alias
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: struct name=Point
// DEFAULT-NEXT: decl[1]: union name=Value
// DEFAULT-NEXT: decl[2]: struct name=<anonymous>
// DEFAULT-NEXT: decl[3]: enum name=Color
// DEFAULT-NEXT: decl[4]: declaration type=struct Forward declarator=_
// DEFAULT-NEXT: decl[5]: declaration type=struct Point declarator=name=point
// DEFAULT-NEXT: decl[6]: declaration type=enum Color declarator=name=color
// DEFAULT-NEXT: decl[7]: typedef name=PointAlias type=struct Point
// DEFAULT-NEXT: decl[8]: declaration type=PointAlias declarator=name=alias
// SLATE-FILECHECK-END DEFAULT
