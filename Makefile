main:	clean
	mkdir src-compiled
	emcc -g src/sudoku.c src/utils.c src/wasm.c -o src-compiled/sudoku.js -s EXPORTED_FUNCTIONS='["_generateBoard"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'


clean:
	rm -rf src-compiled
