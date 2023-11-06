<template>
<div class="Judgelist">
    <h1>Judgelist</h1>
    <li v-for="(submission, index) in submissions" :key="index" style="display:block">  
      <h2>Judge id: {{submission.id}}</h2>  
      <h3>User: {{submission.user}} Contest: {{submission.contest}} Status:{{submission.status}}</h3>
      <h3 v-if="submission.status == 'OK'">Score: {{submission.score}}</h3>
      <et @click="getJudgeDetail(submission.id)">ShowDetails</et>
    </li>
</div>
</template>

<script>
import { GetSubmissions } from '../../api/api.js'

export default {
    name: 'JudgeList',
    data () {
        return {
            submissions: [],
        }
    },
    components: {
    },
    methods: {
        LoadSubmissions () {
        GetSubmissions().then(response => {
            this.submissions = response.data
        })
        },
        getJudgeDetail(judgeid) {
        this.$router.push({
            path: `/judgelist/${judgeid}`,
        })
        },
    },
    created: function () {
        this.LoadSubmissions()
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
</style>