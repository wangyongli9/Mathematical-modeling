import pandas as pd
import time

class Variable:
    PLANE_WIDTH_NAME = ["332", "333", "33E", "33H", "33L", "773"]
    PLANE_NARROW_NAME = ["319", "320", "321", "323", "325", "738", "73A", "73E", "73H", "73L"]
    GATE_ATTRIBUTE = {"I/I":1,"I/D":2,"D/I":3,"D/D":4,"D I/I":5,"D I/D":6,"I/D I":7,"D/D I":8,"D I/D I":9}
    W = 0
    N = 1
    T = 0
    S = 1
    T_North  = 0
    T_Center = 1
    T_South = 2
    S_North = 3
    S_Center = 4
    S_South = 5
    S_East = 6

class Data:
    def __init__(self):
        pass
    def readExcel(self, file_name):
        df = pd.read_excel(file_name)
        dropList = []
        df = df.drop(dropList)
        for i in range(len(df["到达\n时刻"])):
            arriveDate = df["到达\n日期"][i]
            goDate = df["出发\n日期"][i]
            arriveTime = df["到达\n时刻"][i]
            goTime = df["出发\n时刻"][i]
            goSecond = 0
            arriveSecond = 0
            if type(goTime) is str:
                timeList = goTime.split(":")
                goHour = int(timeList[0]) + 8
                goMinute = int(timeList[1])
            else:
                goHour = goTime.hour + 8
                goMinute = goTime.minute
            if type(arriveTime) is str:
                timeList = arriveTime.split(":")
                arriveHour = int(timeList[0]) + 8
                arriveMinute = int(timeList[1])
            else:
                arriveHour = arriveTime.hour + 8
                arriveMinute = arriveTime.minute
            arriveTuple = (arriveDate.year, arriveDate.month,arriveDate.day,
                           arriveHour, arriveMinute, arriveSecond,0, 0 ,0)
            newArriveDate = time.strftime("%b-%d-%Y %H:%M:%S",
                                          time.gmtime(time.mktime(arriveTuple)))
            goTuple = (goDate.year, goDate.month, goDate.day,
                       goHour, goMinute, goSecond, 0, 0, 0)
            newGoDate = time.strftime("%b-%d-%Y %H:%M:%S",
                                          time.gmtime(time.mktime(goTuple)))
            df["到达\n时刻"][i] = newArriveDate
            df["出发\n时刻"][i] = newGoDate
        df = df.drop(["到达\n日期", "出发\n日期"], axis = 1)
        df.to_excel("./DataSet/newOutData.xlsx")
        print("test")
    def refData(self, file_name):
        df = pd.read_excel(file_name)
        initTimeStamp = int(time.mktime(time.strptime("Jan-19-2018 05:00:00", "%b-%d-%Y %H:%M:%S")))
        for i in range(len(df["到达\n时刻"])):
            arriveTime = time.strptime(df["到达\n时刻"][i], "%b-%d-%Y %H:%M:%S")
            goTime = time.strptime(df["出发\n时刻"][i], "%b-%d-%Y %H:%M:%S")
            arrivetTimeStamp = int(time.mktime(arriveTime))
            goTimeStamp = int(time.mktime(goTime))
            df["到达\n时刻"][i] = (arrivetTimeStamp - initTimeStamp) / 60
            df["出发\n时刻"][i] = (goTimeStamp - initTimeStamp) / 60
        df.to_excel("./DataSet/test.xlsx")
    def parsePlane(self,file_name):
        df = pd.read_excel(file_name)
        planeList = []
        for i in range(len(df["到达\n时刻"])):
            plane = Plane()
            plane.planeIndex = i
            plane.arriveTime = int(time.mktime(time.strptime(df["到达\n时刻"][i], "%b-%d-%Y %H:%M:%S")))
            plane.goTime = int(time.mktime(time.strptime(df["出发\n时刻"][i], "%b-%d-%Y %H:%M:%S")))
            plane.gateAttribute = Variable.GATE_ATTRIBUTE[df["到达类型"][i] + "/" + df["出发类型"][i]]
            widthName = df["飞机型号"][i]
            if widthName in Variable.PLANE_WIDTH_NAME:
                plane.widthAttribute = 1
            else:
                plane.widthAttribute = 0
            planeList.append(plane)
        return planeList


    def parseGate(self,file_name):
        df = pd.read_excel(file_name)
        gateList = []
        for i in range(len(df["登机口"])):
            gate = Gate()
            gateStr = df["到达类型"][i].replace(",","").strip() + "/" + df["出发类型"][i].replace(",", "").strip()
            gate.gateAttribute = Variable.GATE_ATTRIBUTE[gateStr]
            if df["机体类别"][i] == "W":
                gate.gateWidth = 1
            else:
                gate.gateWidth = 0
            gateList.append(gate)
        return gateList
    def run(self):
        return self.parsePlane("./DataSet/Pucks.xlsx"),self.parseGate("./DataSet/Gates.xlsx")
class Plane:
    planeIndex = 0
    arriveTime = None
    goTime = None
    arriveFlight = None
    goFight = None
    widthAttribute = 0
    gateAttribute = 0
class Gate:
    gateAttribute = 0
    gateBelong = 0
    gateWidth = 0
    gatePosition = 0

