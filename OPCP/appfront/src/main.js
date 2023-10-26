import Vue from 'vue'
import App from './App.vue'
import router from './router'
import Vuex from 'vuex'
// import Cookies from 'js-cookie'

Vue.use(Vuex)
Vue.config.productionTip = false

new Vue({
  router,
  render: h => h(App)
}).$mount('#app')
