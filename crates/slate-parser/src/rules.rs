type RuleCheck<'a, T> = dyn Fn(&T) -> Result<(), RuleError> + 'a;

pub struct Rule<'a, T> {
    check: Box<RuleCheck<'a, T>>,
}

impl<'a, T> Rule<'a, T> {
    pub fn named(name: impl Into<String>, check: impl Fn(&T) -> bool + 'a) -> Self {
        let name = name.into();
        Self::validate(name.clone(), move |value| {
            if check(value) {
                Ok(())
            } else {
                Err(name.clone())
            }
        })
    }

    pub fn validate(
        name: impl Into<String>,
        check: impl Fn(&T) -> Result<(), String> + 'a,
    ) -> Self {
        let name = name.into();
        Self::custom(move |value| {
            check(value).map_err(|message| RuleError::Failed {
                rule: name.clone(),
                message,
            })
        })
    }

    pub fn custom(check: impl Fn(&T) -> Result<(), RuleError> + 'a) -> Self {
        Self {
            check: Box::new(check),
        }
    }

    pub fn check(&self, value: &T) -> Result<(), RuleError> {
        (self.check)(value)
    }
}

pub struct Rules;

impl Rules {
    pub fn pipeline<'a, T: 'a>(rules: impl IntoIterator<Item = Rule<'a, T>>) -> Rule<'a, T> {
        Self::all(rules)
    }

    pub fn all<'a, T: 'a>(rules: impl IntoIterator<Item = Rule<'a, T>>) -> Rule<'a, T> {
        let rules = rules.into_iter().collect::<Vec<_>>();
        Rule::custom(move |value| {
            let errors = rules
                .iter()
                .filter_map(|rule| rule.check(value).err())
                .collect::<Vec<_>>();
            if errors.is_empty() {
                Ok(())
            } else {
                Err(RuleError::All(RuleErrors(errors)))
            }
        })
    }

    pub fn any<'a, T: 'a>(rules: impl IntoIterator<Item = Rule<'a, T>>) -> Rule<'a, T> {
        let rules = rules.into_iter().collect::<Vec<_>>();
        Rule::custom(move |value| {
            let mut errors = Vec::new();
            for rule in &rules {
                match rule.check(value) {
                    Ok(()) => return Ok(()),
                    Err(error) => errors.push(error),
                }
            }
            Err(RuleError::Any(RuleErrors(errors)))
        })
    }

    pub fn when<'a, T: 'a>(condition: impl Fn(&T) -> bool + 'a, rule: Rule<'a, T>) -> Rule<'a, T> {
        Rule::custom(move |value| {
            if condition(value) {
                rule.check(value)
            } else {
                Ok(())
            }
        })
    }

    pub fn branch<'a, T: 'a, K: Eq + 'a>(
        select: impl Fn(&T) -> K + 'a,
        branches: impl IntoIterator<Item = (K, Rule<'a, T>)>,
    ) -> Rule<'a, T> {
        let branches = branches.into_iter().collect::<Vec<_>>();
        Rule::custom(move |value| {
            let key = select(value);
            branches
                .iter()
                .find(|(branch, _)| *branch == key)
                .map_or_else(
                    || {
                        Err(RuleError::Failed {
                            rule: "rule branch".into(),
                            message: "no matching branch".into(),
                        })
                    },
                    |(_, rule)| rule.check(value),
                )
        })
    }
}

#[derive(Debug, thiserror::Error)]
pub enum RuleError {
    #[error("{rule}: {message}")]
    Failed { rule: String, message: String },
    #[error("all rules failed: {0}")]
    All(RuleErrors),
    #[error("none of the alternatives matched: {0}")]
    Any(RuleErrors),
}

impl miette::Diagnostic for RuleError {}

#[derive(Debug)]
pub struct RuleErrors(Vec<RuleError>);

impl std::fmt::Display for RuleErrors {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for (index, error) in self.0.iter().enumerate() {
            if index > 0 {
                formatter.write_str("; ")?;
            }
            write!(formatter, "{error}")?;
        }
        Ok(())
    }
}
