# coding=gb2312
# test for RMC criticality calculation
import os
import re
import glob
import time
import psutil
import shutil

result_file_header = "Criticality Test"
dirs = ['RMC']
population = 'population = 100 30 100'
exes = 'RMC.exe';


for dir in dirs:
	print dir
	
	#delete *.out & result in the last computation
	patterns = ['*.out','*result','*binary','keff','*.bak','*.exe','*o','xsdir','*.sh','*.bat','*.temp','*.material']
	files = sum([glob.glob(dir+'\\'+pattern) for pattern in patterns],[])
	for file in files:
		os.remove(file)
		
	#open result file
	result = open(dir+'.result','w')
	print >> result, "%s. Start at %s" % (result_file_header, time.strftime('%Y-%m-%d %A %X',time.localtime(time.time()))  )
	
	#reprocess the inp file
	files = glob.glob(dir+'\\*.rmc')
	files.sort()
	for inp in files:
		inp_ptr = open(inp,'r')
		#modify kcode
		kcode = re.compile(r'population\s*=\s*\d+\s*\d+\s*\d+')
		inp1 = kcode.sub(population,inp_ptr.read())
		inp_ptr.close()
		inp_ptr = open(inp,'w')
		print >> inp_ptr, inp1
		inp_ptr.close()
	
	#run the sample
	shutil.copy(exes,dir)
	shutil.copy('xsdir',dir)
	for inp in files:
		print '>>>>>>>>>>>>>'+inp+'<<<<<<<<<<<<<<<<<<<'
		_inp = re.findall(r'([-.\w]+)',inp)
		os.system('cd %s & %s %s' % (dir, exes, _inp[-1]))
		os.remove(inp+'.material')
	os.remove(dir+'\\'+exes)
	os.remove(dir+'\\xsdir')
	
	
	#result process
	files = glob.glob(dir+'\\*.out')
	print >> result, "Total Number of Samples: %d" % (len(files))
	print >> result, "Neutron Population: %s" % population
	print >> result, ''
	
	for oup in files:
		print '>>>>>>>>>>>>>'+oup+'<<<<<<<<<<<<<<<<<<<'
		outptr = open(oup, 'r')
		str = outptr.read()
		keff = ['-1']+re.findall(r'Final\s*Keff:\s*([.\d]+)',str)
		var = ['-1']+re.findall(r'Variance:\s*([.\d]+)',str)
		cmt = ['-1']+re.findall(r'Total\s*Calculational\s*Time:\s*([.\d]+)',str)
		oupname = re.findall(r'([-.\w]+)',oup)
		print >> result , '%-50s keff=%12.6f  var=%12.6f  cmt(min)=%12.6f' % (oupname[-1], float(keff[-1]), float(var[-1]), float(cmt[-1]))
		outptr.close()
	
	print >> result, ''
	print >> result, "%s. Finish at %s" % (result_file_header, time.strftime('%Y-%m-%d %A %X',time.localtime(time.time()))  )
	result.close()

