<template>
<div>
    <h1>Judgelist</h1>
    <el-table :data="submissions" class="data_table">
        <el-table-column prop="id" label="Submission Id" width="180">
        </el-table-column>
        <el-table-column prop="user" label="User" width="180">
        </el-table-column>
        <el-table-column prop="contest" label="Contest" width="180">
        </el-table-column>
        <el-table-column prop="status" label="Status" width="180">
        </el-table-column>
        <el-table-column prop="score" label="Score" width="180">
        </el-table-column>
        <el-table-column label="" width="180">
            <template slot-scope="scope">
                <el-button type="primary" @click="getJudgeDetail(scope.row.id)" class="enter-button">Details</el-button>
            </template>
        </el-table-column>
    </el-table>
</div>
</template>

    
    
<script>
import {
    GetSubmissions
} from '../api/api.js'

export default {
    name: 'JudgelistPage',
    data() {
        return {
            submissions: [],
        }
    },
    components: {},
    methods: {
        LoadSubmissions() {
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
