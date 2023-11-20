<template>
<div class="modifyprofile">
    <h1>Modify Profile</h1>
    <div>
        <h2>Name</h2>
        <input type="text" v-model="Profile.username">
    </div>
    <div>
        <h2>Email</h2>
        <input type="text" v-model="Profile.email">
    </div>
    <div>
        <h2>Statement</h2>
        <input type="text" v-model="Profile.description">
    </div>
    <div>
        <el-button class="submit" @click="ContestSubmit()">Submit</el-button>
    </div>
</div>
</template>

<script>
import store from '@/store';
import {
    GetUser
} from '../api/api.js'
export default {
    name: 'CreateContest',
    data() {
        return {
            Profile: {
                "id": "",
                "username": "",
                "password": "",
                "email": "",
                "description": ""
            },
        }
    },
    methods: {
        GetUser() {
            GetUser(store.state.num).then(response => {
                this.Profile = response.data
                console.log(response.data)
            })

        },
    },
    created: function () {
        this.GetUser()
    },
}
</script>

<style scoped>
input[type="text"] {
    border: 2px solid #42b983;
    padding: 10px;
    font-size: 20px;
    border-radius: 5px;
}

.submit {
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    padding: 10px;
    font-weight: 800;
    font-size: 15px;
    margin-top: 50px;
    border-radius: 15px;
}
</style>
