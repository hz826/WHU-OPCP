<template>
<div class="registerblock">
    <h2>Create a new account for attending the contest!</h2>
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
            <h3>User: <input type="text" v-model="Register.name"></h3>
            <h3>Password:<input type="password" v-model="Register.author"></h3>
        </form>
        <el-button @click="RegisterSubmit()" class="elregister">Register</el-button>
    </div>
</div>
</template>

<script>
import store from '@/store';
import {
    Register
} from '../api/api.js'
export default {
    name: 'RegisterPage',
    data() {
        return {
            Register: {
                "name": "",
                "author": "",
            },
            Msg: "",
            IfSuccess: false,
        }
    },
    methods: {
        RegisterSubmit() {
            Register(this.Register.name, this.Register.author).then(response => {
                this.Msg = "You have successfully registered!"
                this.IfSuccess = true
                store.commit('Clean')
            }).catch(error => {
                this.Msg = "The user name already exists!"
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

.elregister {
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
