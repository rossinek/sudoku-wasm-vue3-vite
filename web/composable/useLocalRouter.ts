import { ref, readonly, onBeforeMount, onUnmounted } from 'vue'

const getCurrentPath = () => location.hash.slice(1)

export default (paths: string[]) => {
  if (!paths || !paths.length) {
    paths = ['']
  }
  const defaultPath = paths[0]

  const route = ref<string>('')

  const setState = (path: string, replace: boolean = false) => {
    if (paths.includes(path)) {
      if (replace) {
        history.replaceState(undefined, undefined, `#${path}`)
      } else {
        history.pushState(undefined, undefined, `#${path}`)
      }
      updateRoute()
    } else {
      console.warn('No such path')
    }
  }

  const routerPush = (path: string) => setState(path)

  const routerReplace = (path: string) => setState(path, true)

  const updateRoute = () => {
    const path = getCurrentPath()
    if (paths.includes(path)) {
      route.value = path
    } else {
      setState(defaultPath, true)
    }
  }

  onBeforeMount(() => {
    window.addEventListener('hashchange', updateRoute)
    updateRoute()
  })

  onUnmounted(() => {
    window.removeEventListener('hashchange', updateRoute)
  })

  return {
    route: readonly(route),
    routerPush,
    routerReplace,
  }
}
