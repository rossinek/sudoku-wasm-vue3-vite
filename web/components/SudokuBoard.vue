<template>
  <div :class="['sudoku-board', size > 3 && 'sudoku-board--dense']">
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
      default: 3,
    },
    level: {
      type: Number,
      default: 5,
    },
    showConflicts: {
      type: Boolean,
      default: false,
    },
  },
  setup(props, context) {
    const gridStyle = computed(() => ({
      gridTemplateColumns: `repeat(${props.size}, 1fr)`
    }))

    const {
      boardValidation,
      CellContextProvider,
    } = useBoard(toRef(props, 'size'), toRef(props, 'level'), toRef(props, 'showConflicts'))

    watch(() => boardValidation.value.status, (val: BoardValidationResult) => {
      if (val === BoardValidationResult.VALID_COMPLETE) {
        context.emit('solved')
      }
    })

    return {
      gridStyle,
      boardValidation,
      CellContextProvider,
    }
  }
})
</script>

<style scoped>
.sudoku-board {
  --grid-padding: 10px;
  --column-gap: 5px;
  --cell-font-size: 20px;
}
.sudoku-board--dense {
  --grid-padding: 5px;
  --column-gap: 2px;
  --cell-font-size: 15px;
}
@media (max-width: 480px), (max-height: 480px) {
  .sudoku-board {
    --grid-padding: 5px;
    --column-gap: 2px;
    --cell-font-size: 15px;
  }
  .sudoku-board--dense {
    --grid-padding: 3px;
    --cell-font-size: 13px;
  }
}

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
  padding: var(--grid-padding);
  margin: 0 auto;
  border-radius: var(--grid-padding);
  background-color: var(--border-color);
}
.grid--small {
  padding: var(--grid-padding);
  column-gap: var(--column-gap);
  row-gap: var(--column-gap);
}
.cell {
  appearance: none;
  -webkit-appearance: none;
  width: 100%;
  height: 100%;
  background-color: var(--background);
  border-radius: 5px;
  border: 0;
  padding: 0;
  text-align: center;
  font-size: var(--cell-font-size);
  color: var(--color-primary--dark);
}
.cell:focus {
  box-shadow: 0 0 0px 2px var(--color-primary) !important;
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
.cell[data-error] {
  box-shadow: 0 0 0px 2px var(--color-error);
  color: var(--color-error);
}
</style>
