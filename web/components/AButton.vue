<template>
  <button class="a-button">
    <slot />
  </button>
</template>

<style scoped>
.a-button {
  appearance: none;
  display: inline-block;
  border: 0;
  outline: 0;
  background: var(--color-primary--dark);
  color: var(--color-light);
  text-transform: uppercase;
  padding: 10px 30px;
  font-size: 1rem;
  cursor: pointer;
}
.a-button::-moz-focus-inner {
  border: 0;
}
.a-button:hover,
.a-button:focus {
  background: var(--color-primary);
}
.a-button:focus {
  outline: 2px dashed var(--color-primary--dark)
}
.a-button:active {
  opacity: 0.8;
}
</style>
