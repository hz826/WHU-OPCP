import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

const store = new Vuex.Store({
  state: {
    name: 'NOUSER',
    login: false,
    num: 0
  },
  mutations: {
    Modify (state, name) {
      state.name = name 
      state.login = true
    },
    ChangeId (state, id) {
      state.num = id
    },
    Clean (state) {
      state.login = false
    }
  }
})

export default store