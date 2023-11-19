<template>
<div>
    <h1>{{ ContestName }}</h1>
    <h2>Modify</h2>
    <div class="text-area">
        <el-input :rows="8" type="textarea" class="textarea" v-model="Description"></el-input>
    </div>
    <div class="button-area">
        <el-button class="submit-button" @click="ModifyDescription()">Submit</el-button>
    </div>
</div>
</template>

<script>
import {
    GetDescription,
    ModifyDescription
} from '../api/api.js'
export default {
    name: 'ModifyContest',
    data() {
        return {
            ContestName: this.$route.params.name,
            ContestId: this.$route.query.contestid,
            Content: [],
            Description: ''
        }
    },
    methods: {
        LoadContest() {
            GetDescription(this.ContestId).then(response => {
                this.Content = response.data
            })
        },
        ModifyDescription() {
            ModifyDescription(this.ContestId, this.Description).then(response => {
                this.$message.success("Successfully Modifyed")
            }).catch((error) => {
                console.log("QWQ")
            })
        }
    },
    created: function () {
        this.LoadContest()
    }
}
</script>

<style scoped>
.submit-button {
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    color: black;
    padding: 15px;
    font-weight: 800;
    font-size: 15px;
    border-radius: 15px;
    margin-top: 50px;
}
.textarea {
    width: 50%;
    height: 252px;
    border: 2px solid #42b983;
    font-size: 20px;
    border-radius: 5px;
}
</style>
