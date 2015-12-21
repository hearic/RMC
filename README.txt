
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
Website��http://reallab.ep.tsinghua.edu.cn




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
    
        �� Copy all the source codes of Depth (in folder DEPTH_src ) 
          to the RMC_make_linux/Depth/src directory;
          (Line 5 in DepthMain.cpp 
          "int old_exponent_format = _set_output_format(_TWO_DIGIT_EXPONENT);" 
          should be commentted)
        
        �� Enter into RMC_make_linux/Depth/build
        
        �� Execute cmake command as follows
                  
                   cmake ..
          (.. means upper directory where CMakeLists.txt locates)
        
        �� Makefile and corresponding files are generated in build directory now. 
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
        
        �� Copy all the source codes of RMC (in folder RMC_src ) 
          to the RMC_make_linux/RMC/src directory
        
        �� Enter into build directory.
            
                   cd RMC_make_linux/RMC/build
                   
        �� Execute cmake command.
                  
                   cmake ..
                
        �� Execute make command. Executable RMC will be generated then.
           
                   make

				   
				   
* �����ɿ�����RMC����˵��
======================


1. ���ڳ���Դ��
---------------
   �����ɿ��������C++���Կ�����Դ�����.h��.cpp�ļ��������RMC_src�ļ����£������������Ҫ�����ⲿ��̬�⣺Depth��Libpng��ǰ��Ϊ��ϵĵ�ȼ�ļ���⣨����������Դ������Depth_src��������Ϊ��ͼ������Ҫ�ľ�̬�⣨��Դ���������ظ��°棩



2. Windowsϵͳ�±���
--------------------

a. ����Ҫ��

(1). ��װvisual studio��2010����߰汾��
(2). ��װMPI���л���������MPICH2 1.41����߰汾��

b. ����ָ��

(1). ����SourceĿ¼��˫���򿪽������ RMC.sln �򹤳��ļ� RMC.vcxproj

(2). ִ�С����ɽ����������F7������Release�ļ���������win32��ִ���ļ�

ע��

����ǰ��ȷ��MPI����������ȷ��
��������->
VC++Ŀ¼->����Ŀ¼�����MPI includeĿ¼���硰C:\Program Files(x86)\MPICH2\include��
VC++Ŀ¼->��Ŀ¼�����MPI ��Ŀ¼���硰C:\Program Files (x86)\MPICH2\lib��




3. Linuxϵͳ�±���
------------------
   
   Linux������ cmake ��ɱ��루�Զ�����makefile���� 
  

a. ����Ҫ��

(1). ��װcmake�����2.8����߰汾��

(2). ��װMPI���л���������MPICH2 1.41����߰汾��

(3). ���±��������CMakeLists.txt�ļ�ָ�����������
     ��ָ��c++������������ʹ��intel��������	 
	   SET(CMAKE_CXX_FLAGS "-cxx=icpc -std ")


b. ����ָ��

(1). ����Դ����
     
     ��Depth��RMCԴ���򿽱������Ե�src�ļ��С�
       Depth: Source/DEPTH_src/* ������ RMC_make_linux/Depth/src/
       RMC: Source/RMC_src/* ������ RMC_make_linux/RMC/src/

(2). ����Depth����������
     
	 * ����RMC_make_linuxĿ¼����Depth��һ��cpp�ļ� Depth/src/DepthMain.cpp��ע�͵����еĵ�5��
	 
	   // int old_exponent_format = _set_output_format(_TWO_DIGIT_EXPONENT);
	   
	 * ����Depth/buildĿ¼
	 
	   cd Depth/build/
	   
	 * ִ��cmake����
	 
	   cmake ..
	   (..��ʾCMakeLists.txt����·��Ϊ��һ��Ŀ¼)
	   
     * ִ��make����DepthĿ¼������libDepth.a
	 
	   make

(3). ����libpng����������
     libpngĿ¼�°���libpng.a��libz.a��̬���Ѿ���Ӧ��ͷ�ļ���������롣
	 Ҳ�����������°��libpng�⡣
	 
(4). ����RMC
     
	 * ����RMC/buildĿ¼��ִ��cmake����
	 
	 cd RMC/build/
	 cmake ..
	 
	 * ִ�� make����buildĿ¼������RMC��ִ���ļ���
	 
	 make
	 
	 
	 
Linux����˵����

1������libDepth.a��
   ������DEPTHԴ����ŵ�Depth/src���У�
   ���� Depth/bulidĿ¼����������ִ�� ��cmake ..����
   Ȼ��ִ�� ��make������������libDepth.a��
   
2������libpng��
   ��Ҫ����libpngԴ�룬
   Ȼ�󽫱���õ�libpng.a��libz.a�ŵ�libpngĿ¼��
   ��һ�����Ѿ�����ã�����Ŀ¼�С�
   
3������RMC��
   ������RMCԴ����ŵ�RMC/src���У�
   ����RMC/buildĿ¼����������ִ�� ��cmake ..����
   Ȼ��ִ�� ��make������������RMC��

   
   
   


Drafter: LIANG, 2013.11.29