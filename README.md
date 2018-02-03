# 飞思卡尔智能车

## 代码描述

这是一份关于大学生飞思卡尔智能车比赛的代码，参加的比赛日期和类型是2014年东北赛区光电组。取得了东北赛区第13名的成绩。准备比赛过程中非常的不顺利，希望代码对部分人有点用。这种比赛准备周期比较长，付出的劳动比较多，如果不能取得成绩是很可惜的事情，毕竟很多大学生参加这个比赛就是为了获奖希望以后找工作顺利点。我现在已经不从事电子行业了，目前打算发展的行业是iOS和前端。还有些朋友曾经问我代码的事情，就放github上吧，CSDN那个要积分的就留在那里吧。

## 比赛规则

### 初赛
 每位比赛选手要在闭合的跑道上面逐次按照抽签顺序，完成如下要求：
 - 车辆要求在放置在贴有黑色胶带的边界的跑道上面进行比赛，发车阶段要求必须在按下开关后至少静止3秒后，车辆自动出发。
 - 车辆要根据赛道的信息自动在包含直道，弯道，十字路口元素的赛道上面自动转向行驶。
 - 在完成一圈后车辆应该自动检测到起跑线做出停车操作。
 - 比赛过程中不允许认为干涉车辆的运行，不允许更改车辆的结构和重新下载代码。

### 决赛
 - 在初赛的基础上面增加了障碍物和坡道以及人字道路
 
### 使用的硬件信息
  - 主控制器:MC9X12XS128 16位单片机
  - 摄像头传感器:TSL1401 CCD传感器，大光圈无畸变广角镜头，放在直道上，左边黑线像素点的左边35左右，右边黑线像素点95左右。比赛时候带偏振片。
  - 舵机型号:DM-S0160
  - 软件开发环境：CodeWarrior
  - 控制算法：PID调节

## 代码思路
http://blog.csdn.net/a158337/article/details/38925757

就不复制粘贴了。
## 备注
车的速度与很多因素有关比如机械，赛道上的灰尘，以及光线不均匀等等，本程序可以参考，但是更多的需要自己根据自己的车调试，硬件程序不可直接移植。
