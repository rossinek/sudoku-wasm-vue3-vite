<template>
  <label :class="['a-toggle', $attrs.class]">
    <input v-model="internalModel" type="checkbox" v-bind="inputAttrs">
    <div class="a-toggle__inner" />
    <div v-if="$slots.default" class="a-toggle__label">
      <slot />
    </div>
  </label>
</template>

<script lang="ts">
import { defineComponent, ref, watch, computed } from 'vue'

export default defineComponent({
  inheritAttrs: false,
  setup (props, context) {
    const internalModel = ref((context.attrs.modelValue as boolean) || false)
    const inputAttrs = computed(() => {
      const { class: className, ...attrs } = context.attrs
      return attrs
    })
    return { internalModel, inputAttrs }
  }
})
</script>

<style scoped>
.a-toggle {
  position: relative;
  display: inline-flex;
  align-items: center;
  cursor: pointer;
}
.a-toggle input {
  position: absolute;
  opacity: 0;
  width: 0;
  height: 0;
}
.a-toggle__inner {
  position: relative;
  width: 50px;
  height: 28px;
  background-color: var(--border-color);
  border-radius: 14px;
  transition: 0.3s;
}
.a-toggle__inner::before {
  border-radius: 50%;
  position: absolute;
  content: "";
  height: 20px;
  width: 20px;
  left: 4px;
  bottom: 4px;
  background-color: var(--color-light);
  transition: 0.3s;
}
input:checked + .a-toggle__inner {
  background-color: var(--color-primary--dark);
}
input:checked + .a-toggle__inner::before {
  transform: translateX(22px);
}
.a-toggle__label {
  margin-left: 15px;
}
</style>
