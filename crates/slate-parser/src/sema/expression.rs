use super::builtins::{ClangBuiltin, CustomBuiltin};
use super::ctype::convert::CastKind;
use super::ctype::{CTypeKind, CTypes, Extent, QualType};
use super::initializer::InitializerSource;
use super::numeric::{Context, ResolveError};
use super::operand::{Lvalue, Operand};
use super::typer::{Choice, Lanes, Slot, Step, StepKind};
use super::types::TypeResolver;
use crate::ast::{Expr, ExprKind, NodeId, Span, StmtKind};
use crate::const_expr::{AssignOp, BinaryOp, PostfixOp, UnaryOp};
use crate::diagnostics::Warning;
use crate::ir::*;
use std::collections::{HashMap, HashSet};

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
    pub switches: Vec<BindingId>,
    pub in_function: bool,
    pub in_naked_function: bool,
    pub files: crate::files::Files,
    pub return_type: Option<QualType>,
    pub ms_asm_return: Vec<BindingId>,
    pub floating_pragmas: super::pragmas::FloatingPragmas,
    pub compound_start: bool,
    pub reserved_extents: HashMap<NodeId, BindingId>,
    pub bare_weakrefs: HashSet<BindingId>,
}

impl Lowerer {
    fn call(
        &mut self,
        e: &Expr,
        callee: Callee,
        signature: QualType,
        arguments: &[Expr],
    ) -> Result<Operand, ResolveError> {
        let (returned, ty, lowered) = self.call_arguments(e, signature, arguments)?;
        self.lowered_call(e, callee, signature, (returned, ty, lowered))
    }

    pub(super) fn lowered_call(
        &mut self,
        e: &Expr,
        callee: Callee,
        signature: QualType,
        (returned, ty, lowered): (QualType, Type, Vec<Value>),
    ) -> Result<Operand, ResolveError> {
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

    pub(super) fn call_arguments(
        &mut self,
        e: &Expr,
        signature: QualType,
        arguments: &[Expr],
    ) -> Result<(QualType, Type, Vec<Value>), ResolveError> {
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
            let Some(&parameter) = params.get(index) else {
                lowered.push(self.converted(e, Slot::Argument(index), value)?.value);
                continue;
            };
            let to = self.types.ctypes.adjust_parameter(parameter);
            let reason = ConversionReason::Arg;
            let value = match self.types.transparent_arguments.get(&argument.id).copied() {
                Some((index, member)) => {
                    let member = self.convert_recorded(argument, value, member, reason)?;
                    self.value(
                        argument,
                        self.types.ir_type(to),
                        ValueKind::Aggregate {
                            members: vec![AggregateMember {
                                target: AggregateTarget::Field(index),
                                value: member.value,
                            }],
                            zero_fill: false,
                        },
                    )
                }
                None => self.convert_recorded(argument, value, to, reason)?.value,
            };
            lowered.push(value);
        }
        Ok((returned, ty, lowered))
    }

