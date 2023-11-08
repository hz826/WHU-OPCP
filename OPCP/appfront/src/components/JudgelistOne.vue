<template>
<div class="judgelistone">
    <h2>Judge id: {{submission.id}}</h2>
    <h3>User: {{submission.user}} Contest: {{submission.contest}} Status:{{submission.status}}</h3>
    <h3 v-if="submission.status == 'OK'">Score: {{submission.score}}</h3>
    <a :href="linkURL">Downloadfile</a>
</div>
</template>

<script>
import {
    GetSubmissionsOne
} from '../api/api.js'
export default {
    name: 'JudgeListOne',
    data() {
        return {
            linkURL: '',
            submission: [],
        }
    },
    methods: {
        LoadSubmissions() {
            GetSubmissionsOne(this.$route.params.judgeid).then(response => {
                this.submission = response.data
                this.linkURL = `http://localhost:8000/api/download/${this.submission.file}`
            })
        }
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
