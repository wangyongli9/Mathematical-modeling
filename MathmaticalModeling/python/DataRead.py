import pandas as pd
import datetime
import time
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
def test():
    data = Data()
    data.refData("./DataSet/newOutData.xlsx")
if __name__ == '__main__':
    test()
