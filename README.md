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
  * 同时包含了ROOT、QT6、Python(with Jupyter, Matplotlib, NumPy, SciPy and Sphinx)、CLHEP、Jupyter、gnuplot、Tex Live等实用工具；
  * 建议定期保存虚拟机快照（VMWare的快照功能十分好用），这样如遇系统崩溃可以一键恢复。
* [vGATE](http://www.opengatecollaboration.org/)
  * 针对GATE发布的虚拟机，基于Ubuntu系统和Virtual Box虚拟机软件；
  * 依GATE版本持续更新
  * 包含新版本的Geant4和ROOT等程序

## G4 Official Website
---
* G4主页：[http://geant4.web.cern.ch/](http://geant4.web.cern.ch/)
* 源代码查询：https://geant4.kek.jp/LXR/ ，
https://geant4.kek.jp/Reference/10.05.p01/
* [Geant4 User Forum](https://geant4-forum.web.cern.ch/)，官方论坛，有问题可以去搜索或提问，对Geant4学习非常有帮助！

## G4 Official Documents
---
官网Geant4手册:
查看官方手册和示例是最全面、最高效的Geant4学习途径
* Geant4 user guide for application developer
* Geant4中文手册
* Geant4 User's Guide for Toolkit Developers
* Physics Lists Guide（模拟计算一定要对物理过程有十分清晰的认识）
* Physics Reference Manual

## Q & A
---
* [Geant4 User Forum](https://geant4-forum.web.cern.ch/)，官方论坛，可得到专业问题回答
* AI大模型，如deepseek、grok、Gemini，已是程序开发必备的工具，实测很多问题可省去查手册的时间，甚至可直接生成一整个可用的G4项目
* ...
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
### 一. 基于CADMesh的CAD模型导入方法
##### 1. 模型绘制与网格剖分：
~方法1：solidworks绘制模型+freecad剖分网格（此方法已过失，仅针对老版本solidworks)~
  * solidworks绘制模型，保存为STEP格式；
  * solidworks剖分网格需要建立算例，simulation工具需要solidworks的产品序列号。且solidworks只能保存二进制格式的stl文件，无法用CADMesh直接读取。（仅针对老版本solidworks）
  * 用FreeCAD进行格式转换：打开step格式文件 -- Mesh Design -- 从形体创建网格 -- 剖分网格（standard模式，表面切割最小值0.01 mm，角度分割最小值1度）-- 在工程项目下右键 -- 导出网格 -- 输出为ASCII stl格式 -- 打开stl文件，删除第一行的（Meshed）及前面的空格；
  * FreeCAD保存为ASCII格式的stl文件后要更改solid的名称（第一行：solid name），且name不能包含除字母外的字符（如数字、下划线、空格等），否则会报错。
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

---
方法2：solidworks绘制模型并转换为STL网格实体（亲测soliworks2024可直接将零件转换为stl格式网格实体）
* 绘制各部分零件，注意一个零件只能包含一种材料。
* 注意所有零件应该有一个共同的原点坐标，也即各零件的相对坐标位置是固定的。因此最好的做法是待所有零件完成统一装配后再转换网格实体。
* 转换网格实体时保持零件的绝对坐标不变
* 网格实体选择为STL（ASCII）格式
* 可设置网格剖分精度，即角度误差和长度误差
* STL文件名即solid名称，可随意设置，但应打开STL文件确认solid name合法（name仅由字母组成）
---
##### 2. geant4读取与构建实体
* 将CADMesh.hh文件复制到Geant4项目的include目录下
* 将模型的stl文件放到指定目录
* 编写DetectorConstruction.cc文件，引入头文件CADMesh.hh，并添加构造实体：
```cpp
// CADMesh :: STL
  auto test = CADMesh::TessellatedMesh::FromSTL("./test.stl"); //指定stl文件
  test -> SetScale(1);  // 模型比例
  test -> SetOffset(G4ThreeVector(-25,25,-10)); // 模型相对坐标原点的偏置
  auto test_solid = test->GetSolid();
```
* stl模型导入后绝对坐标不变
* 需要设置模型相对坐标原点的偏置，若原点坐标为（x，y，z），则偏置坐标为（-x，-y，-z）
---
# MCNP-resources
MCNP的一些基础资料：中英文教程、源描述、材料库、实例等
