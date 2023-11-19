<template>
<div>
    <div class="container">
        <div class="judge-info">
            <h1> Judge Details</h1>
            <p><strong>Judge id:</strong> {{ submission.id }}</p>
            <p><strong>User:</strong> {{ submission.user }}</p>
            <p><strong>Contest:</strong> {{ submission.contest }}</p>
            <p><strong>Status:</strong> {{ submission.status }}</p>
            <p v-if="submission.status == 'OK'"><strong>Score:</strong> {{ submission.score }}</p>
        </div>
    </div>
    <div class="container">
        <div class="judge-detail">
            <el-table :data="array" class="data_table">
                <el-table-column prop="user2" label="Opponent" width="180">
                </el-table-column>
                <el-table-column prop="score" label="Score" width="180">
                </el-table-column>
                <el-table-column prop="status" label="Status" width="180">
                    <template slot-scope="scope">
                        <div v-if="scope.row.status == 'Waiting'">
                            {{scope.row.status}}
                            <i class="el-icon-more"></i>
                        </div>
                        <div v-else-if="scope.row.status == 'Judging'">
                            {{scope.row.status}}
                            <i class="el-icon-refresh"></i>
                        </div>
                        <div v-else>
                            {{scope.row.status}}
                            <i class="el-icon-check"></i>
                        </div>
                    </template>
                </el-table-column>
                <el-table-column prop="detail" label="Download Details" width="180">
                    <template slot-scope="scope">
                        <el-button type="primary" @click="getJudgeDetail(scope.row.detail)" class="enter-button">Details</el-button>
                    </template>
                </el-table-column>
            </el-table>
        </div>
    </div>
</div>
</template>

<script>
import {
    GetSubmissionsOne,
    GetJudge
} from '../api/api.js';

export default {
    name: 'JudgeListOne',
    data() {
        return {
            linkURL: '',
            submission: [],
            array: []
        };
    },
    methods: {
        LoadSubmissions() {
            GetSubmissionsOne(this.$route.params.judgeid).then((response) => {
                    this.submission = response.data;
                }),
                GetJudge(this.$route.params.judgeid).then((response) => {
                    this.array = response.data
                    console.log(this.array)
                })
        },
        getJudgeDetail(judgeid) {
            this.linkURL = `http://localhost:8000/api/download/${judgeid}`;
            window.open(this.linkURL);
        }
    },
    created: function () {
        this.LoadSubmissions();
    },
};
</script>

<style scoped>
p {
    font-size: 25px;
}

.container {
    display: flex;
    justify-content: center;
}

.judge-info {
    text-align: left;
    display: flex;
    flex-direction: column;
    margin-bottom: 40px;
}

.judge-info p {
    margin: 10px 0;
}

.download-button {
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    color: black;
    padding: 15px;
    font-weight: 800;
    font-size: 15px;
    border-radius: 15px;
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

.data_table {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    font-size: 22px;
    font-weight: bold;
}
</style>
