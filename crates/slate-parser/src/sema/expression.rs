use super::builtins::{ClangBuiltin, CustomBuiltin, DerivedSignature};
use super::ctype::convert::{CastKind, Conversion, ConversionContext};
use super::ctype::{CTypeKind, CTypes, QualType};
use super::numeric::{Context, ResolveError};
use super::operand::{Lvalue, Operand};
use super::types::TypeResolver;
use crate::ast::{Expr, ExprKind, Initializer, NodeId, Span, StmtKind};
use crate::const_expr::{AssignOp, BinaryOp, PostfixOp, UnaryOp};
use crate::diagnostics::Warning;
use crate::ir::*;
use std::collections::HashMap;

enum Projection {
    Place(Lvalue),
    Value(Operand),
}

pub(super) struct Lowerer {
    pub context: Context,
    pub types: TypeResolver,
    pub module: Module,
    pub names: NameResolution,
    pub function_declarations: HashMap<BindingId, super::function::FunctionDeclarations>,
    pub builtin_declarations: HashMap<&'static str, BindingId>,
    pub alias_annotations: HashMap<TypeId, Vec<(String, String)>>,
    pub next_id: u32,
    pub break_targets: Vec<BindingId>,
    pub continue_targets: Vec<BindingId>,
    pub switches: Vec<(BindingId, QualType)>,
    pub in_function: bool,
    pub in_naked_function: bool,
    pub files: crate::files::Files,
    pub return_type: Option<QualType>,
    pub ms_asm_return: Vec<BindingId>,
    pub floating_pragmas: super::pragmas::FloatingPragmas,
    pub compound_start: bool,
    pub reserved_extents: HashMap<NodeId, BindingId>,
}

impl Lowerer {
    fn call(
        &mut self,
        e: &Expr,
        callee: Callee,
        signature: QualType,
        arguments: &[Expr],
    ) -> Result<Operand, ResolveError> {
        let (returned, params, _, _) = self
            .types
            .ctypes
            .function_parts(signature)
            .ok_or(ResolveError::Internal("non-function callee"))?;
        let params = params.to_vec();
        let ty = self.types.ir_type(signature);
        let Type::Function {
            parameters,
            variadic,
            prototyped,
            ..
        } = &ty
        else {
            return Err(ResolveError::Internal("non-function callee"));
        };
        if *prototyped
            && (arguments.len() < parameters.len()
                || (!*variadic && arguments.len() != parameters.len()))
        {
            return Err(ResolveError::Internal("call argument count"));
        }
        let mut lowered = Vec::new();
        for (index, argument) in arguments.iter().enumerate() {
            let value = self.expr(argument)?;
            let (value, to, reason) = if let Some(to) = params.get(index) {
                let to = self.types.ctypes.adjust_parameter(*to);
                (value, to, ConversionReason::Arg)
            } else {
                let value = self.enum_operand(value);
                let to = self
                    .types
                    .ctypes
                    .default_promotion(value.c, &self.context.target);
                (value, to, ConversionReason::Vararg)
            };
            lowered.push(self.convert_recorded(argument, value, to, reason)?.value);
        }
        let abi = self.c_abi_signature(signature, &ty, Some(&lowered))?;
        Ok(self.operand(
            e,
            returned,
            ValueKind::Call {
                callee,
                signature: ty,
                abi,
                arguments: lowered,
            },
        ))
    }

    fn builtin_declaration(
        &mut self,
        e: &Expr,
        builtin: &'static ClangBuiltin,
        signature: QualType,
    ) -> Result<BindingId, ResolveError> {
        if let Some(&id) = self.builtin_declarations.get(builtin.name) {
            return Ok(id);
        }
        let declared = self.types.builtin_signature(builtin).unwrap_or(signature);
        let ty = self.types.ir_type(declared);
        let Type::Function {
            return_type,
            parameters,
            variadic,
            prototyped,
            ..
        } = &ty
        else {
            return Err(ResolveError::Internal("non-function builtin"));
        };
        let fixed = parameters
            .iter()
            .map(|ty| {
                Span::new(
                    Parameter {
                        id: self.fresh(),
                        name: None,
                        ty: ty.clone(),
                        restrict: false,
                        is_const: false,
                        access: Access::default(),
                        array: None,
                    },
                    e.spelling,
                    e.expansion,
                )
                .with_provenance(e.provenance)
            })
            .collect();
        let parameters = if *prototyped {
            Parameters::Prototype {
                fixed,
                variadic: *variadic,
            }
        } else {
            Parameters::Unprototyped
        };
        let id = self.fresh();
        let function = Function {
            id,
            name: builtin.name.into(),
            parameters,
            return_type: return_type.as_deref().cloned(),
            abi: self.abi_signature(&ty, None)?,
            linkage: Linkage::External,
            symbol: SymbolAttributes::default(),
            semantics: FunctionSemantics {
                noreturn: builtin.noreturn(self.types.compiler_flavor()),
                memory: builtin.memory_effects(self.types.compiler_flavor()),
                ..FunctionSemantics::default()
            },
            body: None,
            fallthrough: None,
        };
        let function = Span::new(function, e.spelling, e.expansion).with_provenance(e.provenance);
        self.module
            .annotate(&function, [("c_builtin".into(), builtin.name.into())]);
        self.module.functions.push(function);
        self.builtin_declarations.insert(builtin.name, id);
        Ok(id)
    }

    fn function_like_builtin(
        &mut self,
        e: &Expr,
        callee: &Expr,
        arguments: &[Expr],
    ) -> Result<Option<Operand>, ResolveError> {
        let Some((builtin, declaration)) = self.types.builtin_callee(callee, arguments) else {
            return Ok(None);
        };
        let name = builtin.name.to_owned();
        let custom = super::builtins::custom_builtin(builtin);
        if let Some(custom) = custom {
            let value = self.custom_builtin(e, custom, arguments)?;
            self.module
                .annotate(&value.value.node, [("c_builtin".into(), name)]);
            return Ok(Some(value));
        }
        let signature = match super::builtins::derived_signature(builtin) {
            Some(derived) => Some(self.derived_signature(builtin, derived, arguments)?),
            None => self.types.builtin_signature(builtin),
        };
        let Some(signature) = signature else {
            return Ok(None);
        };
        let id = match declaration {
            Some(id) => id,
            None => self.builtin_declaration(e, builtin, signature)?,
        };
        Ok(Some(self.call(
            e,
            Callee::Direct(id),
            signature,
            arguments,
        )?))
    }

    fn custom_builtin(
        &mut self,
        e: &Expr,
        custom: CustomBuiltin,
        arguments: &[Expr],
    ) -> Result<Operand, ResolveError> {
        match custom {
            CustomBuiltin::Overflow(op) => self.overflow_builtin(e, op, arguments),
            CustomBuiltin::FloatClass(test) => {
                let [operand] = arguments else {
                    return Err(ResolveError::Internal("float class builtin arity"));
                };
                let operand = self.real_floating_operand(operand)?;
                Ok(self.truth(
                    e,
                    ValueKind::FloatClass {
                        test,
                        operand: Box::new(operand.value),
                    },
                ))
            }
            CustomBuiltin::QuietCompare(op) => {
                let (left, right) = self.real_floating_pair(arguments)?;
                Ok(self.truth(
                    e,
                    ValueKind::Compare {
                        op,
                        left: Box::new(left.value),
                        right: Box::new(right.value),
                        exceptions: Some(Exceptions::Ignore),
                        reason: None,
                    },
                ))
            }
            CustomBuiltin::Unordered => {
                let (left, right) = self.real_floating_pair(arguments)?;
                let left = self.float_class_int(e, FloatClassTest::Nan, left.value)?;
                let right = self.float_class_int(e, FloatClassTest::Nan, right.value)?;
                Ok(self.either(e, left, right))
            }
            CustomBuiltin::LessGreater => {
                let (left, right) = self.real_floating_pair(arguments)?;
                let less = self.quiet_compare_int(e, CompareOp::Lt, &left, &right)?;
                let greater = self.quiet_compare_int(e, CompareOp::Gt, &left, &right)?;
                Ok(self.either(e, less, greater))
            }
            CustomBuiltin::InfSign => {
                let [operand] = arguments else {
                    return Err(ResolveError::Internal("float class builtin arity"));
                };
                let operand = self.real_floating_operand(operand)?;
                let c = self.types.ctypes.int();
                let int = self.types.ir_type(c);
                let infinite = self.value(
                    e,
                    Type::Bool,
                    ValueKind::FloatClass {
                        test: FloatClassTest::Infinite,
                        operand: Box::new(operand.value.clone()),
                    },
                );
                let negative = self.value(
                    e,
                    Type::Bool,
                    ValueKind::FloatClass {
                        test: FloatClassTest::SignBit,
                        operand: Box::new(operand.value),
                    },
                );
                let minus_one = self.value(
                    e,
                    int.clone(),
                    ValueKind::Constant(Number::SignedInteger((-1).into())),
                );
                let one = self.value(
                    e,
                    int.clone(),
                    ValueKind::Constant(Number::Integer(1u32.into())),
                );
                let zero = self.value(e, int, ValueKind::Constant(Number::Integer(0u32.into())));
                let signed = self.value(
                    e,
                    self.types.ir_type(c),
                    ValueKind::Conditional {
                        condition: Box::new(negative),
                        then_value: Box::new(minus_one),
                        else_value: Box::new(one),
                    },
                );
                Ok(self.operand(
                    e,
                    c,
                    ValueKind::Conditional {
                        condition: Box::new(infinite),
                        then_value: Box::new(signed),
                        else_value: Box::new(zero),
                    },
                ))
            }
            CustomBuiltin::Complex => {
                let (real, imaginary) = self.real_floating_pair(arguments)?;
                let c = self.types.complex_of(real.c);
                Ok(self.operand(
                    e,
                    c,
                    ValueKind::Aggregate {
                        members: vec![
                            AggregateMember {
                                target: AggregateTarget::Index(0),
                                value: real.value,
                            },
                            AggregateMember {
                                target: AggregateTarget::Index(1),
                                value: imaginary.value,
                            },
                        ],
                        zero_fill: false,
                    },
                ))
            }
            CustomBuiltin::FloatClassify => {
                let [nan, infinite, normal, subnormal, zero, value] = arguments else {
                    return Err(ResolveError::Internal("classification builtin arity"));
                };
                let value = self.real_floating_operand(value)?;
                let c = self.types.ctypes.int();
                let int = self.types.ir_type(c);
                let mut selected = self.expr(zero)?;
                selected = self.convert(selected, c, ConversionReason::UsualArith)?;
                let mut selected = selected.value;
                for (test, arm) in [
                    (FloatClassTest::Subnormal, subnormal),
                    (FloatClassTest::Normal, normal),
                    (FloatClassTest::Infinite, infinite),
                    (FloatClassTest::Nan, nan),
                ] {
                    let condition = self.value(
                        e,
                        Type::Bool,
                        ValueKind::FloatClass {
                            test,
                            operand: Box::new(value.value.clone()),
                        },
                    );
                    let arm = self.expr(arm)?;
                    let arm = self.convert(arm, c, ConversionReason::UsualArith)?;
                    selected = self.value(
                        e,
                        int.clone(),
                        ValueKind::Conditional {
                            condition: Box::new(condition),
                            then_value: Box::new(arm.value),
                            else_value: Box::new(selected),
                        },
                    );
                }
                Ok(self.operand(e, c, selected.node.value))
            }
            CustomBuiltin::Shuffle => self.shuffle_builtin(e, arguments),
            CustomBuiltin::AddressOf => {
                let [operand] = arguments else {
                    return Err(ResolveError::Internal("addressof builtin arity"));
                };
                let place = self.place(operand)?;
                if matches!(place.kind, PlaceKind::Field { bits: Some(_), .. }) {
                    return Err(ResolveError::Internal("address of a bit-field"));
                }
                if temporary_rooted(&place.place) {
                    return Err(ResolveError::Internal("address of a temporary"));
                }
                let c = self.types.ctypes.pointer(place.c);
                Ok(self.operand(e, c, ValueKind::AddressOf(place.place)))
            }
            CustomBuiltin::ClassifyType => {
                let [operand] = arguments else {
                    return Err(ResolveError::Internal("classify builtin arity"));
                };
                let (resolved, _) = self.operand_type(operand)?;
                let resolved = self.types.ctypes.lvalue_conversion(resolved);
                let class = type_class(&self.types.ctypes, resolved)
                    .ok_or(ResolveError::Unimplemented("classify builtin operand"))?;
                let c = self.types.ctypes.int();
                let number = match u32::try_from(class) {
                    Ok(class) => Number::Integer(class.into()),
                    Err(_) => Number::SignedInteger(class.into()),
                };
                Ok(self.operand(e, c, ValueKind::Constant(number)))
            }
        }
    }

