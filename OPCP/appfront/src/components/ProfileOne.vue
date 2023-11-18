<template>
<div>
    <h1>User Information</h1>
    <div style="display: flex; justify-content: center; align-items: flex-start; height: 100vh;">

        <div class="user-details">
            <div class="user-avatar">
                <img :src="user.avatar" alt="User Avatar" />
            </div>
            <div class="user-info">
                <p><strong>Name:</strong> {{ user.name }}</p>
                <p><strong>Email:</strong> {{ user.email }}</p>
                <p><strong>Statement:</strong> {{ user.statement }}</p>
            </div>
        </div>
    </div>
</div>
</template>

<script>
import store from '../store'
export default {
    name: 'ProfileOne',
    data() {
        return {
            user: {
                name: this.$route.params.name,
                email: store.state.email,
                statement: store.state.statement,
                avatar: 'https://mikukuovo.github.io/images/avatar.png'
            }
        };
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
