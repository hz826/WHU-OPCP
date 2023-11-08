<template>
<div class="logout">
    <el-button class="ellogout" @click="Logout()">Logout</el-button>
</div>
</template>

    
    
<script>
import store from '@/store';
export default {
    name: 'LogoutPage',
    methods: {
        Logout() {
            store.commit('Clean')
        }
    }
}
</script>
    
    
<style scoped>
.ellogout {
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    color: black;
    padding: 10px;
    font-weight: 800;
    font-size: 15px;
    border-radius: 15px;
}
</style>
