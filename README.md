# Geant4-Resource
---
整理初学Geant4时搜集的一些资料，分享给需要的同学。
有些资料可能没什么用处，但一并放过来了。
**希望有更多的小伙伴一起来分享、完善G4的学习资料！**

## G4 Virtual Machine
---
我想很多人像我一样，初学G4时从没用过Linxu系统，写程序停留在边写边百度的水平，那么建议先不要自己去安装配置Gean4，可以下载一个已经配置好的虚拟机。
* [Geant4 @ IN2P3 - Geant4 Virtual Machine](https://geant4.lp2ib.in2p3.fr/)【首推！】
  * 法国国家核与粒子物理研究所（IN2P3）针对Geant4发布的虚拟机，基于RockyLinux 9系统和VMware虚拟机软件；
  * 持续更新，包含最新版本的Geant4程序（with all sets of data files），同时Linux系统也会更新；
  * 同时包含了ROOT、QT6、Python(with Jupyter, Matplotlib, NumPy, SciPy and Sphinx)、CLHEP、Jupyter、gnuplot、Tex Live等实用工具。
* [vGATE](http://www.opengatecollaboration.org/)
  * 针对GATE发布的虚拟机，基于Ubuntu系统和Virtual Box虚拟机软件；
  * 依GATE版本持续更新
  * 包含新版本的Geant4和ROOT等程序
* [Geant4CernVM](https://twiki.cern.ch/twiki/bin/view/Geant4/Geant4CernVM)
CERNVM is a CERN project to build appliances for HEP applications. A customised appliance has been prepared with the latest version of Geant4 and chosen additional software. The virtual machine contains: A Linux operating system A (minimal) GNOME desktop environment A pre-installed release of Geant4 9.3 A pre-installed release of ROOT 5.26
没用过，感觉版本比较老了。

## G4 Official Website
---
* G4主页：[http://geant4.web.cern.ch/](http://geant4.web.cern.ch/)
* 源代码查询：https://geant4.kek.jp/LXR/ ，
https://geant4.kek.jp/Reference/10.05.p01/

## G4 Official Documents
---
官网Geant4手册（可在上面文件夹中直接下载）

查看官方手册和示例是最全面、最高效的Geant4学习途径
* Geant4 user guide for application developer
* Geant4中文手册
* Geant4 User's Guide for Toolkit Developers
* Physics Lists Guide
* Physics Reference Manual
## Forums、WeChat_Subscriptions
---
* [Geant4 User Forum](https://geant4-forum.web.cern.ch/)，官方论坛，有问题可以去搜索或提问，对Geant4学习非常有帮助！
* [核技术论坛](http://www.hejishult.cn/forum.php)
* [Geant4中国用户群](https://geant4cn.livejournal.com/)
* QQ学习交流群
## Personal Blog
---
网上关于Geant4学习的个人总结和感想，仅供参考（上面部分文件来自网络博客）
* [心蛛的 GEANT4 笔记](https://exaos.github.io/topics/physics-simulation/cern-geant4.html)
* [cnscott的博客](http://cnscott.blog.163.com/blog/#m=0&t=3&c=geant4)
* http://www.lofter.com/tag/Geant4
* [小帕](http://littlepascal.lofter.com/?page=1&t=-1457572101347)
* [jack_chf的geant4不完全学习指南](https://blog.csdn.net/jack_chf/article/category/9162227)
* [知乎_人芳觅](https://www.zhihu.com/people/wan-jie-35-59/posts)
* ...
欢迎补充有参考意义的G4经验分享博文！
## Courses、Lectures
---
（可在上面文件夹中下载）
* G4 Japanese Tutorial for Detector Simulation 2017
* 清华大学杨振伟等老师的课件，包含Linux、ROOT、Geant4的内容[~/G4_Lectures_THU]
* Geant4系列讲座（出处未知，缺少第一讲）[~/G4_Lectures]
* IN2P3 geant4 course 2011，IN2P3出品必属精品，geant4.9.6版本教程     
* IN2P3 and PHENIICS Geant4 Tutorial，教程加实例                                                       
* ...

## G4 Tutorial
---
欢迎大神分享自己的Geant4项目实例，G4小白们当不胜感激！
### 1. 基于CADMesh的CAD模型导入方法
* 基本方法：
  * solidworks绘制模型，保存为STEP格式；
  * 用FreeCAD进行格式转换：打开step格式文件 -- Mesh Design -- 从形体创建网格 -- 剖分网格（standard模式，表面切割最小值0.01 mm，角度分割最小值1度）-- 在工程项目下右键 -- 导出网格 -- 输出为ASCII stl格式 -- 打开stl文件，删除第一行的（Meshed）及前面的空格；
  * 或者直接用FreeCAD绘图并剖分网格后存为stl格式。
  * 通过CADMesh读取stl文件并接入Geant4。
* 下载CADMesh
https://github.com/christopherpoole/CADMesh
将根目录下新版本的CADMesh.hh文件替换掉include目录下的文件
* 安装FreeCAD
``` bash
sudo add-apt-repository ppa:freecad-maintainers/freecad-stable
sudo apt-get update
sudo apt install freecad
sudo apt install gmsh      # 网格划分
sudo apt install calculix-ccx     # 有限元求解
```
* 将CADMesh.hh文件复制到Geant4项目的include目录下
* 将模型的stl文件放到Geant4项目的根目录下，并修改CMakeList.txt文件，添加stl文件到脚本列表中
* 编写DetectorConstruction.cc文件，引入头文件CADMesh.hh，并添加构造实体：
```cpp
// CADMesh :: STL
  auto test = CADMesh::TessellatedMesh::FromSTL("./test.stl");
  test -> SetScale(1);  // 模型比例
  test -> SetOffset(G4ThreeVector(-25,25,-10)); // 模型相对坐标原点的偏置，单位：mm
  auto test_solid = test->GetSolid();
  auto logictest = new G4LogicalVolume(test_solid,kapton,"logictest",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(),logictest,"test",fLWorld,false,0);
```
* 注意事项：
  * solidworks剖分网格需要建立算例，simulation工具需要solidworks的产品序列号。且solidworks只能保存二进制格式的stl文件，无法用CADMesh直接读取。
  * solidworks模型（零件或装配体）一定要将坐标原点放到感兴趣的坐标轴上，并且知道原点的空间坐标（x，y，z），模型导入到FreeCAD后坐标信息不变。
  * FreeCAD保存为ASCII格式的stl文件后要更改solid的名称（第一行：solid name），且name不能包含除字母外的字符（如数字、下划线、空格等），否则会报错。
  * 需要设置模型相对坐标原点的偏置，若原点坐标为（x，y，z），则偏置坐标为（-x，-y，-z）
---
# MCNP-resources
MCNP的一些基础资料：中英文教程、源描述、材料库、实例等
