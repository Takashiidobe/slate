int values[3][4];
int (*handler)(char, int *);
int *factory(void);
int callback(int value, ...);

int main() {
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=array(array(name=values,size=3),size=4)
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(pointer(name=handler),params=char,ptr(int),variadic=false)
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(pointer(name=factory),params=,variadic=false)
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=function(name=callback,params=int value,variadic=true)
// DEFAULT-NEXT: decl[4]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 0
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=array(array(name=values,size=3),size=4)
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(pointer(name=handler),params=char,ptr(int),variadic=false)
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(pointer(name=factory),params=,variadic=false)
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=function(name=callback,params=int value,variadic=true)
// DEFAULT-NEXT: decl[4]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 0
// SLATE-FILECHECK-END DEFAULT
