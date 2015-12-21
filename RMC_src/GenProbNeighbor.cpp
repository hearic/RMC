//# include "Geometry.h"
//
//void GeometryClass::GenProbNeighbor()
//{
//	int i,j,iTemp;
//	int cellsurfnum,NeiCellNum,jc,js;
//	if(TrackingMode==0){goto go_ray;}
//
//	//////// step 1 :  original neighbor cell sort ///////
//	for(i=1;i<=cell_num;i++)
//	{
//		for(j=1;j<=cell_num;j++){	neighbor_cell[i][j]=j;}
//	}
//	/////// step 2 sort by neighbor_count //////////
//	for(jc=1;jc<=cell_num;jc++)    
//	{
//		SortProbNeighbor(jc, 1, cell_num);
////	printf("%d %d %d\n",jc,neighbor_count[jc][1],neighbor_count[jc][2]);
//	}
//	goto go_end;
//go_ray:
//	//////////////////////////////////////////////////
//	/////// step 2 sort by neighbor_count //////////
//	for(jc=1;jc<=cell_num;jc++)    
//	{
//		cellsurfnum=CellSurfIndexU[jc][0];
//		for(js=1;js<=cellsurfnum;js++)
//		{
//			NeiCellNum=CellSurfNeighCell[jc][js][0];
//			for(i=NeiCellNum;i>1;i--)    
//			{
//				for(j=1;j<i;j++)
//				{  
//					if(cell_surf_cell_count[jc][js][j]<cell_surf_cell_count[jc][js][j+1])//found and swap
//					{
//						iTemp = cell_surf_cell_count[jc][js][j+1];
//						cell_surf_cell_count[jc][js][j+1] = cell_surf_cell_count[jc][js][j];
//						cell_surf_cell_count[jc][js][j]=iTemp;
//
//						iTemp = CellSurfNeighCell[jc][js][j+1];
//						CellSurfNeighCell[jc][js][j+1] = CellSurfNeighCell[jc][js][j];
//						CellSurfNeighCell[jc][js][j]=iTemp;
//					}
//				}
//			}
//		}
//	}
//
//	//////////////////////////////////////////////////
//go_end:
//	prob_neighbor_flag=2;
//	prob_neighbor_cycle=-1;
//}
//
//
//void GeometryClass::SortProbNeighbor(int jc, int left, int right)
//{
//      int i,j,middle,iTemp;
//	  i=left;
//	  j=right;
//	  middle=neighbor_count[jc][(left+right)/2];//求中间值
//	  do
//	  {
//		  while((neighbor_count[jc][i]>middle)&&(i<right)){i++;}//从左扫描小于中值的数		  　　
//		  while((neighbor_count[jc][j]<middle) && (j>left)){j--;}//从右扫描大于中值的数	　　
//		  if(i<=j)//找到了一对值,交换
//		  {
//			  iTemp=neighbor_count[jc][j];
//			  neighbor_count[jc][j]=neighbor_count[jc][i];
//			  neighbor_count[jc][i]=iTemp;
//			  iTemp=neighbor_cell[jc][j];
//			  neighbor_cell[jc][j]=neighbor_cell[jc][i];
//			  neighbor_cell[jc][i]=iTemp;
//			  i++;
//			  j--;
//		  }
//	  }while(i<=j);//如果两边扫描的下标交错，就停止（完成一次）
////	  if(neighbor_count[jc][j]==0){return;}        // zeros need not to be sorted
//	  if(left<j){SortProbNeighbor(jc,left,j);}      //当左边部分有值(left<j)，递归左半边 
//	  if(right>i){SortProbNeighbor(jc,i,right);} //当右边部分有值(right>i)，递归右半边
//}