hljs.registerLanguage('ir', (hljs) => ({
    name: 'Slate IR',
    keywords: {
        keyword: [
            'module target freestanding nostdinc null_pointer_is_valid machine linker',
            'type struct union enum extern global fn block_fn nested_fn let asm',
            'if else for while do switch case default label block goto break continue return yield',
            'seh_try seh_leave except finally omp_parallel omp_for',
            'const volatile atomic comment macro_definition macro_expansion macro_body',
        ].join(' '),
        literal: 'true false null undef None inf',
    },
    contains: [
        hljs.C_LINE_COMMENT_MODE,
        hljs.C_BLOCK_COMMENT_MODE,
        hljs.QUOTE_STRING_MODE,
        { className: 'type', begin: /@type\d+\b/ },
        { className: 'variable', begin: /%\d+\b/ },
        { className: 'symbol', begin: /@[A-Za-z_.$][\w.$]*/ },
        { className: 'meta', begin: /#\d+\b/ },
        { className: 'attr', begin: /\b[A-Za-z_]\w*(?=\s*=)/ },
        {
            className: 'type',
            begin: /\b(?:[iu]\d+b?|f(?:16|32|64|80|128)|bf16|d(?:32|64|128)|void|bool|va_list|ptr|array|vla|vector|sizeless|complex|imaginary|(?:sat_)?fixed)\b/,
        },
        {
            className: 'built_in',
            begin: /\b(?:[A-Za-z_]\w*(?=<)|(?:deref|field\d+|index|lane|dynamic|Some|va_start|va_end|va_copy)(?=\())/,
        },
        hljs.C_NUMBER_MODE,
    ],
}));

hljs.registerLanguage('ebnf', () => {
    const identifier = /[A-Za-z][\w-]*(?:[ \t]+[A-Za-z][\w-]*)*/;
    const terminal = /"[^"\t\r\n\f]*"|'[^'\t\r\n\f]*'/;
    const specialSequence = /\?[^?\t\r\n\f]*\?/;

    return {
        name: 'Extended Backus-Naur Form',
        contains: [
            {
                className: 'comment',
                begin: /\(\*/,
                end: /\*\)/,
                contains: ['self', { begin: terminal }, { begin: specialSequence }],
            },
            { className: 'string', begin: terminal },
            { className: 'meta', begin: specialSequence },
            {
                className: 'title',
                begin: new RegExp(identifier.source + '(?=[ \\t]*=)'),
            },
            { begin: identifier },
            { className: 'number', begin: /\b\d+\b/ },
            { className: 'keyword', begin: /\([/:]|[/:]\)|[=,|/!;.*()[\]{}-]/ },
        ],
    };
});

// mdBook highlights its built-in languages before loading additional scripts.
for (const code of document.querySelectorAll('pre > :is(code.language-ir, code.language-ebnf)')) {
    hljs.highlightBlock(code);
}
