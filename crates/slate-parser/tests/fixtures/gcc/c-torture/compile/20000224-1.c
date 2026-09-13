// SLATE-FILECHECK-DEFINES DEFAULT

enum Lisp_Type
{
  Lisp_Int                     
  ,Lisp_Record                 
  ,Lisp_Cons                   
  ,Lisp_String                 
  ,Lisp_Vector                 
  ,Lisp_Symbol
  ,Lisp_Char                     
};
typedef
union Lisp_Object
  {
    struct
      {
        enum Lisp_Type type: 3L ;
        unsigned long  markbit: 1;
        unsigned long  val: 32;
      } gu;
    long  i;
  }
Lisp_Object;
extern int initialized;
extern void call_critical_lisp_code (Lisp_Object);
void
init_device_faces (int *d)
{
  if (initialized)
    {
      Lisp_Object tdevice;
      do {
          tdevice = (union Lisp_Object)
                        { gu:
                          { markbit: 0,
                            type: Lisp_Record,
                            val: ((unsigned long )d)
                          }
                        };
      } while (0);
      call_critical_lisp_code (tdevice);
    }
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Enum(
// DEFAULT-NEXT:       EnumDecl {
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "Lisp_Type",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "Lisp_Int",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "Lisp_Record",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "Lisp_Cons",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "Lisp_String",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "Lisp_Vector",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "Lisp_Symbol",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "Lisp_Char",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 1,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tagged {
// DEFAULT-NEXT:                   kind: Union,
// DEFAULT-NEXT:                   name: Some(
// DEFAULT-NEXT:                       "Lisp_Object",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: Some(
// DEFAULT-NEXT:                       Fields(
// DEFAULT-NEXT:                           [
// DEFAULT-NEXT:                               FieldDecl {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Tagged {
// DEFAULT-NEXT:                                           kind: Struct,
// DEFAULT-NEXT:                                           name: None,
// DEFAULT-NEXT:                                           body: Some(
// DEFAULT-NEXT:                                               Fields(
// DEFAULT-NEXT:                                                   [
// DEFAULT-NEXT:                                                       FieldDecl {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Tagged {
// DEFAULT-NEXT:                                                                   kind: Enum,
// DEFAULT-NEXT:                                                                   name: Some(
// DEFAULT-NEXT:                                                                       "Lisp_Type",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               FieldDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                       "type",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   bit_width: Some(
// DEFAULT-NEXT:                                                                       Const(
// DEFAULT-NEXT:                                                                           Integer(
// DEFAULT-NEXT:                                                                               3,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: 0,
// DEFAULT-NEXT:                                                               header: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       FieldDecl {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Ranked {
// DEFAULT-NEXT:                                                                       rank: Long,
// DEFAULT-NEXT:                                                                       signed: false,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               FieldDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                       "markbit",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   bit_width: Some(
// DEFAULT-NEXT:                                                                       Const(
// DEFAULT-NEXT:                                                                           Integer(
// DEFAULT-NEXT:                                                                               1,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: 0,
// DEFAULT-NEXT:                                                               header: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       FieldDecl {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Ranked {
// DEFAULT-NEXT:                                                                       rank: Long,
// DEFAULT-NEXT:                                                                       signed: false,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               FieldDeclarator {
// DEFAULT-NEXT:                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                       "val",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   bit_width: Some(
// DEFAULT-NEXT:                                                                       Const(
// DEFAULT-NEXT:                                                                           Integer(
// DEFAULT-NEXT:                                                                               32,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           provenance: Provenance {
// DEFAULT-NEXT:                                                               file: FileId(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               kind: System,
// DEFAULT-NEXT:                                                               line: 0,
// DEFAULT-NEXT:                                                               header: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarators: [
// DEFAULT-NEXT:                                       FieldDeclarator {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "gu",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   provenance: Provenance {
// DEFAULT-NEXT:                                       file: FileId(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       kind: System,
// DEFAULT-NEXT:                                       line: 0,
// DEFAULT-NEXT:                                       header: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               FieldDecl {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Long,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarators: [
// DEFAULT-NEXT:                                       FieldDeclarator {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "i",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   provenance: Provenance {
// DEFAULT-NEXT:                                       file: FileId(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       kind: System,
// DEFAULT-NEXT:                                       line: 0,
// DEFAULT-NEXT:                                       header: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "Lisp_Object",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 11,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "initialized",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 23,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "call_critical_lisp_code",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Named(
// DEFAULT-NEXT:                                   "Lisp_Object",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 24,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "init_device_faces",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "d",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "initialized",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "Lisp_Object",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "tdevice",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       DoWhile {
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "tdevice",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: CompoundLiteral {
// DEFAULT-NEXT:                                               ty: Tagged {
// DEFAULT-NEXT:                                                   kind: Union,
// DEFAULT-NEXT:                                                   name: Some(
// DEFAULT-NEXT:                                                       "Lisp_Object",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               initializer: [
// DEFAULT-NEXT:                                                   InitializerItem {
// DEFAULT-NEXT:                                                       designators: [
// DEFAULT-NEXT:                                                           Field(
// DEFAULT-NEXT:                                                               "gu",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                       value: List(
// DEFAULT-NEXT:                                                           [
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [
// DEFAULT-NEXT:                                                                       Field(
// DEFAULT-NEXT:                                                                           "markbit",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Const(
// DEFAULT-NEXT:                                                                           Integer(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [
// DEFAULT-NEXT:                                                                       Field(
// DEFAULT-NEXT:                                                                           "type",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Const(
// DEFAULT-NEXT:                                                                           Identifier(
// DEFAULT-NEXT:                                                                               "Lisp_Record",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               InitializerItem {
// DEFAULT-NEXT:                                                                   designators: [
// DEFAULT-NEXT:                                                                       Field(
// DEFAULT-NEXT:                                                                           "val",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                                   value: Expr(
// DEFAULT-NEXT:                                                                       Const(
// DEFAULT-NEXT:                                                                           Cast {
// DEFAULT-NEXT:                                                                               ty: Integer(
// DEFAULT-NEXT:                                                                                   Ranked {
// DEFAULT-NEXT:                                                                                       rank: Long,
// DEFAULT-NEXT:                                                                                       signed: false,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                                               value: Identifier(
// DEFAULT-NEXT:                                                                                   "d",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "call_critical_lisp_code",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "tdevice",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 25,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
