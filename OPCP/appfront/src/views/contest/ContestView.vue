<template>
<div style="display: flex; justify-content: center; align-items: flex-start; height: 100vh;">
    <ContestPage />
</div>

</template>

<script>
// @ is an alias to /src
import ContestPage from '@/components/ContestPage.vue'

export default {
    name: 'Contest',
    components: {
        ContestPage
    }
}
</script>


<style>
</style>