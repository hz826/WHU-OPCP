<template>
  <div id="app">
    <nav v-if="GetState == false">
      <router-link to="/">
        <span class="bordered-text">Welcome</span>
      </router-link> 
      <router-link to="/register">
        <span class="bordered-text">Register</span>
      </router-link>
      <router-link to="/login">
        <span class="bordered-text">Login</span>
      </router-link>
      <router-link to="/contest">
        <span class="bordered-text">Contest</span>
      </router-link>
      <router-link to="/userlist">
        <span class="bordered-text">UserList</span>
      </router-link>
    </nav>
    <nav v-else>
      <router-link to="/">
        <span class="bordered-text">Welcome</span>
      </router-link> 
      <router-link to="/register">
        <span class="bordered-text">Register</span>
      </router-link>
      <router-link to="/contest">
        <span class="bordered-text">Contest</span>
      </router-link>
      <router-link to="/userlist">
        <span class="bordered-text">UserList</span>
      </router-link>
      <router-link to="/logout">
        <span class="bordered-text">Logout</span>
      </router-link>
    </nav>
    <router-view/>
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
  name: 'App',
  computed: {
    GetName() {
      return store.state.name
    },
    GetState() {
      return store.state.login
    }
  },
  methods: {
  },
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #03110c;
}

a {
  text-decoration: none;
}

nav {
  padding: 30px;
}

nav a {
  font-weight: bold;
  color: #03110c;
  font-size: 25px;
}

nav a.router-link-exact-active {
  color: #42b983;
  font-size: 25px;
}

.bordered-text {
  border: 2px solid #d4ede2;
  padding: 5px;
  margin-right: 8px;
  border-radius: 8px;
}
</style>
