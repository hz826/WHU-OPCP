<template>
    <div class="UserList">
      <h1>UserList</h1>
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
      