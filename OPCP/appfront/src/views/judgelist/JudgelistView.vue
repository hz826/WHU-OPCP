<template>
<div style="display: flex; justify-content: center; align-items: flex-start; height: 100vh;">
    <JudgelistPage />
</div>
</template>

<script>
import JudgelistPage from '@/components/JudgelistPage.vue'

export default {
    name: 'JudgelistView',
    components: {
        JudgelistPage
    }
}
</script>
