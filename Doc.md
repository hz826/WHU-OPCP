## User

**GET** ```/api/users```

- **返回用户列表**

```json
[
    {
        "id": 1,
        "username": "test",
        "password": "111222333"
    },
    {
        "id": 2,
        "username": "hz",
        "password": "pbkdf2_sha256$600000$wdW1BDx8gzU8K3BufjRqC9$PkuQW04vkhfjr3Nudekixd0B/z+gB5S10oSlMyQeipo="
    },
    {
        "id": 3,
        "username": "zhangchi",
        "password": "pbkdf2_sha256$600000$FRl4Wp03AsFFLa9Vpz4r95$2AkI/ECAmW4+N9ENyFEbiZw0ClH/HyjWAQojnnupdQE="
    },
    {
        "id": 4,
        "username": "tokentest",
        "password": "pbkdf2_sha256$600000$EbzxvI8BOAvyn2VBm0eQON$HC/xfmwRjA55C2GZPsBW1Cei37IMXV8vYWZ0SM+N1Ek="
    }
]
```

---

**GET**```/api/users/<int:pk>```

- **返回 id=pk 的用户信息**

```json
{
    "id": 1,
    "username": "test",
    "password": "111222333"
}
```

---

**（待修复）PUT** ```/api/users/<int:pk>```

- **修改 id=pk 的用户信息**

- **返回值为修改后的用户信息**

  ```json
  {
      "id": 1,
      "username": "test",
      "password": "111222333"
  }
  ```

- 需要 token

- | Parameter | Description    |
  | --------- | -------------- |
  | username  | 修改后的用户名 |
  | password  | 修改后的密码   |

---

**DELETE** ```/api/users/<int:pk>```

- **删除 id=pk 的用户**
- **返回状态码为 204 No Content**
- 需要 token

---

**POST** ```/api/register```

- **注册用户**

- **返回值为注册的用户信息**

  ```json
  {
      "id": 1,
      "username": "test",
      "password": "111222333"
  }
  ```

- | Parameter | Description |
  | --------- | ----------- |
  | username  | 用户名      |
  | password  | 密码        |

---

**POST** ```/api/login```

- **用户登录**

- **返回值为登录用户的 id**

  ```json
  {
  	"id": 1
  }
  ```

  

- | Parameter | Description |
  | --------- | ----------- |
  | username  | 用户名      |
  | password  | 密码        |

---

**POST** ```/api/token/```

- **获取指定用户的 token**

- **返回值为 access token 与 refresh token**

  ```json
  {
      "refresh": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ0b2tlbl90eXBlIjoicmVmcmVzaCIsImV4cCI6MTY5OTE5MDgzNiwiaWF0IjoxNjk5MTA0NDM2LCJqdGkiOiJmMGM5N2QzYTM3ZjA0OTg2YTRlMTBjODUyNjFmZTQwZiIsInVzZXJfaWQiOjR9.DfCPSc7CVUokmtCCSc_DJkVk8WAPqMYzkTGc6tsiSnY",
      "access": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ0b2tlbl90eXBlIjoiYWNjZXNzIiwiZXhwIjoxNjk5MTA4MDM2LCJpYXQiOjE2OTkxMDQ0MzYsImp0aSI6IjVjMTdhNzlmZDczZjQ4MDZhMzA0MjljYmZkYmY1NWQ1IiwidXNlcl9pZCI6NH0.OYjM3XGq9rw9jyhKTiGR-rf0I-ZkjgMDm4Qn3ppzK8o"
  }
  ```

- | Parameter | Description |
  | --------- | ----------- |
  | username  | 用户名      |
  | password  | 密码        |

---

**POST** ```/api/token/refresh/```

- **根据 refresh token 获取新的 access token**

- **返回值为 access token 与 refresh token**

  ```json
  {
      "access": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ0b2tlbl90eXBlIjoiYWNjZXNzIiwiZXhwIjoxNjk5MTA4MDM2LCJpYXQiOjE2OTkxMDQ0MzYsImp0aSI6IjVjMTdhNzlmZDczZjQ4MDZhMzA0MjljYmZkYmY1NWQ1IiwidXNlcl9pZCI6NH0.OYjM3XGq9rw9jyhKTiGR-rf0I-ZkjgMDm4Qn3ppzK8o"
  }
  ```
  
- | Parameter | Description   |
  | --------- | ------------- |
  | refresh   | refresh token |

---

**POST** ```/api/upload```

- **上传文件**

- **返回值为上传的文件信息**

  ```json
  {
      "id": 6,
      "filename": "m",
      "file": "http://127.0.0.1:8000/uploads/2023/11/05/m_suNfjer.cpp"
  }
  ```

- | Parameter | Description |
  | --------- | ----------- |
  | filename  | filename    |
  | file      | 上传的文件  |

---

**GET** ```/api/download/<int:pk>```

- **下载文件**
- **返回 id=pk 的文件**

---

**GET** ```/api/submissions```

- **返回提交列表**

  ```json
  [
      {
          "id": 1,
          "status": "Judging",
          "score": 0.0,
          "user": 1,
          "contest": 1,
          "file": 1
      }
  ]
  ```

---

**GET** ```/api/submissions/<int:pk>```

- **返回 id=pk 的提交信息**

  ```json
  {
      "id": 1,
      "status": "Judging",
      "score": 0.0,
      "user": 1,
      "contest": 1,
      "file": 1
  }
  ```

---

**PATCH** ```/api/submissions/<int:pk>```

- **修改 id=pk 的提交信息**

  ```json
  {
      "id": 1,
      "status": "Judging",
      "score": 0.0,
      "user": 1,
      "contest": 1,
      "file": 1
  }
  ```

- | Parameter | Description |
  | --------- | ----------- |
  | *status   | 评测状态    |
  | *score    | 评测分数    |

  (*为可选参数)

---

