from DataRead import Data
import queue
from pandas import DataFrame

class Main():
    def sequencePlane(self, planeList):
        gateStatus = [[0,0],[0,0],[0,0],[0,0],[0,0], [0,0], [0,0], [0,0]]
        IINqueue = queue.PriorityQueue()
        IIWqueue = queue.PriorityQueue()
        IDNqueue = queue.PriorityQueue()
        IDWqueue = queue.PriorityQueue()
        DINqueue = queue.PriorityQueue()
        DIWqueue = queue.PriorityQueue()
        DDNqueue = queue.PriorityQueue()
        DDWqueue = queue.PriorityQueue()
        QueueList = [IINqueue, IIWqueue, IDNqueue, IDWqueue, DINqueue, DIWqueue, DDNqueue, DDWqueue]
        names = ["IIN1","IIN0","IIW1","IIW0","IDN1","IDN0","IDW1","IDW0","DIN1","DIN0","DIW1","DIW0","DDN1","DDN0","DDW1","DDW0"]
        excelData = {"IIN1":[],"IIN0":[],"IIW1":[],"IIW0":[],"IDN1":[],"IDN0":[],"IDW1":[],"IDW0":[],"DIN1":[],"DIN0":[],"DIW1":[],"DIW0":[],"DDN1":[],
                     "DDN0":[],"DDW1":[],"DDW0":[]}
        for i,plane in enumerate(planeList):
            index = (plane.gateAttribute - 1 ) * 2 + plane.widthAttribute
            self.dealQueue(gateStatus, index, QueueList[index], plane)
            gateString = ""
            for i in range(len(gateStatus)):
                gateString += str(gateStatus[i][0]) +  "/" +str(gateStatus[i][1]) + "     "
                excelData[names[2 * i]].append(gateStatus[i][0])
                excelData[names[2 * i + 1]].append(gateStatus[i][1])
        df = DataFrame(excelData,columns=names)
        df.to_excel("./DataSet/PlaneNeed.xlsx")


    def dealQueue(self, gateStatus, index, planeQueue, plane):
        if planeQueue.empty():
            planeQueue.put(PlaneQueue(plane.goTime,plane))
            gateStatus[index][0] += 1
        else:
            while not planeQueue.empty():
                headPlane = planeQueue.get()
                if headPlane.plane.goTime + 45 * 60 > plane.arriveTime:
                    if gateStatus[index][1] == 0:
                        gateStatus[index][0] += 1
                    else:
                        gateStatus[index][0] += 1
                        gateStatus[index][1] -= 1
                    planeQueue.put(headPlane)
                    planeQueue.put(PlaneQueue(plane.goTime, plane))
                    break
                else:
                    gateStatus[index][0] -= 1
                    gateStatus[index][1] += 1


    def timeConsumptionFunc(self, planeList, gateList):
        gateUsePlaneList = []
        for gate in gateList:
            gateUsePlane = GateUsePlane(gate)
            gateUsePlaneList.append(gateUsePlane)
        for plane in planeList:
            for gateUsePlane in gateUsePlaneList:
                
    def timeCalculate(self):
class GateUsePlane:
    def __init__(self, gate):
        self.plane = None
        self.gate = gate
        pass
    def set(self,plane):
        self.plane = plane
    def free(self):
        self.plane = None

class PlaneQueue(object):
    def __init__(self, priority, plane):
        self.priority = priority
        self.plane = plane

    def __lt__(self, other):
        return self.priority < other.priority
if __name__ == '__main__':
    data = Data()
    planeList,gateList = data.run()
    main = Main()
    main.sequencePlane(planeList)



