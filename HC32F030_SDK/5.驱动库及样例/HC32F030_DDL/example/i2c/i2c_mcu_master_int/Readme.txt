﻿================================================================================
                                样例使用说明
================================================================================
版本历史 
日期        版本    负责人         IAR     MDK   描述
2018-05-03  0.1     CJ             7.70    5.16 first version
================================================================================
功能描述
================================================================================
本样例主要涉及I2C读写主机功能

说明：
配置PB08、PB09为SCL SDA 
内部24M pclk时钟
使能波特率1M
I2C模块使能
主机功能
================================================================================
测试环境
================================================================================
测试用板:
HC32F030_STK
---------------------
（需要填）

辅助工具:
---------------------

辅助软件:
---------------------
串口调试工具
================================================================================
使用步骤
1、PB8、PB09端口与从机SCL/SDA正确连接
2、重新编译此代码
3、烧录代码
4、数据正常从机
5、读出数据仿真查看是否与写入相同
================================================================================

================================================================================
注意：
================================================================================
数据正常连接，外部要接上拉电阻
================================================================================
