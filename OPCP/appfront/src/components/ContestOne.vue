<template>
  <div>
    <h1>{{ ContestName }}</h1>
    <h2>Description</h2>
    <h3>{{ Content.description }}</h3>
  </div>
</template>

<script>
import {GetDescription} from '../api/api.js'
export default {
  name: 'ContstOne',
  data () {
    return {
      ContestName: this.$route.params.name,
      ContestId: this.$route.params.id,
      Content: []
    }
  },
  methods: {
    LoadContest () {
      GetDescription(this.ContestId).then(response => {
        this.Content = response.data
      })
    }
  }
}
</script>
