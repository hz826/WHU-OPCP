<template>
  <div class="contestblock">
    <h1>Contest List</h1>
    <h2>Contest name | Creator ID</h2>
    <li v-for="(contest, index) in contests" :key="index" style="display:block">
      
      <h2>{{contest.name}} | {{contest.creator}}</h2>
      <et @click="getContest(contest.name, contest.id)">Enter</et>
      <mt v-if="contest.creator == GetId()" @click="modifyContest(contest.name, contest.id)">Modify</mt>
      <v-else></v-else>
    </li>
    <h1>Interesting in Contest? Create One!</h1>
    <et @click="CreateContest()">Create One!</et>
  </div>
</template>

<script>
import {GetUserList, GetContestList} from '../api/api.js'
import store from '@/store'
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
    getContest(name, contestid) {
      this.$router.push({
        path: `/contest/${name}`,
        query: { contestid: contestid }
      })
    },
    modifyContest(name, contestid) {
      this.$router.push({
        path: `/modifycontest/${name}`,
        query: { contestid: contestid }
      })
    },
    CreateContest() {
      this.$router.push({
        path: `/createcontest`
      })
    },
    GetId() {
      return store.state.num
    }
  },
  computed: {
    
  },
  created: function () {
    this.LoadContest()
    this.LoadUsers()
    this.GetId()
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
  margin: 10px;
}
mt {
  color: #1b2d9f;
  font-size: 20px;
  font-weight: 600;
  border: 3px solid rgb(242, 107, 4);
  border-radius: 10px;
  background-color: rgb(223, 168, 136);
  padding: 10px;
  margin: 10px;
}
</style>