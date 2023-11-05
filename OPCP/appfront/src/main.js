import Vue from 'vue'
import App from './App.vue'
import router from './router'
import Vuex from 'vuex'
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
// import Cookies from 'js-cookie'

Vue.use(Vuex)
Vue.use(ElementUI);
Vue.config.productionTip = false

new Vue({
  router,
  render: h => h(App)
}).$mount('#app')
