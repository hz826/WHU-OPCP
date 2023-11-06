<template>
<div class="judgelistone">
    <h2>Judge id: {{submission.id}}</h2>  
    <h3>User: {{submission.user}} Contest: {{submission.contest}} Status:{{submission.status}}</h3>
    <h3 v-if="submission.status == 'OK'">Score: {{submission.score}}</h3>
</div>
</template>


<script>
import { GetSubmissionsOne } from '../api/api.js'
export default {  
    name: 'JudgeListOne',
    data () {
        return {
            submission: [],
        }
    },
    components: {
    },
    methods: {
        LoadSubmissions () {
        GetSubmissionsOne(this.$route.params.judgeid).then(response => {
            this.submission = response.data
        })
        }
    },
    created: function () {
        this.LoadSubmissions()
    }
}
</script>