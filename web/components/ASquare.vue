<template>
  <div class="a-square">
    <div class="a-square__inner">
      <slot />
    </div>
  </div>
</template>

<style scoped>
.a-square {
  position: relative;
  height: 0;
  padding-bottom: 100%;
}
.a-square__inner {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
}
</style>
