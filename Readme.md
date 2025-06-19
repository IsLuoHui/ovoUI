# ovoUI
一个简易的OLED菜单，其中数据结构和对象的定义非常易于理解，对于新手来说可以很方便的移植到其他平台或者是用于其他型号屏幕，其中OLED显示库也可以单独使用并进行二次开发。

已经支持的屏幕通信方式：
- 软件SPI
- 软件I2C

仓库中为基于STM32F103C8T6的EIDE工程文件，使用了两个定时器，一个用于显示缓存刷新，一个用于EC11编码器扫描，使用OLED型号为SSD1306并使用SPI通信。

![IMG](/ReadMe/template1.png)
![IMG](/ReadMe/template2.png)

> [!IMPORTANT]
> 项目仍在施工中……