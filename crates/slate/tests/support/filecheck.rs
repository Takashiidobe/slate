use std::path::{Path, PathBuf};
use std::process::Command;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Profile {
    Lowering,
    Rewrites,
}

impl Profile {
    pub fn active() -> Self {
        match std::env::var("NEXTEST_PROFILE").as_deref() {
            Ok("lowering") => Self::Lowering,
            _ => Self::Rewrites,
        }
    }

    fn prefix(self) -> &'static str {
        match self {
            Self::Lowering => "LOWERING",
            Self::Rewrites => "REWRITES",
        }
    }
}

pub fn has_checks(fixture: &str, profile: Profile) -> bool {
    has_checks_with_prefixes(fixture, profile, &[])
}

pub fn has_checks_with_prefixes(fixture: &str, profile: Profile, prefixes: &[&str]) -> bool {
    fixture
        .lines()
        .any(|line| directive(line, profile, prefixes).is_some())
}

pub fn check_generated_rust(
    fixture: &str,
    generated: &str,
    profile: Profile,
    work_dir: &Path,
) -> Result<(), String> {
    check_generated_rust_with_prefixes(fixture, generated, profile, &[], work_dir)
}

pub fn check_generated_rust_with_prefixes(
    fixture: &str,
    generated: &str,
    profile: Profile,
    prefixes: &[&str],
    work_dir: &Path,
) -> Result<(), String> {
    if profile == Profile::Rewrites {
        return Ok(());
    }
    let groups = check_groups(fixture, profile, prefixes)?;
    if groups.is_empty() {
        return Ok(());
    }
    std::fs::create_dir_all(work_dir)
        .map_err(|error| format!("create {}: {error}", work_dir.display()))?;
    let input = work_dir.join("generated.rs");
    std::fs::write(&input, generated)
        .map_err(|error| format!("write {}: {error}", input.display()))?;
    for (index, group) in groups.iter().enumerate() {
        let spec = work_dir.join(format!("checks-{index}.txt"));
        std::fs::write(&spec, group.join("\n") + "\n")
            .map_err(|error| format!("write {}: {error}", spec.display()))?;
        run_filecheck(&spec, &input)?;
    }
    Ok(())
}

fn check_groups(
    fixture: &str,
    profile: Profile,
    prefixes: &[&str],
) -> Result<Vec<Vec<String>>, String> {
    let mut groups: Vec<Vec<String>> = Vec::new();
    let mut global = Vec::new();
    let mut current: Option<Vec<String>> = None;
    for (line_number, line) in fixture.lines().enumerate() {
        let Some((directive, pattern)) = directive(line, profile, prefixes) else {
            continue;
        };
        let normalized = normalize_directive(directive).ok_or_else(|| {
            format!(
                "unsupported FileCheck directive `{directive}` at fixture line {}",
                line_number + 1
            )
        })?;
        let check = format!("// {normalized}: {pattern}");
        if normalized == "CHECK-LABEL" {
            if let Some(group) = current.replace(vec![check]) {
                groups.push(group);
            }
        } else if let Some(group) = &mut current {
            group.push(check);
        } else {
            global.push(check);
        }
    }
    if let Some(group) = current {
        groups.push(group);
    }
    let mut global_groups: Vec<Vec<String>> = Vec::new();
    for check in global {
        if check.starts_with("// CHECK-NEXT:")
            || check.starts_with("// CHECK-SAME:")
            || check.starts_with("// CHECK-EMPTY:")
        {
            let group = global_groups
                .last_mut()
                .ok_or_else(|| format!("`{check}` requires a preceding global check"))?;
            group.push(check);
        } else {
            global_groups.push(vec![check]);
        }
    }
    global_groups.extend(groups);
    groups = global_groups;
    groups.into_iter().try_fold(Vec::new(), |mut out, group| {
        if group
            .first()
            .is_some_and(|check| check.starts_with("// CHECK-LABEL:"))
        {
            out.extend(expand_label_group(&group)?);
        } else {
            out.push(group);
        }
        Ok(out)
    })
}

