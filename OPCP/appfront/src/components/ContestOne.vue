<template>
  <div>
    <h1>{{ ContestName }}</h1>
    <h2>Description</h2>
    <h3>{{ Content.description }}</h3>
    <h2>Upload your code</h2>
    <input style="width: 74px;" type="file" id="uFile" name="uFile" @change="upload($event)"/>
  </div>
</template>

<script>
import {GetDescription} from '../api/api.js'
export default {
  name: 'ContstOne',
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
    upload(e){
      var that=this
      var files = document.getElementById('uFile').value;
      if (!/\.(c|cpp|py)$/i.test(files)) {
        this.$message.warning("Must be .c/.cpp/.py file")
        return false;
      }
      let file = e.target.files[0]    
      let formData = new FormData()
      formData.append('filename', 'mdlxr');
      formData.append('file', file);
      this.axios.post(`http://localhost:8000/api/upload/`, formData).then((response)=>{   
              this.$message.success("Successfully added")  //需要引入elemrnt
      }).catch((error)=>{
          console.log(error)
              this.$message.warning("Addition failed")
        })
     }
  },
  created: function () {
    this.LoadContest()
  }
}
</script>
