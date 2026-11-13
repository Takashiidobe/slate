use anyhow::{Context, Result};
use rayon::prelude::*;
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::path::{Path, PathBuf};
use std::process::Command;

pub const HEADERS: &[&str] = &[
    "stdio.h",
    "stdint.h",
    "errno.h",
    "getopt.h",
    "limits.h",
    "math.h",
    "ctype.h",
    "fenv.h",
    "sys/cpuset.h",
    "stdlib.h",
    "string.h",
    "time.h",
    "wchar.h",
    "locale.h",
    "setjmp.h",
    "signal.h",
    "pthread.h",
    "sched.h",
    "semaphore.h",
    "unistd.h",
    "fcntl.h",
    "dirent.h",
    "poll.h",
    "termios.h",
    "dlfcn.h",
    "grp.h",
    "pwd.h",
    "regex.h",
    "glob.h",
    "fnmatch.h",
    "netdb.h",
    "ifaddrs.h",
    "net/if.h",
    "netinet/in.h",
    "netinet/tcp.h",
    "arpa/inet.h",
    "langinfo.h",
    "mqueue.h",
    "syslog.h",
    "utime.h",
    "ucontext.h",
    "spawn.h",
    "search.h",
    "sys/types.h",
    "sys/stat.h",
    "sys/time.h",
    "sys/times.h",
    "sys/select.h",
    "sys/socket.h",
    "sys/un.h",
    "sys/uio.h",
    "sys/wait.h",
    "sys/mman.h",
    "sys/resource.h",
    "sys/utsname.h",
    "sys/ioctl.h",
    "sys/statvfs.h",
    "sys/ipc.h",
    "sys/sem.h",
    "sys/shm.h",
    "sys/msg.h",
    "sys/epoll.h",
    "sys/event.h",
    "sys/sysinfo.h",
    "sys/timerfd.h",
    "sys/eventfd.h",
    "sys/inotify.h",
    "sys/ucontext.h",
];

#[derive(Clone, Debug)]
pub struct Field {
    pub name: String,
    pub offset: u64,
    pub aggregate: bool,
    pub bitfield: bool,
    pub shape: String,
    pub records: Vec<String>,
}

#[derive(Clone, Debug)]
pub struct CType {
    pub size: u64,
    pub align: u64,
    pub fields: Option<Vec<Field>>,
    pub record_name: Option<String>,
    pub incomplete: bool,
}

pub type CMap = BTreeMap<String, CType>;

enum Def {
    Record(CType),
    Incomplete,
    Alias(String),
    Pointer,
    Other,
}

pub fn collect(
    parser: &Path,
    target: &str,
    scratch: &Path,
    providers: &crate::providers::Inventory,
) -> Result<CMap> {
    let source = scratch.join("empty.c");
    std::fs::write(&source, "")?;
    let headers: Vec<&str> = HEADERS
        .iter()
        .copied()
        .chain(providers.headers.iter().map(String::as_str))
        .collect();
    let dumps: Vec<String> = headers
        .par_iter()
        .filter_map(|header| {
            [true, false].into_iter().find_map(|all_declarations| {
                let output = Command::new(parser)
                    .arg("ir")
                    .arg(&source)
                    .args(target_args(target))
                    .args(["-include", header])
                    .args(all_declarations.then_some("--all-declarations"))
                    .output()
                    .ok()?;
                output
                    .status
                    .success()
                    .then(|| String::from_utf8_lossy(&output.stdout).into_owned())
            })
        })
        .collect();
    let mut merged = CMap::new();
    for dump in &dumps {
        for (name, ty) in parse_ir(dump) {
            match merged.entry(name) {
                std::collections::btree_map::Entry::Vacant(slot) => {
                    slot.insert(ty);
                }
                std::collections::btree_map::Entry::Occupied(mut slot) => {
                    if slot.get().incomplete && !ty.incomplete {
                        slot.insert(ty);
                    }
                }
            }
        }
    }
    if merged.is_empty() {
        return Err(anyhow::anyhow!("no header produced IR for {target}"))
            .context("is slate-parser built and the sysroot installed?");
    }
    Ok(merged)
}

fn leading_number(text: &str) -> Option<u64> {
    let end = text
        .find(|c: char| !c.is_ascii_digit())
        .unwrap_or(text.len());
    text[..end].parse().ok()
}

fn after<'a>(text: &'a str, key: &str) -> Option<&'a str> {
    text.find(key).map(|i| &text[i + key.len()..])
}

