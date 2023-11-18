<template>
<div class="ranklist">
    <h1>Ranklist</h1>
    <el-table :data="users" class="data_table">
        <el-table-column prop="username" label="User name" width="270">
        </el-table-column>
        <el-table-column prop="id" label="Email" width="180">
        </el-table-column>

        <el-table-column label="Enter" width="180">
            <template slot-scope="scope">
                <el-button type="primary" @click="GetProfile(scope.row.username, scope.row.id)" class="enter-button">View</el-button>
            </template>
        </el-table-column>
    </el-table>
</div>
</template>

<script>
import {
    GetUserList
} from '../api/api.js'
export default {
    name: 'RankList',
    data() {
        return {
            users: []
        }
    },
    methods: {
        LoadUsers() {
            GetUserList().then(response => {
                this.users = response.data
            })
        },
        GetProfile(name, email) {
            this.$router.push({
                path: `/profile/${name}`,
                query: {
                    email: email
                }
            })
        },
    },
    created: function () {
        this.LoadUsers()
    }
}
</script>

<style scoped>
.data_table {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    font-size: 22px;
    font-weight: bold;
}

.enter-button {
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    color: black;
    padding: 15px;
    font-weight: 800;
    font-size: 15px;
    border-radius: 15px;
}
</style>
