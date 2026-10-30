import { readFile } from 'node:fs/promises';
import { resolve } from 'node:path';
import { pathToFileURL } from 'node:url';

const directory = resolve(process.argv[2]);
const html = await readFile(resolve(directory, 'index.html'), 'utf8');
const worker = html.match(/new Worker\('([^']+)'/)[1];
globalThis.fetch = async (path) => new Response(await readFile(resolve(directory, path)));
globalThis.onmessage = null;
let result;
globalThis.postMessage = (message) => {
  if (message.type === 'result') result = message;
};
await import(pathToFileURL(resolve(directory, worker)).href);

const groups = JSON.parse(await readFile(resolve(directory, 'examples/index.json'), 'utf8'));
for (const example of groups.flatMap(({ examples }) => examples)) {
  const source = await readFile(resolve(directory, 'examples', example.file), 'utf8');
  result = undefined;
  await onmessage({ data: { id: 0, source, flags: '', ...example } });
  if (!result?.ok) throw new Error(`${example.file}: ${result?.error ?? 'no result'}`);
  if (result.outputs && !result.outputs.ast.text.replace(/^===.*===\n/gm, '').trimStart().startsWith('(TranslationUnit')) {
    throw new Error(`${example.file}: AST output is not an S-expression`);
  }
  const outputs = result.outputs ? Object.values(result.outputs) : [{ ok: true, text: result.rust }];
  for (const output of outputs) {
    if (!output.ok || !output.text?.trim()) {
      throw new Error(`${example.file}: ${JSON.stringify(output)}`);
    }
  }
  console.log(`${directory}: ${example.file} passed`);
}

result = undefined;
await onmessage({ data: { id: 1, source: 'int main( {', flags: '', compiler: 'clang', targets: ['x86_64-unknown-linux-gnu'] } });
if (result?.outputs) {
  if (Object.values(result.outputs).some((output) => output.ok || !output.diagnostics.length)) {
    throw new Error('malformed input did not produce parser diagnostics');
  }
} else if (result?.ok || !result?.diagnostics?.length) {
  throw new Error(`malformed input did not produce translation diagnostics: ${JSON.stringify(result)}`);
}
console.log(`${directory}: malformed input diagnostics passed`);
