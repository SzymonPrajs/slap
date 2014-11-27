import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import sys
import os



class Data:
    mjd = np.array([])
    flux = np.array([])
    error = np.array([])
    flt = np.array([]) 


class Plots:
    z = ""
    SNName = ""
    title = ""

    dataPlots = []  
    Filters = []
    UniqueFilters = np.array([])
    Files = []
    Types = []

    def __init__(self):
        pass


    def assignParam(self, param):
        param = param.split("=")
        if (len(param) == 2):
            
            if (param[0] == "z"):
                z = param[1]

            elif (param[0] == "name"):
                SNName = param[1]

            elif (param[0] == "title"):
                title = param[1] 


    def readData(self, fileName):
        data = Data()
        LC = open(fileName, 'r')
        LCData = LC.readlines()
        Temp = [[],[],[],[]]

        for i in range(len(LCData)):
            LCData[i] = LCData[i].rstrip('\n')
            LCData[i] = LCData[i].split()
            if (LCData[i][0][0] == '#'):
                pass

            elif (len(LCData[i]) == 3):
                Temp[0].append(float(LCData[i][0]))
                Temp[1].append(float(LCData[i][1]))
                Temp[3].append(LCData[i][2])

            elif (len(LCData[i]) == 4):
                Temp[0].append(float(LCData[i][0]))
                Temp[1].append(float(LCData[i][1]))
                Temp[2].append(float(LCData[i][2]))
                Temp[3].append(LCData[i][3])

        data.mjd = np.array(Temp[0])
        data.flux = np.array(Temp[1])
        data.error = np.array(Temp[2])
        data.flt = np.array(Temp[3])

        return data


    def addPlot(self, params):
        ID = params[0]
        LC = ""
        Type = ""
        Filters = ""

        for i in range(1, len(params)):
            param = params[i].split("=")

            if (len(param) == 2):
                
                if (param[0] == "type"):
                    Type = param[1]

                elif (param[0] == "filters"):
                    Filters = param[1].split(",")

                elif (param[0] == "file"):
                    LC = param[1] 

        self.dataPlots.append(self.readData(LC))
        self.Types.append(Type)
        self.Files.append(LC)
        self.Filters.append(np.unique(plots.dataPlots[-1].flt))
        self.UniqueFilters = np.append(self.UniqueFilters, self.Filters[-1])
        self.UniqueFilters = np.unique(self.UniqueFilters)



class Canvas:
    fig = plt.figure()
    ax1 = fig.add_subplot(111)

    def __init__(self):
        pass


    def plotAll(self, data):
        for f in data.UniqueFilters:
            for i in range(len(data.dataPlots)):
                if (data.Types[i] == "data"):
                    idx = np.where(data.dataPlots[i].flt == f)
                    self.ax1.scatter(data.dataPlots[i].mjd[idx], data.dataPlots[i].flux[idx])
                    self.ax1.errorbar(data.dataPlots[i].mjd[idx], data.dataPlots[i].flux[idx], yerr=data.dataPlots[i].error[idx], fmt='o')

                elif (data.Types[i] == "model"):
                    idx = np.where(data.dataPlots[i].flt == f)
                    self.ax1.plot(data.dataPlots[i].mjd[idx], data.dataPlots[i].flux[idx])

        # TODO: The max value needs to be implemented
        # TODO: Colours needs to be implemented
        plt.ylim(0, 1e-17) 
        plt.show()




if __name__ == "__main__":
    if (len(sys.argv) > 1):
        plotPath = sys.argv[1]

    if os.path.exists(plotPath + "/info.dat"):
        INFO = open(plotPath + "/info.dat", 'r')
        INFOData = INFO.readlines()
        INFO.close()

        plots = Plots()

        for i in range(len(INFOData)):
            INFOData[i] = INFOData[i].rstrip("\n")
            INFOData[i] = INFOData[i].split()
            
            if (len(INFOData[i]) == 1):
                plots.assignParam(INFOData[i][0])

            elif (len(INFOData[i]) > 1):
                plots.addPlot(INFOData[i])

        canvas = Canvas()
        canvas.plotAll(plots)

    else:
        print "ERROR! No INFO file found."