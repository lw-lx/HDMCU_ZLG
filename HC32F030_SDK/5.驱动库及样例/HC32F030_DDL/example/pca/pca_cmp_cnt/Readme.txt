================================================================================
                                样例使用说明
================================================================================
版本历史 
日期        版本    负责人         IAR     MDK   描述
2018-05-03  0.1     Husj           7.70    5.16  first version
================================================================================
功能描述
================================================================================
本样例主要展示PCA的比较计数功能。
说明：
本样例对展示PCA的比较计数功能，当计数值达到设定的比较值时，产生中断。
当计数值达到周期值时，溢出并重新从0开始计数（溢出标志置1）。

================================================================================
测试环境
================================================================================
测试用板:
---------------------
HC32F030_STK

辅助工具:
---------------------
无

辅助软件:
---------------------
无。

================================================================================
使用步骤
================================================================================
1）打开工程并重新编译；
2）启动IDE的下载和调试功能；
3）在PCA2的中断处设置断点；
4）运行；
5）程序到断点处停止，观察计数寄存器的值，比设置的比较值稍大
6）可以继续单步执行至设置的周期值产生溢出，或者全速执行至下一个中断进断点。


================================================================================
注意
================================================================================

================================================================================
