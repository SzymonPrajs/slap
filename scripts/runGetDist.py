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



chainDir = sys.argv[1]
SNName = chainDir.split("/")[-1]
SNModel = "magnetar"

getDistOutput = chainDir+"/getDist"
if not os.path.exists(getDistOutput):
	os.mkchainDir(getDistOutput)
	print getDistOutput

shutil.copy(chainDir+"/nest-.txt", getDistOutput+"/"+SNName+"_1.txt")

fHandle = open(getDistOutput+"/"+SNName+".ranges","w")
if SNModel == "magnetar" or SNModel == "Magnetar":
	fHandle.write(rangesMagnetar)
fHandle.close()

fHandle = open(getDistOutput+"/"+SNName+".paramnames","w")
if SNModel == "magnetar" or SNModel == "Magnetar":
	fHandle.write(paramnamesMagnetar)
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