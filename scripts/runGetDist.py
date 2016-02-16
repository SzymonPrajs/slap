import sys
import os
import shutil
import subprocess

rangesMagnetar = \
"par1	10	100\n\
par2	0.1 20\n\
par3	0.1	20\n\
par4	52000	58000"

paramnamesMagnetar = \
"par1      \\tau_{M}\n\
par2      B\n\
par3      P\n\
par4      expMJD"

rangesMagnetarK = \
"par1	10	100\n\
par2	0.1 20\n\
par3	0.1	20\n\
par4	0.01 1.0\n\
par5	52000	58000"

paramnamesMagnetarK = \
"par1      \\tau_{M}\n\
par2      B\n\
par3      P\n\
par4	  \kappa\n\
par5      expMJD"

rangesMagnetarE = \
"par1	10	100\n\
par2	0.1 20\n\
par3	0.1	20\n\
par4	5.0e49 2.0e52\n\
par5	52000	58000"

paramnamesMagnetarE = \
"par1      \\tau_{M}\n\
par2      B\n\
par3      P\n\
par4	  E_{k}\n\
par5      expMJD"

rangesMagnetarR = \
"par1	10	100\n\
par2	0.1 20\n\
par3	0.1	20\n\
par4	0.01 100\n\
par5	52000	58000"

paramnamesMagnetarR = \
"par1      \\tau_{M}\n\
par2      B\n\
par3      P\n\
par4	  R_{0} \\times 10^{14} cm \n\
par5      expMJD"

chainDir = sys.argv[1]
SNName = chainDir.split("/")[-1]
SNModel = sys.argv[2]

getDistOutput = chainDir+"/getDist"
if not os.path.exists(getDistOutput):
	os.makedirs(getDistOutput)
	print getDistOutput

shutil.copy(chainDir+"/nest-.txt", getDistOutput+"/"+SNName+"_1.txt")

fHandle = open(getDistOutput+"/"+SNName+".ranges","w")
if SNModel == "magnetar" or SNModel == "Magnetar":
	fHandle.write(rangesMagnetar)
elif SNModel == "magnetarK" or SNModel == "MagnetarK":
	fHandle.write(rangesMagnetarK)
elif SNModel == "magnetarE" or SNModel == "MagnetarE":
	fHandle.write(rangesMagnetarE)
elif SNModel == "magnetarR" or SNModel == "MagnetarR":
	fHandle.write(rangesMagnetarR)
fHandle.close()

fHandle = open(getDistOutput+"/"+SNName+".paramnames","w")
if SNModel == "magnetar" or SNModel == "Magnetar":
	fHandle.write(paramnamesMagnetar)
elif SNModel == "magnetarK" or SNModel == "MagnetarK":
	fHandle.write(paramnamesMagnetarK)
elif SNModel == "magnetarE" or SNModel == "MagnetarE":
	fHandle.write(paramnamesMagnetarE)
elif SNModel == "magnetarR" or SNModel == "MagnetarR":
	fHandle.write(paramnamesMagnetarR)
fHandle.close()


paramLines = \
"file_root = "+getDistOutput+"/"+SNName+"\n\
out_root = "+ SNName +"\n\
out_dir = "+getDistOutput+"/"+"\n\
plot_data_dir = "+getDistOutput+"/"+"\n"

fHandle = open("/Users/szymon/Projects/slap/scripts/generic_params.ini","r")
content = fHandle.read()
fHandle.close()

fHandle = open(getDistOutput+"/"+SNName+".ini","w")
fHandle.write(paramLines + content)
fHandle.close()

os.environ['PYTHONPATH'] = "/Users/szymon/Projects/cosmomc/python"
subprocess.call(["python", "/Users/szymon/Projects/cosmomc/python/GetDist.py", getDistOutput+"/"+SNName+".ini"])
subprocess.call(["python", getDistOutput+"/"+SNName+"_tri.py"])
shutil.copy(getDistOutput+"/"+SNName+"_tri.pdf", chainDir+"/"+SNName+"_tri.pdf")
