use super::*;
use slate_parser::target_info::TargetFamily;

const X86_FEATURES: &[(&str, &str)] = &[
    ("adx", "adx"),
    ("aes", "aes"),
    ("avx", "avx"),
    ("avx2", "avx2"),
    ("avx512bf16", "avx512bf16"),
    ("avx512bitalg", "avx512bitalg"),
    ("avx512bw", "avx512bw"),
    ("avx512cd", "avx512cd"),
    ("avx512dq", "avx512dq"),
    ("avx512f", "avx512f"),
    ("avx512ifma", "avx512ifma"),
    ("avx512vbmi", "avx512vbmi"),
    ("avx512vbmi2", "avx512vbmi2"),
    ("avx512vl", "avx512vl"),
    ("avx512vnni", "avx512vnni"),
    ("avx512vp2intersect", "avx512vp2intersect"),
    ("avx512vpopcntdq", "avx512vpopcntdq"),
    ("bmi", "bmi1"),
    ("bmi2", "bmi2"),
    ("cx16", "cmpxchg16b"),
    ("f16c", "f16c"),
    ("fma", "fma"),
    ("fxsr", "fxsr"),
    ("gfni", "gfni"),
    ("lzcnt", "lzcnt"),
    ("movbe", "movbe"),
    ("pclmul", "pclmulqdq"),
    ("popcnt", "popcnt"),
    ("rdrnd", "rdrand"),
    ("rdseed", "rdseed"),
    ("rtm", "rtm"),
    ("sha", "sha"),
    ("sse", "sse"),
    ("sse2", "sse2"),
    ("sse3", "sse3"),
    ("sse4.1", "sse4.1"),
    ("sse4.2", "sse4.2"),
    ("sse4a", "sse4a"),
    ("ssse3", "ssse3"),
    ("tbm", "tbm"),
    ("vaes", "vaes"),
    ("vpclmulqdq", "vpclmulqdq"),
    ("xsave", "xsave"),
    ("xsavec", "xsavec"),
    ("xsaveopt", "xsaveopt"),
    ("xsaves", "xsaves"),
];

fn rustc_x86_feature(clang: &str) -> Option<&'static str> {
    X86_FEATURES
        .iter()
        .find_map(|(name, rustc)| (*name == clang).then_some(*rustc))
}

impl Tables<'_> {
    fn is_x86(&self) -> bool {
        matches!(self.target.family, TargetFamily::X86 | TargetFamily::X86_64)
    }

    pub(super) fn target_feature_attr(&self, function: &ir::Function) -> Result<Option<Attr>> {
        let unsupported = |detail: String| -> Failure {
            Construct::Function {
                name: function.name.clone(),
                detail,
            }
            .into()
        };
        let mut enabled = Vec::new();
        for feature in &function.semantics.target {
            match feature {
                ir::TargetFeature::Enable(name) if name == "default" => {}
                ir::TargetFeature::Enable(name) => {
                    let rustc = self
                        .is_x86()
                        .then(|| rustc_x86_feature(name))
                        .flatten()
                        .ok_or_else(|| unsupported(format!("target feature `{name}`")))?;
                    if !enabled.contains(&rustc) {
                        enabled.push(rustc);
                    }
                }
                ir::TargetFeature::Arch(cpu) => {
                    return Err(unsupported(format!("target arch `{cpu}`")));
                }
                ir::TargetFeature::Disable(_)
                | ir::TargetFeature::Tune(_)
                | ir::TargetFeature::BranchProtection(_) => {}
            }
        }
        Ok((!enabled.is_empty()).then(|| Attr::TargetFeature(enabled.join(","))))
    }

    fn string_argument(&self, value: &ir::Value) -> Option<&str> {
        match &value.node.value {
            ValueKind::Convert {
                kind: ir::ConversionKind::PointerCast,
                operand,
                ..
            } => self.string_argument(operand),
            ValueKind::ArrayDecay {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ..
            } => std::str::from_utf8(self.strings.get(id)?)
                .ok()
                .map(|text| text.trim_end_matches('\0')),
            _ => None,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_cpu_builtin(
        &mut self,
        value: &ir::Value,
        builtin: BindingId,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        if !self.tables.is_x86() {
            return Err(unsupported_value(value));
        }
        match (self.tables.builtin_name(builtin), arguments) {
            (Some("__builtin_cpu_init"), []) => Ok(Expr::Block(Box::default())),
            (Some("__builtin_cpu_supports"), [feature]) => {
                let rustc = self
                    .tables
                    .string_argument(feature)
                    .and_then(rustc_x86_feature)
                    .ok_or_else(|| unsupported_value(value))?;
                Ok(Expr::Macro {
                    name: "std::arch::is_x86_feature_detected".into(),
                    args: vec![Expr::Str(rustc.into())],
                })
            }
            _ => Err(unsupported_value(value)),
        }
    }
}
