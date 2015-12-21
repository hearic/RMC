import re
import math
import os
import shutil
import glob


#tally
files = sorted(glob.glob('inp.tally*'),key=os.path.getmtime)

for file in files:
	fp = open('g1_'+file[4:],'w')
	ff = open(file,'r')
	str = ff.read()
	fave = ['ave']+re.findall('\s+1\s+[+-]*[.\d]+[Ee][+-]*[\d]+\s+([+-]*[.\d]+[Ee][+-]*[\d]+)',str)
	fstd = ['std']+re.findall('\s+1\s+[+-]*[.\d]+[Ee][+-]*[\d]+\s+[+-]*[.\d]+[Ee][+-]*[\d]+\s+([+-]*[.\d]+[Ee][+-]*[\d]+)',str)
	for i in range(len(fave)):
		print >> fp, '%s   %s'%(fave[i],fstd[i])
		
for file in files:
	fp = open('g2_'+file[4:],'w')
	ff = open(file,'r')
	str = ff.read()
	fave = ['ave']+re.findall('\s+2\s+[+-]*[.\d]+[Ee][+-]*[\d]+\s+([+-]*[.\d]+[Ee][+-]*[\d]+)',str)
	fstd = ['std']+re.findall('\s+2\s+[+-]*[.\d]+[Ee][+-]*[\d]+\s+[+-]*[.\d]+[Ee][+-]*[\d]+\s+([+-]*[.\d]+[Ee][+-]*[\d]+)',str)
	for i in range(len(fave)):
		print >> fp, '%s   %s'%(fave[i],fstd[i])