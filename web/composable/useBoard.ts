import { ref, Ref, reactive, defineComponent, markRaw, watch, computed } from 'vue'
import { KeyCode } from '../utils/keyboard'
import { generateBoard, validateBoard, BoardValidationResult } from '../utils/board'

export default (size: Ref<number>, level: Ref<number> = ref(0), showConflicts: Ref<boolean> = ref(false)) => {
  const refs = reactive<Array<Ref<HTMLInputElement>>>([])
  const model = reactive<string[]>([])
  const boardData = reactive<number[]>([])
  const boardValidation = computed(() => {
    if (!model.length) return { status: BoardValidationResult.VALID_INCOMPLETE, cells: [] }
    return validateBoard(size.value, model.map(Number))
  })

  watch(() => [size.value, level.value], ([size, level]) => {
    boardData.splice(0, boardData.length, ...generateBoard(size, level))
    model.splice(0)
    model.length = size**4
    model.fill('').forEach((_, i) => model[i] = boardData[i] ? `${boardData[i]}` : '')
    refs.splice(0)
    refs.length = size**4
    refs.fill('' as any).forEach((_, i) => refs[i] = ref())
    // focus first input once after mounted
    const unwatch = watch(() => refs[0].value, () => {
      const index = refs.findIndex(r => !r.value.readOnly)
      refs[index] && refs[index].value.select()
      unwatch()
    })
  }, { immediate: true })

  const squareIndexToBoardIndex = (squareIndex: number, cellIndex: number) => {
    const squareColumn = cellIndex % size.value
    const squareRow = Math.floor(cellIndex / size.value)
    const boardColumn = (squareIndex % size.value) * size.value + squareColumn
    const boardRow = Math.floor(squareIndex / size.value) * size.value + squareRow
    return boardRow * size.value * size.value + boardColumn
  }

  // works fine only for size*size < 100 :wink:
  const allowedPrefixes = new Array(Math.floor(size.value * size.value * 0.1)).fill('').map((_, i) => `${i + 1}`)
  const isCellValid = (v: string) => v === '' || (+v > 0 && +v <= size.value * size.value)
  const isCellReadonly = (index: number) => !!boardData[index]
  const moveLeft = (index: number) => index % (size.value * size.value) > 0 ? index - 1 : index
  const moveRight = (index: number) => index % (size.value * size.value) < size.value * size.value - 1 ? index + 1 : index
  const moveUp = (index: number) => index / (size.value * size.value) >= 1 ? index - (size.value * size.value) : index
  const moveDown = (index: number) => index / (size.value * size.value) < (size.value * size.value) - 1 ? index + (size.value * size.value) : index

  const inputAttributes = (index) => ({
    type: 'text',
    inputmode: 'numeric',
    value: model[index],
    readonly: isCellReadonly(index),
    'data-error': (!showConflicts.value || boardValidation.value.cells[index]) ? undefined : true
  })

  const inputListeners = (index) => ({
    keydown: (ev: KeyboardEvent) => {
      switch (ev.keyCode) {
        case KeyCode.LEFT_ARROW:
          (refs[moveLeft(index)].value as HTMLInputElement).select()
          ev.preventDefault()
          break
        case KeyCode.RIGHT_ARROW:
          (refs[moveRight(index)].value as HTMLInputElement).select()
          ev.preventDefault()
          break
        case KeyCode.UP_ARROW:
          (refs[moveUp(index)].value as HTMLInputElement).select()
          ev.preventDefault()
          break
        case KeyCode.DOWN_ARROW:
          (refs[moveDown(index)].value as HTMLInputElement).select()
          ev.preventDefault()
          break
      }
    },
    input: (ev: InputEvent) => {
      const target = ev.target as HTMLInputElement
      if (isCellValid(target.value) && !isCellReadonly(index)) {
        model[index] = target.value || ''
      } else {
        target.value = model[index]
      }
      if (!allowedPrefixes.includes(target.value)) {
        target.select()
      }
    },
    focus: (ev: InputEvent) => (ev.target as HTMLInputElement).select(),
    click: (ev: InputEvent) => (ev.target as HTMLInputElement).select(),
  })

  const CellContextProvider = markRaw(defineComponent({
    props: {
      squareIndex: { type: Number },
      cellIndex: { type: Number },
    },
    render ({ squareIndex, cellIndex }) {
      const index = squareIndexToBoardIndex(squareIndex, cellIndex)
      return this.$slots.default({
        index,
        attributes: inputAttributes(index),
        listeners: inputListeners(index),
        ref: refs[index],
      })
    },
  }))

  return {
    boardValidation,
    CellContextProvider,
  }
}
