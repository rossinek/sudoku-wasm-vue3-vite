<template>
  <div class="sudoku-board">
    <div class="grid grid--big" :style="gridStyle">
      <a-square v-for="i in size * size" :key="i">
        <div class="grid grid--small" :style="gridStyle">
          <a-square v-for="j in size * size" :key="j">
            <component
              :is="CellContextProvider"
              :squareIndex="i - 1"
              :cellIndex="j - 1"
              v-slot="{ index, ref, attributes, listeners }"
            >
              <input
                class="cell"
                :ref="ref"
                v-bind="attributes"
                v-on="listeners"
              >
            </component>
          </a-square>
        </div>
      </a-square>
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent, ref, reactive, toRef, computed, watch } from 'vue'
import ASquare from './ASquare.vue'

import useBoard from '../composable/useBoard'
import { BoardValidationResult } from '../utils/board'

export default defineComponent({
  components: {
    ASquare,
  },
  props: {
    size: {
      type: Number,
      default: 3 // current sudoku core support only 3 but web is ready for more
    },
    level: {
      type: Number,
      default: 3 // current sudoku core support only 3 but web is ready for more
    }
  },
  async setup(props, context) {
    const gridStyle = computed(() => ({
      gridTemplateColumns: `repeat(${props.size}, 1fr)`
    }))

    const {
      boardValidation,
      CellContextProvider,
    } = await useBoard(toRef(props, 'size'), toRef(props, 'level'))

    watch(() => boardValidation.value, (val: BoardValidationResult) => {
      if (val === BoardValidationResult.VALID_COMPLETE) {
        context.emit('solved')
      }
    })

    return {
      size: props.size,
      gridStyle,
      boardValidation,
      CellContextProvider,
    }
  }
})
</script>

<style scoped>
.sudoku-board {
  width: 100%;
  max-width: 70vh;
}
.grid {
  position: relative;
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  grid-auto-rows: 1fr;
}
.grid--big {
  padding: 10px;
  margin: 0 auto;
  border-radius: 10px;
  background-color: var(--border-color);
}
.grid--small {
  padding: 10px;
  column-gap: 5px;
  row-gap: 5px;
}
.cell {
  width: 100%;
  height: 100%;
  background-color: var(--background);
  border-radius: 5px;
  border: 0;
  text-align: center;
  font-size: 20px;
  color: var(--color-primary--dark);
}
.cell:focus {
  box-shadow: 0 0 0px 2px var(--color-primary);
  outline: 0;
}
.cell::selection {
  background-color: transparent;
}
.cell[readonly] {
  background-color: var(--background--readonly);
  color: var(--color-text);
  font-weight: bold;
}
input[type=number]::-webkit-outer-spin-button,
input[type=number]::-webkit-inner-spin-button {
  -webkit-appearance: none;
  margin: 0;
}
input[type=number] {
  -moz-appearance: textfield;
}
</style>
