<template>
<div>
    <h1>User Information</h1>
    <div class="user-details">
        <div class="user-avatar">
            <img :src="avatar" alt="User Avatar" />
        </div>
        <div class="user-info">
            <p><strong>Name:</strong> {{ user.username }}</p>
            <p><strong>Email:</strong> {{ user.email }}</p>
            <p><strong>Statement:</strong> {{ user.description }}</p>
        </div>
    </div>
    <el-button @click="ModifyProfile()" class="create-button">Modify my Profile</el-button>
</div>
</template>

<script>
import store from '../store'
import {
    GetProfile,
} from '../api/api.js'
export default {
    data() {
        return {
            user: {
                username: store.state.name,
                email: store.state.email,
                description: store.state.statement
            },
            userid: store.state.num,
            avatar: 'https://mikukuovo.github.io/images/avatar.png'
        }
    },
    methods: {
        ModifyProfile() {
            this.$router.push({
                path: `/modifyprofile`,
            })
        },
        GetProfile() {
            GetProfile(this.userid).then(response => {
                console.log(response.data)
                console.log(this.userid)
                this.user = response.data
            })
        }
    },
    created: function () {
        this.GetProfile()
    }
};
</script>

<style scoped>
.user-details {
    display: flex;
    align-items: center;
}

.user-avatar img {
    width: 100px;
    height: 100px;
    border-radius: 50%;
}

.user-info {
    margin-left: 20px;
    text-align: left;
    font-family: Avenir, Helvetica, Arial, sans-serif;
    font-size: 20px;
}

.create-button {
    margin-top: 40px;
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    color: black;
    padding: 15px;
    font-weight: 800;
    font-size: 15px;
    border-radius: 15px;
}
</style>
