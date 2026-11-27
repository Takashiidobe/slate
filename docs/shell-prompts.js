for (const code of document.querySelectorAll('pre > :is(code.language-sh, code.language-bash)')) {
    let continued = false;
    const prompts = code.textContent.split('\n').map((line) => {
        const prompt = !continued && line.trim() ? '$' : '';
        continued = /(?:^|[^\\])(?:\\\\)*\\$/.test(line);
        return prompt;
    });

    const gutter = document.createElement('span');
    gutter.className = 'shell-prompts';
    gutter.setAttribute('aria-hidden', 'true');
    gutter.dataset.prompts = prompts.join('\n');
    code.classList.add('with-shell-prompts');
    code.prepend(gutter);
}
