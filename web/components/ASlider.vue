<template>
  <label class="a-slider">
    <span
      class="a-slider__label"
      :style="labelStyle"
    >
      {{ internalModel }}
    </span>
    <input
      class="a-slider__input"
      type="range"
      min="0"
      max="100"
      v-model="internalModel"
      v-bind="$attrs"
    />
  </label>
</template>

<script lang="ts">
import { defineComponent, ref, watch, computed } from 'vue'

export default defineComponent({
  inheritAttrs: false,
  setup (props, context) {
    const internalModel = ref((context.attrs.modelValue as number) || 0)
    const labelStyle = computed(() => {
      const min = +context.attrs.min || 0
      const max = +context.attrs.max || 100
      const factor = ((internalModel.value - min) / (max - min)) - 0.5
      return {
        transform: `translateX(calc(${factor * 100}% - ${24 * factor}px))`
      }
    })
    return { internalModel, labelStyle }
  }
})
</script>

<style scoped>
.a-slider {
  display: inline-block;
  position: relative;
  width: 100%;
  max-width: 300px;
}
.a-slider__label {
  display: block;
  font-size: 30px;
  margin-bottom: 20px;
  width: 100%;
  text-align: center;
  transition: transform 0.2s ease;
}
.a-slider__input {
  -webkit-appearance: none;
  appearance: none;
  width: 100%;
  height: 5px;
  background: var(--border-color);
  outline: 0;
  transition: opacity .2s;
}
.a-slider__input:hover {
  opacity: 1;
}
.a-slider__input::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 24px;
  height: 24px;
  background: var(--color-primary);
  cursor: pointer;
  border: 0;
  outline: 0;
  border-radius: 50%;
}
.a-slider__input::-moz-range-thumb {
  width: 24px;
  height: 24px;
  background: var(--color-primary);
  cursor: pointer;
  border: 0;
  outline: 0;
  border-radius: 50%;
}
</style>
