import re
import math
import os
import shutil
import glob

	
fp = open('fetch','w')
files = sorted(glob.glob('inp.innerproduct*'),key=os.path.getmtime)

#power
for file in files:
	ff = open(file,'r')
	str = ff.read()
	aaa  = ['-1']+re.findall('Power:([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	print >> fp, '%s Power:%s' % (file,aaa[-1])
	
print >> fp, ' '	
	
#T(t)
for file in files:
	ff = open(file,'r')
	str = ff.read()
	aaa  = ['-1']+re.findall('T\(0\):([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	print >> fp, '%s T:%s' % (file,aaa[-1])

print >> fp, ' '	
	
#C(t)
for file in files:
	ff = open(file,'r')
	str = ff.read()
	aaa  = ['-1']+re.findall('c[\d]+\(0\):([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	print >> fp, '%s c:%s %s %s %s %s %s' %(file,aaa[1], aaa[2], aaa[3], aaa[4], aaa[5], aaa[6])

print >> fp, ' '		

#rho(t)
for file in files:
	ff = open(file,'r')
	str = ff.read()
	aaa  = ['-1']+re.findall('Reactivity:[+-]*[.\d]+[Ee][+-]*[\d]+\s*([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	bbb  = ['-1']+re.findall('Reactivity:[+-]*[.\d]+[Ee][+-]*[\d]+\s*[+-]*[.\d]+[Ee][+-]*[\d]+\s+([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	print >> fp, '%s rho:%s  %s' %(file,aaa[-1],bbb[-1])
	
print >> fp, ' '


#gtime(t)
for file in files:
	ff = open(file,'r')
	str = ff.read()
	aaa  = ['-1']+re.findall('GenTime\(sec\):[+-]*[.\d]+[Ee][+-]*[\d]+\s*([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	bbb  = ['-1']+re.findall('GenTime\(sec\):[+-]*[.\d]+[Ee][+-]*[\d]+\s*[+-]*[.\d]+[Ee][+-]*[\d]+\s+([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	print >> fp, '%s gtime:%s  %s' %(file,aaa[-1],bbb[-1])
	
print >> fp, ' '
	
	
#beta(t)
for file in files:
	ff = open(file,'r')
	str = ff.read()
	aaa  = ['-1']+re.findall('Beta\s*[\d]+:[+-]*[.\d]+[Ee][+-]*[\d]+\s*([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	if len(aaa)>1:
		print >> fp, '%s beta:%s %s %s %s %s %s' %(file,aaa[-6], aaa[-5], aaa[-4], aaa[-3], aaa[-2], aaa[-1])

print >> fp, ' '
#beta_std(t)
for file in files:
	ff = open(file,'r')
	str = ff.read()
	aaa  = ['-1']+re.findall('Beta\s*[\d]+:[+-]*[.\d]+[Ee][+-]*[\d]+\s*[+-]*[.\d]+[Ee][+-]*[\d]+\s+([+-]*[.\d]+[Ee][+-]*[\d]+)', str)
	if len(aaa)>1:
		print >> fp, '%s beta:%s %s %s %s %s %s' %(file,aaa[-6], aaa[-5], aaa[-4], aaa[-3], aaa[-2], aaa[-1])		
		
		
print >> fp, ' '
