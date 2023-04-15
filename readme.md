# 简介

这是一个LAC文法编译器项目，不过可以基本了解编译器前端的大致功能。这个项目的来源是SPEIT的编译课程项目，感谢Alain老师。

# 一个简单的尝试

使用`make lacbuild` 来编译toy编译器，使用`make lacrun` 来编译运行需要的运行时。

然后就可以使用lacbuild来编译lac文法的程序文件并用lacrun来运行编译好的文件了。

**示例**

./lacbuild ./examples/factorielle.lac ./project.out
./lacrun ./project.out

# 未来

可能考虑下数据压缩的方法来压缩编译的结果，还有接入LLVM。
但我大概率不会在这个项目上花时间了。
