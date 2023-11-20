<template>
<div>
    <h1>{{ ContestName }}</h1>
    <div class="limitation">
        Time limit : 1 second Memory limit : 256 megabytes
    </div>
    <div class="divider"></div>
    <div class="description">
        <h2>Description</h2>
        {{ Content.description }}
    </div>
    <div class="divider"></div>
    <div class="upload">
        <h2>Upload Your Code</h2>
        <input class="button" type="file" id="uFile" name="uFile" @change="upload($event)" />
    </div>
    <div class="divider"></div>
</div>
</template>

<script>
import {
    GetDescription,
} from '../api/api.js'
import store from '../store'
export default {
    name: 'ContstOne',
    componets: {},
    data() {
        return {
            ContestName: this.$route.params.name,
            ContestId: this.$route.query.contestid,
            Content: [],
            submission: [],
            Status: 'Waiting',
            socre: 0.0
        }
    },
    methods: {
        LoadContest() {
            GetDescription(this.ContestId).then(response => {
                console.log(response.data)
                this.Content = response.data
            })
        },
        upload(e) {
            var that = this
            var files = document.getElementById('uFile').value;
            if (!/\.(c|cpp|py)$/i.test(files)) {
                this.$message.warning("Must be .c/.cpp/.py file")
                return false;
            }
            let file = e.target.files[0]
            let formData = new FormData()
            formData.append('filename', 'mdlxr');
            formData.append('file', file);
            this.axios.post(`http://localhost:8000/api/upload/`, formData).then((response) => {
                this.$message.success("Successfully added")
                this.submission = response.data
                this.axios.post(`http://localhost:8000/api/submit/`, {
                    'user': store.state.num,
                    'contest': this.ContestId,
                    'file': this.submission.id,
                }, {
                    headers: {
                        Authorization: 'Bearer ' + store.state.token,
                    }
                }).then((response) => {}).catch((error) => {})

            }).catch((error) => {
                console.log(error)
                this.$message.warning("Addition failed")
            })
        }
    },
    created: function () {
        this.LoadContest()
    }
}
</script>

<style>
.divider {
    margin-top: 40px;
    border-bottom: 4px solid #42b983;
    /* 添加分割线样式 */
    margin-bottom: 40px;
    margin-left: 300px;
    margin-right: 300px;
    /* 调整分割线与上下内容的间距 */
}

input[type="file"]::file-selector-button {
    border-radius: 15px;
    padding: 10px 50px;
    background-color: #b3dbc9;
}

.button {
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    color: rgb(37, 152, 160);
    padding: 20px 20;
    /* 调整按钮的长度 */
    font-weight: 800px;
    font-size: 14px;
    border-radius: 15px;

}

.limitation {
    /* 添加左边距 */
}

.description {
    text-align: left;
    padding-left: 300px;
    padding-right: 300px;
    /* 添加左边距 */
}

.upload {
    text-align: center;
}

h1 {
    font-size: 40px;
}

h2 {
    font-size: 30px;
}
</style>