fn expand_label_group(group: &[String]) -> Result<Vec<Vec<String>>, String> {
    let [label, middle @ .., close] = group else {
        return Err("a label block requires assertions and a closing check".into());
    };
    if close != "// CHECK: {{^}}}" {
        return Err(format!(
            "a label block must end with `// <PROFILE>: {{{{^}}}}}}`, got `{close}`"
        ));
    }
    let boundary = "// CHECK-NOT: {{^}}}".to_string();
    let mut expanded = Vec::new();
    let mut ordered = Vec::new();
    for check in middle {
        if let Some(pattern) = check.strip_prefix("// CHECK-DAG: ") {
            expanded.push(vec![
                label.clone(),
                boundary.clone(),
                format!("// CHECK: {pattern}"),
                boundary.clone(),
                close.clone(),
            ]);
        } else if check.starts_with("// CHECK-NOT: ") {
            expanded.push(vec![label.clone(), check.clone(), close.clone()]);
        } else if check.starts_with("// CHECK:")
            || check.starts_with("// CHECK-NEXT:")
            || check.starts_with("// CHECK-SAME:")
            || check.starts_with("// CHECK-EMPTY:")
        {
            ordered.push(check.clone());
        } else {
            return Err(format!(
                "unsupported assertion in function label block: `{check}`"
            ));
        }
    }
    if !ordered.is_empty() {
        let mut group = vec![label.clone(), boundary.clone()];
        group.extend(ordered);
        group.push(boundary);
        group.push(close.clone());
        expanded.push(group);
    }
    Ok(expanded)
}

fn directive<'a>(line: &'a str, profile: Profile, prefixes: &[&str]) -> Option<(&'a str, &'a str)> {
    let line = line.trim_start().strip_prefix("//")?.trim_start();
    let (name, pattern) = line.split_once(':')?;
    for prefix in prefixes.iter().copied().chain(["COMMON", profile.prefix()]) {
        if name == prefix {
            return Some(("", pattern.trim_start()));
        }
        if let Some(suffix) = name.strip_prefix(prefix)
            && normalize_directive(suffix).is_some()
        {
            return Some((suffix, pattern.trim_start()));
        }
    }
    None
}

fn normalize_directive(directive: &str) -> Option<&'static str> {
    match directive {
        "" => Some("CHECK"),
        "-LABEL" => Some("CHECK-LABEL"),
        "-DAG" => Some("CHECK-DAG"),
        "-NOT" => Some("CHECK-NOT"),
        "-NEXT" => Some("CHECK-NEXT"),
        "-SAME" => Some("CHECK-SAME"),
        "-EMPTY" => Some("CHECK-EMPTY"),
        _ => None,
    }
}

fn run_filecheck(spec: &Path, input: &Path) -> Result<(), String> {
    let program = filecheck();
    let output = Command::new(&program)
        .arg(spec)
        .arg("--input-file")
        .arg(input)
        .arg("--dump-input=fail")
        .output()
        .map_err(|error| format!("spawn {}: {error}", program.display()))?;
    if output.status.success() {
        return Ok(());
    }
    Err(format!(
        "FileCheck failed for {} against {}:\n{}{}",
        spec.display(),
        input.display(),
        String::from_utf8_lossy(&output.stdout),
        String::from_utf8_lossy(&output.stderr)
    ))
}

fn filecheck() -> PathBuf {
    if let Some(path) = std::env::var_os("SLATE_FILECHECK") {
        return path.into();
    }
    let clang = std::env::var_os("SLATE_CLANG")
        .map(PathBuf::from)
        .or_else(|| {
            std::env::var_os("HOME")
                .map(PathBuf::from)
                .map(|home| home.join("llvm-project/build-cir/bin/clang"))
        });
    if let Some(clang) = clang {
        let sibling = clang.with_file_name("FileCheck");
        if sibling.is_file() {
            return sibling;
        }
    }
    PathBuf::from("FileCheck")
}
