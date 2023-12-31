import Vue from 'vue'
import Vuex from 'vuex'
import createPersistedState from "vuex-persistedstate"

Vue.use(Vuex)

const store = new Vuex.Store({
  state: {
    name: 'NOUSER',
    login: false,
    num: 0,
    token: '',
    email: 'mikukuovo@gmail.com',
    statement: '这个人很懒，什么也没有写'
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
      state.num = 0
    },
    ModifyToken (state, token) {
      state.token = token
    }
  },
  plugins: [createPersistedState({
    storage: window.sessionStorage
  })]
})

export default store