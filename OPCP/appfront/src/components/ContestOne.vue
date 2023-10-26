<template>
  <div>
    <h1>Contest</h1>
    <h2>{{ ContestName }}</h2>
  </div>
</template>

<script>
export default {
  name: 'ContstOne',
  data () {
    return {
      ContestName: this.$route.params.name
    }
  }
}
</script>
