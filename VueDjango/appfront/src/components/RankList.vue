<template>
<div class="hello">
    <!-- show books list -->
    <h1>
        Rank | User | Password
    </h1>
    <h2>
        <li v-for="(book, index) in books" :key="index" style="display:block">
            {{index + 1}} | {{book.name}} | {{book.author}}
        </li>
    </h2>
</div>
</template>
  
<script>
import {getBooks, postBook} from '../api/api.js'
export default {
name: 'RankList',
data () {
    return {
        // books list data
        books: [ ],
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
},
created: function () {
    this.loadBooks()
}
}
</script>
  
<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h3 {
margin: 40px 0 0;
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
