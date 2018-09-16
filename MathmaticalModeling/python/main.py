from DataRead import Data
import queue


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
        for plane in planeList:
            index = (plane.gateAttribute - 1 ) * 2 + plane.widthAttribute
            self.dealQueue(gateStatus, index, QueueList[index], plane)
            gateString = ""
            for i in range(len(gateStatus)):
                gateString += str(gateStatus[i][0]) +  "/" +str(gateStatus[i][1]) + "     "
            print(gateString)



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



