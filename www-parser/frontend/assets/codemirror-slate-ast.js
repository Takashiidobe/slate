CodeMirror.defineSimpleMode('slate-ast', {
  start: [
    { regex: /===.*===/, token: 'comment' },
    { regex: /"(?:[^"\\]|\\.)*"|'(?:[^'\\]|\\.)*'/, token: 'string' },
    { regex: /-?\b(?:0x[\da-f]+|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)\b/i, token: 'number' },
    { regex: /\b(?:true|false)\b/, token: 'atom' },
    { regex: /\b[A-Z][\w]*/, token: 'type' },
    { regex: /[a-z_][\w-]*/, token: 'property' },
    { regex: /[()[\]{}]/, token: 'bracket' },
  ],
});