    fn derived_signature(
        &mut self,
        builtin: &ClangBuiltin,
        derived: DerivedSignature,
        arguments: &[Expr],
    ) -> Result<QualType, ResolveError> {
        let first = match (derived, arguments.first()) {
            (DerivedSignature::Declared, _) | (_, None) => None,
            (_, Some(argument)) => {
                let (resolved, _) = self.operand_type(argument)?;
                Some(self.types.ctypes.lvalue_conversion(resolved))
            }
        };
        self.types
            .derived_signature(builtin, derived, arguments.len(), first)
    }

    fn source_location(
        &mut self,
        e: &Expr,
        callee: &Expr,
        builtin: SourceLocationBuiltin,
    ) -> Result<Operand, ResolveError> {
        let file = callee.spelling.file;
        match builtin {
            SourceLocationBuiltin::Line | SourceLocationBuiltin::Column => {
                let (line, column) = self
                    .files
                    .position(file, callee.spelling.offset)
                    .ok_or(ResolveError::Internal("unknown source position"))?;
                let n = match builtin {
                    SourceLocationBuiltin::Line => line,
                    _ => column,
                };
                let c = self.types.ctypes.int();
                let value = i64::try_from(n.saturating_add(1))
                    .map_err(|_| ResolveError::Internal("source location out of range"))?;
                Ok(self.operand(
                    e,
                    c,
                    ValueKind::Constant(Number::SignedInteger(value.into())),
                ))
            }
            SourceLocationBuiltin::File
            | SourceLocationBuiltin::FileName
            | SourceLocationBuiltin::Function => {
                let text = match builtin {
                    SourceLocationBuiltin::Function => self
                        .types
                        .function_names
                        .as_ref()
                        .map(|names| names.plain.clone())
                        .unwrap_or_default(),
                    SourceLocationBuiltin::FileName => {
                        let path = self
                            .files
                            .get_path(file)
                            .ok_or(ResolveError::Internal("unknown source file"))?;
                        path.file_name().map_or_else(
                            || crate::files::display_path(path),
                            |name| name.to_string_lossy().into_owned(),
                        )
                    }
                    _ => {
                        let path = self
                            .files
                            .get_path(file)
                            .ok_or(ResolveError::Internal("unknown source file"))?;
                        crate::files::display_path(path)
                    }
                };
                let lvalue = self.string_global(e, &text)?;
                self.read(e, lvalue)
            }
        }
    }

    fn string_global(&mut self, e: &Expr, text: &str) -> Result<Lvalue, ResolveError> {
        let mut units: Vec<u32> = text.bytes().map(u32::from).collect();
        units.push(0);
        let char_type = self.types.ctypes.qual(CTypeKind::Char);
        let c = self.types.ctypes.qual(CTypeKind::Array {
            element: char_type,
            extent: super::ctype::Extent::Fixed(units.len() as u64),
        });
        let ty = self.types.ir_type(c);
        let id = self.fresh();
        let initializer = self.value(e, ty.clone(), ValueKind::CodeUnits(units));
        self.module.globals.push(e.derive(Global {
            variable: Variable {
                id,
                name: format!(".str{}", id.0),
                ty: ty.clone(),
                storage: StorageDuration::Static,
                restrict: false,
                is_const: false,
                access: Access::default(),
                constexpr: false,
                alignment: None,
                cleanup: None,
                register: None,
                initializer: Some(initializer),
            },
            linkage: Linkage::Internal,
            symbol: SymbolAttributes::default(),
            definition: true,
            common: false,
        }));
        Ok(Lvalue {
            c,
            place: Place {
                ty,
                kind: PlaceKind::Binding(id),
                access: Access::default(),
            },
        })
    }

    fn overflow_builtin(
        &mut self,
        e: &Expr,
        op: ArithOp,
        arguments: &[Expr],
    ) -> Result<Operand, ResolveError> {
        let [left, right, result] = arguments else {
            return Err(ResolveError::Internal("overflow builtin argument count"));
        };
        let left = self.expr(left)?;
        let right = self.expr(right)?;
        let pointer = self.expr(result)?;
        let result = self.deref(pointer)?;
        if !self.types.ctypes.is_integer(left.c)
            || !self.types.ctypes.is_integer(right.c)
            || !self.types.ctypes.is_integer(result.c)
        {
            return Err(ResolveError::Internal("overflow builtin operand type"));
        }
        let c = self.types.ctypes.qual(CTypeKind::Bool);
        Ok(self.operand(
            e,
            c,
            ValueKind::Overflow {
                op,
                left: Box::new(left.value),
                right: Box::new(right.value),
                result: result.place,
            },
        ))
    }

    fn float_class_int(
        &mut self,
        e: &Expr,
        test: FloatClassTest,
        operand: Value,
    ) -> Result<Value, ResolveError> {
        let value = self.value(
            e,
            Type::Bool,
            ValueKind::FloatClass {
                test,
                operand: Box::new(operand),
            },
        );
        self.promote_truth(value)
    }

    fn quiet_compare_int(
        &mut self,
        e: &Expr,
        op: CompareOp,
        left: &Operand,
        right: &Operand,
    ) -> Result<Value, ResolveError> {
        let value = self.value(
            e,
            Type::Bool,
            ValueKind::Compare {
                op,
                left: Box::new(left.value.clone()),
                right: Box::new(right.value.clone()),
                exceptions: Some(Exceptions::Ignore),
                reason: None,
            },
        );
        self.promote_truth(value)
    }

    fn promote_truth(&mut self, value: Value) -> Result<Value, ResolveError> {
        let int = self.context.int_type();
        self.context
            .emit_arithmetic_conversion(value, int, ConversionReason::Promotion)
    }

    fn either(&mut self, e: &Expr, left: Value, right: Value) -> Operand {
        let c = self.types.ctypes.int();
        self.operand(
            e,
            c,
            ValueKind::Arith {
                op: ArithOp::Or,
                left: Box::new(left),
                right: Box::new(right),
                semantics: ArithSema::Exact,
            },
        )
    }

    fn real_floating_operand(&mut self, argument: &Expr) -> Result<Operand, ResolveError> {
        let operand = self.expr(argument)?;
        let c = self.types.real_floating_component(operand.c)?;
        self.convert(operand, c, ConversionReason::UsualArith)
    }

    fn real_floating_pair(
        &mut self,
        arguments: &[Expr],
    ) -> Result<(Operand, Operand), ResolveError> {
        let [left, right] = arguments else {
            return Err(ResolveError::Internal("float class builtin arity"));
        };
        let left = self.real_floating_operand(left)?;
        let right = self.real_floating_operand(right)?;
        let target = self.context.target.clone();
        let common = self
            .types
            .ctypes
            .usual_real_type(left.c, right.c, &target)?;
        let left = self.convert(left, common, ConversionReason::UsualArith)?;
        let right = self.convert(right, common, ConversionReason::UsualArith)?;
        Ok((left, right))
    }

    pub(super) fn builtin_operand(
        &mut self,
        e: &Expr,
        kind: CTypeKind,
        value: ValueKind,
    ) -> Operand {
        let c = self.types.ctypes.qual(kind);
        self.operand(e, c, value)
    }
    pub(super) fn operand<T: Clone>(&self, e: &Span<T>, c: QualType, kind: ValueKind) -> Operand {
        Operand {
            value: self.value(e, self.types.ir_type(c), kind),
            c,
        }
    }

    pub(super) fn truth(&mut self, e: &Expr, kind: ValueKind) -> Operand {
        Operand {
            value: self.value(e, Type::Bool, kind),
            c: self.types.ctypes.int(),
        }
    }

    pub(super) fn convert(
        &mut self,
        value: Operand,
        to: QualType,
        reason: ConversionReason,
    ) -> Result<Operand, ResolveError> {
        self.convert_classified(None, value, to, reason)
    }

    pub(super) fn convert_expr(
        &mut self,
        e: &Expr,
        value: Operand,
        to: QualType,
        reason: ConversionReason,
    ) -> Result<Operand, ResolveError> {
        self.convert_classified(Some(e), value, to, reason)
    }

    pub(super) fn convert_recorded(
        &mut self,
        e: &Expr,
        value: Operand,
        to: QualType,
        reason: ConversionReason,
    ) -> Result<Operand, ResolveError> {
        let conversion = *self
            .types
            .conversions
            .get(&e.id)
            .ok_or(ResolveError::Internal(
                "conversion not recorded by the checker",
            ))?;
        self.apply_conversion(conversion, value, to, reason)
    }

    fn convert_classified(
        &mut self,
        e: Option<&Expr>,
        value: Operand,
        to: QualType,
        reason: ConversionReason,
    ) -> Result<Operand, ResolveError> {
        let c = self.types.ctypes.unqualified(to);
        let null = self.is_null_pointer_constant(e, &value);
        let conversion =
            self.types
                .ctypes
                .classify_conversion(value.c, c, conversion_context(reason), null)?;
        if let Some((warning, message)) = conversion.warning {
            if let Some(e) = e {
                self.warn(warning, message, e);
            } else {
                self.warn(warning, message, &value.value.node);
            }
        }
        self.apply_conversion(conversion, value, c, reason)
    }

    fn apply_conversion(
        &mut self,
        conversion: Conversion,
        value: Operand,
        to: QualType,
        reason: ConversionReason,
    ) -> Result<Operand, ResolveError> {
        let c = self.types.ctypes.unqualified(to);
        Ok(Operand {
            value: self.emit_cast(conversion.kind, value, c, reason)?,
            c,
        })
    }

