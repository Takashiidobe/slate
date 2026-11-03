use anyhow::{Context, Result};
use rayon::prelude::*;
use std::collections::{BTreeMap, HashMap};
use std::path::Path;
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
    let dumps: Vec<Option<String>> = headers
        .par_iter()
        .map(|header| {
            let output = Command::new(parser)
                .arg("ir")
                .arg(&source)
                .args(target_args(target))
                .args(["-include", header])
                .args((*header == "windows.h").then_some("--all-declarations"))
                .output()
                .context("run header parser")?;
            if *header == "windows.h" && !output.status.success() {
                anyhow::bail!(
                    "{target}: windows.h probe failed: {}",
                    String::from_utf8_lossy(&output.stderr)
                );
            }
            Ok(output
                .status
                .success()
                .then(|| String::from_utf8_lossy(&output.stdout).into_owned()))
        })
        .collect::<Result<_>>()?;
    let mut merged = CMap::new();
    for dump in dumps.iter().flatten() {
        for (name, ty) in parse_ir(dump) {
            merged.entry(name).or_insert(ty);
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

pub fn target_args(target: &str) -> Vec<String> {
    let mut args = vec![format!("-target={target}")];
    if target.contains("-linux-gnu")
        && slate_parser::target_info::TargetInfo::for_triple(target)
            .is_ok_and(|info| info.pointer_width == 32)
    {
        args.extend(["-D_FILE_OFFSET_BITS=64".into(), "-D_TIME_BITS=64".into()]);
    }
    args
}
