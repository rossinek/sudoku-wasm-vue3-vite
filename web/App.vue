<template>
  <a-container>
    <h1 class="title">WASM Sudoku Generator</h1>

    <template v-if="route === ''">
      <div class="option">
        <h2 class="option__label">Difficulty</h2>
        <a-slider v-model="level" min="0" max="20" />
      </div>

      <div class="button-wrapper">
        <a-button @click="routerPush('game')">start</a-button>
      </div>
    </template>

    <template v-if="route === 'game' && coreReady">
      <div class="board-wrapper">
        <sudoku-board :level="level" @solved="onSolved" />
        <div class="hint">Tip: You can use keyboard for navigation.</div>
        <h2 :class="['win-info', !isGameSolved && 'win-info--hidden']">You win! 🎉</h2>
      </div>
      <div class="button-wrapper">
        <a-button @click="restart">restart</a-button>
      </div>
    </template>
  </a-container>
</template>

<script lang="ts">
import { defineComponent, ref, watch } from 'vue'
import useLocalRouter from './composable/useLocalRouter'
import AButton from './components/AButton.vue'
import AContainer from './components/AContainer.vue'
import ASlider from './components/ASlider.vue'
import SudokuBoard from './components/SudokuBoard.vue'
import { ensureCoreReady } from './utils/board'

export default defineComponent({
  components: {
    AButton,
    AContainer,
    ASlider,
    SudokuBoard,
  },
  setup () {
    const { route, routerPush } = useLocalRouter(['', 'game'])
    const level = ref(10)
    const coreReady = ref(false)
    ensureCoreReady().then(() => { coreReady.value = true })
    const isGameSolved = ref(false)
    const onSolved = () => { isGameSolved.value = true }
    const restart = () => {
      isGameSolved.value = false
      routerPush('')
    }

    return {
      level,
      coreReady,
      isGameSolved,
      onSolved,
      route,
      routerPush,
      restart,
    }
  }
})
</script>

<style scoped>
.title {
  margin-bottom: 40px;
}
.option {
  display: flex;
  align-items: flex-end;
  justify-content: center;
}
.option__label {
  margin: 0 40px 0 0;
}
.option ::v-deep(*:nth-child(2)) {
  flex-grow: 1;
  margin-bottom: 5px;
}
@media (max-width: 480px) {
  .option {
    display: block;
    text-align: center;
  }
  .option__label {
    margin: 0 0 30px 0;
  }
}
.button-wrapper {
  margin-top: 50px;
}
.board-wrapper {
  position: relative;
  overflow: hidden;
  display: flex;
  flex-direction: column;
  align-items: center;
}
.win-info {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: var(--background);
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 0;
  opacity: 0.8;
  transition: opacity 0.5s, transform 0.5s;
  font-size: 2.5em;
}
.win-info--hidden {
  transform: scale(2);
  opacity: 0;
  width: 0;
  height: 0;
}
.hint {
  margin-top: 20px;
  opacity: 0.5;
}
</style>
