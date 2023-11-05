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
      if (!/\.(gif|jpg|jpeg|png|gif|jpg|png)$/i.test(files)) {
        this.$message.warning("图片类型必须是.gif,jpeg,jpg,png中的一种,请重新上传")
        return false;
      }
      let file = e.target.files[0]    
      let formData = new FormData()
      formData.append('filename', 'mdlxr');
      formData.append('file', file);
      this.axios.post(`http://localhost:8000/api/upload/`, formData).then((response)=>{   
 
              this.$message.success("添加成功")  //需要引入elemrnt
      }).catch((error)=>{
          console.log(error)
              this.$message.warning("添加失败")
        })
     }
  },
  created: function () {
    this.LoadContest()
  }
}
</script>
