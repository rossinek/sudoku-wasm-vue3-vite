declare global {
  interface Window {
    __CORE_READY__: Promise<void>;
    __DEV__: boolean;
  }
}

export enum BoardValidationResult {
  VALID_INCOMPLETE = -1,
  INVALID = 0,
  VALID_COMPLETE = 1,
}

export const ensureCoreReady = () => window.__CORE_READY__

export const initializeCore = () => {
  let resolveInitialization: () => void
  window.__CORE_READY__ = new Promise(resolve => resolveInitialization = resolve)
  const basePath = process.env.NODE_ENV === 'production' ? '/sudoku-wasm-vue3-vite' : ''
  window.Module = {
    onRuntimeInitialized: () => { resolveInitialization() },
    locateFile: url => `${basePath}/src-compiled/${url}`,
  } as any
  const script = document.createElement('script')
  script.src = `${basePath}/src-compiled/sudoku.js`
  document.body.appendChild(script)
}

export const generateBoard = (size: number, level: number) => {
  const boardPointer = ccall('generateBoard', 'number', ['number', 'number'], [size, level]) as number
  const boardData: number[] = []
  for (let i = 0; i < size**4; i++) {
    boardData.push(Module.HEAPU8[boardPointer / Uint8Array.BYTES_PER_ELEMENT + i])
  }
  return boardData
}

export const validateBoard = (size: number, boardData: number[]) => {
  let buffer: number
  let error: any
  let result: BoardValidationResult
  const dataLength = size**4
  try {
    const typedBoardData = new Uint8Array(dataLength)
    for (let i = 0; i < dataLength; i++) {
      typedBoardData[i] = boardData[i]
    }
    buffer = Module._malloc(dataLength * typedBoardData.BYTES_PER_ELEMENT)
    Module.HEAPU8.set(typedBoardData, buffer)
    result = ccall('validateBoard', 'number', ['number', 'number'], [size, buffer])
  } catch (e) {
    error = e
    console.error(e)
  } finally {
    Module._free(buffer)
  }
  if (error) {
    throw error
  }
  return result
}
