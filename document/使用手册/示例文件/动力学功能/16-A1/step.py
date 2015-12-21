import re
import math
import os
import shutil
import glob

class CrossSection:
	def __init__(self,name,dxs1,dxs2, v1,v2,keff): 
		#xs: sigmaT, sigmaF, sigmaA, sigmaSgg,sigmaSgg'
		#dxs:sigmaT,sigmaF,sigmaA,sigmaSgg,sigmaSgg'
		self.name=name
		self.xs1_0 = dxs1
		self.xs2_0 = dxs2
		self.xs1_0[2] = self.xs1_0[0]-self.xs1_0[1]-self.xs1_0[3]-self.xs1_0[4]
		self.xs2_0[2] = self.xs2_0[0]-self.xs2_0[1]-self.xs2_0[3]-self.xs2_0[4]
		self.rate1 = [0.0 for i in range(5)]
		self.rate2 = [0.0 for i in range(5)]
		self.xs1 = self.xs1_0
		self.xs2 = self.xs2_0
		self.nu = 1.0/keff
		m = 939.58
		c = 299.7925
		self.ergcent1 = m*(c/math.sqrt(c*c-(v1*1e-8)*(v1*1e-8))-1)   # erg:Mev, v:cm/s
		self.ergcent2 = m*(c/math.sqrt(c*c-(v2*1e-8)*(v2*1e-8))-1)   # erg:Mev, v:cm/s
		self.ergwidth1 = 2*(self.ergcent1 - 2*self.ergcent2)
		self.ergwidth2 = 2*self.ergcent2
		
	
	def CalcRate(self,deltaT,dxs1,dxs2):
			xs1 = dxs1
			xs1[2] = xs1[0]-xs1[1]-xs1[3]-xs1[4]
			xs2 = dxs2
			xs2[2] = xs2[0]-xs2[1]-xs2[3]-xs2[4]
			self.rate1 = [(xs1[i]-self.xs1_0[i])/deltaT for i in range(5)]
			self.rate2 = [(xs2[i]-self.xs2_0[i])/deltaT for i in range(5)]
	
	def GetXst(self, t):
		self.xs1 = [self.xs1_0[i] + self.rate1[i]*t for i in range(5)]
		self.xs2 = [self.xs2_0[i] + self.rate2[i]*t for i in range(5)]
		

def PrintXS(xs):
	
	f = file('xsdata','w')

	for i in range(len(xs)):
		print >> f, "%10s  233.025921 0.00000E+00  04/22/87 "%xs[i].name
		print >> f, ""
		print >> f, "      0   0.000000      0   0.000000      0   0.000000      0   0.000000"
		print >> f, "      0   0.000000      0   0.000000      0   0.000000      0   0.000000"
		print >> f, "      0   0.000000      0   0.000000      0   0.000000      0   0.000000"
		print >> f, "      0   0.000000      0   0.000000      0   0.000000      0   0.000000"
		print >> f, "       20%9s        0        0        2        0        1        0" % (xs[i].name[:-4])
		print >> f, "        0        1        0        1        0        0        0        0"
		print >> f, "        1        5        7        9       11       13        0        0"
		print >> f, "        0        0        0        0       15        0        0       19"
		print >> f, "       20        0        0        0        0        0        0        0"
		print >> f, "        0        0        0        0        0        0        0        0"
		print >> f, "%20.12E%20.12E%20.12E%20.12E" % (xs[i].ergcent1,xs[i].ergcent2,xs[i].ergwidth1,xs[i].ergwidth2)    #energy grid
		print >> f, "%20.12E%20.12E%20.12E%20.12E" % (xs[i].xs1[0],xs[i].xs2[0],xs[i].xs1[1],xs[i].xs2[1])
		print >> f, "%20.12E%20.12E  1.000000000000E+00  0.000000000000E+00" % (xs[i].nu,xs[i].nu)
		print >> f, "%20.12E%20.12E                  16%20.12E" % (xs[i].xs1[2],xs[i].xs2[2],xs[i].xs1[3])
		print >> f, "%20.12E%20.12E  0.000000000000E+00  0.000000000000E+00" % (xs[i].xs1[4],xs[i].xs2[3])
		print >> f, ""		
		

		
xs = [CrossSection('10001.01m',[2.411e-1, 8.3441e-4, 0.0, 2.336e-1, 3.598e-3],[4.172e-1, 3.2776e-4, 0.0, 4.070e-1, 0.0], 5.4025e8, 9.1912e7, 0.998273)]
xs.append( CrossSection('10002.01m',[1.849e-1, 7.4518e-3, 0.0, 1.777e-1, 2.085e-3],[3.668e-1, 1.1061e-2, 0.0, 3.537e-1, 0.0], 5.4025e8, 9.1912e7, 0.998273) )
xs.append( CrossSection('10003.01m',[9.432e-2, 0.0, 0.0, 8.571e-2, 1.717e-3],[1.876e-1, 0.0, 0.0, 1.713e-1, 0.0], 5.4025e8, 9.1912e7, 0.998273) )

PrintXS(xs)	
	
#timepoint = [1*i for i in range(12)]
timepoint = [0,1.00E-08,1.00E-07,1.00E-06,1.00E-05,2.00E-05,3.00E-05,4.00E-05,5.00E-05,6.00E-05,7.00E-05,8.00E-05,9.00E-05,1.00E-04,2.00E-04,3.00E-04,4.00E-04,5.00E-04,6.00E-04,7.00E-04,8.00E-04,9.00E-04,1.00E-03,1.00E-02,2.00E-02,4.00E-02,6.00E-02,8.00E-02,1.00E-01,2.00E-01,4.00E-01,6.00E-01,8.00E-01,1.00E+00,2.00E+00,4.00E+00,6.00E+00,8.00E+00,1.00E+01,11]
	
#0 step
inp  = '16-A1_0'
timestep = 'timestep deltat = %.5E'%(timepoint[1]-timepoint[0])
f = open(inp,'r')
find = re.compile(r'timestep\s*deltat\s*=\s*[+-]*[.\d]+[Ee][+-]*\d+')
stri = find.sub(timestep,f.read())
f.close()
f = open(inp,'w')
print >> f, stri
f.close()
shutil.copy(inp,'inp')
os.system('mpiexec -n 20 RMC.exe inp')
os.rename('inp.innerproduct','inp.innerproduct0')
os.rename('inp.tally','inp.tally0')	


inp = '16-A1_1'
for i in range(1,len(timepoint)-1):
	timestep = 'timestep deltat = %.5E'%(timepoint[i+1]-timepoint[i])
	f = open(inp,'r')
	find = re.compile(r'timestep\s*deltat\s*=\s*[+-]*[.\d]+[Ee][+-]*\d+')
	stri = find.sub(timestep,f.read())
	f.close()
	f = open(inp,'w')
	print >> f, stri
	f.close()
	shutil.copy(inp,'inp')
	os.system('mpiexec -n 20 RMC.exe inp')
	os.rename('inp.innerproduct','inp.innerproduct'+str(i))
	os.rename('inp.tally','inp.tally'+str(i))
	

