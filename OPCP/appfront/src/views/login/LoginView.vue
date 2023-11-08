<template>
<div class="login">
    <div v-if="Login == false">
        <LoginPage />
    </div>
    <div v-else>
        <h2>You have successfully registered!</h2>
    </div>
</div>
</template>

<script>
// @ is an alias to /src
import LoginPage from '@/components/LoginPage.vue'

export default {
    name: 'LoginView',
    components: {
        LoginPage
    },
    data() {
        return {
            Login: false
        }
    }
}
</script>
