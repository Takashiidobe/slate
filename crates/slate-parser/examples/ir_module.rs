use slate_parser::ast::{FileId, Loc, Span};
use slate_parser::ir::*;
use slate_parser::target_info::TargetInfo;

const I32: Type = Type::Numeric(NumericType::Integer {
    width: 32,
    signed: true,
});

fn node<T>(value: T) -> Span<T> {
    Span::new(value, Loc::new(FileId(0), 0, 0), Loc::new(FileId(0), 0, 0))
}

fn value(kind: ValueKind) -> Value {
    Value {
        ty: I32,
        node: node(kind),
    }
}

fn integer(number: u32) -> Value {
    value(ValueKind::Constant(Number::Integer(number.into())))
}

fn place(id: u32) -> Place {
    Place {
        ty: I32,
        kind: PlaceKind::Binding(BindingId(id)),
    }
}

fn read(id: u32) -> Value {
    value(ValueKind::Read(place(id)))
}

fn definition(id: u32, name: Option<&str>, kind: TypeDefinitionKind) -> Span<TypeDefinition> {
    node(TypeDefinition {
        id: TypeId(id),
        name: name.map(str::to_string),
        kind,
    })
}

fn main() {
    let mut module = Module::new(TargetInfo::default());
    let alias = definition(0, Some("word"), TypeDefinitionKind::Alias(I32));
    module
        .metadata
        .insert(alias.id, vec![("c".into(), "typedef int word".into())]);
    let field = node(Field {
        name: Some("value".into()),
        ty: I32,
        bit_width: None,
    });
    module
        .metadata
        .insert(field.id, vec![("c".into(), "int".into())]);
    module.types = vec![
        alias,
        definition(
            1,
            Some("Node"),
            TypeDefinitionKind::Record {
                kind: RecordKind::Struct,
                fields: Some(vec![field]),
                layout: Some(RecordLayout {
                    size: 4,
                    align: 4,
                    offsets: vec![0],
                    bit_offsets: vec![None],
                    bit_units: vec![],
                    field_units: vec![None],
                }),
            },
        ),
        definition(
            2,
            None,
            TypeDefinitionKind::Pointer {
                pointee: Type::Defined(TypeId(1)),
                is_const: false,
            },
        ),
        definition(
            3,
            Some("Choice"),
            TypeDefinitionKind::Enum {
                underlying: Some(I32),
                enumerators: Some(vec![
                    node(Enumerator {
                        id: BindingId(3),
                        name: "FIRST".into(),
                        value: integer(0),
                    }),
                    node(Enumerator {
                        id: BindingId(4),
                        name: "SECOND".into(),
                        value: integer(1),
                    }),
                ]),
                layout: Some(slate_parser::target_info::StorageLayout {
                    size_bytes: 4,
                    alignment_bytes: 4,
                }),
            },
        ),
        definition(
            4,
            Some("Payload"),
            TypeDefinitionKind::Record {
                kind: RecordKind::Union,
                fields: Some(vec![
                    node(Field {
                        name: Some("number".into()),
                        ty: I32,
                        bit_width: None,
                    }),
                    node(Field {
                        name: Some("alias".into()),
                        ty: Type::Defined(TypeId(0)),
                        bit_width: None,
                    }),
                ]),
                layout: Some(RecordLayout {
                    size: 4,
                    align: 4,
                    offsets: vec![0, 0],
                    bit_offsets: vec![None, None],
                    bit_units: vec![],
                    field_units: vec![None, None],
                }),
            },
        ),
        definition(
            5,
            Some("Opaque"),
            TypeDefinitionKind::Record {
                kind: RecordKind::Struct,
                fields: None,
                layout: None,
            },
        ),
        definition(
            6,
            None,
            TypeDefinitionKind::Array {
                element: I32,
                length: Some(3),
            },
        ),
        definition(
            7,
            None,
            TypeDefinitionKind::Array {
                element: I32,
                length: None,
            },
        ),
        definition(
            8,
            Some("FutureChoice"),
            TypeDefinitionKind::Enum {
                underlying: None,
                enumerators: None,
                layout: None,
            },
        ),
        definition(
            9,
            None,
            TypeDefinitionKind::Pointer {
                pointee: I32,
                is_const: true,
            },
        ),
    ];
    let global = node(Global {
        variable: Variable {
            id: BindingId(5),
            name: "Node".into(),
            ty: I32,
            storage: StorageDuration::Static,
            initializer: Some(integer(0)),
        },
        linkage: Linkage::External,
        definition: true,
    });
    module
        .metadata
        .insert(global.id, vec![("c".into(), "int Node".into())]);
    module.globals = vec![
        global,
        node(Global {
            variable: Variable {
                id: BindingId(6),
                name: "thread_value".into(),
                ty: I32,
                storage: StorageDuration::Thread,
                initializer: None,
            },
            linkage: Linkage::External,
            definition: false,
        }),
    ];
    let a = node(Parameter {
        id: BindingId(0),
        name: Some("a".into()),
        ty: I32,
    });
    let b = node(Parameter {
        id: BindingId(1),
        name: Some("b".into()),
        ty: I32,
    });
    module
        .metadata
        .insert(a.id, vec![("c".into(), "int".into())]);
    let left = read(0);
    module
        .metadata
        .insert(left.node.id, vec![("c".into(), "a".into())]);
    let addition = value(ValueKind::Arith {
        op: ArithOp::Add,
        left: Box::new(left),
        right: Box::new(read(1)),
        semantics: ArithSema::Integer {
            overflow: Overflow::Undefined,
        },
    });
    module
        .metadata
        .insert(addition.node.id, vec![("source".into(), "a + b".into())]);
    let local = node(Statement::Let(Variable {
        id: BindingId(2),
        name: "c".into(),
        ty: I32,
        storage: StorageDuration::Automatic,
        initializer: Some(addition),
    }));
    module
        .metadata
        .insert(local.id, vec![("c".into(), "int c".into())]);
    let add = node(Function {
        id: BindingId(10),
        name: "add".into(),
        parameters: Parameters::Prototype {
            fixed: vec![a, b],
            variadic: false,
        },
        return_type: Some(I32),
        linkage: Linkage::External,
        body: Some(vec![local, node(Statement::Return(Some(read(2))))]),
    });
    module
        .metadata
        .insert(add.id, vec![("source".into(), "add.c".into())]);
    module.functions.push(add);
    module.functions.push(node(Function {
        name: "log_values".into(),
        id: BindingId(11),
        parameters: Parameters::Prototype {
            fixed: vec![node(Parameter {
                id: BindingId(7),
                name: None,
                ty: I32,
            })],
            variadic: true,
        },
        return_type: None,
        linkage: Linkage::External,
        body: None,
    }));
    module.functions.push(node(Function {
        name: "legacy".into(),
        id: BindingId(12),
        parameters: Parameters::Unprototyped,
        return_type: Some(I32),
        linkage: Linkage::External,
        body: None,
    }));
    let block = node(Statement::Block(vec![
        node(Statement::Let(Variable {
            id: BindingId(8),
            name: "node".into(),
            ty: Type::Defined(TypeId(2)),
            storage: StorageDuration::Automatic,
            initializer: Some(Value {
                ty: Type::Defined(TypeId(2)),
                node: node(ValueKind::Null),
            }),
        })),
        node(Statement::Write {
            place: place(5),
            value: integer(1),
        }),
        node(Statement::Expression(Value {
            ty: Type::Defined(TypeId(9)),
            node: node(ValueKind::AddressOf(place(5))),
        })),
    ]));
    module
        .metadata
        .insert(block.id, vec![("source".into(), "nested block".into())]);
    module.functions.push(node(Function {
        name: "update".into(),
        id: BindingId(13),
        parameters: Parameters::Prototype {
            fixed: Vec::new(),
            variadic: false,
        },
        return_type: None,
        linkage: Linkage::Internal,
        body: Some(vec![block, node(Statement::Return(None))]),
    }));
    let char_type = Type::Numeric(NumericType::Integer {
        width: 8,
        signed: true,
    });
    module.types.push(definition(
        10,
        None,
        TypeDefinitionKind::Array {
            element: char_type,
            length: Some(4),
        },
    ));
    module.types.push(definition(
        11,
        None,
        TypeDefinitionKind::Pointer {
            pointee: char_type,
            is_const: true,
        },
    ));
    module.globals.push(node(Global {
        variable: Variable {
            id: BindingId(14),
            name: "format".into(),
            ty: Type::Defined(TypeId(10)),
            storage: StorageDuration::Static,
            initializer: Some(Value {
                ty: Type::Defined(TypeId(10)),
                node: node(ValueKind::Bytes(b"%d\n\0".to_vec())),
            }),
        },
        linkage: Linkage::Internal,
        definition: true,
    }));
    let printf = node(Function {
        id: BindingId(15),
        name: "printf".into(),
        return_type: Some(I32),
        linkage: Linkage::External,
        body: None,
        parameters: Parameters::Prototype {
            fixed: vec![node(Parameter {
                id: BindingId(16),
                name: Some("format".into()),
                ty: Type::Defined(TypeId(11)),
            })],
            variadic: true,
        },
    });
    module
        .metadata
        .insert(printf.id, vec![("origin".into(), "system:stdio.h".into())]);
    module.functions.push(printf);
    let format = Value {
        ty: Type::Defined(TypeId(11)),
        node: node(ValueKind::ArrayDecay {
            place: Place {
                ty: Type::Defined(TypeId(10)),
                kind: PlaceKind::Binding(BindingId(14)),
            },
            length: Some(4),
        }),
    };
    module
        .metadata
        .insert(format.node.id, vec![("c".into(), "char[4]".into())]);
    let add_call = value(ValueKind::Call {
        function: BindingId(10),
        arguments: vec![integer(2), integer(3)],
    });
    module.metadata.insert(
        add_call.node.id,
        vec![("vararg_promotion".into(), "none".into())],
    );
    let implicit_return = node(Statement::Return(Some(integer(0))));
    module.metadata.insert(
        implicit_return.id,
        vec![("implicit".into(), "main_return".into())],
    );
    module.functions.push(node(Function {
        id: BindingId(17),
        name: "main".into(),
        parameters: Parameters::Prototype {
            fixed: Vec::new(),
            variadic: false,
        },
        return_type: Some(I32),
        linkage: Linkage::External,
        body: Some(vec![
            node(Statement::Expression(value(ValueKind::Call {
                function: BindingId(15),
                arguments: vec![format, add_call],
            }))),
            implicit_return,
        ]),
    }));
    println!("without source metadata:");
    print!("{}", module.display(false));
    println!("with source metadata:");
    print!("{}", module.display(true));
}