    fn warn_comparison(
        &mut self,
        e: &Expr,
        left_expr: &Expr,
        left: &Operand,
        right_expr: &Expr,
        right: &Operand,
    ) {
        if self.types.ctypes.is_nullptr(left.c) || self.types.ctypes.is_nullptr(right.c) {
            return;
        }
        let pointers = (
            self.types.ctypes.is_pointer(left.c),
            self.types.ctypes.is_pointer(right.c),
        );
        match pointers {
            (true, true) => {
                let (a, b) = (left.c, right.c);
                if self.types.ctypes.is_void(a) || self.types.ctypes.is_void(b) {
                    return;
                }
                if self
                    .types
                    .ctypes
                    .merge_pointer(a, b, super::PointerMerge::EXACT)
                    .is_none()
                {
                    self.warn(
                        Warning::CompareDistinctPointerTypes,
                        "comparison of distinct pointer types",
                        e,
                    );
                }
            }
            (true, false) | (false, true) => {
                let (other_expr, other) = if pointers.0 {
                    (right_expr, right)
                } else {
                    (left_expr, left)
                };
                if !self.is_null_pointer_constant(Some(other_expr), other) {
                    self.warn(
                        Warning::PointerIntegerCompare,
                        "comparison between pointer and integer",
                        e,
                    );
                }
            }
            _ => {}
        }
    }

    fn is_null_pointer_constant(&mut self, e: Option<&Expr>, value: &Operand) -> bool {
        if matches!(value.value.node.value, ValueKind::Null) {
            return true;
        }
        if !self.types.ctypes.is_integer(value.c) {
            return false;
        }
        match e {
            Some(e) => self.types.integer_constant_zero(e),
            None => {
                matches!(&value.value.node.value, ValueKind::Constant(Number::Integer(n)) if *n == num_bigint::BigUint::default())
            }
        }
    }

    fn emit_cast(
        &mut self,
        kind: CastKind,
        value: Operand,
        to: QualType,
        reason: ConversionReason,
    ) -> Result<Value, ResolveError> {
        let ty = self.types.ir_type(to);
        let value = value.value;
        match kind {
            CastKind::Identity => self.emit_convert_to(value, to, reason),
            CastKind::RecordCopy => {
                let node = value.node.clone();
                Ok(self.value(
                    &node,
                    ty,
                    ValueKind::Copy {
                        operand: Box::new(value),
                        reason,
                    },
                ))
            }
            CastKind::Arithmetic => {
                if ty == Type::Bool {
                    return self.condition(value, Some(reason));
                }
                self.context.emit_arithmetic_conversion(value, ty, reason)
            }
            CastKind::Vector => self.context.vector_convert(value, ty, reason),
            CastKind::EnumToInt => {
                let integer = self.enum_integer(value);
                let c = self
                    .types
                    .ctypes
                    .enum_underlying(to)
                    .unwrap_or(self.types.ctypes.int());
                let operand = Operand { value: integer, c };
                self.convert(operand, to, reason)
                    .map(|operand| operand.value)
            }
            CastKind::IntToEnum => {
                let underlying = self
                    .types
                    .ctypes
                    .enum_underlying(to)
                    .ok_or(ResolveError::Internal("enum without an underlying type"))?;
                let value = self.emit_convert_to(value, underlying, reason)?;
                let node = value.node.clone();
                Ok(self.value(
                    &node,
                    ty,
                    ValueKind::Convert {
                        kind: ConversionKind::IntToEnum,
                        operand: Box::new(value),
                        reason,
                        semantics: ConversionSema::Exact,
                    },
                ))
            }
            CastKind::NullPointer => Ok(self.value(&value.node, ty, ValueKind::Null)),
            CastKind::PtrToBool => self.condition(value, Some(reason)),
            CastKind::Pointer if value.ty == ty && self.types.ctypes.is_variably_modified(to) => {
                Ok(value)
            }
            CastKind::Pointer | CastKind::PtrToInt | CastKind::IntToPtr => {
                let kind = match (kind, &value.ty, &ty) {
                    (CastKind::PtrToInt, ..) => ConversionKind::PtrToInt,
                    (CastKind::IntToPtr, ..) => ConversionKind::IntToPtr,
                    (_, Type::Pointer { space: from, .. }, Type::Pointer { space: to, .. })
                        if from != to =>
                    {
                        ConversionKind::AddressSpaceCast
                    }
                    _ => ConversionKind::PointerCast,
                };
                let node = value.node.clone();
                Ok(self.value(
                    &node,
                    ty,
                    ValueKind::Convert {
                        kind,
                        operand: Box::new(value),
                        reason,
                        semantics: ConversionSema::Exact,
                    },
                ))
            }
        }
    }

    fn emit_convert_to(
        &mut self,
        value: Value,
        to: QualType,
        reason: ConversionReason,
    ) -> Result<Value, ResolveError> {
        let ty = self.types.ir_type(to);
        if value.ty == ty {
            return Ok(value);
        }
        if ty == Type::Bool {
            return self.condition(value, Some(reason));
        }
        self.context.emit_arithmetic_conversion(value, ty, reason)
    }

    pub(super) fn promote(&mut self, value: Operand) -> Result<Operand, ResolveError> {
        let value = self.enum_operand(value);
        self.types.promote_operand(&self.context, value, None)
    }

    pub(super) fn enum_operand(&self, operand: Operand) -> Operand {
        self.types.enum_operand(operand)
    }

    fn warn_arguments_without_prototype(
        &mut self,
        e: &Expr,
        callee: &Expr,
        direct: bool,
        signature: QualType,
    ) {
        let Some((_, params, _, false)) = self.types.ctypes.function_parts(signature) else {
            return;
        };
        if !params.is_empty() {
            return;
        }
        let target = match &callee.value {
            ExprKind::Identifier(name) if direct => format!("'{name}'"),
            _ => "a function".to_owned(),
        };
        let message = format!(
            "passing arguments to {target} without a prototype is deprecated in all versions of C and is not supported in C23"
        );
        self.warn(Warning::DeprecatedNonPrototype, &message, e);
    }

    pub(super) fn warn<T>(&mut self, warning: Warning, message: &str, node: &Span<T>) {
        self.types.warn(warning, message, node);
    }

    pub fn fresh(&mut self) -> BindingId {
        let id = BindingId(self.next_id);
        self.next_id += 1;
        id
    }

    pub fn declaration_id(&self, node: NodeId, name: &str) -> Result<BindingId, ResolveError> {
        if let Some(id) = self.names.declarations.get(&node) {
            return Ok(*id);
        }
        self.names
            .bindings
            .iter()
            .find(|b| b.id == node && b.name == name)
            .or_else(|| self.names.bindings.iter().find(|b| b.name == name))
            .map(|b| b.value.id)
            .ok_or(ResolveError::Internal("missing declaration binding"))
    }

    fn binding_kind(&self, e: &Expr) -> Option<BindingKind> {
        let id = self.types.references.get(&e.id)?;
        Some(self.names.bindings.get(id.0 as usize)?.kind)
    }

    pub(super) fn reference(&self, e: &Expr) -> Result<BindingId, ResolveError> {
        self.types
            .references
            .get(&e.id)
            .copied()
            .ok_or_else(|| ResolveError::MissingExpressionBinding(e.value.to_string()))
    }

    pub fn kind(&self, ty: &Type) -> Option<&TypeDefinitionKind> {
        self.types.kind(ty)
    }

    pub(super) fn pointee(&self, ty: &Type) -> Result<Type, ResolveError> {
        match ty {
            Type::Pointer { pointee, .. } => Ok((**pointee).clone()),
            _ => Err(ResolveError::Internal("expected pointer")),
        }
    }

    pub(super) fn deref(&self, pointer: Operand) -> Result<Lvalue, ResolveError> {
        let Type::Pointer {
            pointee, access, ..
        } = &pointer.ty
        else {
            return Err(ResolveError::Internal("expected pointer"));
        };
        let c = self
            .types
            .ctypes
            .pointee(pointer.c)
            .ok_or(ResolveError::Internal("expected C pointer"))?;
        Ok(Lvalue {
            c,
            place: Place {
                ty: (**pointee).clone(),
                access: *access,
                kind: PlaceKind::Deref(Box::new(pointer.value)),
            },
        })
    }

    pub fn value<T: Clone>(&self, e: &Span<T>, ty: Type, kind: ValueKind) -> Value {
        Value {
            ty,
            node: e.derive(kind),
        }
    }

    pub fn condition(
        &self,
        value: Value,
        reason: Option<ConversionReason>,
    ) -> Result<Value, ResolveError> {
        if self.enum_underlying(&value.ty).is_some() {
            return self.condition(self.enum_integer(value), reason);
        }
        let mut result = match &value.ty {
            Type::Bool => return Ok(value),
            Type::Numeric(_) | Type::Imaginary(_) | Type::FixedPoint(_) => {
                self.context.condition(value)
            }
            Type::Complex(component) => {
                let component = *component;
                let zero = self.value(
                    &value.node,
                    Type::Numeric(component),
                    ValueKind::Constant(match component {
                        NumericType::Integer { .. } => Number::Integer(0u32.into()),
                        NumericType::Float(format) => Number::float_zero(format),
                    }),
                );
                let zero = self.context.emit_arithmetic_conversion(
                    zero,
                    value.ty.clone(),
                    ConversionReason::UsualArith,
                )?;
                let node = value.node.clone();
                self.value(
                    &node,
                    Type::Bool,
                    ValueKind::Compare {
                        op: CompareOp::Ne,
                        left: Box::new(value),
                        right: Box::new(zero),
                        exceptions: matches!(component, NumericType::Float(_))
                            .then_some(self.context.region.floating.exceptions),
                        reason,
                    },
                )
            }
            ty if self.pointee(ty).is_ok() => {
                let zero = self.value(&value.node, ty.clone(), ValueKind::Null);
                let node = value.node.clone();
                self.value(
                    &node,
                    Type::Bool,
                    ValueKind::Compare {
                        op: CompareOp::Ne,
                        left: Box::new(value),
                        right: Box::new(zero),
                        exceptions: None,
                        reason,
                    },
                )
            }
            _ => return Err(ResolveError::Internal("non-scalar condition")),
        };
        if let ValueKind::Compare { reason: why, .. } = &mut result.node.value {
            *why = reason;
        }
        Ok(result)
    }

    fn voided(&mut self, e: &Expr, value: Operand) -> Operand {
        if self.types.ctypes.is_void(value.c) {
            return value;
        }
        let void = self.types.ctypes.qual(CTypeKind::Void);
        let end = self.value(e, Type::Void, ValueKind::Void);
        self.operand(
            e,
            void,
            ValueKind::Sequence {
                left: Box::new(value.value),
                right: Box::new(end),
            },
        )
    }

    fn enum_underlying(&self, ty: &Type) -> Option<Type> {
        match self.kind(ty) {
            Some(TypeDefinitionKind::Enum { underlying, .. }) => underlying.clone(),
            Some(TypeDefinitionKind::Alias(inner)) => self.enum_underlying(inner),
            _ => None,
        }
    }

    pub fn enum_integer(&self, value: Value) -> Value {
        if let Some(ty) = self.enum_underlying(&value.ty) {
            let node = value.node.clone();
            return self.value(
                &node,
                ty,
                ValueKind::Convert {
                    kind: ConversionKind::EnumToInt,
                    operand: Box::new(value),
                    reason: ConversionReason::Promotion,
                    semantics: ConversionSema::Exact,
                },
            );
        }
        value
    }

