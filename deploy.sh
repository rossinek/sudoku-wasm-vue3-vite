
#!/usr/bin/env sh

# abort on errors
set -e

yarn build
cd sudoku-wasm-vue3-vite/

git init
git add -A
git commit -m 'deploy'
git push -f git@github.com:rossinek/sudoku-wasm-vue3-vite.git master:gh-pages

cd -
