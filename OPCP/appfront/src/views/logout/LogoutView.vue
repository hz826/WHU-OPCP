<template>
<div class="logout">
    <button type="submit" @click="Logout()">Logout</button>
</div>
</template>

<script>
import store from '@/store';
export default {
name: 'LogoutView',
    methods: {
        Logout() {
            store.commit('Clean')
        }
    }
}
</script>

<style>
button[type="submit"] {
  border: 2px solid #42b983;
  background-color: #b3dbc9;
  padding: 10px;
  font-weight: 800;
  font-size: 15px;
  border-radius: 15px;
}
</style>