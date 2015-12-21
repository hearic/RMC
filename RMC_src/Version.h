# if ! defined __VERSION__H__
# define  __VERSION__H__


# define CODE_VERSION "Beta 2.5.7"

#endif ///__VERSION__H__



/* Version Log
   2014/12/12.   Beta 2.5.7.        Fix bugs: negative value in URR ptable, zero power burnup step too many nuclides, generate inpfile error.
   2014/12/12.   Beta 2.5.6.        Adding feature to vary material during burnup (VARY card).
   2014/12/09.   Beta 2.5.5.        Adding feature to read new Depth library(from JEFF).
   2014/11/30.   Beta 2.5.4.        Fix particle enters into wrong cell after reflecting(crossing).
   2014/11/10.   Beta 2.5.3.        Fix zero heat release/power case in burnup step.
   2014/10/29.   Beta 2.5.2.        Fix update absolute atom density into material after every burnup step.
   2014/10/05.   Beta 2.5.1.        Adding 3D Plot(vtk-format 3D image file), Fixbug for vector out of range in Cell plotting.
   2014/09/19.   Beta 2.5.0.        Adding Kinetics calculation(Ph.D. thesis work by Qi XU).
   2014/09/09.   Beta 2.4.9.        Check duplicated nuclide input and support absolute density input.
   2014/09/05.   Beta 2.4.8.        Fixing bug on cell tally type 4 for multigroup and add type 5 fission neutron production rate.
   2014/08/17.   Beta 2.4.7.        Fixing bug on NAECF-neutron average energy causing fission.
   2014/07/02.   Beta 2.4.6.        Adding Output Formated Input files during calculation.
   2014/06/29.   Beta 2.4.5.        Refactoring for Criticality and Fixed Source.
   2014/05/10.   Beta 2.4.0.        Adding Fixed Source.
   2014/04/13.   Beta 2.3.          Fixing bugs on Geometry/Burnup, Material input for repeated cells.
   2013/10/19.   Beta 2.2.          Refactoring, Improving Parallel and Burnup.
   2013/07/13.   Beta 2.0.          Adding Plot, MeshTally. [Released on ShanDong Conference]
   2013/06/22.   Initial Version.   Basic features.
*/

/* 
Please update version number and log in the last submit of a new development
Formats:
   Fixing (bug)     - 修复错误
   Adding (feature) - 添加功能
   Refactoring      - 重构
*/