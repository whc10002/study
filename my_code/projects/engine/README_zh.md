#C++后台开发指南

##系统环境
C++后台程序主要在linux服务器上运行，目前为CentOS，与大家的开发用虚拟机环境接近。

##项目结构

##模块结构
在 C++ 项目中，没有像 maven 这样的依赖管理系统，我们使用Blade来管理模块间的依赖。

以search_and_rec中的base子模块为例，目录结构如下：

    base/
    ├── BUILD
    ├── internal
    └── public

我们对C++的开发秉承模块化的思路，所有source文件和不需要暴露给其它模块的头文件，都放到internal文件夹中。其它提供给外部使用的头文件，则放到public子文件夹中。相应地，一个模块需要依赖于其它模块时，应该依赖public中的头文件，而不是internal中的。

##Blade构建系统
我们使用开源工具 blade 来编译 C++ 项目。

使用blade需要安装 scons，并推荐安装ccache提升编译速度。

CENTOS:

    sudo yum install scons g++ ccache

UBUNTU:

    sudo apt-get install scons g++ ccache

blade已经集成到 search_and_rec 的 tools/typhoon-blade-master 路径下，使用方法：

    ./tools/typhoon-blade-master/blade build base

更详细的使用说明请参考
[Blade文档](https://github.com/chen3feng/typhoon-blade/blob/master/README.md)

##Code Style
我们的code style请参考: 
[Google C++ Code Style](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml)
([中文版](http://zh-google-styleguide.readthedocs.org/en/latest/google-cpp-styleguide/))

[cpplint](http://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py)
这个脚本可以检查是否符合code style，已经集成到blade中，编译前会自动检查。

虽然有错误不会中断编译，但请在提交前修改完所有的style问题。

##C++11

##gperftools
我们的C++项目中缺省集成了google的gperftools，好处有：

* 使用其中的tc_malloc管理内存，效率更高
* 使用其中的heap_check，可以自动在unittest中检查内存泄露
* 使用其中的heap profile / cpu profile，能够了解程序中的CPU/Memory的使用情况，便于优化

为了做到2和3,需要使用 third_party/gperftools/pprof，这是已经编译好的工具，需要在执行blade的终端下设置 PPROF_PATH 到相应的路径，才能正确使用。

为了能够使用pprof进行CPU和内存profiling, 还需要安装一些工具，在ubuntu上，需要安装：

CENTOS:
    sudo yum install graphviz gv

UBUNTU:
    sudo apt-get install graphviz gv

