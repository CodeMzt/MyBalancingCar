# MyBalancingCar
***基于STM32实现的PID平衡小车。个人学习记录。***

## 硬件驱动
- OLED显示
- 电机驱动
- 超声波测距
- 电机驱动
- 编码器测速
- 6轴传感器
## 工作原理
### 主控
使用stm32f103c8t6作为主控芯片
### 平衡算法
使用串级PID控制算法来实现平衡
## 调试过程中的问题
1. I2C传输频率过慢，应将操作传输线间隔改为3us或者更短(原10us)
2. MPU采样率与PID控制器输出频率不协调，应该统一为100Hz，不得低于50Hz
3. 由于dmp姿态解算有一定耗时导致2.问题仍存在，应该适当更改PID控制周期，这里改为了80ms，理论上改为100ms+比如150ms也合适，或者增大采样频率。
4. 当速度坏KP达到临界后，需要返回来增大直立环KD
## 参考资料
- [江协科技STM32标准库开发](https://www.bilibili.com/video/BV1th411z7sn/?spm_id_from=333.1387.favlist.content.click&vd_source=da485681aab603b38b1daa0ea0829424)
- [草履虫都能学会的STM32平衡小车教程](https://www.bilibili.com/video/BV1Gc411v73h/?spm_id_from=333.1387.favlist.content.click&vd_source=da485681aab603b38b1daa0ea0829424)
- [mpu6050在stm32f1中使用dmp]()