    pub(super) fn addressable(&self, place: &Place) -> Result<(), ResolveError> {
        match &place.kind {
            PlaceKind::Binding(id) if self.types.entities.is_register(id) => Err(
                ResolveError::Internal("address of register variable requested"),
            ),
            PlaceKind::Field { bits: Some(_), .. } => {
                Err(ResolveError::Internal("address of a bit-field"))
            }
            PlaceKind::Lane { .. } => Err(ResolveError::Internal("address of a vector element")),
            _ if temporary_rooted(place) => Err(ResolveError::Internal("address of a temporary")),
            _ => Ok(()),
        }
    }

    pub(super) fn place(&mut self, e: &Expr) -> Result<Lvalue, ResolveError> {
        let place = self.lower_place(e).map_err(|error| error.at(e.expansion))?;
        if let Ok(typed) = self.types.typed(e)
            && typed.lvalue
            && typed.c != place.c
        {
            return Err(ResolveError::Internal(
                "expression typer disagrees with lowering",
            ));
        }
        Ok(place)
    }

    fn lower_place(&mut self, e: &Expr) -> Result<Lvalue, ResolveError> {
        match &e.value {
            ExprKind::Paren(inner) => self.place(inner),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selected(controlling, associations)?;
                self.place(selected)
            }
            ExprKind::Call { callee, arguments }
                if choose_expr_operands(callee, arguments).is_some() =>
            {
                let chosen = self.types.chosen_expr(callee, arguments)?;
                self.place(chosen)
            }
            ExprKind::Identifier(name) if predefined_function_name(name) => {
                let text = self.types.predefined_name(name).to_owned();
                self.string_global(e, &text)
            }
            ExprKind::Identifier(_) => {
                let id = self.reference(e)?;
                let ty = self
                    .types
                    .entities
                    .ty(&id)
                    .ok_or(ResolveError::Internal("untyped binding"))?;
                Ok(Lvalue {
                    c: ty,
                    place: Place {
                        ty: self.types.ir_type(ty),
                        kind: PlaceKind::Binding(id),
                        access: self.types.access_of(ty),
                    },
                })
            }
            ExprKind::CompoundLiteral { ty, initializer } => {
                let extents = self.type_name_extents(ty)?;
                let resolved = self.resolve_type_name(ty)?;
                let access = self.types.access_of(resolved);
                let _declared = self.types.object_type(resolved, "void compound literal")?;
                let anchor = e.derive(());
                let value = self.initializer_value(
                    resolved,
                    &Initializer::List(initializer.clone()),
                    &anchor,
                )?;
                let value = self.captured(e, extents, value);
                let object = self.fresh();
                let ty = value.ty.clone();
                let c = self.with_length(resolved, &ty);
                self.types.entities.declare(object, c, false);
                let storage = if self.in_function {
                    StorageDuration::Automatic
                } else {
                    StorageDuration::Static
                };
                Ok(Lvalue {
                    c,
                    place: Place {
                        ty,
                        kind: PlaceKind::CompoundLiteral {
                            object,
                            storage,
                            initializer: Box::new(value),
                        },
                        access,
                    },
                })
            }
            ExprKind::Unary {
                op: UnaryOp::Deref,
                operand,
            } => {
                let value = self.expr(operand)?;
                self.deref(value)
            }
            ExprKind::Unary {
                op: UnaryOp::Real | UnaryOp::Imag,
                operand,
            } => {
                let base = self.place(operand)?;
                let Type::Complex(component) = base.ty else {
                    return Err(ResolveError::Unimplemented(
                        "complex component of non-complex place",
                    ));
                };
                let c = self
                    .types
                    .ctypes
                    .arithmetic_component(base.c)
                    .with(self.types.ctypes.quals(base.c));
                Ok(Lvalue {
                    c,
                    place: Place {
                        ty: Type::Numeric(component),
                        access: base.access,
                        kind: PlaceKind::ComplexPart {
                            base: Box::new(base.place),
                            imaginary: matches!(
                                &e.value,
                                ExprKind::Unary {
                                    op: UnaryOp::Imag,
                                    ..
                                }
                            ),
                        },
                    },
                })
            }
            ExprKind::Index { base, index } => match self.subscript(e, base, index)? {
                Projection::Place(place) => Ok(place),
                Projection::Value(_) => Err(ResolveError::Internal(
                    "vector element of a value is not a place",
                )),
            },
            ExprKind::Member { base, field, arrow } => {
                match self.member(e, base, &field.value, *arrow)? {
                    Projection::Place(place) => Ok(place),
                    Projection::Value(_) => Err(ResolveError::Internal(
                        "vector components of a value are not a place",
                    )),
                }
            }
            _ => Err(ResolveError::Internal(
                "expression is not a supported place",
            )),
        }
    }

    fn subscript(
        &mut self,
        e: &Expr,
        base: &Expr,
        index: &Expr,
    ) -> Result<Projection, ResolveError> {
        let object = match self.place(base) {
            Ok(object) if self.types.ctypes.is_vector(object.c) => {
                let index = self.expr(index)?;
                return Ok(Projection::Place(self.lane(object, index)?));
            }
            Ok(object) => self.read(base, object)?,
            Err(_) => self.expr(base)?,
        };
        let index = self.expr(index)?;
        if self.types.ctypes.is_vector(object.c) {
            let c = self.vector_element(object.c, &index)?;
            return Ok(Projection::Value(self.operand(
                e,
                c,
                ValueKind::Lane {
                    vector: Box::new(object.value),
                    index: Box::new(index.value),
                },
            )));
        }
        let pointer = self.binary(e, BinaryOp::Add, object, index)?;
        Ok(Projection::Place(self.deref(pointer)?))
    }

    fn vector_parts(&mut self, vector: QualType) -> Result<(QualType, u32), ResolveError> {
        match self.types.ctypes.canonical_kind(vector) {
            CTypeKind::Vector { element, lanes, .. } => Ok((*element, *lanes)),
            _ => Err(ResolveError::Internal("expected a vector operand")),
        }
    }

    fn shuffle_builtin(&mut self, e: &Expr, arguments: &[Expr]) -> Result<Operand, ResolveError> {
        let [left, right, indices @ ..] = arguments else {
            return Err(ResolveError::Internal("shuffle builtin arity"));
        };
        let left = self.expr(left)?;
        let right = self.expr(right)?;
        let (c, mask) = self.types.shuffle(left.c, right.c, indices)?;
        let (right, mask) = match mask {
            Some(mask) => (Some(Box::new(right.value)), ShuffleMask::Lanes(mask)),
            None => (None, ShuffleMask::Dynamic(Box::new(right.value))),
        };
        Ok(self.operand(
            e,
            c,
            ValueKind::Shuffle {
                left: Box::new(left.value),
                right,
                mask,
            },
        ))
    }

    fn materialize(&mut self, value: Operand) -> Lvalue {
        let object = self.fresh();
        let c = value.c;
        self.types.entities.declare(object, c, false);
        Lvalue {
            c,
            place: Place {
                ty: value.value.ty.clone(),
                kind: PlaceKind::Temporary {
                    object,
                    initializer: Box::new(value.value),
                },
                access: self.types.access_of(c),
            },
        }
    }

    fn member(
        &mut self,
        e: &Expr,
        base: &Expr,
        field: &str,
        arrow: bool,
    ) -> Result<Projection, ResolveError> {
        let object = if arrow {
            let value = self.expr(base)?;
            self.deref(value)?
        } else {
            match self.place(base) {
                Ok(object) => object,
                Err(error) => {
                    let value = self.expr(base)?;
                    if self.types.ctypes.is_record(value.c) {
                        self.materialize(value)
                    } else if !self.types.ctypes.is_vector(value.c) {
                        return Err(error);
                    } else {
                        let (c, mask) = self.types.swizzle(value.c, field)?;
                        return Ok(Projection::Value(self.operand(
                            e,
                            c,
                            ValueKind::Shuffle {
                                left: Box::new(value.value),
                                right: None,
                                mask: ShuffleMask::Lanes(mask),
                            },
                        )));
                    }
                }
            }
        };
        if !self.types.ctypes.is_vector(object.c) {
            return self
                .project(object, field)?
                .map(Projection::Place)
                .ok_or(ResolveError::Internal("unknown member"));
        }
        let (c, mask) = self.types.swizzle(object.c, field)?;
        let lanes: Option<Vec<u32>> = mask.iter().copied().collect();
        let assignable = lanes.as_ref().is_some_and(|lanes| {
            lanes
                .iter()
                .enumerate()
                .all(|(position, lane)| !lanes[..position].contains(lane))
        });
        match lanes.filter(|_| assignable) {
            Some(lanes) if lanes.len() == 1 => {
                let index = self.types.ctypes.int();
                let index = self.operand(
                    e,
                    index,
                    ValueKind::Constant(Number::Integer(lanes[0].into())),
                );
                Ok(Projection::Place(self.lane(object, index)?))
            }
            Some(lanes) => Ok(Projection::Place(Lvalue {
                c,
                place: Place {
                    ty: self.types.ir_type(c),
                    access: object.place.access,
                    kind: PlaceKind::Swizzle {
                        base: Box::new(object.place),
                        lanes,
                    },
                },
            })),
            None => {
                let value = self.read(base, object)?;
                Ok(Projection::Value(self.operand(
                    e,
                    c,
                    ValueKind::Shuffle {
                        left: Box::new(value.value),
                        right: None,
                        mask: ShuffleMask::Lanes(mask),
                    },
                )))
            }
        }
    }

    fn lane(&mut self, object: Lvalue, index: Operand) -> Result<Lvalue, ResolveError> {
        let element = self.vector_element(object.c, &index)?;
        let c = element.with(self.types.ctypes.quals(object.c));
        Ok(Lvalue {
            c,
            place: Place {
                ty: self.types.ir_type(c),
                access: object.place.access,
                kind: PlaceKind::Lane {
                    base: Box::new(object.place),
                    index: Box::new(index.value),
                },
            },
        })
    }

    fn vector_element(
        &mut self,
        vector: QualType,
        index: &Operand,
    ) -> Result<QualType, ResolveError> {
        if !self.types.ctypes.is_integer(index.c) {
            return Err(ResolveError::Internal("vector index is not an integer"));
        }
        match self.types.ctypes.canonical_kind(vector) {
            CTypeKind::Vector { element, .. } => Ok(*element),
            _ => Err(ResolveError::Internal("expected vector")),
        }
    }

    fn record_body(&self, ty: &Type) -> Option<(Vec<Span<Field>>, Option<RecordLayout>)> {
        match self.kind(ty)? {
            TypeDefinitionKind::Record {
                fields: Some(fields),
                layout,
                ..
            } => Some((fields.clone(), layout.clone())),
            TypeDefinitionKind::Alias(inner) => {
                let inner = inner.clone();
                self.record_body(&inner)
            }
            _ => None,
        }
    }

    fn field_place(
        &self,
        base: Lvalue,
        index: usize,
        field: &Field,
        layout: Option<&RecordLayout>,
    ) -> Result<Lvalue, ResolveError> {
        let bits = match field.bit_width {
            Some(width) if width != 0 => {
                let layout =
                    layout.ok_or(ResolveError::Internal("bit-field in unlaid-out record"))?;
                let unit = layout
                    .field_units
                    .get(index)
                    .copied()
                    .flatten()
                    .ok_or(ResolveError::Internal("bit-field without storage unit"))?;
                let position = layout
                    .bit_offsets
                    .get(index)
                    .copied()
                    .flatten()
                    .ok_or(ResolveError::Internal("bit-field without bit offset"))?;
                let unit = layout
                    .bit_units
                    .get(unit)
                    .map(|storage| (unit, storage))
                    .ok_or(ResolveError::Internal("bit-field without storage unit"))?;
                Some(BitFieldAccess {
                    unit: unit.0,
                    unit_offset: unit.1.offset,
                    unit_size: unit.1.size,
                    bit_offset: position - unit.1.offset * 8,
                    width,
                })
            }
            Some(_) => return Err(ResolveError::Internal("zero-width bit-field access")),
            None => None,
        };
        let CTypeKind::Record { id, .. } = self.types.ctypes.canonical_kind(base.c) else {
            return Err(ResolveError::Internal("field of non-record C type"));
        };
        let c = self
            .types
            .record_fields
            .get(id)
            .and_then(|fields| fields.get(index))
            .copied()
            .ok_or(ResolveError::Internal("missing field C type"))?
            .with(self.types.ctypes.quals(base.c));
        Ok(Lvalue {
            c,
            place: Place {
                ty: field.ty.clone(),
                access: base.access.union(field.access),
                kind: PlaceKind::Field {
                    base: Box::new(base.place),
                    index,
                    bits,
                },
            },
        })
    }

    fn project(&self, base: Lvalue, name: &str) -> Result<Option<Lvalue>, ResolveError> {
        let Some((fields, layout)) = self.record_body(&base.ty) else {
            return Err(ResolveError::Internal("member of incomplete or non-record"));
        };
        if let Some((index, field)) = fields
            .iter()
            .enumerate()
            .find(|(_, f)| f.name.as_deref() == Some(name))
        {
            return self
                .field_place(base, index, field, layout.as_ref())
                .map(Some);
        }
        for (index, field) in fields.iter().enumerate() {
            if field.name.is_some() || self.record_body(&field.ty).is_none() {
                continue;
            }
            let nested = self.field_place(base.clone(), index, field, layout.as_ref())?;
            if let Some(found) = self.project(nested, name)? {
                return Ok(Some(found));
            }
        }
        Ok(None)
    }

    pub(super) fn generic_selected<'e>(
        &mut self,
        controlling: &crate::ast::GenericControl,
        associations: &'e [crate::ast::GenericAssociation],
    ) -> Result<&'e Expr, ResolveError> {
        let controlling = match controlling {
            crate::ast::GenericControl::Type { ty } => self.resolve_type_name(ty)?,
            crate::ast::GenericControl::Expr(expr) => {
                let ty = self.unevaluated(expr)?;
                self.types.ctypes.lvalue_conversion(ty)
            }
        };
        for association in associations {
            if let crate::ast::GenericAssociation::Type { ty, .. } = association {
                self.prepare_typeof(&ty.specifiers, &ty.declarator)?;
            }
        }
        self.types.select_association(controlling, associations)
    }

    fn unevaluated(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        let typed = self.types.typed(e)?;
        Ok(self.types.ctypes.lvalue_conversion(typed.c))
    }

    fn callee(&mut self, e: &Expr) -> Result<(Callee, QualType), ResolveError> {
        let value = self.expr(e)?;
        let signature = self
            .types
            .ctypes
            .pointee(value.c)
            .ok_or(ResolveError::Internal("non-function callee"))?;
        if !self.types.ctypes.is_function(signature) {
            return Err(ResolveError::Internal("non-function callee"));
        }
        if let ValueKind::FunctionDecay {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
        } = &value.node.value
        {
            return Ok((Callee::Direct(*id), signature));
        }
        Ok((Callee::Indirect(Box::new(value.value)), signature))
    }

    pub(super) fn read(&mut self, e: &Expr, lvalue: Lvalue) -> Result<Operand, ResolveError> {
        let c = self.types.ctypes.lvalue_conversion(lvalue.c);
        let place = lvalue.place;
        let kind = match &place.ty {
            Type::Array { length, .. } => ValueKind::ArrayDecay {
                length: *length,
                place,
            },
            Type::VariableArray { .. } => ValueKind::ArrayDecay {
                length: None,
                place,
            },
            Type::Function { .. } => {
                if let PlaceKind::Deref(pointer) = place.kind {
                    return Ok(Operand { value: *pointer, c });
                }
                ValueKind::FunctionDecay { place }
            }
            _ => {
                let bits = match &place.kind {
                    PlaceKind::Field {
                        bits: Some(bits), ..
                    } => Some(bits.width),
                    _ => None,
                };
                let ordering = place.implicit_ordering();
                let value = self.operand(e, c, ValueKind::Read { place, ordering });
                return if bits.is_some() {
                    self.types.promote_operand(&self.context, value, bits)
                } else {
                    Ok(value)
                };
            }
        };
        Ok(self.operand(e, c, kind))
    }

    fn binary(
        &mut self,
        e: &Expr,
        op: BinaryOp,
        left: Operand,
        right: Operand,
    ) -> Result<Operand, ResolveError> {
        let lp = self.types.ctypes.pointee(left.c);
        let rp = self.types.ctypes.pointee(right.c);
        match (op, lp, rp) {
            (BinaryOp::Sub, Some(element), Some(other)) => {
                let a = self.types.ctypes.canonical(element).local_unqualified();
                let b = self.types.ctypes.canonical(other).local_unqualified();
                if !self.types.ctypes.compatible(a, b) {
                    return Err(ResolveError::Internal("incompatible pointer subtraction"));
                }
                let element = self.types.ir_type(element);
                self.types.require_pointer_element(&element)?;
                let c = self.types.ctypes.ptrdiff_type(&self.context.target);
                Ok(self.operand(
                    e,
                    c,
                    ValueKind::PointerDifference {
                        left: Box::new(left.value),
                        right: Box::new(right.value),
                        element,
                    },
                ))
            }
            (BinaryOp::Add | BinaryOp::Sub, Some(element), None) => {
                self.pointer_offset(e, left, right, element, op == BinaryOp::Sub)
            }
            (BinaryOp::Add, None, Some(element)) => {
                self.pointer_offset(e, right, left, element, false)
            }
            _ => self.types.binary_operand(&self.context, e, op, left, right),
        }
    }

    fn pointer_offset(
        &mut self,
        e: &Expr,
        pointer: Operand,
        amount: Operand,
        element: QualType,
        subtract: bool,
    ) -> Result<Operand, ResolveError> {
        let element = self.types.ir_type(element);
        self.types.require_pointer_element(&element)?;
        let amount = self.promote(amount)?;
        if !matches!(amount.ty, Type::Numeric(NumericType::Integer { .. })) {
            return Err(ResolveError::Internal("noninteger pointer offset"));
        }
        Ok(self.operand(
            e,
            pointer.c,
            ValueKind::PointerOffset {
                pointer: Box::new(pointer.value),
                amount: Box::new(amount.value),
                subtract,
                element,
                overflow: if self.context.pointer_wrap {
                    Overflow::Wrap
                } else {
                    Overflow::Undefined
                },
            },
        ))
    }

    fn update(
        &mut self,
        e: &Expr,
        target: &Expr,
        op: BinaryOp,
        rhs: Operand,
        postfix: bool,
    ) -> Result<Operand, ResolveError> {
        let place = self.place(target)?;
        self.types.require_modifiable_lvalue(place.c)?;
        if temporary_rooted(&place.place) {
            return Err(ResolveError::Internal("expression is not assignable"));
        }
        let c = self.types.ctypes.unqualified(place.c);
        let old = self.operand(target, c, ValueKind::OldValue);
        let bits = match &place.kind {
            PlaceKind::Field {
                bits: Some(bits), ..
            } => Some(bits.width),
            _ => None,
        };
        let old = if bits.is_some() {
            self.types.promote_operand(&self.context, old, bits)?
        } else {
            old
        };
        let computation = self.binary(e, op, old, rhs)?;
        let conversion = self.types.ctypes.classify_conversion(
            computation.c,
            c,
            ConversionContext::Assign,
            false,
        )?;
        let computation =
            self.apply_conversion(conversion, computation, c, ConversionReason::Assign)?;
        Ok(self.operand(
            e,
            c,
            ValueKind::Update {
                ordering: place.implicit_ordering(),
                place: place.place,
                computation: Box::new(computation.value),
                postfix,
            },
        ))
    }

    fn unevaluated_type(
        &mut self,
        operand: &Expr,
    ) -> Result<(QualType, Option<Value>), ResolveError> {
        if let ExprKind::Paren(inner) = &operand.value {
            return self.unevaluated_type(inner);
        }
        if let ExprKind::StringLiteral(lit) = &operand.value {
            return Ok((self.types.string_type(lit), None));
        }
        let typed = self.types.typed(operand)?;
        if typed.bits.is_some() {
            return Err(ResolveError::Internal(
                "application of sizeof or alignof to a bit-field",
            ));
        }
        if !matches!(self.types.ir_type(typed.c), Type::VariableArray { .. }) {
            return Ok((typed.c, None));
        }
        let (c, value) = if typed.lvalue {
            let lvalue = self.place(operand)?;
            let ty = self.types.ir_type(lvalue.c);
            let address = self.value(
                operand,
                Type::Pointer {
                    pointee: Box::new(ty),
                    is_const: false,
                    access: Access::default(),
                    space: PointerSpace::Default,
                },
                ValueKind::AddressOf(lvalue.place),
            );
            (lvalue.c, address)
        } else {
            let value = self.expr(operand)?;
            (value.c, value.value)
        };
        Ok((c, super::effects::has_effects(&value).then_some(value)))
    }

    fn type_name_extents(
        &mut self,
        ty: &crate::ast::TypeName,
    ) -> Result<Vec<(BindingId, Value)>, ResolveError> {
        let mut extents = Vec::new();
        if self.in_function {
            self.typeof_evaluations(&ty.specifiers.ty, &mut extents)?;
            self.extents(&ty.declarator, &mut extents)?;
        }
        Ok(extents)
    }

    fn with_extents(
        &mut self,
        e: &Expr,
        extents: Vec<(BindingId, Value)>,
        value: Operand,
    ) -> Operand {
        Operand {
            c: value.c,
            value: self.captured(e, extents, value.value),
        }
    }

    fn captured(&mut self, e: &Expr, extents: Vec<(BindingId, Value)>, value: Value) -> Value {
        extents
            .into_iter()
            .rev()
            .fold(value, |value, (id, extent)| {
                let ty = value.ty.clone();
                self.value(
                    e,
                    ty,
                    ValueKind::Capture {
                        id,
                        extent: Box::new(extent),
                        value: Box::new(value),
                    },
                )
            })
    }

    fn runtime_size(&mut self, e: &Expr, ty: &Type) -> Result<Operand, ResolveError> {
        let c = self.types.ctypes.size_type(&self.context.target);
        let size_type = self.types.ir_type(c);
        let Type::VariableArray { element, extent } = ty else {
            let size = self.types.storage(ty.clone())?.size_bytes;
            return Ok(self.operand(e, c, ValueKind::Constant(Number::Integer(size.into()))));
        };
        let VariableExtent::Captured(extent) = *extent else {
            return Err(ResolveError::Internal(
                "size of an unspecified variable length array",
            ));
        };
        let place = Place {
            ty: size_type.clone(),
            kind: PlaceKind::Binding(extent),
            access: Access::default(),
        };
        let count = self.operand(
            e,
            c,
            ValueKind::Read {
                place,
                ordering: None,
            },
        );
        let element = self.runtime_size(e, element)?;
        self.binary(e, BinaryOp::Mul, count, element)
    }

    fn layout_constant(&mut self, e: &Expr, amount: u64, key: &str, detail: String) -> Operand {
        let c = self.types.ctypes.size_type(&self.context.target);
        let value = self.operand(e, c, ValueKind::Constant(Number::Integer(amount.into())));
        self.module.annotate(&value.node, [(key.into(), detail)]);
        value
    }

    pub fn expr(&mut self, e: &Expr) -> Result<Operand, ResolveError> {
        let lowered = self.lower_expr(e).map_err(|error| error.at(e.expansion))?;
        if let Ok(typed) = self.types.typed(e)
            && self.types.rvalue_type(typed) != lowered.c
        {
            return Err(ResolveError::Internal(
                "expression typer disagrees with lowering",
            ));
        }
        Ok(lowered)
    }

    fn lower_expr(&mut self, e: &Expr) -> Result<Operand, ResolveError> {
        if let ExprKind::Call { callee, arguments } = &e.value {
            if let Some(value) = self.function_like_builtin(e, callee, arguments)? {
                return Ok(value);
            }
            if let Some((builtin, _)) = self.types.builtin_callee(callee, arguments) {
                return Err(ResolveError::UnsupportedBuiltin(builtin.name.to_owned()));
            }
        }
        match &e.value {
            ExprKind::Paren(inner) => self.expr(inner),
            ExprKind::Identifier(_) if self.binding_kind(e) == Some(BindingKind::Enumerator) => {
                let value = self
                    .types
                    .constant(e)
                    .cloned()
                    .ok_or(ResolveError::Internal("unresolved enumerator constant"))?;
                Ok(self.operand(e, value.c, value.value.node.value))
            }
            ExprKind::Index { base, index } => match self.subscript(e, base, index)? {
                Projection::Place(place) => self.read(e, place),
                Projection::Value(value) => Ok(value),
            },
            ExprKind::Member { base, field, arrow } => {
                match self.member(e, base, &field.value, *arrow)? {
                    Projection::Place(place) => self.read(e, place),
                    Projection::Value(value) => Ok(value),
                }
            }
            ExprKind::Identifier(_)
            | ExprKind::CompoundLiteral { .. }
            | ExprKind::Unary {
                op: UnaryOp::Deref, ..
            } => {
                let place = self.place(e)?;
                self.read(e, place)
            }
            ExprKind::CharLiteral(lit) => {
                let (ty, number) = self.types.character_constant(lit)?;
                Ok(self.operand(e, ty, ValueKind::Constant(number)))
            }
            ExprKind::LabelAddress(label) => {
                let id = self
                    .names
                    .references
                    .iter()
                    .find(|r| r.id == label.id)
                    .map(|r| r.binding)
                    .ok_or(ResolveError::Internal("missing label address binding"))?;
                let void = self.types.ctypes.qual(CTypeKind::Void);
                let ty = self.types.ctypes.pointer(void);
                Ok(self.operand(e, ty, ValueKind::LabelAddress(id)))
            }
            ExprKind::NullPtrLiteral => {
                let ty = self.types.ctypes.qual(CTypeKind::NullPtr);
                Ok(self.operand(e, ty, ValueKind::Null))
            }
            ExprKind::StringLiteral(lit) => {
                let mut units = lit.execution_units(self.context.target.wchar_width);
                units.push(0);
                let c = self.types.string_type(lit);
                let ty = self.types.ir_type(c);
                let id = self.fresh();
                let initializer = self.value(e, ty.clone(), ValueKind::CodeUnits(units));
                self.module.globals.push(e.derive(Global {
                    variable: Variable {
                        id,
                        name: format!(".str{}", id.0),
                        ty: ty.clone(),
                        storage: StorageDuration::Static,
                        restrict: false,
                        is_const: false,
                        access: Access::default(),
                        constexpr: false,
                        alignment: None,
                        cleanup: None,
                        register: None,
                        initializer: Some(initializer),
                    },
                    linkage: Linkage::Internal,
                    symbol: SymbolAttributes::default(),
                    definition: true,
                    common: false,
                }));
                self.read(
                    e,
                    Lvalue {
                        c,
                        place: Place {
                            ty,
                            kind: PlaceKind::Binding(id),
                            access: Access::default(),
                        },
                    },
                )
            }
            ExprKind::Cast { ty, value } => {
                let extents = self.type_name_extents(ty)?;
                let to = self.resolve_type_name(ty)?;
                let to = self.types.ctypes.unqualified(to);
                let is_void = self.types.ctypes.is_void(to);
                let value_expr = value;
                let value = self.expr(value)?;
                let cast = if let Some(index) = self.types.union_cast_member(to, value.c)? {
                    self.operand(
                        e,
                        to,
                        ValueKind::Aggregate {
                            members: vec![AggregateMember {
                                target: AggregateTarget::Field(index),
                                value: value.value,
                            }],
                            zero_fill: false,
                        },
                    )
                } else if !is_void {
                    self.convert_recorded(value_expr, value, to, ConversionReason::Explicit)?
                } else {
                    let end = self.value(e, Type::Void, ValueKind::Void);
                    self.operand(
                        e,
                        to,
                        ValueKind::Sequence {
                            left: Box::new(value.value),
                            right: Box::new(end),
                        },
                    )
                };
                Ok(self.with_extents(e, extents, cast))
            }
            ExprKind::Unary {
                op: UnaryOp::AddrOf,
                operand,
            } => {
                let place = self.place(operand)?;
                self.addressable(&place.place)?;
                let ty = self.types.ctypes.pointer(place.c);
                Ok(self.operand(e, ty, ValueKind::AddressOf(place.place)))
            }
            ExprKind::Unary {
                op: UnaryOp::PreIncrement | UnaryOp::PreDecrement,
                operand,
            }
            | ExprKind::Postfix { operand, .. } => {
                let decrement = matches!(
                    e.value,
                    ExprKind::Unary {
                        op: UnaryOp::PreDecrement,
                        ..
                    } | ExprKind::Postfix {
                        op: PostfixOp::Decrement,
                        ..
                    }
                );
                let c = self.types.ctypes.int();
                let rhs = self.operand(e, c, ValueKind::Constant(Number::Integer(1u32.into())));
                self.update(
                    e,
                    operand,
                    if decrement {
                        BinaryOp::Sub
                    } else {
                        BinaryOp::Add
                    },
                    rhs,
                    matches!(e.value, ExprKind::Postfix { .. }),
                )
            }
            ExprKind::Unary {
                op: UnaryOp::Real | UnaryOp::Imag,
                operand,
            } => {
                if let Ok(place) = self.place(e) {
                    return self.read(e, place);
                }
                let value = self.expr(operand)?;
                let real = matches!(
                    &e.value,
                    ExprKind::Unary {
                        op: UnaryOp::Real,
                        ..
                    }
                );
                let Type::Complex(_component) = value.ty else {
                    let value = self.promote(value)?;
                    if real {
                        return Ok(value);
                    }
                    let c = value.c;
                    return Ok(self.operand(
                        e,
                        c,
                        ValueKind::Constant(Number::Integer(0u8.into())),
                    ));
                };
                let c = self.types.ctypes.arithmetic_component(value.c);
                Ok(self.operand(
                    e,
                    c,
                    ValueKind::Convert {
                        kind: if real {
                            ConversionKind::ComplexToReal
                        } else {
                            ConversionKind::ComplexToImag
                        },
                        operand: Box::new(value.value),
                        reason: ConversionReason::Explicit,
                        semantics: ConversionSema::Exact,
                    },
                ))
            }
            ExprKind::Unary { op, operand } => {
                let value = self.expr(operand)?;
                let value = self.enum_operand(value);
                match op {
                    UnaryOp::Plus => {
                        if !matches!(
                            value.ty,
                            Type::Bool
                                | Type::Numeric(_)
                                | Type::Complex(_)
                                | Type::Imaginary(_)
                                | Type::FixedPoint(_)
                                | Type::Vector { .. }
                        ) {
                            return Err(ResolveError::Internal("non-numeric unary plus"));
                        }
                        self.promote(value)
                    }
                    UnaryOp::Not => {
                        let value = self.condition(value.value, None)?;
                        Ok(self.truth(
                            e,
                            ValueKind::Unary {
                                op: UnaryArithOp::Not,
                                operand: Box::new(value),
                                semantics: ArithSema::Exact,
                            },
                        ))
                    }
                    UnaryOp::Minus | UnaryOp::BitNot => {
                        self.types.unary_operand(&self.context, e, *op, value)
                    }
                    _ => Err(ResolveError::Internal("advanced unary operator")),
                }
            }
            ExprKind::Binary { op, left, right } => {
                let left_expr = left;
                let right_expr = right;
                let left = self.expr(left)?;
                let left = self.enum_operand(left);
                let right = self.expr(right)?;
                let right = self.enum_operand(right);
                if matches!(op, BinaryOp::And | BinaryOp::Or) {
                    return Ok(self.truth(
                        e,
                        ValueKind::Logical {
                            op: if *op == BinaryOp::And {
                                LogicalOp::And
                            } else {
                                LogicalOp::Or
                            },
                            left: Box::new(self.condition(left.value, None)?),
                            right: Box::new(self.condition(right.value, None)?),
                        },
                    ));
                }
                if matches!(op, BinaryOp::Equal | BinaryOp::NotEqual)
                    && (self.pointee(&left.ty).is_ok() || self.pointee(&right.ty).is_ok())
                {
                    let is_c_pointer = |operand: &Operand| {
                        self.pointee(&operand.ty).is_ok()
                            && !self.types.ctypes.is_nullptr(operand.c)
                    };
                    let ty = if is_c_pointer(&left) {
                        left.c
                    } else if is_c_pointer(&right) || self.pointee(&left.ty).is_err() {
                        right.c
                    } else {
                        left.c
                    };
                    self.warn_comparison(e, left_expr, &left, right_expr, &right);
                    let left =
                        self.convert_expr(left_expr, left, ty, ConversionReason::UsualArith)?;
                    let right =
                        self.convert_expr(right_expr, right, ty, ConversionReason::UsualArith)?;
                    return Ok(self.truth(
                        e,
                        ValueKind::Compare {
                            op: if *op == BinaryOp::Equal {
                                CompareOp::Eq
                            } else {
                                CompareOp::Ne
                            },
                            left: Box::new(left.value),
                            right: Box::new(right.value),
                            exceptions: None,
                            reason: None,
                        },
                    ));
                }
                if matches!(
                    op,
                    BinaryOp::Less
                        | BinaryOp::LessEqual
                        | BinaryOp::Greater
                        | BinaryOp::GreaterEqual
                ) && (self.pointee(&left.ty).is_ok() && self.pointee(&right.ty).is_ok())
                {
                    let ty = left.c;
                    self.warn_comparison(e, left_expr, &left, right_expr, &right);
                    let left =
                        self.convert_expr(left_expr, left, ty, ConversionReason::UsualArith)?;
                    let right =
                        self.convert_expr(right_expr, right, ty, ConversionReason::UsualArith)?;
                    return Ok(self.truth(
                        e,
                        ValueKind::Compare {
                            op: match op {
                                BinaryOp::Less => CompareOp::Lt,
                                BinaryOp::LessEqual => CompareOp::Le,
                                BinaryOp::Greater => CompareOp::Gt,
                                _ => CompareOp::Ge,
                            },
                            left: Box::new(left.value),
                            right: Box::new(right.value),
                            exceptions: None,
                            reason: None,
                        },
                    ));
                }
                self.binary(e, *op, left, right)
            }
            ExprKind::Assign { op, target, value } => {
                let value_expr = value;
                let value = self.expr(value)?;
                if *op == AssignOp::Assign {
                    let place = self.place(target)?;
                    self.types.require_modifiable_lvalue(place.c)?;
                    if temporary_rooted(&place.place) {
                        return Err(ResolveError::Internal("expression is not assignable"));
                    }
                    let value = self.convert_recorded(
                        value_expr,
                        value,
                        place.c,
                        ConversionReason::Assign,
                    )?;
                    let c = value.c;
                    return Ok(self.operand(
                        e,
                        c,
                        ValueKind::Store {
                            ordering: place.implicit_ordering(),
                            place: place.place,
                            value: Box::new(value.value),
                        },
                    ));
                }
                self.update(e, target, assignment_operator(*op)?, value, false)
            }
            ExprKind::Comma { left, right } => {
                let left = self.expr(left)?;
                let right = self.expr(right)?;
                Ok(Operand {
                    c: right.c,
                    value: self.value(
                        e,
                        right.ty.clone(),
                        ValueKind::Sequence {
                            left: Box::new(left.value),
                            right: Box::new(right.value),
                        },
                    ),
                })
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let (test, mut left, then_value, shared) = match then_value {
                    Some(then_value) => {
                        let test = self.expr(condition)?;
                        let test = self.condition(test.value, None)?;
                        (test, self.expr(then_value)?, then_value, None)
                    }
                    None => {
                        let shared = self.expr(condition)?;
                        let id = self.fresh();
                        self.types.entities.declare(id, shared.c, false);
                        let place = Place {
                            ty: shared.value.ty.clone(),
                            kind: PlaceKind::Binding(id),
                            access: self.types.access_of(shared.c),
                        };
                        let read = |this: &mut Self| {
                            this.operand(
                                condition,
                                shared.c,
                                ValueKind::Read {
                                    place: place.clone(),
                                    ordering: None,
                                },
                            )
                        };
                        let left = read(self);
                        let test = read(self).value;
                        let test = self.condition(test, None)?;
                        (test, left, condition, Some((id, shared.value)))
                    }
                };
                let mut right = self.expr(else_value)?;
                if self.types.ctypes.is_void(left.c) || self.types.ctypes.is_void(right.c) {
                    left = self.voided(then_value, left);
                    right = self.voided(else_value, right);
                } else if matches!(
                    left.ty,
                    Type::Bool | Type::Numeric(_) | Type::Complex(_) | Type::Imaginary(_)
                ) && matches!(
                    right.ty,
                    Type::Bool | Type::Numeric(_) | Type::Complex(_) | Type::Imaginary(_)
                ) {
                    (left, right) = self.types.arithmetic_operands(&self.context, left, right)?;
                } else if self.types.ctypes.is_pointer(left.c)
                    && self.types.ctypes.is_pointer(right.c)
                {
                    let merged = if self.is_null_pointer_constant(Some(else_value), &right) {
                        left.c
                    } else if self.is_null_pointer_constant(Some(then_value), &left) {
                        right.c
                    } else {
                        let rules = self.types.features().conditional_pointers;
                        self.types
                            .ctypes
                            .merge_pointer(left.c, right.c, rules)
                            .ok_or(ResolveError::Internal(
                                "conditional operands are pointers to incompatible types",
                            ))?
                    };
                    left =
                        self.convert_expr(then_value, left, merged, ConversionReason::UsualArith)?;
                    right =
                        self.convert_expr(else_value, right, merged, ConversionReason::UsualArith)?;
                } else if self.types.ctypes.is_pointer(left.c) {
                    right =
                        self.convert_expr(else_value, right, left.c, ConversionReason::UsualArith)?;
                } else if self.types.ctypes.is_pointer(right.c) {
                    left =
                        self.convert_expr(then_value, left, right.c, ConversionReason::UsualArith)?;
                }
                if !self.types.ctypes.compatible_unqualified(left.c, right.c) {
                    return Err(ResolveError::Internal(
                        "conditional operands have incompatible types",
                    ));
                }
                let result = self.operand(
                    e,
                    left.c,
                    ValueKind::Conditional {
                        condition: Box::new(test),
                        then_value: Box::new(left.value),
                        else_value: Box::new(right.value),
                    },
                );
                let Some((id, extent)) = shared else {
                    return Ok(result);
                };
                let c = result.c;
                Ok(self.operand(
                    e,
                    c,
                    ValueKind::Capture {
                        id,
                        extent: Box::new(extent),
                        value: Box::new(result.value),
                    },
                ))
            }
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selected(controlling, associations)?;
                self.expr(selected)
            }
            ExprKind::Call {
                callee: builtin,
                arguments,
            } if super::atomic::atomic_builtin(builtin).is_some() => {
                let atomic = super::atomic::atomic_builtin(builtin)
                    .ok_or(ResolveError::Internal("atomic builtin"))?;
                self.atomic_builtin(e, builtin, atomic, arguments)
            }
            ExprKind::Call { callee, arguments }
                if constant_p_operand(callee, arguments).is_some() =>
            {
                let operand = constant_p_operand(callee, arguments)
                    .ok_or(ResolveError::Internal("__builtin_constant_p"))?;
                let constant = super::types::is_folded(&self.expr(operand)?.value);
                let c = self.types.ctypes.int();
                let value = self.operand(
                    e,
                    c,
                    ValueKind::Constant(Number::SignedInteger(u8::from(constant).into())),
                );
                self.module.annotate(
                    &value.node,
                    [("c_builtin".into(), "__builtin_constant_p".into())],
                );
                Ok(value)
            }
            ExprKind::Call { callee, arguments }
                if arguments.is_empty() && source_location_builtin(callee).is_some() =>
            {
                let builtin = source_location_builtin(callee)
                    .ok_or(ResolveError::Internal("source location builtin"))?;
                self.source_location(e, callee, builtin)
            }
            ExprKind::Call { callee, arguments }
                if choose_expr_operands(callee, arguments).is_some() =>
            {
                let chosen = self.types.chosen_expr(callee, arguments)?;
                self.expr(chosen)
            }
            ExprKind::Call { callee, arguments } if va_builtin(callee).is_some() => {
                let builtin = va_builtin(callee).ok_or(ResolveError::Internal("va builtin"))?;
                self.va_builtin(e, builtin, arguments)
            }
            ExprKind::Call { callee, arguments } => {
                let (lowered, signature) = self.callee(callee)?;
                if !arguments.is_empty() {
                    let direct = matches!(lowered, Callee::Direct(_));
                    self.warn_arguments_without_prototype(e, callee, direct, signature);
                }
                self.call(e, lowered, signature, arguments)
            }
            ExprKind::IntegerLiteral(_) | ExprKind::FloatLiteral(_) | ExprKind::BoolLiteral(_) => {
                self.types.literal(&self.context, e)
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => {
                let extents = self.type_name_extents(ty)?;
                let resolved = self.resolve_type_name(ty)?;
                let atomic = self.types.ctypes.quals(resolved).is_atomic;
                let ty = self.types.ir_type(resolved);
                if matches!(e.value, ExprKind::SizeOfType { .. })
                    && matches!(ty, Type::VariableArray { .. })
                {
                    let size = self.runtime_size(e, &ty)?;
                    return Ok(self.with_extents(e, extents, size));
                }
                let layout = self
                    .types
                    .sizeof_storage(super::typer::fixed_element(&ty).clone(), atomic)
                    .map_err(ResolveError::checked)?;
                let layout = self.types.declared_storage(resolved, layout)?;
                let value = if matches!(e.value, ExprKind::SizeOfType { .. }) {
                    layout.size_bytes
                } else {
                    u64::from(layout.alignment_bytes)
                };
                let key = if matches!(e.value, ExprKind::SizeOfType { .. }) {
                    "size_of"
                } else {
                    "align_of"
                };
                Ok(self.layout_constant(e, value, key, ty.to_string()))
            }
            ExprKind::SizeOfExpr(operand) | ExprKind::AlignOfExpr(operand) => {
                let (c, evaluated) = self.unevaluated_type(operand)?;
                let ty = self.types.ir_type(c);
                if matches!(e.value, ExprKind::SizeOfExpr(_)) && self.types.ctypes.is_function(c) {
                    return Ok(self.layout_constant(e, 1, "size_of", ty.to_string()));
                }
                let access = self.types.access_of(c);
                if matches!(e.value, ExprKind::SizeOfExpr(_))
                    && matches!(ty, Type::VariableArray { .. })
                {
                    let size = self.runtime_size(e, &ty)?;
                    let Some(evaluated) = evaluated else {
                        return Ok(size);
                    };
                    let size_ty = size.value.ty.clone();
                    let value = self.value(
                        e,
                        size_ty,
                        ValueKind::Sequence {
                            left: Box::new(evaluated),
                            right: Box::new(size.value),
                        },
                    );
                    return Ok(Operand { value, c: size.c });
                }
                let layout = self
                    .types
                    .qualified_storage(super::typer::fixed_element(&ty).clone(), access.atomic)
                    .map_err(ResolveError::checked)?;
                let layout = self.types.declared_storage(c, layout)?;
                let amount = if matches!(e.value, ExprKind::SizeOfExpr(_)) {
                    layout.size_bytes
                } else {
                    self.types
                        .object_alignment(operand, u64::from(layout.alignment_bytes))
                };
                let key = if matches!(e.value, ExprKind::SizeOfExpr(_)) {
                    "size_of"
                } else {
                    "align_of"
                };
                Ok(self.layout_constant(e, amount, key, ty.to_string()))
            }
            ExprKind::TypesCompatible { left_ty, right_ty } => {
                let (compatible, compared) = self.types.types_compatible(left_ty, right_ty)?;
                let c = self.types.ctypes.int();
                let value = self.operand(
                    e,
                    c,
                    ValueKind::Constant(Number::SignedInteger(u8::from(compatible).into())),
                );
                self.module
                    .annotate(&value.node, [("types_compatible".into(), compared)]);
                Ok(value)
            }
            ExprKind::OffsetOf { ty, member } => {
                let resolved = self.resolve_type_name(ty)?;
                let ty = self.types.object_type(resolved, "void offsetof")?;
                let (_, offset) = self.types.offsetof_member(ty.clone(), member)?;
                Ok(self.layout_constant(e, offset, "offset_of", format!("{ty}.{member}")))
            }
            ExprKind::BitCast { ty, value } => {
                let resolved = self.resolve_type_name(ty)?;
                let ty = self
                    .types
                    .layout(resolved)
                    .ok_or(ResolveError::Internal("bit cast to void"))?;
                if matches!(ty, Type::Array { .. } | Type::VariableArray { .. }) {
                    return Err(ResolveError::Internal("bit cast to an array type"));
                }
                let value = self.expr(value)?;
                if self.types.storage(ty.clone())?.size_bytes
                    != self.types.storage(value.ty.clone())?.size_bytes
                {
                    return Err(ResolveError::Internal(
                        "bit cast between types of different sizes",
                    ));
                }
                Ok(self.operand(
                    e,
                    resolved,
                    ValueKind::Convert {
                        kind: ConversionKind::BitCast,
                        operand: Box::new(value.value),
                        reason: ConversionReason::Explicit,
                        semantics: ConversionSema::Exact,
                    },
                ))
            }
            ExprKind::StatementExpression(body) => {
                if !self.in_function {
                    return Err(ResolveError::Internal(
                        "statement expression outside a function",
                    ));
                }
                let (leading, labels, result) = statement_expression_parts(body);
                let (statements, value) = self.compound(|lower| {
                    let mut statements = lower.statements(leading, lower.return_type)?;
                    if let Some(labels) = &labels {
                        statements.extend(
                            lower.statements(std::slice::from_ref(labels), lower.return_type)?,
                        );
                    }
                    let value = match result {
                        Some(result) => lower.expr(result)?,
                        None => {
                            let c = lower.types.ctypes.qual(CTypeKind::Void);
                            lower.operand(e, c, ValueKind::Void)
                        }
                    };
                    Ok((statements, value))
                })?;
                Ok(Operand {
                    c: value.c,
                    value: self.value(
                        e,
                        value.ty.clone(),
                        ValueKind::StatementExpression(Box::new(Evaluation {
                            statements,
                            value: value.value,
                        })),
                    ),
                })
            }
            ExprKind::ConvertVector { ty, value } => {
                let resolved = self.resolve_type_name(ty)?;
                let value = self.expr(value)?;
                let (element, lanes) = self.vector_parts(value.c)?;
                let (target, target_lanes) = self.vector_parts(resolved)?;
                if lanes != target_lanes {
                    return Err(ResolveError::Internal(
                        "convertvector operands differ in lane count",
                    ));
                }
                let from = self.types.ir_type(element);
                let to = self.types.ir_type(target);
                let converted = self.context.elementwise_conversion(
                    value.value,
                    from,
                    to,
                    lanes,
                    ConversionReason::Explicit,
                )?;
                Ok(Operand {
                    value: converted,
                    c: resolved,
                })
            }
            ExprKind::VaArg { list, ty } => {
                let list = self.place(list)?;
                if !self.types.is_va_list(&list.ty) {
                    return Err(ResolveError::Internal("va_arg of non-va_list"));
                }
                let resolved = self.resolve_type_name(ty)?;
                let ty = self.types.object_type(resolved, "va_arg of void")?;
                self.types.storage(ty.clone())?;
                Ok(self.operand(e, resolved, ValueKind::VaArg { list: list.place }))
            }
        }
    }
}

