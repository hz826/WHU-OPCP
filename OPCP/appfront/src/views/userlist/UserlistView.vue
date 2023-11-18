<template>
<div style="display: flex; justify-content: center; align-items: flex-start; height: 100vh;">
    <RankList />
</div>
</template>

  
  
<script>
// @ is an alias to /src
import RankList from '@/components/RankList.vue'

export default {
    name: 'Userlist',
    components: {
        RankList
    }
}
</script>
