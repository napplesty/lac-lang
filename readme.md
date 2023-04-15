#简介

这是一个玩具编译器项目，开发时间只有四天(虽然是爆肝)，不过可以基本了解编译器前端的大致功能。

#一个简单的尝试

使用`make lacbuild` 来编译toy编译器，使用`make lacrun` 来编译运行需要的虚拟机环境。

然后就可以使用lacbuild来编译lac文法的程序文件并用lacrun来运行编译好的文件了。

**示例**

./lacbuild ./examples/factorielle.lac ./project.out

./lacrun ./project.out