pub(super) fn statement_expression_parts(
    body: &[crate::ast::Stmt],
) -> (&[crate::ast::Stmt], Option<crate::ast::Stmt>, Option<&Expr>) {
    let last = body
        .iter()
        .rposition(|statement| !matches!(statement.value, StmtKind::Comment(_)));
    match last {
        Some(index) => match &body[index].value {
            StmtKind::Expr(result) => (&body[..index], None, Some(result)),
            StmtKind::Labeled { .. } => match labeled_result(&body[index]) {
                Some((labels, result)) => (&body[..index], Some(labels), Some(result)),
                None => (body, None, None),
            },
            _ => (body, None, None),
        },
        None => (body, None, None),
    }
}

fn labeled_result(statement: &crate::ast::Stmt) -> Option<(crate::ast::Stmt, &Expr)> {
    match &statement.value {
        StmtKind::Expr(result) => Some((statement.clone().with_value(StmtKind::Null), result)),
        StmtKind::Labeled { label, body } => {
            let (inner, result) = labeled_result(body)?;
            Some((
                statement.clone().with_value(StmtKind::Labeled {
                    label: label.clone(),
                    body: Box::new(inner),
                }),
                result,
            ))
        }
        _ => None,
    }
}

#[derive(Clone, Copy)]
pub(super) enum VaBuiltin {
    Start,
    End,
    Copy,
}

