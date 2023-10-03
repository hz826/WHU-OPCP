<template>
    <div class="loginblock">
      <form action="">
        <h3>User: <input type="text" v-model="inputBook.name"></h3>
        <h3>Password:<input type="text" v-model="inputBook.author"></h3>
      </form>
      <button type="submit" @click="bookSubmit()">Login</button>
    </div>
  </template>
  
  <script>
  import {getBooks, postBook} from '../api/api.js'
  export default {
    name: 'LoginPage',
    data () {
      return {
        msg: 'Welcome to Your Vue.js App',
        // books list data
        books: [],
        // book data in the form
        inputBook: {
          "name": "",
          "author": "",
        }
      }
    },
    methods: {
      loadBooks () {
        getBooks().then(response => {
          this.books = response.data
        })
      }, // load books list when visit the page
      bookSubmit () {
        postBook(this.inputBook.name, this.inputBook.author).then(response => {
          console.log(response)
          this.loadBooks()
        })
      } // add a book to backend when click the button
    },
    created: function () {
      this.loadBooks()
    }
  }
  </script>
  
  <!-- Add "scoped" attribute to limit CSS to this component only -->
  <style scoped>
  h3 {
    justify-content: center;
  }
  ul {
    list-style-type: none;
    padding: 0;
  }
  li {
    display: inline-block;
    margin: 0 10px;
  }
  a {
    color: #42b983;
  }
  </style>
  