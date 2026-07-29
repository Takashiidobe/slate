use crate::fixups::trace::Pass;
use crate::function_identity::Known;

use super::{
    ByteSource, CallRecord, CallTarget, ExprRecipe, ExprRule, ExprSite, NulPosition, Predicate,
    QueryContext, Rejection, RejectionReason, ReplaceExpr, RuleCase, RuleIdentity, RuleResult,
    StableExpr,
};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) struct CallArg(usize);

type CallCaseFn = for<'case, 'snapshot> fn(
    &mut CallCaseContext<'case, 'snapshot>,
) -> Result<ExprRecipe<'snapshot>, Rejection>;

struct DeclarativeCallCase {
    name: String,
    apply: CallCaseFn,
}

pub(in crate::fixups) struct CallRule {
    identity: RuleIdentity,
    target: CallTarget,
    arity: usize,
    cases: Vec<DeclarativeCallCase>,
    replace_trivial_unsafe: bool,
}

impl CallRule {
    pub(in crate::fixups) fn generated(
        pass: Pass,
        rule: impl Into<String>,
        symbol: impl Into<String>,
        arity: usize,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            target: CallTarget::Generated(symbol.into()),
            arity,
            cases: Vec::new(),
            replace_trivial_unsafe: false,
        }
    }

    pub(in crate::fixups) fn known(
        pass: Pass,
        rule: impl Into<String>,
        target: Known,
        arity: usize,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            target: CallTarget::Known(target),
            arity,
            cases: Vec::new(),
            replace_trivial_unsafe: false,
        }
    }

    pub(in crate::fixups) fn replace_trivial_unsafe(mut self) -> Self {
        self.replace_trivial_unsafe = true;
        self
    }

    pub(in crate::fixups) fn case(mut self, name: impl Into<String>, apply: CallCaseFn) -> Self {
        self.cases.push(DeclarativeCallCase {
            name: name.into(),
            apply,
        });
        self
    }
}

pub(in crate::fixups) struct CallCaseContext<'case, 'snapshot> {
    query: &'case QueryContext<'snapshot>,
    call: &'case CallRecord,
    evidence: Vec<super::Evidence>,
}

impl<'snapshot> CallCaseContext<'_, 'snapshot> {
    pub(in crate::fixups) fn args<const N: usize>(&self) -> [CallArg; N] {
        assert_eq!(N, self.call.args.len());
        std::array::from_fn(CallArg)
    }

    pub(in crate::fixups) fn byte_source(
        &mut self,
        arg: CallArg,
    ) -> Result<ByteSource<'snapshot>, Rejection> {
        let site = self.arg(arg).clone();
        self.prove(self.query.byte_source(&site))
    }

    pub(in crate::fixups) fn u8_eq(&mut self, arg: CallArg, expected: u8) -> Result<(), Rejection> {
        let site = self.arg(arg).clone();
        let proof = self.query.const_u8(&site);
        let actual = self.prove(proof)?;
        if actual == expected {
            return Ok(());
        }
        Err(self.reject(
            Predicate::ConstantU8,
            Some(site),
            RejectionReason::Contradicted,
            Vec::new(),
        ))
    }

    pub(in crate::fixups) fn pure(&mut self, arg: CallArg) -> Result<StableExpr, Rejection> {
        let site = self.arg(arg).clone();
        self.prove(self.query.pure(&site))
    }

    pub(in crate::fixups) fn never_returning_extern(&mut self) -> Result<(), Rejection> {
        self.prove(self.query.never_returning_extern(self.call))
            .map(drop)
    }

    pub(in crate::fixups) fn full_byte_view(
        &mut self,
        source: &ByteSource<'snapshot>,
        count: CallArg,
    ) -> Result<(), Rejection> {
        let site = self.arg(count).clone();
        self.prove(self.query.full_byte_view(source, &site))
            .map(drop)
    }

    pub(in crate::fixups) fn first_nul(
        &mut self,
        source: &ByteSource<'snapshot>,
    ) -> Result<NulPosition, Rejection> {
        self.prove(self.query.first_nul(source))
    }

    pub(in crate::fixups) fn prefix_contains(
        &mut self,
        count: CallArg,
        nul: NulPosition,
    ) -> Result<(), Rejection> {
        let site = self.arg(count).clone();
        self.prove(self.query.prefix_contains(&site, nul))
    }

    fn arg(&self, arg: CallArg) -> &ExprSite {
        &self.call.args[arg.0]
    }

    fn prove<T>(&mut self, result: super::QueryResult<T>) -> Result<T, Rejection> {
        match result {
            Ok(proof) => {
                self.evidence.extend(proof.evidence);
                Ok(proof.value)
            }
            Err(mut rejection) => {
                let mut evidence = self.evidence.clone();
                evidence.append(&mut rejection.evidence);
                rejection.evidence = evidence;
                Err(rejection)
            }
        }
    }

    fn reject(
        &self,
        predicate: Predicate,
        site: Option<ExprSite>,
        reason: RejectionReason,
        mut evidence: Vec<super::Evidence>,
    ) -> Rejection {
        let mut accumulated = self.evidence.clone();
        accumulated.append(&mut evidence);
        Rejection::new(predicate, site, reason, accumulated)
    }
}

impl ExprRule for CallRule {
    type Candidate = CallRecord;

    fn identity(&self) -> RuleIdentity {
        self.identity.clone()
    }

    fn candidates(&self, query: &QueryContext<'_>) -> Vec<Self::Candidate> {
        query.calls(&self.target, self.arity).to_vec()
    }

    fn target(&self, candidate: &Self::Candidate) -> ExprSite {
        if self.replace_trivial_unsafe {
            candidate
                .trivial_unsafe_site
                .clone()
                .unwrap_or_else(|| candidate.site.clone())
        } else {
            candidate.site.clone()
        }
    }

    fn cases(&self, query: &QueryContext<'_>, candidate: &Self::Candidate) -> Vec<RuleCase> {
        self.cases
            .iter()
            .map(|case| {
                let mut context = CallCaseContext {
                    query,
                    call: candidate,
                    evidence: candidate.evidence.clone(),
                };
                let target = self.target(candidate);
                let result = (case.apply)(&mut context).and_then(|recipe| {
                    recipe.lower(query, &candidate.site).map(|replacement| {
                        ReplaceExpr::new(target, replacement).with_evidence(context.evidence)
                    })
                });
                RuleCase::new(case.name.clone(), RuleResult::from(result))
            })
            .collect()
    }
}
