struct __attribute__((packed)) Packed {
  char tag;
  int value;
};

union __attribute__((packed)) Pair {
  int left;
  char right;
};

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: struct name=Packed [attributes=packed]
// DEFAULT-NEXT:   field: type=char declarator=name=tag
// DEFAULT-NEXT:   field: type=int declarator=name=value
// DEFAULT-NEXT: decl[1]: union name=Pair [attributes=packed]
// DEFAULT-NEXT:   field: type=int declarator=name=left
// DEFAULT-NEXT:   field: type=char declarator=name=right
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: struct name=Packed
// DEFAULT-NEXT: decl[1]: union name=Pair
// SLATE-FILECHECK-END DEFAULT
