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

export const validateBoard = (size: number, boardData: number[]): { status: BoardValidationResult, cells: boolean[] } => {
  let bufferPointer: number
  let validationResultsPointer: number
  let error: any
  let status: BoardValidationResult
  let validationResults: boolean[] = []
  const dataLength = size**4
  try {
    const typedBoardData = new Uint8Array(dataLength)
    for (let i = 0; i < dataLength; i++) {
      typedBoardData[i] = boardData[i]
    }
    bufferPointer = Module._malloc(dataLength * typedBoardData.BYTES_PER_ELEMENT)
    validationResultsPointer = Module._malloc(dataLength * Uint8Array.BYTES_PER_ELEMENT)
    Module.HEAPU8.set(typedBoardData, bufferPointer)
    status = ccall('validateBoard', 'number', ['number', 'number', 'number'], [bufferPointer, size, validationResultsPointer])
    for (let i = 0; i < size**4; i++) {
      validationResults.push(Boolean(Module.HEAPU8[validationResultsPointer / Uint8Array.BYTES_PER_ELEMENT + i]))
    }
  } catch (e) {
    error = e
    console.error(e)
  } finally {
    Module._free(bufferPointer)
    Module._free(validationResultsPointer)
  }
  if (error) {
    throw error
  }
  return {
    status,
    cells: validationResults,
  }
}
