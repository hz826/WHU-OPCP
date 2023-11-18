<template>
<div style="display: flex; justify-content: center; align-items: flex-start; height: 100vh;">
    <ProfilePage />
</div>
</template>    
    
<script>
// @ is an alias to /src
import ProfilePage from '@/components/ProfilePage.vue'

export default {
    name: 'ProfileView',
    components: {
        ProfilePage
    }
}
</script>