fn parse_ir(text: &str) -> CMap {
    let mut pointer = (8, 8);
    let mut defs: HashMap<String, Def> = HashMap::new();
    let mut names: Vec<(String, String)> = Vec::new();
    let mut tag_names: HashMap<String, String> = HashMap::new();
    let lines: Vec<&str> = text.lines().collect();
    let mut i = 0;
    while i < lines.len() {
        let line = lines[i];
        i += 1;
        let trimmed = line.trim_start();
        if let Some(rest) = trimmed.strip_prefix("pointer [size=") {
            if let (Some(size), Some(align)) = (
                leading_number(rest),
                after(rest, "align=").and_then(leading_number),
            ) {
                pointer = (size, align);
            }
            continue;
        }
        let Some(rest) = line.strip_prefix("    type @") else {
            continue;
        };
        let Some((id, rest)) = rest.split_once(' ') else {
            continue;
        };
        let (name, body) = match rest.strip_prefix("= ") {
            Some(body) => (None, body),
            None => match rest.split_once(" = ") {
                Some((name, body)) => (Some(name.to_string()), body),
                None => continue,
            },
        };
        let id = id.to_string();
        let def = if body.starts_with("struct incomplete") || body.starts_with("union incomplete") {
            if let Some(name) = &name {
                tag_names.insert(id.clone(), name.clone());
            }
            Def::Incomplete
        } else if (body.starts_with("struct") || body.starts_with("union")) && body.ends_with('{') {
            let mut fields = Vec::new();
            let mut footer = "";
            while i < lines.len() {
                let inner = lines[i];
                i += 1;
                if inner.starts_with("    }") {
                    footer = inner;
                    break;
                }
                if let Some(field) = inner.trim_start().strip_prefix("field")
                    && let Some((_, rest)) = field.split_once(' ')
                    && let Some((field_name, ty)) = rest.split_once(':')
                {
                    fields.push((
                        field_name.to_string(),
                        ty.trim_start().starts_with("@type"),
                        ty.contains(" : "),
                        ty.split(" [")
                            .next()
                            .unwrap_or(ty)
                            .split(" : ")
                            .next()
                            .unwrap_or(ty)
                            .trim_end_matches(';')
                            .to_string(),
                    ));
                }
            }
            let size = after(footer, "size=").and_then(leading_number);
            let align = after(footer, "align=").and_then(leading_number);
            let offsets: Option<Vec<u64>> = after(footer, "offsets=[").and_then(|rest| {
                let inside = &rest[..rest.find(']')?];
                if inside.trim().is_empty() {
                    return Some(Vec::new());
                }
                inside
                    .split(',')
                    .map(|part| part.trim().parse().ok())
                    .collect()
            });
            match (size, align) {
                (Some(size), Some(align)) => {
                    let fields =
                        offsets
                            .filter(|offsets| offsets.len() == fields.len())
                            .map(|offsets| {
                                fields
                                    .into_iter()
                                    .zip(offsets)
                                    .map(|((name, aggregate, bitfield, shape), offset)| Field {
                                        name,
                                        offset,
                                        aggregate,
                                        bitfield,
                                        records: type_ids(&shape),
                                        shape,
                                    })
                                    .collect()
                            });
                    if let Some(name) = &name {
                        tag_names.insert(id.clone(), name.clone());
                    }
                    Def::Record(CType {
                        size,
                        align,
                        fields,
                        record_name: name.clone(),
                        incomplete: false,
                    })
                }
                _ => Def::Other,
            }
        } else if let Some(target) = body.strip_prefix('@') {
            let end = target.find([' ', ';']).unwrap_or(target.len());
            Def::Alias(target[..end].to_string())
        } else if body.starts_with("ptr<") {
            Def::Pointer
        } else {
            Def::Other
        };
        if let Some(name) = name {
            names.push((name, id.clone()));
        }
        defs.insert(id, def);
    }

    let mut map = CMap::new();
    for (name, id) in names {
        let mut current = id;
        let mut resolved = None;
        for _ in 0..16 {
            match defs.get(&current) {
                Some(Def::Alias(next)) => current = next.clone(),
                Some(Def::Record(ty)) => {
                    let mut ty = ty.clone();
                    ty.record_name = tag_names.get(&current).cloned();
                    for field in ty.fields.iter_mut().flatten() {
                        field.records = field
                            .records
                            .iter()
                            .filter_map(|id| record_tag(&defs, &tag_names, id))
                            .collect();
                    }
                    resolved = Some(ty);
                    break;
                }
                Some(Def::Incomplete) => {
                    resolved = Some(CType {
                        size: 0,
                        align: 1,
                        fields: Some(Vec::new()),
                        record_name: tag_names.get(&current).cloned(),
                        incomplete: true,
                    });
                    break;
                }
                Some(Def::Pointer) => {
                    resolved = Some(CType {
                        size: pointer.0,
                        align: pointer.1,
                        fields: Some(Vec::new()),
                        record_name: None,
                        incomplete: false,
                    });
                    break;
                }
                _ => break,
            }
        }
        if let Some(ty) = resolved {
            map.entry(name).or_insert(ty);
        }
    }
    map
}

