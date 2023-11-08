<template>
<div class="wrap" id="appFooter">
    <h1 v-if="GetState == true">
        Welcome, {{ GetName }}!
    </h1>
    <h1 v-else>
        NOT LOGIN YET
    </h1>
</div>
</template>

<script>
import store from './store'
export default {
    name: 'appFooter',
    computed: {
        GetName() {
            return store.state.name
        },
        GetState() {
            return store.state.login
        }
    },
}
</script>
