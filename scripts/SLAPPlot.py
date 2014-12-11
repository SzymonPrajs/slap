import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import sys
import os


class Data:
    """
    Data: Storing the data for a single lightcurve of any type.
    """
    mjd = np.array([])
    flux = np.array([])
    error = np.array([])
    flt = np.array([]) 


class Plots:
    """
    Plots: Read and understand the data stored in the info.dat file and each LC file
    """
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
        Filter = ""

        for i in range(1, len(params)):
            param = params[i].split("=")

            if (len(param) == 2):
                
                if (param[0] == "type"):
                    Type = param[1]

                elif (param[0] == "filters"):
                    Filter = param[1].split(",")

                elif (param[0] == "file"):
                    LC = param[1] 

        self.dataPlots.append(self.readData(LC))
        self.Types.append(Type)
        self.Files.append(LC)
        self.Filters.append(Filter)
        self.UniqueFilters = np.append(self.UniqueFilters, self.Filters[-1])
        self.UniqueFilters = np.unique(self.UniqueFilters)



class Canvas:
    """
    Canvas: The plotting canvas. Depending on what settings were passed a set of plots is made.
    """
    fig = plt.figure()
    ax1 = fig.add_subplot(111)
    ax2 = ""
    data = Plots()
    fltColour = dict()


    def col(self, i):
        colour = {0 : "#2ecc71", 1 : "#3498db", 2 : "#9b59b6", \
                  3 : "#34495e", 4 : "#f1c40f", 5 : "#e67e22", \
                  6 : "#e74c3c", 7 : "#1abc9c"}

        return colour[i % 8]



    def __init__(self, data):
        self.data = data
        for i in range(len(self.data.UniqueFilters)):
            self.fltColour.update({self.data.UniqueFilters[i] : self.col(i)})

        if (self.data.Types.count("residual") > 0):
            self.ax2 = self.fig.add_subplot(211)

    def plotAll(self):
        ymax = -1000

        for i in range(len(self.data.dataPlots)):
            if (ymax < self.data.dataPlots[i].flux.max()):
                ymax = self.data.dataPlots[i].flux.max()

            for f in self.data.Filters[i]:
                if (self.data.Types[i] == "data"):
                    idx = np.where(self.data.dataPlots[i].flt == f)
                    self.ax1.errorbar(self.data.dataPlots[i].mjd[idx], self.data.dataPlots[i].flux[idx], yerr=self.data.dataPlots[i].error[idx], fmt='o', color=self.fltColour[f], label="data - "+f)

                elif (self.data.Types[i] == "model"):
                    idx = np.where(self.data.dataPlots[i].flt == f)
                    self.ax1.plot(self.data.dataPlots[i].mjd[idx], self.data.dataPlots[i].flux[idx], color=self.fltColour[f], label="model - "+f)

                elif (self.data.Types[i] == "residual"):
                    pass
                    # idx = np.where(self.data.dataPlots[i].flt == f)
                    # self.ax2.errorbar(self.data.dataPlots[i].mjd[idx], self.data.dataPlots[i].flux[idx], yerr=self.data.dataPlots[i].error[idx], fmt='o', color=self.fltColour[f])


        plt.legend()
        plt.xlabel("Time (MJD)")
        plt.ylabel("Flux ($erg$ $s^{-1} cm^{-2} A^{-1}$)")
        plt.ylim(-0.2*ymax, 1.2*ymax) 
        plt.show()


"""
Main body of the program
"""
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

        canvas = Canvas(plots)
        canvas.plotAll()

    else:
        print "ERROR! No INFO file found."