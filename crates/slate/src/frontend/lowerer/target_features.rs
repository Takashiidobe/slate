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

const AARCH64_FEATURES: &[(&str, &[&str])] = &[
    ("aes", &["aes"]),
    ("bf16", &["bf16"]),
    ("bti", &["bti"]),
    ("complxnum", &["fcma"]),
    ("crc", &["crc"]),
    ("cssc", &["cssc"]),
    ("dotprod", &["dotprod"]),
    ("f32mm", &["f32mm"]),
    ("f64mm", &["f64mm"]),
    ("faminmax", &["faminmax"]),
    ("fp16", &["fp16"]),
    ("fp16fml", &["fhm"]),
    ("fp8", &["fp8"]),
    ("fp8dot2", &["fp8dot2"]),
    ("fp8dot4", &["fp8dot4"]),
    ("fp8fma", &["fp8fma"]),
    ("fptoint", &["frintts"]),
    ("fullfp16", &["fp16"]),
    ("i8mm", &["i8mm"]),
    ("jsconv", &["jsconv"]),
    ("lse", &["lse"]),
    ("lut", &["lut"]),
    ("mops", &["mops"]),
    ("neon", &["neon"]),
    ("rand", &["rand"]),
    ("rcpc", &["rcpc"]),
    ("rdm", &["rdm"]),
    ("rdma", &["rdm"]),
    ("sha2", &["sha2"]),
    ("sha3", &["sha3"]),
    ("simd", &["neon"]),
    ("sm4", &["sm4"]),
    ("sme", &["sme"]),
    ("sme2", &["sme2"]),
    ("sve", &["sve"]),
    ("sve2", &["sve2"]),
    ("sve2-aes", &["sve2-aes"]),
    ("sve2-bitperm", &["sve2-bitperm"]),
    ("sve2-sha3", &["sve2-sha3"]),
    ("sve2-sm4", &["sve2-sm4"]),
    ("sve2p1", &["sve2p1"]),
    ("v8.1a", &["rdm"]),
    ("v8.3a", &["fcma", "jsconv"]),
    ("v8.5a", &["frintts"]),
];

pub(super) const AARCH64_UNSTABLE_FEATURES: &[&str] = &[
    "cssc", "faminmax", "fp8", "fp8dot2", "fp8dot4", "fp8fma", "lut", "mops", "sme", "sme2",
    "sve2p1",
];

fn rustc_x86_feature(clang: &str) -> Option<&'static str> {
    X86_FEATURES
        .iter()
        .find_map(|(name, rustc)| (*name == clang).then_some(*rustc))
}

pub(super) fn unit_target_features(target: &TargetInfo) -> Vec<&'static str> {
    let spellings: Vec<&'static str> = match target.isa {
        slate_parser::target::isa::TargetIsa::X86(isa) => isa
            .enabled_beyond(slate_parser::target::x86_isa::X86Isa::baseline(
                target.family,
            ))
            .collect(),
        slate_parser::target::isa::TargetIsa::AArch64(isa) => {
            isa.enabled_beyond(slate_parser::target::aarch64_isa::AArch64Isa::baseline())
        }
        _ => return Vec::new(),
    };
    let mut features = Vec::new();
    for rustc in spellings
        .into_iter()
        .filter_map(|spelling| rustc_features(target.family, spelling))
        .flatten()
    {
        if !features.contains(rustc) {
            features.push(*rustc);
        }
    }
    features
}

fn rustc_features(family: TargetFamily, name: &str) -> Option<&'static [&'static str]> {
    match family {
        TargetFamily::AArch64 => rustc_aarch64_feature(name.strip_prefix('+').unwrap_or(name)),
        TargetFamily::Arm32 => (name == "neon").then_some(&["neon"]),
        family if family.is_x86() => X86_FEATURES
            .iter()
            .find(|(clang, _)| *clang == name)
            .map(|(_, rustc)| std::slice::from_ref(rustc)),
        _ => None,
    }
}

fn rustc_aarch64_feature(clang: &str) -> Option<&'static [&'static str]> {
    AARCH64_FEATURES
        .iter()
        .find_map(|(name, rustc)| (*name == clang).then_some(*rustc))
}

impl Tables<'_> {
    fn is_x86(&self) -> bool {
        self.target.family.is_x86()
    }

    pub(super) fn target_feature_attr(&self, function: &ir::Function) -> Result<Option<Attr>> {
        let enabled = self.enabled_target_features(function)?;
        Ok((!enabled.is_empty()).then(|| Attr::TargetFeature(enabled.join(","))))
    }

    pub(super) fn vector_abi_feature(
        &self,
        function: &ir::Function,
    ) -> Result<Option<&'static str>> {
        if !self.function_has_vector(function) {
            return Ok(None);
        }
        let barrier = |detail: String| -> Failure {
            Construct::Function {
                name: function.name.clone(),
                detail,
            }
            .into()
        };
        let Some(signature) = &function.abi else {
            return Err(barrier("vector C ABI".into()));
        };
        if function.symbol.weak
            || !matches!(self.target.family, TargetFamily::AArch64) && !self.is_x86()
        {
            return Err(barrier(format!("vector C ABI {signature}")));
        }
        let passes = fixed_parameters(&function.parameters)
            .iter()
            .map(|parameter| &parameter.ty)
            .zip(&signature.arguments)
            .chain(function.return_type.iter().zip([&signature.result]));
        let mut widest = 0;
        for (ty, pass) in passes {
            if !matches!(self.resolve_type(ty), ir::Type::Vector { .. }) {
                continue;
            }
            if *pass != ir::AbiPass::Direct {
                return Err(barrier(format!("vector C ABI {signature}")));
            }
            let (size, _) = self.storage_of(ty).ok_or_else(|| unsupported_type(ty))?;
            widest = widest.max(size);
        }
        Ok(match widest {
            32 if self.is_x86() => Some("avx"),
            64 if self.is_x86() => Some("avx512f"),
            _ => None,
        })
    }

    pub(super) fn reject_sme_state(&self, function: &ir::Function) -> Result<()> {
        let Some(sme) = function.semantics.sme.first() else {
            return Ok(());
        };
        let states = if sme.states.is_empty() {
            String::new()
        } else {
            format!("({})", sme.states.join(","))
        };
        Err(Construct::Function {
            name: function.name.clone(),
            detail: format!("SME function state `{:?}{states}`", sme.kind),
        }
        .into())
    }

    pub(super) fn enabled_target_features(
        &self,
        function: &ir::Function,
    ) -> Result<Vec<&'static str>> {
        let unsupported = |detail: String| -> Failure {
            Construct::Function {
                name: function.name.clone(),
                detail,
            }
            .into()
        };
        self.reject_sme_state(function)?;
        let mut enabled = self.unit_features.clone();
        for feature in &function.semantics.target {
            match feature {
                ir::TargetFeature::Enable(name) if name == "default" => {}
                ir::TargetFeature::Enable(name) => {
                    let rustc = rustc_features(self.target.family, name)
                        .ok_or_else(|| unsupported(format!("target feature `{name}`")))?;
                    for feature in rustc {
                        if !enabled.contains(feature) {
                            enabled.push(feature);
                        }
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
        Ok(enabled)
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
