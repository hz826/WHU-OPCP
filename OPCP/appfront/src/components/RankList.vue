<template>
<div class="ranklist">
  <h1>
    ID | Username
  </h1>
  <h2>
    <li v-for="(user, index) in users" :key="index" style="display:block">
      {{index + 1}} | {{user.username}}
    </li>
  </h2>
</div>
</template>
  
<script>
import {GetUserList} from '../api/api.js'
export default {
  name: 'RankList',
  data () {
    return {
      users: []
    }
  },
  methods: {
    LoadUsers () {
      GetUserList().then(response => {
        this.users = response.data
      })
    },
  },
  created: function () {
    this.LoadUsers()
  }
}
</script>

<style scoped>
h3 {
  margin: 40px 0 0;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
}
</style>
