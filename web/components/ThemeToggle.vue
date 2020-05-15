<template>
  <a-toggle v-model="darkMode" class="theme-toggle">
    <div class="theme-toggle__label">
      <div>light theme</div>
      <div>dark theme</div>
    </div>
  </a-toggle>
</template>

<script lang="ts">
import { defineComponent, ref, watch } from 'vue'
import AToggle from './AToggle.vue'

export const initializeTheme = () => {
  const preferredTheme = localStorage.getItem(STORAGE_KEY)
  if (preferredTheme) {
    document.body.setAttribute(DATA_ATTR, preferredTheme)
  } else if (window.matchMedia('(prefers-color-scheme: dark)').matches) {
    document.body.setAttribute(DATA_ATTR, THEME_DARK)
  } else {
    document.body.setAttribute(DATA_ATTR, THEME_LIGHT)
  }
}

const STORAGE_KEY = 'sv-theme'
const DATA_ATTR = 'data-theme'
const THEME_LIGHT = 'light'
const THEME_DARK = 'dark'

export default defineComponent({
  components: {
    AToggle,
  },
  setup (props, context) {
    const theme = document.body.getAttribute(DATA_ATTR)
    const darkMode = ref(theme === THEME_DARK)
    watch(() => darkMode.value, darkMode => {
      const theme = darkMode ? THEME_DARK : THEME_LIGHT
      localStorage.setItem(STORAGE_KEY, theme)
      document.body.setAttribute(DATA_ATTR, theme)
    })
    return { darkMode }
  }
})
</script>

<style scoped>
.theme-toggle {
  margin-top: 10px;
}
.theme-toggle__label {
  height: 1.5em;
  overflow: hidden;
}
.theme-toggle__label > * {
  line-height: 1.5em;
  white-space: nowrap;
  transition: transform 0.3s ease 0.4s;
}
.theme-toggle ::v-deep(input:checked ~ .a-toggle__label) .theme-toggle__label > * {
  transform: translateY(-1.5em);
}
.theme-toggle ::v-deep(.a-toggle__inner) {
  background: linear-gradient(-15deg, #def, #8bf);
}
.theme-toggle ::v-deep(input:checked + .a-toggle__inner) {
  background: linear-gradient(15deg, #a9d, #458 80%);
  box-shadow: inset 0 0 8px 3px rgba(0, 0, 0, 0.3);
}
.theme-toggle ::v-deep(.a-toggle__inner::before) {
  background-color: transparent;
  box-shadow: inset -24px -2px 0 1px white;
}
.theme-toggle ::v-deep(input:checked + .a-toggle__inner::before) {
  background-color: transparent;
  box-shadow: inset -4px -2px 0 1px white;
}
</style>
