<template>
    <div>
      <h1>{{ ContestName }}</h1>
      <h2>Description</h2>
      <h3>{{ Content.description }}</h3>
      <h2>Modify</h2>
      <textarea rows="4" cols="50"></textarea>
    </div>
  </template>
  
  <script>
  import {GetDescription} from '../api/api.js'
  export default {
    name: 'ModifyContest',
    data () {
      return {
        ContestName: this.$route.params.name,
        ContestId: this.$route.query.contestid,
        Content: []
      }
    },
    methods: {
      LoadContest () {
        console.log(this.ContestId)
        GetDescription(this.ContestId).then(response => {
          console.log(response.data)
          this.Content = response.data
        })
      },
    },
    created: function () {
      this.LoadContest()
    }
  }
  </script>
  <style>
  </style>