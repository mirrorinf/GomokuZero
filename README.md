# GomokuZero
C语言课作业-五子棋

## 说明
原本想要适配macOS、Linux、Windows三平台，但是因为难以解决的编码问题放弃了Windows的适配。推测是因为msvc使用了非utf-8编码导致编译失败。但是...这都要全面小康了为什么通用计算机还要用Unicode(UTF-8)以外的编码啊啊啊啊啊啊啊啊啊啊啊！

在
Apple clang version 11.0.0 (clang-1100.0.33.12)
Darwin Kernel Version 19.0.0: Thu Oct 17 16:17:15 PDT 2019; root:xnu-6153.41.3~29/RELEASE_X86_64 x86_64
下测试通过。

Todo: POSIX计时API和gcc工具链

## 有哪些没做好...

感觉AlphaBeta搜索的代码写得不怎么样...应该是要写多线程的，但是被Grand Central Dispatch惯坏之后pthread都不怎么会用了（捂脸 其实既然不适配别的平台的话clang -fblocks也是可以GCD的...所以说就是懒）...（以及并行算法确实还要难写一些）

整个工程的设计模式不够清晰，不同部分的代码之间耦合度还是过高...有一部分是C的原因，没有面向对象很难分离代码；还有一部分是为了不花太多时间所以直接Hack了...

跨平台是本来想做的但是实在力有未逮...于是CMake和那一堆宏定义基本就没啥用了，好吧Darwin和一般的POSIX平台还是区分了一下。

寒假里用swift重写一下吧...