fn conversion_context(reason: ConversionReason) -> ConversionContext {
    match reason {
        ConversionReason::Assign => ConversionContext::Assign,
        ConversionReason::Arg | ConversionReason::Vararg => ConversionContext::Arg,
        ConversionReason::Return => ConversionContext::Return,
        ConversionReason::Explicit | ConversionReason::Promotion | ConversionReason::UsualArith => {
            ConversionContext::Cast
        }
    }
}

pub(super) fn specially_lowered(callee: &Expr, arguments: &[Expr]) -> bool {
    super::atomic::atomic_builtin(callee).is_some()
        || va_builtin(callee).is_some()
        || constant_p_operand(callee, arguments).is_some()
        || choose_expr_operands(callee, arguments).is_some()
        || (arguments.is_empty() && source_location_builtin(callee).is_some())
}

pub(super) fn predefined_function_name(name: &str) -> bool {
    matches!(name, "__func__" | "__FUNCTION__" | "__PRETTY_FUNCTION__")
}

pub(super) fn source_location_builtin(callee: &Expr) -> Option<SourceLocationBuiltin> {
    let ExprKind::Identifier(name) = &callee.value else {
        return None;
    };
    match name.as_str() {
        "__builtin_FILE" => Some(SourceLocationBuiltin::File),
        "__builtin_FILE_NAME" => Some(SourceLocationBuiltin::FileName),
        "__builtin_FUNCTION" => Some(SourceLocationBuiltin::Function),
        "__builtin_LINE" => Some(SourceLocationBuiltin::Line),
        "__builtin_COLUMN" => Some(SourceLocationBuiltin::Column),
        _ => None,
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(super) enum SourceLocationBuiltin {
    File,
    FileName,
    Function,
    Line,
    Column,
}

pub(super) fn choose_expr_operands<'e>(
    callee: &Expr,
    arguments: &'e [Expr],
) -> Option<(&'e Expr, &'e Expr, &'e Expr)> {
    match (&callee.value, arguments) {
        (ExprKind::Identifier(name), [condition, when_true, when_false])
            if name == "__builtin_choose_expr" =>
        {
            Some((condition, when_true, when_false))
        }
        _ => None,
    }
}

pub(super) fn constant_p_operand<'e>(callee: &Expr, arguments: &'e [Expr]) -> Option<&'e Expr> {
    match (&callee.value, arguments) {
        (ExprKind::Identifier(name), [operand]) if name == "__builtin_constant_p" => Some(operand),
        _ => None,
    }
}

