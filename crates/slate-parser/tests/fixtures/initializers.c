int scalar = 7;
char message[6] = "hello";
int matrix[2][2] = {{1, 2}, {3, 4}};
struct Point {
  int x;
  int y;
};
struct Point point = {.y = 9, .x = 4};
int values[4] = {[2] = 8, [0] = 1};
int selected =
#ifdef ENABLED
  11;
#else
  22;
#endif

int main() {
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES ENABLED ENABLED

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=scalar initializer=expr(7)
// DEFAULT-NEXT: decl[1]: declaration type=char declarator=array(name=message,size=6) initializer=expr(string=hello)
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=array(array(name=matrix,size=2),size=2) initializer=list[list[expr(1),expr(2)],list[expr(3),expr(4)]]
// DEFAULT-NEXT: decl[3]: struct name=Point
// DEFAULT-NEXT:   field: type=int declarator=name=x
// DEFAULT-NEXT:   field: type=int declarator=name=y
// DEFAULT-NEXT: decl[4]: declaration type=struct Point declarator=name=point initializer=list[field(y)=expr(9),field(x)=expr(4)]
// DEFAULT-NEXT: decl[5]: declaration type=int declarator=array(name=values,size=4) initializer=list[array(2)=expr(8),array(0)=expr(1)]
// DEFAULT-NEXT: decl[6]: declaration type=int declarator=name=selected initializer=conditional[when=defined(ENABLED)=>expr(11),when=not(defined(ENABLED))=>expr(22)]
// DEFAULT-NEXT: decl[7]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 0
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=scalar initializer=expr(7)
// DEFAULT-NEXT: decl[1]: declaration type=char declarator=array(name=message,size=6) initializer=expr(string=hello)
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=array(array(name=matrix,size=2),size=2) initializer=list[list[expr(1),expr(2)],list[expr(3),expr(4)]]
// DEFAULT-NEXT: decl[3]: struct name=Point
// DEFAULT-NEXT: decl[4]: declaration type=struct Point declarator=name=point initializer=list[field(y)=expr(9),field(x)=expr(4)]
// DEFAULT-NEXT: decl[5]: declaration type=int declarator=array(name=values,size=4) initializer=list[array(2)=expr(8),array(0)=expr(1)]
// DEFAULT-NEXT: decl[6]: declaration type=int declarator=name=selected initializer=expr(22)
// DEFAULT-NEXT: decl[7]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 0
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ENABLED
// ENABLED: polyvariant:
// ENABLED-NEXT: decl[0]: declaration type=int declarator=name=scalar initializer=expr(7)
// ENABLED-NEXT: decl[1]: declaration type=char declarator=array(name=message,size=6) initializer=expr(string=hello)
// ENABLED-NEXT: decl[2]: declaration type=int declarator=array(array(name=matrix,size=2),size=2) initializer=list[list[expr(1),expr(2)],list[expr(3),expr(4)]]
// ENABLED-NEXT: decl[3]: struct name=Point
// ENABLED-NEXT:   field: type=int declarator=name=x
// ENABLED-NEXT:   field: type=int declarator=name=y
// ENABLED-NEXT: decl[4]: declaration type=struct Point declarator=name=point initializer=list[field(y)=expr(9),field(x)=expr(4)]
// ENABLED-NEXT: decl[5]: declaration type=int declarator=array(name=values,size=4) initializer=list[array(2)=expr(8),array(0)=expr(1)]
// ENABLED-NEXT: decl[6]: declaration type=int declarator=name=selected initializer=conditional[when=defined(ENABLED)=>expr(11),when=not(defined(ENABLED))=>expr(22)]
// ENABLED-NEXT: decl[7]: function name=main return=int
// ENABLED-NEXT:   stmt[0]: return 0
// ENABLED-NEXT: concrete:
// ENABLED-NEXT: decl[0]: declaration type=int declarator=name=scalar initializer=expr(7)
// ENABLED-NEXT: decl[1]: declaration type=char declarator=array(name=message,size=6) initializer=expr(string=hello)
// ENABLED-NEXT: decl[2]: declaration type=int declarator=array(array(name=matrix,size=2),size=2) initializer=list[list[expr(1),expr(2)],list[expr(3),expr(4)]]
// ENABLED-NEXT: decl[3]: struct name=Point
// ENABLED-NEXT: decl[4]: declaration type=struct Point declarator=name=point initializer=list[field(y)=expr(9),field(x)=expr(4)]
// ENABLED-NEXT: decl[5]: declaration type=int declarator=array(name=values,size=4) initializer=list[array(2)=expr(8),array(0)=expr(1)]
// ENABLED-NEXT: decl[6]: declaration type=int declarator=name=selected initializer=expr(11)
// ENABLED-NEXT: decl[7]: function name=main return=int
// ENABLED-NEXT:   stmt[0]: return 0
// SLATE-FILECHECK-END ENABLED
