# 高级程序设计II 大实验作业二

**2019201407农钧翔 & 2019201409于倬浩 & 2019201420汪元森**

[toc]

## 实验环境

操作系统：Ubuntu 20.04 LTS

```shell
$ clang --version                 
clang version 11.0.0 (https://github.com/llvm/llvm-project.git ca376782ff8649d1a5405123f06a742e0e94b701)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

## 数据结构

在计算数据依赖关系时，新定义了Use类，对于每一个Value维护对应的Instruction的集合，表示数据使用关系。Use类内维护了一个键值为Value*，存储值为set指针的map。提供了createUse()和deleteUse()两个接口，分别表示创建/删除某个使用关系。





## 实现算法





## 运行结果

