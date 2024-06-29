# 基于opencv的人脸识别系统



- 项目简介

  通过opencv，seetaface，算法实现人脸识别，并将识别数据存放到数据库中

  

  考勤端：1.实现人脸数据的采集->发送到服务端

  ​			  2.人员信息的展示

  服务端：1.对考勤端发送的数据进行人脸识别->查询数据库是否有相应的信息->将查询到人脸信息对应的人员信息发送到考勤端

  ​              2.对人员信息进行维护，（对人员信息-CRUD）

  ​			  3.对考勤信息的记录



- 运行环境：

  windows：qt5.14.2，opencv4.5.2，opencv_contrib-4.5.2，seetface6（[GitHub - SeetaFace6Open/index](https://github.com/SeetaFace6Open/index)）

- 数据表

| 员工信息表     | employee |         |              |             |           |              |              |
| -------------- | -------- | ------- | ------------ | ----------- | --------- | ------------ | ------------ |
| 编号employeeID | 姓名name | 性别sex | 生日birthday | 地址address | 电话phone | 人脸IdfaceID | 头像headfile |
|                |          |         |              |             |           |              |              |
|                |          |         |              |             |           |              |              |
|                |          |         |              |             |           |              |              |
|                |          |         |              |             |           |              |              |

| 考勤表           | attendance         |                 |           |
| ---------------- | ------------------ | --------------- | --------- |
| 编号attendanceID | 员工编号employeeID | 考勤时间attTime | 备注notes |
|                  |                    |                 |           |
|                  |                    |                 |           |
|                  |                    |                 |           |



部分运行结果：

服务端：

![image-20240629194745937](C:\Users\hxl\AppData\Roaming\Typora\typora-user-images\image-20240629194745937.png)

考勤端：

![image-20240629195033615](C:\Users\hxl\AppData\Roaming\Typora\typora-user-images\image-20240629195033615.png)
