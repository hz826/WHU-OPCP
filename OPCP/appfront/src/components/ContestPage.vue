<template>
<div>
    <h1>Contest List</h1>
    <el-table :data="contests" class="data_table">
        <el-table-column prop="name" label="Contest" width="180">
        </el-table-column>
        <el-table-column label="Enter" width="180">
            <template slot-scope="scope">
                <el-button type="primary" @click="getContest(scope.row.name, scope.row.id)" class="enter-button">Enter</el-button>
            </template>
        </el-table-column>
        <el-table-column label="Modify" width="180">
            <template slot-scope="scope">
                <el-button type="primary" v-if="scope.row.creator == GetId()" @click="modifyContest(scope.row.name, scope.row.id)" class="modify-button">Modify</el-button>
            </template>
        </el-table-column>
    </el-table>
</div>
</template>

<script>
import {
    GetUserList,
    GetContestList
} from '../api/api.js'
import store from '@/store'
export default {
    name: 'ContestPage',
    data() {
        return {
            users: [],
            contests: [],
        }
    },
    methods: {
        LoadUsers() {
            GetUserList().then(response => {
                this.users = response.data
            })
        },
        LoadContest() {
            GetContestList().then(response => {
                this.contests = response.data
            })
        },
        getContest(name, contestid) {
            this.$router.push({
                path: `/contest/${name}`,
                query: {
                    contestid: contestid
                }
            })
        },
        modifyContest(name, contestid) {
            this.$router.push({
                path: `/modifycontest/${name}`,
                query: {
                    contestid: contestid
                }
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
    created: function () {
        this.LoadContest()
        this.LoadUsers()
        this.GetId()
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

.modify-button {
    border: 2px solid #ff8c00;
    background-color: #ffa500;
    color: white;
    padding: 15px;
    font-weight: 800;
    font-size: 15px;
    border-radius: 15px;
}
</style>
