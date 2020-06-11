# 高级程序设计II 大作业实验报告

项目地址：[GitHub](https://github.com/zhuohaoyu/TuringAdvancedProgramming19B)

## 代码贡献

+ 农钧翔 2019201407：
    + 构建``Flatten``的数据依赖共享图（Task2）
    + 生成关联规则及置信度（Task3）
    + 库函数对接与格式化输出（Task4）

+ 于倬浩 2019201409：
    + 构建``Transaction``图、规范化指令（Task2）
    + 实现``Apriori``算法生成项集（Task3）
    + 图形化界面的实现 & 与动态链接库的输出对接（Task5）

+ 汪元森 2019201420：
    + 实现迭代数据流算法，构建原始数据依赖图（Task2）
    + 实现``calcu_support``计算项集的支持度（Task3）
    + 缺陷检测算法的实现（Task4）

## 实验环境

操作系统：``Ubuntu 20.04 LTS``

```shell
$ clang --version                 
clang version 10.0.0
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```



## 简单介绍

本次UI的实现采用了QT5，实现了一个简单的多窗口程序，主窗口初始状况下如下图所示：

<img src="README.assets/Screenshot from 2020-06-11 13-35-19.png" alt="Screenshot from 2020-06-11 13-35-19" style="zoom:80%;" />

支持指定需要检测的源文件位置，指定``MFS``、``MIS``、``MCF``的值。

对于三个指定值的输入，选用了``spinbox``，以便控制输入值的上下界、步长等。

选择文件的``PushButton``通过调用``QFileDialog``，调用系统选择文件的窗口，较为美观。文件选择成功后，调用``QMessageBox``给出反馈。

<img src="README.assets/Screenshot from 2020-06-11 13-36-13.png" alt="Screenshot from 2020-06-11 13-36-13" style="zoom: 67%;" />

<img src="README.assets/Screenshot from 2020-06-11 13-36-37.png" alt="Screenshot from 2020-06-11 13-36-37" style="zoom:67%;" />



选定源文件，指定参数后，单击"``Run``"即可运行监测。运行过程中，所有设定参数的按钮均被禁用。

<img src="README.assets/Screenshot from 2020-06-11 13-37-30.png" alt="Screenshot from 2020-06-11 13-37-30" style="zoom:67%;" />



动态链接库``MyPass.so``会将调试信息输出到``stderr``中，并实时更新至"``Debug Output``"中。对于结果和项集的传递，``MyPass.so``将这些信息输出至``stdout``，使用不同的token区分输出类型，``MyPass``在成功执行结束后，在``stdout``中返回一个token``"$$SUCCESS"``，UI在读取到这个token后，才会启用右侧的三个按钮。可以保证在编译错误/运行错误/``MyPass``运行结束前，阻止用户点击右侧的按钮。同时在文件更新后，亦会禁用展示结果的按钮。

单击"``Show ...``"，会打开一个新窗口，展示指定的数据。对于项集和规则，使用文本列表的形式展示，每行一个规则或项集，使用等宽字体展示每条规则。对于缺陷检测结果，以只读纯文本的形式展示，灵活性更好。

<img src="README.assets/Screenshot from 2020-06-11 13-46-34-1591858622161.png" alt="Screenshot from 2020-06-11 13-46-34" style="zoom:80%;" />



最后，展示一张效果图：

<img src="README.assets/Screenshot from 2020-06-11 13-50-15.png" alt="Screenshot from 2020-06-11 13-50-15" style="zoom: 80%;" />
