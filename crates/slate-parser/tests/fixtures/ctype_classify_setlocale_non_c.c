#include <ctype.h>
#include <locale.h>
#include <stdio.h>

int main(void) {
  setlocale(LC_ALL, "");
  char c = 'A';
  if (isalpha(c)) {
    printf("yes\n");
  } else {
    printf("no\n");
  }
  if (isdigit(c)) {
    printf("digit\n");
  } else {
    printf("not-digit\n");
  }
  return 0;
}





// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[{{[0-9]+}}]: Spanned {
// DEFAULT-NEXT:       value: TagDefinition {
// DEFAULT-NEXT:           id: TagId(
// DEFAULT-NEXT:               [[#TAG0:]],
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: Enum,
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:           body: Enum {
// DEFAULT-NEXT:               enumerators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISupper",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 0,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "0",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0:]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 0,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 0,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "0",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISlower",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 1,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 1,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "1",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 1,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISalpha",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 2,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "2",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 2,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "2",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 2,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "2",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISdigit",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 3,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "3",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 3,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "3",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 3,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "3",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISxdigit",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 4,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "4",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 4,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "4",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 4,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "4",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISspace",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 5,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "5",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 5,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "5",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 5,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "5",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISprint",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 6,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "6",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 6,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "6",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 6,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "6",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISgraph",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 7,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "7",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 7,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "7",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 7,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "7",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISblank",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 8,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "8",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_IScntrl",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 9,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "9",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 9,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "9",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 9,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "9",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISpunct",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 10,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "10",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 10,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "10",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 10,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "10",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: Enumerator(
// DEFAULT-NEXT:                           Enumerator {
// DEFAULT-NEXT:                               name: "_ISalnum",
// DEFAULT-NEXT:                               value: Some(
// DEFAULT-NEXT:                                   Spanned {
// DEFAULT-NEXT:                                       value: Paren(
// DEFAULT-NEXT:                                           Spanned {
// DEFAULT-NEXT:                                               value: Conditional {
// DEFAULT-NEXT:                                                   condition: Spanned {
// DEFAULT-NEXT:                                                       value: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Spanned {
// DEFAULT-NEXT:                                                               value: Paren(
// DEFAULT-NEXT:                                                                   Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 11,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "11",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Spanned {
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 8,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_value: Some(
// DEFAULT-NEXT:                                                       Spanned {
// DEFAULT-NEXT:                                                           value: Paren(
// DEFAULT-NEXT:                                                               Spanned {
// DEFAULT-NEXT:                                                                   value: Binary {
// DEFAULT-NEXT:                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                           value: Paren(
// DEFAULT-NEXT:                                                                               Spanned {
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Spanned {
// DEFAULT-NEXT:                                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                               Spanned {
// DEFAULT-NEXT:                                                                                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 11,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "11",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                                           FileId(
// DEFAULT-NEXT:                                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               kind: System,
// DEFAULT-NEXT:                                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                                   FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                                       file: FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       kind: System,
// DEFAULT-NEXT:                                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                                           FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Spanned {
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                                               file: FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               kind: System,
// DEFAULT-NEXT:                                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                               system_header: Some(
// DEFAULT-NEXT:                                                                                   FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                                       file: FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       kind: System,
// DEFAULT-NEXT:                                                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                       system_header: Some(
// DEFAULT-NEXT:                                                                           FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                                               system_header: Some(
// DEFAULT-NEXT:                                                                   FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_value: Spanned {
// DEFAULT-NEXT:                                                       value: Paren(
// DEFAULT-NEXT:                                                           Spanned {
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: ShiftRight,
// DEFAULT-NEXT:                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                       value: Paren(
// DEFAULT-NEXT:                                                                           Spanned {
// DEFAULT-NEXT:                                                                               value: Binary {
// DEFAULT-NEXT:                                                                                   op: ShiftLeft,
// DEFAULT-NEXT:                                                                                   left: Spanned {
// DEFAULT-NEXT:                                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                           Spanned {
// DEFAULT-NEXT:                                                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 11,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "11",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                                       FileId(
// DEFAULT-NEXT:                                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                                           file: FileId(
// DEFAULT-NEXT:                                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           kind: System,
// DEFAULT-NEXT:                                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                                               FileId(
// DEFAULT-NEXT:                                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                                       FileId(
// DEFAULT-NEXT:                                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Spanned {
// DEFAULT-NEXT:                                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                               value: 8,
// DEFAULT-NEXT:                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                   size: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                           system_header: Some(
// DEFAULT-NEXT:                                                                               FileId(
// DEFAULT-NEXT:                                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                   file: FileId(
// DEFAULT-NEXT:                                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   kind: System,
// DEFAULT-NEXT:                                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                                   system_header: Some(
// DEFAULT-NEXT:                                                                       FileId(
// DEFAULT-NEXT:                                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                           file: FileId(
// DEFAULT-NEXT:                                                               [[#FILE0]],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           kind: System,
// DEFAULT-NEXT:                                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                                           system_header: Some(
// DEFAULT-NEXT:                                                               FileId(
// DEFAULT-NEXT:                                                                   [[#FILE0]],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       [[#FILE0]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: {{[0-9]+}},
// DEFAULT-NEXT:                                                   system_header: Some(
// DEFAULT-NEXT:                                                       FileId(
// DEFAULT-NEXT:                                                           [[#FILE0]],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       provenance: Provenance {
// DEFAULT-NEXT:                                           file: FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           kind: System,
// DEFAULT-NEXT:                                           line: {{[0-9]+}},
// DEFAULT-NEXT:                                           system_header: Some(
// DEFAULT-NEXT:                                               FileId(
// DEFAULT-NEXT:                                                   [[#FILE0]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               [[#FILE0]],
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: {{[0-9]+}},
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   [[#FILE0]],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Tag(
// DEFAULT-NEXT:                       Definition(
// DEFAULT-NEXT:                           TagId(
// DEFAULT-NEXT:                               [[#TAG0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               [[#FILE0]],
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: {{[0-9]+}},
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   [[#FILE0]],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Short,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                       is_const: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   storage: Extern,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Function {
// DEFAULT-NEXT:                               inner: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "__ctype_b_loc",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               parameters: Void,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           attributes: [
// DEFAULT-NEXT:                               Spanned {
// DEFAULT-NEXT:                                   value: NoThrow,
// DEFAULT-NEXT:                                   provenance: Provenance {
// DEFAULT-NEXT:                                       file: FileId(
// DEFAULT-NEXT:                                           [[#FILE0]],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       kind: System,
// DEFAULT-NEXT:                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                       system_header: Some(
// DEFAULT-NEXT:                                           FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Spanned {
// DEFAULT-NEXT:                                   value: Const,
// DEFAULT-NEXT:                                   provenance: Provenance {
// DEFAULT-NEXT:                                       file: FileId(
// DEFAULT-NEXT:                                           [[#FILE0]],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       kind: System,
// DEFAULT-NEXT:                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                       system_header: Some(
// DEFAULT-NEXT:                                           FileId(
// DEFAULT-NEXT:                                               [[#FILE0]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE0]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE0]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               [[#FILE0]],
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: {{[0-9]+}},
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   [[#FILE0]],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Char {
// DEFAULT-NEXT:                           signed: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Extern,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Function {
// DEFAULT-NEXT:                               inner: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "setlocale",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               parameters: Prototype {
// DEFAULT-NEXT:                                   parameters: [
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Ranked {
// DEFAULT-NEXT:                                                           rank: Int,
// DEFAULT-NEXT:                                                           signed: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Name(
// DEFAULT-NEXT:                                                   "__category",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           provenance: Provenance {
// DEFAULT-NEXT:                                               file: FileId(
// DEFAULT-NEXT:                                                   [[#FILE1:]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               kind: System,
// DEFAULT-NEXT:                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       [[#FILE1]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Char {
// DEFAULT-NEXT:                                                           signed: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_const: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Pointer {
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                   inner: Name(
// DEFAULT-NEXT:                                                       "__locale",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           provenance: Provenance {
// DEFAULT-NEXT:                                               file: FileId(
// DEFAULT-NEXT:                                                   [[#FILE1]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               kind: System,
// DEFAULT-NEXT:                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       [[#FILE1]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           attributes: [
// DEFAULT-NEXT:                               Spanned {
// DEFAULT-NEXT:                                   value: NoThrow,
// DEFAULT-NEXT:                                   provenance: Provenance {
// DEFAULT-NEXT:                                       file: FileId(
// DEFAULT-NEXT:                                           [[#FILE1]],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       kind: System,
// DEFAULT-NEXT:                                       line: {{[0-9]+}},
// DEFAULT-NEXT:                                       system_header: Some(
// DEFAULT-NEXT:                                           FileId(
// DEFAULT-NEXT:                                               [[#FILE1]],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE1]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE1]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               [[#FILE1]],
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: {{[0-9]+}},
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   [[#FILE1]],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Spanned {
// DEFAULT-NEXT:       value: Declaration(
// DEFAULT-NEXT:           Declaration {
// DEFAULT-NEXT:               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   storage: Extern,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               declarators: [
// DEFAULT-NEXT:                   Spanned {
// DEFAULT-NEXT:                       value: InitDeclaratorKind {
// DEFAULT-NEXT:                           declarator: Function {
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "printf",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               parameters: Prototype {
// DEFAULT-NEXT:                                   parameters: [
// DEFAULT-NEXT:                                       Spanned {
// DEFAULT-NEXT:                                           value: ParameterDeclarationKind {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Char {
// DEFAULT-NEXT:                                                           signed: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_const: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Pointer {
// DEFAULT-NEXT:                                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                                       is_restrict: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   inner: Name(
// DEFAULT-NEXT:                                                       "__format",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           provenance: Provenance {
// DEFAULT-NEXT:                                               file: FileId(
// DEFAULT-NEXT:                                                   [[#FILE2:]],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               kind: System,
// DEFAULT-NEXT:                                               line: {{[0-9]+}},
// DEFAULT-NEXT:                                               system_header: Some(
// DEFAULT-NEXT:                                                   FileId(
// DEFAULT-NEXT:                                                       [[#FILE2]],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   variadic: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               [[#FILE2]],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: {{[0-9]+}},
// DEFAULT-NEXT:                           system_header: Some(
// DEFAULT-NEXT:                               FileId(
// DEFAULT-NEXT:                                   [[#FILE2]],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               [[#FILE2]],
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: {{[0-9]+}},
// DEFAULT-NEXT:           system_header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   [[#FILE2]],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "setlocale",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 6,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "6",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       CharLiteral(
// DEFAULT-NEXT:                                           CharLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   65,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               spelling: "A",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Paren(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: BitAnd,
// DEFAULT-NEXT:                           left: Index {
// DEFAULT-NEXT:                               base: Paren(
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Deref,
// DEFAULT-NEXT:                                       operand: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__ctype_b_loc",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               index: Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: Int,
// DEFAULT-NEXT:                                                   signed: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Paren(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Cast {
// DEFAULT-NEXT:                               ty: TypeName {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Short,
// DEFAULT-NEXT:                                               signed: false,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Identifier(
// DEFAULT-NEXT:                                   "_ISalpha",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: Block(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLiteral(
// DEFAULT-NEXT:                                           StringLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   121,
// DEFAULT-NEXT:                                                   101,
// DEFAULT-NEXT:                                                   115,
// DEFAULT-NEXT:                                                   10,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "yes\\n",
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   else_branch: Some(
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "no\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Paren(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: BitAnd,
// DEFAULT-NEXT:                           left: Index {
// DEFAULT-NEXT:                               base: Paren(
// DEFAULT-NEXT:                                   Unary {
// DEFAULT-NEXT:                                       op: Deref,
// DEFAULT-NEXT:                                       operand: Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "__ctype_b_loc",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               index: Cast {
// DEFAULT-NEXT:                                   ty: TypeName {
// DEFAULT-NEXT:                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: Int,
// DEFAULT-NEXT:                                                   signed: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       declarator: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: Paren(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "c",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Cast {
// DEFAULT-NEXT:                               ty: TypeName {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Short,
// DEFAULT-NEXT:                                               signed: false,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Identifier(
// DEFAULT-NEXT:                                   "_ISdigit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: Block(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "printf",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       StringLiteral(
// DEFAULT-NEXT:                                           StringLiteral {
// DEFAULT-NEXT:                                               encoding: Plain,
// DEFAULT-NEXT:                                               code_units: [
// DEFAULT-NEXT:                                                   100,
// DEFAULT-NEXT:                                                   105,
// DEFAULT-NEXT:                                                   103,
// DEFAULT-NEXT:                                                   105,
// DEFAULT-NEXT:                                                   116,
// DEFAULT-NEXT:                                                   10,
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               pieces: [
// DEFAULT-NEXT:                                                   "digit\\n",
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   else_branch: Some(
// DEFAULT-NEXT:                       Block(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "printf",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           StringLiteral(
// DEFAULT-NEXT:                                               StringLiteral {
// DEFAULT-NEXT:                                                   encoding: Plain,
// DEFAULT-NEXT:                                                   code_units: [
// DEFAULT-NEXT:                                                       110,
// DEFAULT-NEXT:                                                       111,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       45,
// DEFAULT-NEXT:                                                       100,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       103,
// DEFAULT-NEXT:                                                       105,
// DEFAULT-NEXT:                                                       116,
// DEFAULT-NEXT:                                                       10,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   pieces: [
// DEFAULT-NEXT:                                                       "not-digit\\n",
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 0,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "0",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
