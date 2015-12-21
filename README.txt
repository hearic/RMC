
    RRRRRRRR       MM         MM        CCCCCCCC 
    RRRRRRRRR      MMM       MMM      CCCCCCCCC  
    RRR    RRR     MMMM     MMMM     CCC         
    RRR    RR      MMMMM   MMMMM    CCC          
    RRRRRRRR       MM MMM MMM MM    CCC          
    RRR   RRR      MM  MMMMM  MM    CCC          
    RRR    RRR     MM   MMM   MM     CCCCCCCCCC  
    RRR     RRR   MMM         MMM      CCCCCCCCC 

Copyrights Reserved by Reactor Engineering Analysis Laboratory(REAL)


1. General Introduction
2. Compiling




1. General Introduction
=======================

The code RMC intends to solve reactor analysis problems, and is able 
to deal with complex geometry, using continuous energy point cross 
sections of different materials and temperatures.

RMC can deal with criticality and burnup calculation, supporting some 
advanced capabilities, such as complex geometry(including plotting), 
high performance parallelism,  source convergence acceleration, fast 
and massive tallies of multiple physical parameters and so on. 
Many other functions are under developing, such as fix source 
calculation and kinetics simulation, photon simulation, on-the-fly 
cross sections processing with temperature, high efficiency searching
methods, perturbation and sensitivity analysis, N-TH coupling, 
integrated parallelism for memory optimization, and so on.

Contact REAL for more information.

Tel:     8610-62785506
Fax:     8610-62782658
Address: LIUQING Building Room 901, Tsinghua University,
         Beijing, 100084, P.R. China
Website：http://reallab.ep.tsinghua.edu.cn




2. Compiling
============

Source codes of RMC can be compiled under platform of windows and 
linux with MPI parallel environment.


(a) Windows platform with visual studio environment
    
    It is recommended to use visual studio 2010 or higher versions.
    
    (1) Open the solution file RMC.sln or project file RMC.vcxproj.
    
    (2) Modify MPI parallel configurations.
        "Projects Properties" -> "Visual C++ Directories" 
        "Include files", Add your MPI include path, such as 
                "C:\Program Files(x86)\MPICH2\include"
        "Library files", Add your MPI include path, such as
                "C:\Program Files (x86)\MPICH2\lib"
    
    (3) Compile(F7)



(b) Linux platform

    Compiling method in linux system(Red Hat Enterprise recommended) is provided
    using cmake, which should be installed in advance, version 2.8 or
    higher.
    Folder RMC_make_linux contains the necessary instructions file 
    CMakeLists.txt and two empty directories, "src" for source files,
     and "build" for external compiling. 

    Before actual compiling, check the environment as follows:
    (i)   Intel compiler icc and icpc is the default compiler, as instructed
          in CMakeLists.txt:
                   SET(CMAKE_CXX_FLAGS "-cxx=icpc -std...")
          If you want to use other compilers, "cxx" option should be altered, using 
                   SET(CMAKE_CXX_FLAGS "-cxx=g++ -std...")
          for gnu compiler, for example.

    (ii)  MPI library should be installed and add to the environment variables.
          Use "which mpicxx" or "mpicxx -v" to check the directory or version of MPI. 
          
    (iii) 32-bit or 64-bit executable file will be generated according
          to system condition.
          
    (iv)  Compiling option can be set in CMakeLists.txt in case you don't want use
          the default ones.
          
          
    Now do the following three steps to generate RMC:
    
    (1) Compiling library of Depth
    
        · Copy all the source codes of Depth (in folder DEPTH_src ) 
          to the RMC_make_linux/Depth/src directory;
          (Line 5 in DepthMain.cpp 
          "int old_exponent_format = _set_output_format(_TWO_DIGIT_EXPONENT);" 
          should be commentted)
        
        · Enter into RMC_make_linux/Depth/build
        
        · Execute cmake command as follows
                  
                   cmake ..
          (.. means upper directory where CMakeLists.txt locates)
        
        · Makefile and corresponding files are generated in build directory now. 
          Execute make command to generate libDepth.a.
                  
                   make
         
    (2) Compiling libpng
        
        OpenSource library libpng and zlib should be downloaded and compiled. 
        Static library libpng.a, libz.a, and some header files should be 
        copied to the libpng directory.        
        The existing files can be used as well.
        
    (3) Compiling RMC
        
        Just like the compiling of Depth, RMC can be generated using 
        cmake in the build file.
        
        · Copy all the source codes of RMC (in folder RMC_src ) 
          to the RMC_make_linux/RMC/src directory
        
        · Enter into build directory.
            
                   cd RMC_make_linux/RMC/build
                   
        · Execute cmake command.
                  
                   cmake ..
                
        · Execute make command. Executable RMC will be generated then.
           
                   make

				   
				   
