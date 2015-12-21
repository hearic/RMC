# if ! defined __Coordinate__H__
# define  __Coordinate__H__

#include "CommonHead.h"
using namespace std;

class CDVector3{
public:
	double _x,_y,_z;

	//constructor
	CDVector3( )                                                {_x = 0.0;         _y = 0.0;         _z = 0.0;              }
	CDVector3(double dTemp_x, double dTemp_y, double dTemp_z)   {_x = dTemp_x;     _y = dTemp_y;     _z = dTemp_z;          }
	CDVector3(double dPos[3] )                                  {_x = dPos[0];     _y = dPos[1];     _z = dPos[2];          }

	//operator overloading  +, -, *
	CDVector3 operator+(const CDVector3& cPos)	                {return CDVector3(_x + cPos._x, _y + cPos._y, _z + cPos._z);}
	CDVector3 operator-(const CDVector3& cPos)                  {return CDVector3(_x - cPos._x, _y - cPos._y, _z - cPos._z);}
	CDVector3 operator*(double dZoom)                           {return CDVector3(_x*dZoom, _y*dZoom, _z*dZoom);	        }
	double    operator*(const CDVector3& cPos)                  {return _x*cPos._x + _y*cPos._y + _z*cPos._z;               }

	//related operation
	double GetVecLength()
	{	
		return sqrt(_x*_x+_y*_y+_z*_z);	
	}
	void Normalize()
	{
		double len = sqrt(_x*_x+_y*_y+_z*_z);
		if (len == 0)
		{
			cerr<<"Attempt to normalize zero vector!"<<endl;
			exit(1);
		}
		_x = _x/len;
		_y = _y/len;
		_z = _z/len;
	}
	void MoveDistance(CDVector3& cDir, double dDistance)
	{
		//return *this+dir*dis;
		_x += cDir._x * dDistance;
		_y += cDir._y * dDistance;
		_z += cDir._z * dDistance;
	}
};

#endif //__Coordinate__H__
