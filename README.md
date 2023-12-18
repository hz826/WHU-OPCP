# 武汉大学在线程序对抗平台（WHU-OPCP）

WHUCS 20级《大型软件应用设计》作业

## 使用手册

**开启后端**

后端使用 Django REST Framework 编写，运行环境为 python 3.11.5。

运行前，需要安装根目录下 requirements.txt 中的包。

```
pip install -r requirements.txt
```

在 OPCP/server 目录下，运行以下命令以开启服务器：

```
python manage.py makemigrations
python manage.py migrate
python manage.py runserver
```

**开启前端**

在 OPCP/appfront 目录下，运行以下命令开启前端：

```
npm install vue-cli
npm run serve
```

**开启评测机**

在 Judgehost 目录下，运行 server.py 即可开启评测机：

```
python server.py
```

运行目录中的 unittest.py 可以进行测试。