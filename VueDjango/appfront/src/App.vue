<template>
  <div id="app">
    <nav>
      <router-link to="/">Welcome</router-link> | 
      <router-link to="/login">Login</router-link> |
      <router-link to="/competition">Competition</router-link>
    </nav>
    <router-view/>
  </div>
</template>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}

nav {
  padding: 30px;
}

nav a {
  font-weight: bold;
  color: #2c3e50;
  font-size: 20px;
}

nav a.router-link-exact-active {
  color: #42b983;
  font-size: 20px;
}
</style>
