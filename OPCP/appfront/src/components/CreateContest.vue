<template>
  <div class="createcontest">
    <h1>Create Contest</h1>
      <h1>
      <div v-if="IfSuccess == true">
        <RegisterTure>{{ Msg }}</RegisterTure>   
      </div>
      <div v-else>
        <RegisterFalse>{{ Msg }}</RegisterFalse>   
      </div>
    </h1>
    <div v-if="IfSuccess == false">
        <form action="">
        <h3>ContestName: <input type="text" v-model="Contest.name"></h3>
        <h3>ContestDescription: <input type="text" v-model="Contest.description"></h3>
        </form>
        <button type="submit" @click="ContestSubmit()">Submit</button>
    </div>
  </div>
</template>

<script>
import {CreateContest} from '../api/api.js'
import store from '../store'
export default {
  name: 'CreateContest',
  data () {
    return {
        Contest: {
          "name": "",
          "creator": "",
          "description": ""
        },
        Msg: "",
        IfSuccess: false,
        Id: 0,
        
      }
  },
  methods: {
    ContestSubmit () {
      CreateContest(this.Contest.name, store.state.num, this.Contest.description).then(response => {
        this.Msg = "Create a contest successfully!"
        this.IfSuccess = true
      }).catch(error => {
        this.Msg = "Create contest fails!"
        this.IfSuccess = false
      });
    },
  },
  computed: {
    GetId() {
      return store.state.num
    }
  }
}
</script>

<style scoped>
h3 {
  justify-content: center;
  font-size: 25px;
}
h2 {
  font-size: 30px;
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

input[type="text"] {
  border: 2px solid #42b983;
  padding: 10px;
  font-size: 20px;
  border-radius: 5px;
}
button[type="submit"] {
  border: 2px solid #42b983;
  background-color: #b3dbc9;
  padding: 10px;
  font-weight: 800;
  font-size: 15px;
  border-radius: 15px;
}
RegisterTure {
  color: rgb(7, 49, 164);
}
RegisterFalse {
  color: red;
}
</style>
