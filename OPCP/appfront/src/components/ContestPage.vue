<template>
  <div class="contestblock">
    <h1>Contest List</h1>
    <h2>Contest name | Creator ID</h2>
    <li v-for="(contest, index) in contests" :key="index" style="display:block">
      
      <h2>{{contest.name}} | {{contest.creator}}</h2>
      <et @click="getContest(contest.name)">Enter</et>
    </li>
    <h1>Interesting in Contest? Create One!</h1>
    <et @click="CreateContest()">Create One!</et>
  </div>
</template>

<script>
import {GetUserList, GetContestList} from '../api/api.js'
export default {
  name: 'ContestPage',
  data () {
    return {
      users: [],
      contests: [],
    }
  },
  methods: {
    LoadUsers () {
      GetUserList().then(response => {
        this.users = response.data
      })
    },
    LoadContest () {
      GetContestList().then(response => {
        this.contests = response.data
      })
    },
    getContest(name) {
      this.$router.push({
        path: `/contest/${name}`
      })
    },
    CreateContest() {
      this.$router.push({
        path: `/createcontest`
      })
    }
  },
  created: function () {
    this.LoadContest()
    this.LoadUsers()
  }
}
</script>

<style>
et {
  color: #1b2d9f;
  font-size: 20px;
  font-weight: 600;
  border: 3px solid #42b983;
  border-radius: 10px;
  background-color: #b3dbc9;
  padding: 10px;
}
</style>