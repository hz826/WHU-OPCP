<template>
  <div class="home">
    <HelloWorld /> 
    <input style="width: 74px;" type="file" id="uFile" name="uFile" @change="upload($event)"/>
  </div>
</template>

<script>
// @ is an alias to /src
import HelloWorld from '@/components/HelloWorld.vue'

export default {
  name: 'HomeView',
  components: {
    HelloWorld
  },
  data () {
    return {

    }
  },
  methods: {
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
     },
  }
}
</script>