pub(super) fn va_builtin(callee: &Expr) -> Option<VaBuiltin> {
    let ExprKind::Identifier(name) = &callee.value else {
        return None;
    };
    match name.as_str() {
        "__builtin_va_start" | "__builtin_c23_va_start" => Some(VaBuiltin::Start),
        "__builtin_va_end" => Some(VaBuiltin::End),
        "__builtin_va_copy" => Some(VaBuiltin::Copy),
        _ => None,
    }
}

impl Lowerer {
    fn va_list_place(&mut self, argument: &Expr) -> Result<Place, ResolveError> {
        let place = self.place(argument)?;
        if !self.types.is_va_list(&place.ty) {
            return Err(ResolveError::Internal("va builtin on non-va_list"));
        }
        Ok(place.place)
    }

    fn va_builtin(
        &mut self,
        e: &Expr,
        builtin: VaBuiltin,
        arguments: &[Expr],
    ) -> Result<Operand, ResolveError> {
        let kind = match (builtin, arguments) {
            (VaBuiltin::Start, [list] | [list, _]) => ValueKind::VaStart {
                list: self.va_list_place(list)?,
            },
            (VaBuiltin::End, [list]) => ValueKind::VaEnd {
                list: self.va_list_place(list)?,
            },
            (VaBuiltin::Copy, [destination, source]) => ValueKind::VaCopy {
                destination: self.va_list_place(destination)?,
                source: self.va_list_place(source)?,
            },
            _ => return Err(ResolveError::Internal("va builtin argument count")),
        };
        let c = self.types.ctypes.qual(CTypeKind::Void);
        Ok(self.operand(e, c, kind))
    }
}