fn type_ids(shape: &str) -> Vec<String> {
    shape
        .match_indices("@type")
        .map(|(at, prefix)| {
            let digits = shape[at + prefix.len()..]
                .chars()
                .take_while(char::is_ascii_digit)
                .collect::<String>();
            format!("type{digits}")
        })
        .collect()
}

fn record_tag(
    defs: &HashMap<String, Def>,
    tag_names: &HashMap<String, String>,
    id: &str,
) -> Option<String> {
    let mut current = id.to_string();
    for _ in 0..16 {
        match defs.get(&current)? {
            Def::Alias(next) => current = next.clone(),
            Def::Record(_) | Def::Incomplete => return tag_names.get(&current).cloned(),
            _ => return None,
        }
    }
    None
}

pub fn macro_members(
    target: &str,
    scratch: &Path,
    providers: &crate::providers::Inventory,
) -> Result<BTreeSet<String>> {
    let source = scratch.join("macros.c");
    std::fs::write(&source, "")?;
    let mut bodies: HashMap<String, String> = HashMap::new();
    let mut texts: HashMap<PathBuf, Option<String>> = HashMap::new();
    for header in HEADERS
        .iter()
        .copied()
        .chain(providers.headers.iter().map(String::as_str))
    {
        let mut args = target_args(target);
        args.extend(["-include".into(), header.to_string()]);
        let Ok((unit, trace)) =
            slate::frontend::preprocess_traced(&source, slate::target::parse_args(&args)?)
        else {
            continue;
        };
        for event in &trace.events {
            if event.skipped
                || event.command_line
                || event.kind != slate_parser::pp::MacroEventKind::Define
            {
                continue;
            }
            let Some(path) = unit.files().get_path(event.loc.file) else {
                continue;
            };
            let Some(text) = texts
                .entry(path.to_path_buf())
                .or_insert_with(|| std::fs::read_to_string(path).ok())
            else {
                continue;
            };
            bodies.insert(
                event.name.clone(),
                macro_body(text, event.loc.offset, &event.name),
            );
        }
    }
    let mut pending: Vec<&String> = bodies
        .keys()
        .filter(|name| !name.starts_with('_'))
        .collect();
    let mut seen: BTreeSet<&String> = pending.iter().copied().collect();
    let mut members = BTreeSet::new();
    while let Some(name) = pending.pop() {
        let body = &bodies[name];
        members.extend(reached_members(body));
        for word in body.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')) {
            if let Some((referenced, _)) = bodies.get_key_value(word)
                && seen.insert(referenced)
            {
                pending.push(referenced);
            }
        }
    }
    Ok(members)
}

fn macro_body(text: &str, offset: usize, name: &str) -> String {
    let offset = offset.min(text.len());
    let start = text[..offset].rfind('\n').map_or(0, |n| n + 1);
    let mut body = String::new();
    for line in text[start..].lines() {
        match line.strip_suffix('\\') {
            Some(continued) => {
                body.push_str(continued);
                body.push(' ');
            }
            None => {
                body.push_str(line);
                break;
            }
        }
    }
    match body.find(name) {
        Some(at) => body[at + name.len()..].to_string(),
        None => String::new(),
    }
}

fn reached_members(body: &str) -> Vec<String> {
    let bytes = body.as_bytes();
    let mut members = Vec::new();
    let mut index = 0;
    while index < bytes.len() {
        let accessed = body[..index]
            .trim_end()
            .ends_with(|c: char| c.is_ascii_alphanumeric() || c == '_' || c == ')' || c == ']');
        let after = match bytes[index] {
            b'.' if accessed && bytes.get(index + 1) != Some(&b'.') => index + 1,
            b'-' if bytes.get(index + 1) == Some(&b'>') => index + 2,
            _ => {
                index += 1;
                continue;
            }
        };
        let rest = body[after..].trim_start();
        let name: String = rest
            .chars()
            .take_while(|c| c.is_ascii_alphanumeric() || *c == '_')
            .collect();
        if name.starts_with('_') {
            members.push(name);
        }
        index = after;
    }
    members
}

pub fn target_args(target: &str) -> Vec<String> {
    vec![format!("-target={target}")]
}