* 堆用蒙卡程序RMC编译说明
======================


1. 关于程序源码
---------------
   堆用蒙卡程序采用C++语言开发，源码包括.h和.cpp文件（存放于RMC_src文件夹下）。程序编译需要两个外部静态库：Depth和Libpng，前者为耦合的点燃耗计算库（自主开发，源码存放于Depth_src），后者为画图功能需要的静态库（开源，可以下载更新版）



2. Windows系统下编译
--------------------

a. 环境要求

(1). 安装visual studio（2010或更高版本）
(2). 安装MPI并行环境（建议MPICH2 1.41或更高版本）

b. 编译指令

(1). 进入Source目录，双击打开解决方案 RMC.sln 或工程文件 RMC.vcxproj

(2). 执行“生成解决方案”（F7），在Release文件夹下生成win32可执行文件

注：

编译前，确保MPI并行配置正确：
工程属性->
VC++目录->包含目录，添加MPI include目录，如“C:\Program Files(x86)\MPICH2\include”
VC++目录->库目录，添加MPI 库目录，如“C:\Program Files (x86)\MPICH2\lib”




3. Linux系统下编译
------------------
   
   Linux下利用 cmake 完成编译（自动生成makefile）。 
  

a. 环境要求

(1). 安装cmake软件（2.8或更高版本）

(2). 安装MPI并行环境（建议MPICH2 1.41或更高版本）

(3). 更新编译参数。CMakeLists.txt文件指定编译参数。
     如指定c++编译器（建议使用intel编译器）	 
	   SET(CMAKE_CXX_FLAGS "-cxx=icpc -std ")


b. 编译指令

(1). 拷贝源程序
     
     将Depth和RMC源程序拷贝至各自的src文件夹。
       Depth: Source/DEPTH_src/* 拷贝至 RMC_make_linux/Depth/src/
       RMC: Source/RMC_src/* 拷贝至 RMC_make_linux/RMC/src/

(2). 编译Depth（可跳过）
     
	 * 进入RMC_make_linux目录，打开Depth的一个cpp文件 Depth/src/DepthMain.cpp，注释掉其中的第5行
	 
	   // int old_exponent_format = _set_output_format(_TWO_DIGIT_EXPONENT);
	   
	 * 进入Depth/build目录
	 
	   cd Depth/build/
	   
	 * 执行cmake编译
	 
	   cmake ..
	   (..表示CMakeLists.txt所在路径为上一层目录)
	   
     * 执行make，在Depth目录下生成libDepth.a
	 
	   make

(3). 编译libpng（可跳过）
     libpng目录下包含libpng.a和libz.a静态库已经相应的头文件，无需编译。
	 也可以下载最新版的libpng库。
	 
(4). 编译RMC
     
	 * 进入RMC/build目录，执行cmake编译
	 
	 cd RMC/build/
	 cmake ..
	 
	 * 执行 make，在build目录下生成RMC可执行文件。
	 
	 make
	 
	 
	 
Linux编译说明：

1、编译libDepth.a。
   将所有DEPTH源代码放到Depth/src当中；
   进入 Depth/bulid目录，命令行中执行 “cmake ..”，
   然后执行 “make”，就生成了libDepth.a。
   
2、编译libpng。
   需要下载libpng源码，
   然后将编译好的libpng.a和libz.a放到libpng目录。
   这一部分已经编译好，存在目录中。
   
3、编译RMC。
   将所有RMC源代码放到RMC/src当中；
   进入RMC/build目录，命令行中执行 “cmake ..”，
   然后执行 “make”，就生成了RMC。

   
   
   


Drafter: LIANG, 2013.11.29