    pub(super) fn builtin_declaration(
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
            abi: Some(self.abi_signature(&ty, None)?),
            linkage: Linkage::External,
            symbol: SymbolAttributes::default(),
            semantics: FunctionSemantics {
                noreturn: builtin.noreturn(self.types.flavor()),
                memory: builtin.memory_effects(self.types.flavor()),
                ..FunctionSemantics::default()
            },
            body: None,
            fallthrough: None,
        };
        let function = Span::new(function, e.spelling, e.expansion).with_provenance(e.provenance);
        self.module
            .annotate(&function, builtin.declaration_metadata());
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
            Some(_) => Some(*self.types.derived_signatures.get(&e.id).ok_or(
                ResolveError::Internal("builtin signature not recorded by the checker"),
            )?),
            None => self.types.builtin_signature(builtin),
        };
        let Some(signature) = signature else {
            return Ok(None);
        };
        let mut lowered = None;
        if let Some(expansion) = super::builtins::expansion(builtin) {
            let parts = self.call_arguments(e, signature, arguments)?;
            match self.expand_builtin(e, expansion, signature, parts)? {
                Ok(value) => {
                    self.module
                        .annotate(&value.value.node, [("c_builtin".into(), name)]);
                    return Ok(Some(value));
                }
                Err(parts) => lowered = Some(parts),
            }
        }
        let id = match declaration {
            Some(id) => id,
            None => self.builtin_declaration(e, builtin, signature)?,
        };
        let value = match lowered {
            Some(parts) => self.lowered_call(e, Callee::Direct(id), signature, parts)?,
            None => self.call(e, Callee::Direct(id), signature, arguments)?,
        };
        Ok(Some(value))
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
                let operand = self.real_floating_operand(e, 0, operand)?;
                Ok(self.truth(
                    e,
                    ValueKind::FloatClass {
                        test,
                        operand: Box::new(operand.value),
                    },
                ))
            }
            CustomBuiltin::QuietCompare(op) => {
                let (left, right) = self.real_floating_pair(e, arguments)?;
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
                let (left, right) = self.real_floating_pair(e, arguments)?;
                let left = self.float_class_int(e, FloatClassTest::Nan, left.value)?;
                let right = self.float_class_int(e, FloatClassTest::Nan, right.value)?;
                Ok(self.either(e, left, right))
            }
            CustomBuiltin::LessGreater => {
                let (left, right) = self.real_floating_pair(e, arguments)?;
                let less = self.quiet_compare_int(e, CompareOp::Lt, &left, &right)?;
                let greater = self.quiet_compare_int(e, CompareOp::Gt, &left, &right)?;
                Ok(self.either(e, less, greater))
            }
            CustomBuiltin::InfSign => {
                let [operand] = arguments else {
                    return Err(ResolveError::Internal("float class builtin arity"));
                };
                let operand = self.real_floating_operand(e, 0, operand)?;
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
                let (real, imaginary) = self.real_floating_pair(e, arguments)?;
                let c = self.result(e)?;
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
                let value = self.real_floating_operand(e, 5, value)?;
                let c = self.types.ctypes.int();
                let int = self.types.ir_type(c);
                let selected = self.expr(zero)?;
                let selected = self.converted(e, Slot::Argument(4), selected)?;
                let mut selected = selected.value;
                for (index, test, arm) in [
                    (3, FloatClassTest::Subnormal, subnormal),
                    (2, FloatClassTest::Normal, normal),
                    (1, FloatClassTest::Infinite, infinite),
                    (0, FloatClassTest::Nan, nan),
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
                    let arm = self.converted(e, Slot::Argument(index), arm)?;
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
                let c = self.result(e)?;
                Ok(self.operand(e, c, ValueKind::AddressOf(place.place)))
            }
            CustomBuiltin::CountedByRef => {
                let [argument] = arguments else {
                    return Err(ResolveError::Internal("counted_by_ref builtin arity"));
                };
                let c = self.result(e)?;
                let Some(counted) = self.types.counted_by_ref(argument)? else {
                    return Ok(self.operand(e, c, ValueKind::Null));
                };
                let Projection::Place(counter) =
                    self.member(argument, counted.base, &counted.counter, counted.arrow)?
                else {
                    return Err(ResolveError::Internal("counted_by counter is not a place"));
                };
                Ok(self.operand(e, c, ValueKind::AddressOf(counter.place)))
            }
            CustomBuiltin::ClassifyType => {
                let [operand] = arguments else {
                    return Err(ResolveError::Internal("classify builtin arity"));
                };
                let (resolved, _) = self.operand_type(operand)?;
                let resolved = self.types.ctypes.lvalue_conversion(resolved);
                let class = type_class(&self.types.ctypes, resolved)
                    .ok_or(ResolveError::Unimplemented("classify builtin operand"))?;
                let c = self.result(e)?;
                let number = match u32::try_from(class) {
                    Ok(class) => Number::Integer(class.into()),
                    Err(_) => Number::SignedInteger(class.into()),
                };
                Ok(self.operand(e, c, ValueKind::Constant(number)))
            }
        }
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
        Ok(self.code_unit_global(e, c, units))
    }

    fn string_literal(
        &mut self,
        e: &Expr,
        literal: &crate::const_expr::StringLiteral,
    ) -> Result<Lvalue, ResolveError> {
        let mut units = literal.execution_units(self.context.target.wchar_width);
        units.push(0);
        let c = self.lvalue_result(e)?;
        Ok(self.code_unit_global(e, c, units))
    }

    fn code_unit_global(&mut self, e: &Expr, c: QualType, units: Vec<u32>) -> Lvalue {
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
        Lvalue {
            c,
            place: Place {
                ty,
                kind: PlaceKind::Binding(id),
                access: Access::default(),
            },
        }
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

    fn real_floating_operand(
        &mut self,
        e: &Expr,
        index: usize,
        argument: &Expr,
    ) -> Result<Operand, ResolveError> {
        let operand = self.expr(argument)?;
        self.converted(e, Slot::Argument(index), operand)
    }

    fn real_floating_pair(
        &mut self,
        e: &Expr,
        arguments: &[Expr],
    ) -> Result<(Operand, Operand), ResolveError> {
        let [left, right] = arguments else {
            return Err(ResolveError::Internal("float class builtin arity"));
        };
        let left = self.real_floating_operand(e, 0, left)?;
        let right = self.real_floating_operand(e, 1, right)?;
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
        self.apply_conversion(conversion.kind, value, to, reason)
    }

    pub(super) fn converted(
        &mut self,
        owner: &Expr,
        slot: Slot,
        value: Operand,
    ) -> Result<Operand, ResolveError> {
        let steps = self.operand_steps(owner, slot)?;
        self.apply_steps(value, &steps)
    }

    pub(super) fn converted_at<T>(
        &mut self,
        owner: &Span<T>,
        slot: Slot,
        value: Operand,
    ) -> Result<Operand, ResolveError> {
        let steps = self
            .types
            .operand_conversions
            .get(&(owner.id, slot))
            .cloned()
            .ok_or(ResolveError::Internal(
                "operand conversion not recorded by the checker",
            ))?;
        self.apply_steps(value, &steps)
    }

    fn operand_steps(&mut self, owner: &Expr, slot: Slot) -> Result<Vec<Step>, ResolveError> {
        let key = (owner.id, slot);
        if let Some(steps) = self.types.operand_conversions.get(&key) {
            if !steps
                .iter()
                .any(|step| self.types.ctypes.has_unbound_extent(step.to))
            {
                return Ok(steps.clone());
            }
            self.types.expression_types.remove(&owner.id);
        }
        self.types.typed(owner)?;
        self.types
            .operand_conversions
            .get(&key)
            .cloned()
            .ok_or(ResolveError::Internal(
                "operand conversion not recorded by the checker",
            ))
    }

    fn computation(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        if let Some(&c) = self.types.computation_types.get(&e.id)
            && !self.types.ctypes.has_unbound_extent(c)
        {
            return Ok(c);
        }
        self.types.expression_types.remove(&e.id);
        self.types.typed(e)?;
        self.types
            .computation_types
            .get(&e.id)
            .copied()
            .ok_or(ResolveError::Internal(
                "operation type not recorded by the checker",
            ))
    }

    fn apply_steps(&mut self, mut value: Operand, steps: &[Step]) -> Result<Operand, ResolveError> {
        for step in steps {
            value = match step.kind {
                StepKind::Arithmetic => {
                    self.types
                        .arithmetic_conversion(&self.context, value, step.to, step.reason)?
                }
                StepKind::Cast(kind) => self.apply_conversion(kind, value, step.to, step.reason)?,
            };
        }
        Ok(value)
    }

    pub(super) fn apply_conversion(
        &mut self,
        kind: CastKind,
        value: Operand,
        to: QualType,
        reason: ConversionReason,
    ) -> Result<Operand, ResolveError> {
        let c = self.types.ctypes.unqualified(to);
        Ok(Operand {
            value: self.emit_cast(kind, value, c, reason)?,
            c,
        })
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
            CastKind::EnumToInt(tail) => {
                let integer = Operand {
                    value: self.enum_integer(value),
                    c: to,
                };
                self.emit_cast(tail.kind(), integer, to, reason)
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
        let c = self
            .lvalue_result(e)
            .map_err(|error| error.at(e.expansion))?;
        Ok(Lvalue { c, ..place })
    }

    fn lower_place(&mut self, e: &Expr) -> Result<Lvalue, ResolveError> {
        match &e.value {
            ExprKind::Paren(inner) => self.place(inner),
            ExprKind::Generic { associations, .. } => {
                let selected = self.generic_selected(e, associations)?;
                self.place(selected)
            }
            ExprKind::Call { callee, arguments }
                if choose_expr_operands(callee, arguments).is_some() =>
            {
                let chosen = self.chosen_expr(e, arguments)?;
                self.place(chosen)
            }
            ExprKind::Identifier(name) if predefined_function_name(name) => {
                let text = self.types.predefined_name(name).to_owned();
                self.string_global(e, &text)
            }
            ExprKind::StringLiteral(literal) => self.string_literal(e, literal),
            ExprKind::Identifier(_) => {
                let id = self.reference(e)?;
                let c = self.lvalue_result(e)?;
                Ok(Lvalue {
                    c,
                    place: Place {
                        ty: self.types.ir_type(c),
                        kind: PlaceKind::Binding(id),
                        access: self.types.access_of(c),
                    },
                })
            }
            ExprKind::CompoundLiteral {
                ty: ty_name,
                initializer,
            } => {
                let extents = self.type_name_extents(ty_name)?;
                let resolved = self.resolve_type_name(ty_name)?;
                let access = self.types.access_of(resolved);
                let _declared = self.types.object_type(resolved, "void compound literal")?;
                let anchor = e.derive(());
                let value = self.initializer_value(
                    e.id,
                    resolved,
                    InitializerSource::CompoundLiteral(initializer),
                    &anchor,
                )?;
                let value = self.captured(e, extents, value);
                let object = self.fresh();
                let ty = value.ty.clone();
                let c = self.lvalue_result(e)?;
                self.types.entities.declare(object, c, false);
                let storage = if self.in_function {
                    StorageDuration::Automatic
                } else {
                    StorageDuration::Static
                };
                let alignas = ty_name.specifiers.attributes.iter().filter(|attribute| {
                    matches!(attribute.value, crate::ast::Attribute::AlignAs(_))
                });
                let requested = super::types::requested_alignment(&mut self.types, alignas)?;
                let alignment =
                    if requested.is_some() || self.types.ctypes.typedef_alignment(c).is_some() {
                        self.types
                            .object_alignment_override(&ty, Some(c), requested)?
                    } else {
                        None
                    };
                Ok(Lvalue {
                    c,
                    place: Place {
                        ty,
                        kind: PlaceKind::CompoundLiteral {
                            object,
                            storage,
                            alignment,
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
                let c = self.lvalue_result(e)?;
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
                return Ok(Projection::Place(self.lane(e, object, index)?));
            }
            Ok(object) => self.read(base, object)?,
            Err(_) => self.expr(base)?,
        };
        let index = self.expr(index)?;
        if self.types.ctypes.is_vector(object.c) {
            let c = self.result(e)?;
            return Ok(Projection::Value(self.operand(
                e,
                c,
                ValueKind::Lane {
                    vector: Box::new(object.value),
                    index: Box::new(index.value),
                },
            )));
        }
        let object = self.converted(e, Slot::Left, object)?;
        let index = self.converted(e, Slot::Right, index)?;
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
                        let c = self.result(e)?;
                        let mask = self.swizzle_lanes(e)?;
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
        let mask = self.swizzle_lanes(e)?;
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
                Ok(Projection::Place(self.lane(e, object, index)?))
            }
            Some(lanes) => {
                let c = self.lvalue_result(e)?;
                Ok(Projection::Place(Lvalue {
                    c,
                    place: Place {
                        ty: self.types.ir_type(c),
                        access: object.place.access,
                        kind: PlaceKind::Swizzle {
                            base: Box::new(object.place),
                            lanes,
                        },
                    },
                }))
            }
            None => {
                let value = self.read(base, object)?;
                let c = self.result(e)?;
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

    fn lane(&mut self, e: &Expr, object: Lvalue, index: Operand) -> Result<Lvalue, ResolveError> {
        let c = self.lvalue_result(e)?;
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

    fn generic_selected<'e>(
        &self,
        e: &Expr,
        associations: &'e [crate::ast::GenericAssociation],
    ) -> Result<&'e Expr, ResolveError> {
        let id = self.selected_operand(e)?;
        associations
            .iter()
            .map(|association| match association {
                crate::ast::GenericAssociation::Default(value)
                | crate::ast::GenericAssociation::Type { value, .. } => value,
            })
            .find(|value| value.id == id)
            .ok_or(ResolveError::Internal(
                "recorded generic association is missing",
            ))
    }

    fn chosen_expr<'e>(&self, e: &Expr, arguments: &'e [Expr]) -> Result<&'e Expr, ResolveError> {
        let id = self.selected_operand(e)?;
        arguments
            .iter()
            .find(|argument| argument.id == id)
            .ok_or(ResolveError::Internal(
                "recorded __builtin_choose_expr operand is missing",
            ))
    }

    fn swizzle_lanes(&self, e: &Expr) -> Result<Lanes, ResolveError> {
        match self.types.choices.get(&e.id) {
            Some(Choice::Lanes(lanes)) => Ok(lanes.clone()),
            _ => Err(ResolveError::Internal(
                "swizzle not recorded by the checker",
            )),
        }
    }

    fn selected_operand(&self, e: &Expr) -> Result<NodeId, ResolveError> {
        match self.types.choices.get(&e.id) {
            Some(Choice::Operand(id)) => Ok(*id),
            _ => Err(ResolveError::Internal(
                "selection not recorded by the checker",
            )),
        }
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
                if bits.is_none() {
                    return Ok(value);
                }
                let promoted = self.result(e)?;
                return self.types.arithmetic_conversion(
                    &self.context,
                    value,
                    promoted,
                    ConversionReason::Promotion,
                );
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
        let c = self.computation(e)?;
        let lp = self.types.ctypes.pointee(left.c);
        let rp = self.types.ctypes.pointee(right.c);
        match (op, lp, rp) {
            (BinaryOp::Sub, Some(element), Some(_)) => {
                let element = self.types.ir_type(element);
                self.types.require_pointer_element(&element)?;
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
                self.pointer_offset(e, c, left, right, element, op == BinaryOp::Sub)
            }
            (BinaryOp::Add, None, Some(element)) => {
                self.pointer_offset(e, c, right, left, element, false)
            }
            _ => {
                let (ty, kind) = self.context.emit_binary(op, left.value, right.value)?;
                Ok(Operand {
                    value: self.value(e, ty, kind),
                    c,
                })
            }
        }
    }

    fn pointer_offset(
        &mut self,
        e: &Expr,
        c: QualType,
        pointer: Operand,
        amount: Operand,
        element: QualType,
        subtract: bool,
    ) -> Result<Operand, ResolveError> {
        let element = self.types.ir_type(element);
        self.types.require_pointer_element(&element)?;
        if !matches!(amount.ty, Type::Numeric(NumericType::Integer { .. })) {
            return Err(ResolveError::Internal("noninteger pointer offset"));
        }
        Ok(self.operand(
            e,
            c,
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
        let old = self.converted(e, Slot::Left, old)?;
        let rhs = self.converted(e, Slot::Right, rhs)?;
        let computation = self.binary(e, op, old, rhs)?;
        let computation = self.converted(e, Slot::Result, computation)?;
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

    pub(super) fn captured(
        &mut self,
        e: &Expr,
        extents: Vec<(BindingId, Value)>,
        value: Value,
    ) -> Value {
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
        let Type::VariableArray { element, extent } = ty else {
            let size = self.types.storage(ty.clone())?.size_bytes;
            return Ok(self.operand(e, c, ValueKind::Constant(Number::Integer(size.into()))));
        };
        let VariableExtent::Captured(extent) = *extent else {
            return Err(ResolveError::Internal(
                "size of an unspecified variable length array",
            ));
        };
        let count = self.extent_length(e, extent);
        let element = self.runtime_size(e, element)?;
        let (ty, kind) = self
            .context
            .emit_binary(BinaryOp::Mul, count.value, element.value)?;
        Ok(Operand {
            value: self.value(e, ty, kind),
            c,
        })
    }

    fn after_operand(&mut self, e: &Expr, evaluated: Option<Value>, result: Operand) -> Operand {
        let Some(evaluated) = evaluated else {
            return result;
        };
        let ty = result.value.ty.clone();
        let value = self.value(
            e,
            ty,
            ValueKind::Sequence {
                left: Box::new(evaluated),
                right: Box::new(result.value),
            },
        );
        Operand { value, c: result.c }
    }

    fn extent_length(&mut self, e: &Expr, extent: BindingId) -> Operand {
        let c = self.types.ctypes.size_type(&self.context.target);
        let place = Place {
            ty: self.types.ir_type(c),
            kind: PlaceKind::Binding(extent),
            access: Access::default(),
        };
        self.operand(
            e,
            c,
            ValueKind::Read {
                place,
                ordering: None,
            },
        )
    }

    fn element_count(&mut self, e: &Expr, c: QualType) -> Result<Operand, ResolveError> {
        match self.types.count_extent(c)? {
            Extent::Fixed(count) => {
                let detail = self.types.ir_type(c).to_string();
                Ok(self.layout_constant(e, count, "count_of", detail))
            }
            Extent::Variable(Some(extent)) => Ok(self.extent_length(e, extent)),
            Extent::Variable(None) | Extent::Incomplete => Err(ResolveError::Internal(
                "_Countof of an unspecified variable length array",
            )),
        }
    }

    fn layout_constant(&mut self, e: &Expr, amount: u64, key: &str, detail: String) -> Operand {
        let c = self.types.ctypes.size_type(&self.context.target);
        let value = self.operand(e, c, ValueKind::Constant(Number::Integer(amount.into())));
        self.module.annotate(&value.node, [(key.into(), detail)]);
        value
    }

    pub fn expr(&mut self, e: &Expr) -> Result<Operand, ResolveError> {
        let lowered = self.lower_expr(e).map_err(|error| error.at(e.expansion))?;
        let c = self.result(e).map_err(|error| error.at(e.expansion))?;
        if self.types.typed(e)?.source_bits.is_some() {
            return self
                .types
                .arithmetic_conversion(&self.context, lowered, c, ConversionReason::Promotion)
                .map_err(|error| error.at(e.expansion));
        }
        Ok(Operand { c, ..lowered })
    }

    pub(super) fn result(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        let typed = self.types.typed(e)?;
        Ok(self.types.rvalue_type(typed))
    }

    fn lvalue_result(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        let typed = self.types.typed(e)?;
        if !typed.lvalue {
            return Err(ResolveError::Internal("place lowered for an rvalue"));
        }
        Ok(typed.c)
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
                let (_, number) = self.types.character_constant(lit)?;
                let c = self.result(e)?;
                Ok(self.operand(e, c, ValueKind::Constant(number)))
            }
            ExprKind::LabelAddress(label) => {
                let id = self
                    .names
                    .references
                    .iter()
                    .find(|r| r.id == label.id)
                    .map(|r| r.binding)
                    .ok_or(ResolveError::Internal("missing label address binding"))?;
                let c = self.result(e)?;
                Ok(self.operand(e, c, ValueKind::LabelAddress(id)))
            }
            ExprKind::NullPtrLiteral => {
                let c = self.result(e)?;
                Ok(self.operand(e, c, ValueKind::Null))
            }
            ExprKind::StringLiteral(literal) => {
                let lvalue = self.string_literal(e, literal)?;
                self.read(e, lvalue)
            }
            ExprKind::Cast { ty, value } => {
                let extents = self.type_name_extents(ty)?;
                let to = self.resolve_type_name(ty)?;
                let to = self.types.ctypes.unqualified(to);
                let is_void = self.types.ctypes.is_void(to);
                let value_expr = value;
                let value = self.expr(value)?;
                let cast = if let Some(Choice::Member(index)) = self.types.choices.get(&e.id) {
                    let index = *index;
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
                let c = self.result(e)?;
                Ok(self.operand(e, c, ValueKind::AddressOf(place.place)))
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
                    let value = self.converted(e, Slot::Operand, value)?;
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
                let c = self.result(e)?;
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
                match op {
                    UnaryOp::Plus => {
                        let value = self.converted(e, Slot::Operand, value)?;
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
                        Ok(value)
                    }
                    UnaryOp::Not => {
                        let value = self.enum_operand(value);
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
                        let value = self.converted(e, Slot::Operand, value)?;
                        let (ty, kind) = self.context.emit_unary_arith(*op, value.value)?;
                        Ok(Operand {
                            value: self.value(e, ty, kind),
                            c: value.c,
                        })
                    }
                    _ => Err(ResolveError::Internal("advanced unary operator")),
                }
            }
            ExprKind::Binary { op, left, right } => {
                let left = self.expr(left)?;
                let right = self.expr(right)?;
                if matches!(op, BinaryOp::And | BinaryOp::Or) {
                    let left = self.enum_operand(left);
                    let right = self.enum_operand(right);
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
                let compare = match op {
                    BinaryOp::Equal => Some(CompareOp::Eq),
                    BinaryOp::NotEqual => Some(CompareOp::Ne),
                    BinaryOp::Less => Some(CompareOp::Lt),
                    BinaryOp::LessEqual => Some(CompareOp::Le),
                    BinaryOp::Greater => Some(CompareOp::Gt),
                    BinaryOp::GreaterEqual => Some(CompareOp::Ge),
                    _ => None,
                }
                .filter(|_| self.pointee(&left.ty).is_ok() || self.pointee(&right.ty).is_ok());
                let left = self.converted(e, Slot::Left, left)?;
                let right = self.converted(e, Slot::Right, right)?;
                if let Some(op) = compare {
                    return Ok(self.truth(
                        e,
                        ValueKind::Compare {
                            op,
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
                } else {
                    left = self.converted(e, Slot::Then, left)?;
                    right = self.converted(e, Slot::Else, right)?;
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
            ExprKind::Generic { associations, .. } => {
                let selected = self.generic_selected(e, associations)?;
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
                let constant = super::types::is_folded(&self.expr(operand)?.value)
                    || self.types.builtin_constant_p(operand);
                let c = self.result(e)?;
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
                let chosen = self.chosen_expr(e, arguments)?;
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
                    .measured_storage(&ty, atomic, matches!(e.value, ExprKind::SizeOfType { .. }))
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
                    return Ok(self.after_operand(e, evaluated, size));
                }
                let layout = self
                    .types
                    .measured_storage(
                        &ty,
                        access.atomic,
                        matches!(e.value, ExprKind::SizeOfExpr(_)),
                    )
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
            ExprKind::CountOfType { ty } => {
                let extents = self.type_name_extents(ty)?;
                let resolved = self.resolve_type_name(ty)?;
                let count = self.element_count(e, resolved)?;
                if matches!(self.types.count_extent(resolved)?, Extent::Fixed(_)) {
                    return Ok(count);
                }
                Ok(self.with_extents(e, extents, count))
            }
            ExprKind::CountOfExpr(operand) => {
                let (c, evaluated) = self.unevaluated_type(operand)?;
                let count = self.element_count(e, c)?;
                Ok(self.after_operand(e, evaluated, count))
            }
            ExprKind::TypesCompatible { left_ty, right_ty } => {
                let (compatible, compared) = self.types.types_compatible(left_ty, right_ty)?;
                let c = self.result(e)?;
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
                let path = self.types.offsetof_member(ty.clone(), member)?;
                let offset =
                    self.layout_constant(e, path.offset, "offset_of", format!("{ty}.{member}"));
                path.runtime_indices
                    .into_iter()
                    .try_fold(offset, |offset, (index, size)| {
                        let index = self.expr(index)?;
                        let index = self.types.arithmetic_conversion(
                            &self.context,
                            index,
                            offset.c,
                            ConversionReason::Explicit,
                        )?;
                        let size = self.operand(
                            e,
                            offset.c,
                            ValueKind::Constant(Number::Integer(size.into())),
                        );
                        let (ty, kind) =
                            self.context
                                .emit_binary(BinaryOp::Mul, index.value, size.value)?;
                        let scaled = self.value(e, ty, kind);
                        let (ty, kind) =
                            self.context
                                .emit_binary(BinaryOp::Add, offset.value, scaled)?;
                        Ok(Operand {
                            value: self.value(e, ty, kind),
                            c: offset.c,
                        })
                    })
            }
            ExprKind::BitCast { ty, value } => {
                let resolved = self.resolve_type_name(ty)?;
                let ty = self
                    .types
                    .layout(resolved)
                    .ok_or(ResolveError::Internal("bit cast to void"))?;
                if matches!(ty, Type::VariableArray { .. }) {
                    return Err(ResolveError::Internal(
                        "bit cast to a variable length array",
                    ));
                }
                let value = self.expr(value)?;
                if self.types.storage(ty.clone())?.size_bytes
                    != self.types.storage(value.ty.clone())?.size_bytes
                {
                    return Err(ResolveError::Internal(
                        "bit cast between types of different sizes",
                    ));
                }
                let cast = self.operand(
                    e,
                    resolved,
                    ValueKind::Convert {
                        kind: ConversionKind::BitCast,
                        operand: Box::new(value.value),
                        reason: ConversionReason::Explicit,
                        semantics: ConversionSema::Exact,
                    },
                );
                if matches!(ty, Type::Array { .. }) {
                    let object = self.materialize(cast);
                    return self.read(e, object);
                }
                Ok(cast)
            }
            ExprKind::StatementExpression(body) => {
                if !self.in_function {
                    return Err(ResolveError::Internal(
                        "statement expression outside a function",
                    ));
                }
                let (leading, labels, result, trailing) = statement_expression_parts(body);
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
                    statements.extend(lower.statements(trailing, lower.return_type)?);
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
) -> (
    &[crate::ast::Stmt],
    Option<crate::ast::Stmt>,
    Option<&Expr>,
    &[crate::ast::Stmt],
) {
    let last = body.iter().rposition(|statement| {
        !matches!(statement.value, StmtKind::Comment(_) | StmtKind::Pragma(_))
    });
    let Some(index) = last else {
        return (body, None, None, &[]);
    };
    let trailing = &body[index + 1..];
    match &body[index].value {
        StmtKind::Expr(result) => (&body[..index], None, Some(result), trailing),
        StmtKind::Labeled { .. } => match labeled_result(&body[index]) {
            Some((labels, result)) => (&body[..index], Some(labels), Some(result), trailing),
            None => (body, None, None, &[]),
        },
        _ => (body, None, None, &[]),
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
