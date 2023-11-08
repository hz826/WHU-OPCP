<template>
<div class="loginblock">
    <h2>Waiting Login...</h2>
    <h1>
        <div v-if="IfSuccess == true">
            <RegisterTure>{{ Msg }}</RegisterTure>
        </div>
        <div v-else>
            <RegisterFalse>{{ Msg }}</RegisterFalse>
        </div>
    </h1>
    <div v-if="IfSuccess == false">
        <form action="">
            <h3>User: <input type="text" v-model="Login.name"></h3>
            <h3>Password:<input type="password" v-model="Login.author"></h3>
        </form>
        <el-button class="ellogin" @click="LoginSubmit()">Login</el-button>
    </div>
</div>
</template>

<script>
import store from '@/store';
import {
    Login
} from '../api/api.js'
export default {
    name: 'LoginPage',
    data() {
        return {
            Login: {
                "name": "",
                "author": "",
            },
            Msg: "",
            IfSuccess: false,
            dt: []
        }
    },
    computed: {},
    methods: {
        LoginSubmit() {
            Login(this.Login.name, this.Login.author).then(response => {
                this.Msg = "Login Successfully!"
                this.IfSuccess = true
                this.dt = response.data
                store.commit('Modify', this.Login.name)
                store.commit('ChangeId', this.dt.id)
                console.log(store.state.num)
            }).catch(error => {
                this.Msg = "Wrong Username or Password"
                this.IfSuccess = false
            });
        }
    }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->

<style scoped>
h3 {
    justify-content: center;
    font-size: 25px;
}

h2 {
    font-size: 30px;
}

ul {
    list-style-type: none;
    padding: 0;
}

li {
    display: inline-block;
    margin: 0 10px;
}

a {
    color: #42b983;
}

input[type="text"] {
    border: 2px solid #42b983;
    padding: 10px;
    font-size: 20px;
    border-radius: 5px;
}

input[type="password"] {
    border: 2px solid #42b983;
    padding: 10px;
    font-size: 20px;
    border-radius: 5px;
}

.ellogin {
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    color: black;
    padding: 10px;
    font-weight: 800;
    font-size: 15px;
    border-radius: 15px;
}

RegisterTure {
    color: rgb(7, 49, 164);
}

RegisterFalse {
    color: red;
}
</style>
