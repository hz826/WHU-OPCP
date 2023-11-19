<template>
    <div>
      <div class="container">
        <div class="judge-info">
          <h1> Judge Details</h1>
          <p><strong>Judge id:</strong> {{ submission.id }}</p>
          <p><strong>User:</strong> {{ submission.user }}</p>
          <p><strong>Contest:</strong> {{ submission.contest }}</p>
          <p><strong>Status:</strong> {{ submission.status }}</p>
          <p v-if="submission.status == 'OK'"><strong>Score:</strong> {{ submission.score }}</p>
        </div>
      </div>
    </div>
  </template>
  
  <script>
  import { GetSubmissionsOne } from '../api/api.js';
  
  export default {
    name: 'JudgeListOne',
    data() {
      return {
        linkURL: '',
        submission: [],
      };
    },
    methods: {
      LoadSubmissions() {
        GetSubmissionsOne(this.$route.params.judgeid).then((response) => {
          this.submission = response.data;
          this.linkURL = `http://localhost:8000/api/download/${this.submission.file}`;
        });
      },
    },
    created: function () {
      this.LoadSubmissions();
    },
  };
  </script>
  
  <style>
  p {
    font-size: 25px;
  }
  
  .container {
    display: flex;
    justify-content: center;
  }
  
  .judge-info {
    text-align: left;
    display: flex;
    flex-direction: column;
    margin-bottom: 40px;
  }
  
  .judge-info p {
    margin: 10px 0;
  }
  
  .download-button {
    border: 2px solid #42b983;
    background-color: #b3dbc9;
    color: black;
    padding: 15px;
    font-weight: 800;
    font-size: 15px;
    border-radius: 15px;
  }
  </style>