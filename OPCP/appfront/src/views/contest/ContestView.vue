<template>
  <div class="contest">
    <h1>Get involved in the contest and get best score!</h1>
    <ContestPage />
  </div>
</template>

<script>
// @ is an alias to /src
import RankList from '@/components/RankList.vue'
import ContestPage from '@/components/ContestPage.vue'

export default {
  name: 'Contest',
  components: {
    RankList,
    ContestPage
  }
}
</script>
    