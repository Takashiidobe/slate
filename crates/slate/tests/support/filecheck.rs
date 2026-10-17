use std::path::{Path, PathBuf};
use std::process::Command;

const BLOCK_BEGIN: &str = "// SLATE-FILECHECK-BEGIN";

fn prefixes_for_target(triple: &str) -> Vec<String> {
    vec!["CHECK".to_string(), triple.to_ascii_uppercase()]
}

fn block_targets(fixture: &str) -> Option<Vec<&str>> {
    fixture.lines().find_map(|line| {
        line.trim_end()
            .strip_prefix(BLOCK_BEGIN)
            .map(|targets| targets.split_whitespace().collect())
    })
}

pub fn check_generated_rust(
    fixture: &str,
    generated: &str,
    target: &str,
    work_dir: &Path,
) -> Result<(), String> {
    let regenerate = format!("crates/slate/tools/update_filecheck.py --target {target}");
    let Some(targets) = block_targets(fixture) else {
        return Err(format!(
            "FileCheck failed: fixture has no SLATE-FILECHECK block; generate one with {regenerate}"
        ));
    };
    if targets.is_empty() {
        return Err(format!(
            "FileCheck failed: the SLATE-FILECHECK block lists no targets; regenerate it with {regenerate}"
        ));
    }
    if !targets.contains(&target) {
        return Ok(());
    }
    let prefixes = prefixes_for_target(target);
    let groups = check_groups(fixture, &prefixes)?;
    if groups.is_empty() {
        return Err(format!(
            "FileCheck failed: the SLATE-FILECHECK block has no checks for {target}"
        ));
    }
    if let Some((number, line)) = generated
        .lines()
        .enumerate()
        .find(|(_, line)| directive(line, &prefixes).is_some())
    {
        return Err(format!(
            "FileCheck failed: generated Rust line {} carries a check directive: {line}",
            number + 1
        ));
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

fn check_groups(fixture: &str, prefixes: &[String]) -> Result<Vec<Vec<String>>, String> {
    let mut groups: Vec<Vec<String>> = Vec::new();
    let mut global = Vec::new();
    let mut current: Option<Vec<String>> = None;
    for (line_number, line) in fixture.lines().enumerate() {
        let Some((directive, pattern)) = directive(line, prefixes) else {
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
            let is_close = check == "// CHECK: {{^}}}";
            group.push(check);
            if is_close {
                groups.push(current.take().unwrap());
            }
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
        } else if check.starts_with("// CHECK-NOT:")
            && let Some(group) = global_groups.last_mut()
        {
            group.push(check);
        } else {
            global_groups.push(vec![check]);
        }
    }
    global_groups.extend(groups);
    global_groups
        .into_iter()
        .try_fold(Vec::new(), |mut out, group| {
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
            "a label block must end with `// CHECK: {{{{^}}}}}}`, got `{close}`"
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

fn directive<'a>(line: &'a str, prefixes: &[String]) -> Option<(&'a str, &'a str)> {
    let line = line.trim_start().strip_prefix("//")?.trim_start();
    let (name, pattern) = line.split_once(':')?;
    prefixes.iter().find_map(|prefix| {
        let suffix = name.strip_prefix(prefix.as_str())?;
        normalize_directive(suffix).map(|_| (suffix, pattern.trim_start()))
    })
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
    std::env::var_os("SLATE_FILECHECK").map_or_else(|| PathBuf::from("FileCheck"), PathBuf::from)
}
