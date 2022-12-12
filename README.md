# ESP8266-PC-Monitor
本项目是由桌面WiFi天气时钟小电视成品更改成的电脑硬件监控小电视。
![IMG_20221204_190818.jpg](https://s2.loli.net/2022/12/12/bBiKhUMlksqDvPr.jpg)

代码由PlatformIO插件编译并烧录，需根据个人环境更改WiFi信息

![image.png](https://s2.loli.net/2022/12/12/RvJSACUVNt2BQiG.png)

小电视需搭配[LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor)使用，在软件导航栏选择`Options`，`Remove Web Server`，`Run`，运行服务。在`Port`选项中查看地址及端口，软件需设为开机自启动。![image.png](https://s2.loli.net/2022/12/12/wC1JMOHGXQ3k6Ku.png)

代码仍有部分bug，懒得修了。[项目来源](https://github.com/lsmlive/ESP8266-PC-Monitor)

