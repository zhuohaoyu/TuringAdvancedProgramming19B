--

以下列出程序大致流程框架，以及我们需要完成的任务

## Project
  - Mypass.cpp
  - SimpleDataDependenceGraph.cpp
  - SimpleDataDependenceGraph.h
  - Tests
      - (Testfiles)

### SimpleDataDependenceGraph.h
  - Class: SDDGnode
      - 这个类表示关系图中的节点

      - 31~33 行的 3 个变量是在图中的信息

      - 38~41 行的 4 个图相关的成员函数需要自己实现
          - 这 4 个函数会在 SimpleDataDependenceGraph.cpp 的 dotify (可视化相关函数) 被调用
          - 除此以外也可以在别的部分自行调用

  - Class: SDDG
      - 这个类表示整个关系图

      - mFunc (Line 47): 关系图所在的函数对应的 Function 变量
          - 会在 Dotify 和 buildSDDG 函数被调用

      - mNodes (Line 48): 一个 map，表示所有 Instrution* (指令)到 SDDGnode* 的映射

      - mInterestingNodes (Line 49): 一个 map，表示去除无关指令，剩余的 Instrution* (指令)到 SDDGnode* 的映射
          - 以上两个函数都会在 dotify 被调用

      - mShares (Line 50): 一个 set，存储所有 share 关系
          - SimpleDataDependenceGraph.cpp 包含了绝大多数的建立 Share (共享)关系图的代码，我们并不需要对 mShare 做任何操作

      - share(...) (Line 51)
          - 在 SimpleDataDependenceGraph.cpp 给出了实现，不需要操作

      - buildSDDG(...) (Line 57): 功能见文档注释
          - 在 SimpleDataDependenceGraph.cpp 给出了 Share 部分的大部分实现
          - 需要补全的主要是 Dependence (依赖关系) 部分

      - flattenSDDG(...) (Line 59): 功能见文档注释
          - 课件上没有关于这个的详细内容，我打算再问一下老师

    如果没有特殊说明，上面出现的变量都要自行初始化，函数也要自己实现或者补全。

### SimpleDataDependenceGraph.cpp
  - Class: Definition (Line 121~158)
      - Definition 用于计算 Dependence 关系图(也就是我们要自己实现的建图部分) 的 IN 和 OUT
      - 关于概念的更多细节请回顾课件

  - Class: Use (Line 161)
      - 同理，但是要求自行实现

  - Class: ShareDefintion, ShareUse (Line 166~301)
      - 用于 Share 部分，无需操作

  - Function: FindOrCreate (Line 303~318)
      - 提供了一个函数模板
      - 在 Share 部分已经被使用，具体见源码 (Ctrl-F查询)
      - 可以参考他的使用方法，在 Dependence 部分以类似的方式调用

  - Function: MergeTwoMaps (Line 326)
      - 自行补全 map 合并的函数模板。

  - Function: SDDG::buildSDDG (Line 336)
      - 重点补全部分，见代码内中文注释
      - 个人感觉最后的内存释放是不必要的，因为数据范围非常小

    除了上述提到的需要补全的部分，之前提到的也要写在这个文件里

### Mypass.cpp
  - RunOnModule() 函数是整个Project的入口
  - 本 cpp 文件的功能主要是对上面的文件中声明的函数进行调用，以获得最终的 dot 文件

  如果有必要可以使用Task1的编译/运行脚本