fn temporary_rooted(place: &Place) -> bool {
    match &place.kind {
        PlaceKind::Temporary { .. } => true,
        PlaceKind::Field { base, .. }
        | PlaceKind::ComplexPart { base, .. }
        | PlaceKind::Lane { base, .. }
        | PlaceKind::Swizzle { base, .. } => temporary_rooted(base),
        PlaceKind::Binding(_)
        | PlaceKind::Deref(_)
        | PlaceKind::Index { .. }
        | PlaceKind::CompoundLiteral { .. } => false,
    }
}

pub(super) fn assignment_operator(op: AssignOp) -> Result<BinaryOp, ResolveError> {
    Ok(match op {
        AssignOp::AddAssign => BinaryOp::Add,
        AssignOp::SubAssign => BinaryOp::Sub,
        AssignOp::MulAssign => BinaryOp::Mul,
        AssignOp::DivAssign => BinaryOp::Div,
        AssignOp::RemAssign => BinaryOp::Rem,
        AssignOp::BitAndAssign => BinaryOp::BitAnd,
        AssignOp::BitOrAssign => BinaryOp::BitOr,
        AssignOp::BitXorAssign => BinaryOp::BitXor,
        AssignOp::ShiftLeftAssign => BinaryOp::ShiftLeft,
        AssignOp::ShiftRightAssign => BinaryOp::ShiftRight,
        AssignOp::Assign => return Err(ResolveError::Internal("non-compound assignment")),
    })
}

pub(super) fn swizzle_lanes(field: &str, lanes: u32) -> Option<Vec<Option<u32>>> {
    let half = lanes.next_power_of_two() / 2;
    let selected: Vec<u32> = match field {
        "lo" => (0..half).collect(),
        "hi" => (half..half * 2).collect(),
        "even" => (0..half).map(|lane| lane * 2).collect(),
        "odd" => (0..half).map(|lane| lane * 2 + 1).collect(),
        _ => match field.strip_prefix(['s', 'S']) {
            Some(digits) if !digits.is_empty() => digits
                .chars()
                .map(|digit| digit.to_digit(16))
                .collect::<Option<_>>()?,
            _ => {
                let named = |set: &str| {
                    field
                        .chars()
                        .map(|component| set.find(component).map(|lane| lane as u32))
                        .collect::<Option<Vec<u32>>>()
                };
                named("xyzw").or_else(|| named("rgba"))?
            }
        },
    };
    Some(
        selected
            .into_iter()
            .map(|lane| (lane < lanes).then_some(lane))
            .collect(),
    )
}

fn type_class(ctypes: &CTypes, q: QualType) -> Option<i32> {
    Some(match ctypes.canonical_kind(q) {
        CTypeKind::NullPtr => -1,
        CTypeKind::Void => 0,
        CTypeKind::Char
        | CTypeKind::SChar
        | CTypeKind::UChar
        | CTypeKind::Int { .. }
        | CTypeKind::Enum(_) => 1,
        CTypeKind::Bool => 4,
        CTypeKind::Pointer(..) => 5,
        CTypeKind::Float(_) | CTypeKind::Imaginary(_) => 8,
        CTypeKind::Complex(_) => 9,
        CTypeKind::Record { union: false, .. } => 12,
        CTypeKind::Record { union: true, .. } => 13,
        CTypeKind::BitInt { .. } => 18,
        CTypeKind::Vector { .. } => 19,
        _ => return None,
    })
}
