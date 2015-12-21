//==============================================================================
//Copyright (c) 2000-2014 REAL Team Tsinghua University 
//All Rights Reserved.
//
//Description: [画图]该.h文件定义画图相关类
//Reference  : 梁金刚. 反应堆蒙卡程序RMC模型可视化功能开发. 原子能科学技术. 2014
//==============================================================================

# if ! defined __Plot_class__H__
# define  __Plot_class__H__

# include "CommonHead.h"
# include "Geometry.h"
# include "Material.h"
# include "ParticleState.h"
# include "Coordinate.h"

//==============================================================================
//MACRO DEFINITION: image file format in SLICE(2D) plotting
//      If enabled, .png image will be generated, otherwise .ppm file(text)
//==============================================================================
#define  _PNG_IMAGE  
#ifdef _PNG_IMAGE
# include <png.h>
#endif

//==============================================================================
//MACRO DEFINITION: image file format in BOX(3D) plotting
//      If enabled, .vti file will be generated, otherwise .voxel file(text).
//      Vti image can be opened by visIt/ParaView
//FOR DEVELOPER: if compiling debug binary this line should be 
//      commented, because vtk library for debug not exist
//==============================================================================
//#define  _VTK_FILE
# ifdef _VTK_FILE
#include <vtkImageData.h> 
#include <vtkDoubleArray.h> 
#include <vtkXMLImageDataWriter.h> 
#include <vtkCellData.h>
#endif

using namespace std;

//==============================================================================
//stRGB
//Function : [画图]像素类，保存rgb值
//==============================================================================
struct stRGB{
	int r,g,b;
};

//==============================================================================
//CDOneImageInput
//Function : [画图]画图输入参数类，保存单个图像输入参数
//==============================================================================
class CDOneImageInput{
public:
	int p_nPlotID; // ID of plotting（user input）
	int p_nPlotType; // 1: Slice, 2: Box
	int p_nColorType; // Colored by 1: Mat, 2: Cell, 3: Surf, 4: MatSurf, 5: CellSurf
	int p_nPixel_1, p_nPixel_2, p_nPixel_3; // pixels in x,y,z directions
	CDVector3 p_OVertex1,p_OVertex2,p_OVertex3,p_OVertex4; // vertexes for plotting area

	CDOneImageInput(){
		p_nPlotID = 1;
		p_nPlotType = 1;
		p_nColorType = 1;
		p_nPixel_1 = 0;
		p_nPixel_2 = 0;
		p_nPixel_3 = 0;
	};
	~CDOneImageInput(){};
};

//==============================================================================
//CDPlot
//Function : [画图]画图类，包括画图的整个过程
//==============================================================================
class CDPlot {
public:
	bool p_bIsPlot;
	unsigned int p_unColorScheme;

	vector<CDOneImageInput> p_vPlotInput;	

private:
	int m_nPlotID; // ID of plotting（user input）
	int m_nPlotType; // 1: Slice, 2: Box
	int m_nColorType;// Colored by 1: Mat, 2: Cell, 3: Surf, 4: MatSurf, 5: CellSurf
	int m_nPixel_1, m_nPixel_2, m_nPixel_3;// pixels in x,y,z directions
	CDVector3 m_OVertex1,m_OVertex2, m_OVertex3, m_OVertex4;// vertexes for plotting area

	//////////////// time information ///////////////////
# ifdef _IS_PARALLEL_
	double StartPlotTimeTick;
	double FinishPlotTimeTick;
# else
	time_t StartPlotTimeTick;
	time_t FinishPlotTimeTick;
# endif

	//////////////// image file information ///////////////////
	char m_chImageFileName[FILE_NAME_LENGTH];
	FILE*fpImagefilePtr;

#ifdef _PNG_IMAGE // png structs for slice
	png_structp png_ptr;
	png_infop info_ptr;
#endif

#ifdef _VTK_FILE  // VTK data for box
	vtkImageData* m_vtkGrid;
	vtkDoubleArray *m_vtkData;
	vtkXMLImageDataWriter* m_vtkWriter;
#endif

	//////////////// Plot parameters ///////////////////
	enum m_eColorTypeEnum{MAT = 1, CELL = 2, SURF = 3, MAT_SURF = 4, CELL_SURF = 5};
	double m_dDeltaWidth1, m_dDeltaWidth2, m_dDeltaWidth3; // pixel width in 3 direction
	vector<vector<int> > m_vPixelMatrix2D; // pixel data for slice
	vector<int> m_vPixelData3D; // pixel data for box
	vector<stRGB> m_vMatRGBIndex; // RGB value with Mat as index, pre-generated
	vector<stRGB> m_vCellRGBIndex; //  RGB value with Cell as index, pre-generated
	bool m_bIsLowResolution; // judge if pixel width is larger than cell gap
	CDVector3 m_ODir1,m_ODir2,m_ODir3; // direction for plotting
	stRGB m_OBLACKRGB; // const, BLACK color(0,0,0), for surface line pixels
	stRGB m_OWHITERGB; // const, white color(255,255,255), for no mat/cell pixels
	bool m_bIsEverMatColor, m_bIsEverCellColor ,m_bIsEverSurfColor;// judge if one type exists

	CDParticleState m_OPlotPaticleState;

	//Backup non-normal Surface Condition
	vector<int> m_vNonNormalSurfaces;
	vector<int> m_vNonNormalSurfacesBound;

public:
	///////////////////// public functions //////////////////////
	CDPlot();
	~CDPlot(){};
	void RunPlot(CDGeometry &cGeometry, CDMaterial &cMaterial);

private:
	void InitPlot(CDGeometry &cGeometry, CDMaterial &cMaterial);
	void UpdatePlotParas(CDOneImageInput &cImage);
	void GenRGBIndex(CDGeometry &cGeometry, CDMaterial &cMaterial);
	void GenPixelMatrix(CDGeometry &cGeometry);
	void FlyOneLine(CDGeometry &cGeometry, CDVector3 cStartPoint, CDVector3 cFlyDirection,  double  dPixelWdith, int nColorType, vector<int> &vOneLinePixels, bool &bIsLowResolution );	
	void WriteImage();
	void OutputPlotFile(int nPlotTime, CDGeometry &cGeometry, CDMaterial &cMaterial);
	void ResetPlot(CDGeometry &cGeometry);
	void BackupSurfBound(CDGeometry &cGeometry);
	void RecoverSurfBound(CDGeometry &cGeometry);
	int  AdjustPosition(CDGeometry &cGeometry, CDVector3 &cVertex_temp, CDVector3 cDir_temp);
	bool CheckOnSurf(CDGeometry &cGeometry, CDVector3 cVertex_temp, CDVector3 cDir_temp);
	void GetMatbyLocatePos(CDGeometry &cGeometry, CDVector3 cVertex_temp, int &Mat, int &Cell);
};

#endif
