<template>
  <div class="register">
    <div v-if="Register == false">
      <RegisterPage />
    </div>
    <div v-else>
      <h2>You have successfully registered!</h2>
    </div>
  </div>
</template>

<script>
// @ is an alias to /src
import RegisterPage from '@/components/RegisterPage.vue'

export default {
  name: 'RegisterView',
  components: {
    RegisterPage
  },
  data() {
    return {
      Register: false
    }
  }
}
</script>
