#pragma pack(push, 1)
struct packed_record { char c; int i; };
#pragma pack(show)
#pragma pack(pop)

_Pragma("pack(2)")
int pragma_adjacent(void) { _Pragma("STDC FP_CONTRACT ON"); return 1; }
#pragma weak weak_name
#pragma weak weak_alias = weak_target
#pragma GCC visibility push(hidden)
int hidden_function(void) {
#pragma STDC FP_CONTRACT ON
  return 1;
}
#pragma GCC visibility pop
#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF
#pragma STDC CX_LIMITED_RANGE ON
#pragma float_control precise on
#pragma ms_struct push
#pragma ms_struct pop

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[0]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           0,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "packed_record",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "c",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Pack {
// DEFAULT-NEXT:               action: Push,
// DEFAULT-NEXT:               alignment: Some(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 1,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "1",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Pack {
// DEFAULT-NEXT:               action: Show,
// DEFAULT-NEXT:               alignment: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Pack {
// DEFAULT-NEXT:               action: Pop,
// DEFAULT-NEXT:               alignment: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Pack {
// DEFAULT-NEXT:               action: Set,
// DEFAULT-NEXT:               alignment: Some(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 2,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "2",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Function(
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
// DEFAULT-NEXT:                   "pragma_adjacent",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Pragma(
// DEFAULT-NEXT:                   Pragma {
// DEFAULT-NEXT:                       kind: Stdc {
// DEFAULT-NEXT:                           option: FpContract,
// DEFAULT-NEXT:                           enabled: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 1,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "1",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[6]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Weak {
// DEFAULT-NEXT:               name: "weak_name",
// DEFAULT-NEXT:               alias: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[7]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Weak {
// DEFAULT-NEXT:               name: "weak_alias",
// DEFAULT-NEXT:               alias: Some(
// DEFAULT-NEXT:                   "weak_target",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[8]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Visibility {
// DEFAULT-NEXT:               action: Push,
// DEFAULT-NEXT:               visibility: Some(
// DEFAULT-NEXT:                   "hidden",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[9]: Function(
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
// DEFAULT-NEXT:                   "hidden_function",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Pragma(
// DEFAULT-NEXT:                   Pragma {
// DEFAULT-NEXT:                       kind: Stdc {
// DEFAULT-NEXT:                           option: FpContract,
// DEFAULT-NEXT:                           enabled: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 1,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "1",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[10]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Visibility {
// DEFAULT-NEXT:               action: Pop,
// DEFAULT-NEXT:               visibility: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[11]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Stdc {
// DEFAULT-NEXT:               option: FenvAccess,
// DEFAULT-NEXT:               enabled: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[12]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Stdc {
// DEFAULT-NEXT:               option: FpContract,
// DEFAULT-NEXT:               enabled: false,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[13]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: Stdc {
// DEFAULT-NEXT:               option: CxLimitedRange,
// DEFAULT-NEXT:               enabled: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[14]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: FloatControl {
// DEFAULT-NEXT:               option: Precise,
// DEFAULT-NEXT:               enabled: true,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[15]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: MsStruct {
// DEFAULT-NEXT:               action: Push,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[16]: Pragma(
// DEFAULT-NEXT:       Pragma {
// DEFAULT-NEXT:           kind: MsStruct {
// DEFAULT-NEXT:               action: Pop,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
