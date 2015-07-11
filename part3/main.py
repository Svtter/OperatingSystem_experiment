#!/usr/bin/env python
# coding: UTF-8

__author__ = "svtter"


class FreePart:
    def __init__(self, length, start, status):
        """
        Status:
            1 using, 0 not using

        """
        self.length = length
        self.start = start
        self.status = status


    def last(self):
        return self.length + self.start

    def output(self):

        print "|len: ", self.length, \
              "|start: ", self.start, \
              "|status: ", self.status \

    def outputWithNo(self, no):
        print "|no: ", no, \
              "|len: ", self.length, \
              "|start: ", self.start, \
              "|status: ", self.status \



class PartList:
    WHOLE_MEM = 256
    def __init__(self):
        self.flist = [FreePart(8, 20, 1),
                     FreePart(32, 28, 1),
                     FreePart(64, 60, 1),
                     FreePart(132, 124, 0)]


    def checkEmpty(self):
        """
        Check if there enough space.

        """
        empty = self.WHOLE_MEM
        for x in self.flist:
            if x.status == 1:
                empty -= x.length
        return empty

    def findFree(self, usize):
        for index, item in enumerate(self.flist):
            if item.status == 0 and item.length >= usize:
                return index

        return -1

    def add(self, no, new):
        self.flist.insert(no, new)

    def assign(self, usize):
        no = self.findFree(usize)
        f = self.flist[no]

        if f.length > usize:
            self.add(no+1, FreePart(f.length - usize, f.start + usize, 0))
            f.length = usize

        f.status = 1

        # return address of list
        return [no, f.start]

    # problem:
    def resize(self):
        """
        紧凑操作
        """

        length = 0
        for index, item in enumerate(self.flist):
            if item.status == 0:
                length += item.length
                self.flist.pop(index)

        l = len(self.flist)
        for index, item in enumerate(self.flist):
            if index+1 < l:
                self.flist[index+1].start = item.start + item.length
            elif index+1 == l:
                start = item.start + item.length
                self.flist.append(FreePart(length, start, 0))


    def release(self, fno):
        """
        释放
        """
        self.flist[fno].status = 0
        i = fno
        while i >= 0:
            if self.flist[i].status == 0:
                s = i
            else:
                break
            i -= 1

        i = fno
        while i < len(self.flist):
            if self.flist[i].status == 0:
                e = i
            else:
                break
            i += 1

        length = 0
        for index, x in enumerate(self.flist[s+1:e+1]):
            length += x.length
            self.flist.pop(index)

        self.flist[s].length += length
        self.flist[s].status = 0

    def output(self):
        for index, x in enumerate(self.flist):
            x.outputWithNo(index)

    def debug(self):
        self.output()


class Process:
    def __init__(self, fno, usize):
        self.fno = fno
        self.usize = usize

    def output(self):
        print "fno: ", self.fno, "usize: ", self.usize

class ProcessList:

    def __init__(self):
        self.plist = []

    def read(self):
        self.plist = [Process(-1,11), Process(-1, 100)]

    def getUsize(self):
        return self.plist.pop(0)

    def empty(self):
        return len(self.plist) == 0

    def output(self):
        for x in self.plist:
            x.output()


def Running():
    plist = PartList()
    plist.debug()

    slist = ProcessList()
    slist.read()

    done = True
    while not slist.empty():
        if done:
            p = slist.getUsize()
            usize = p.usize

        print "---------------------------------------------------------------"

        # {
        if plist.findFree(usize) != -1:
            infor = plist.assign(usize)
            p.fno = infor[0]

            print "分配成功. 分配空间大小: ", usize
            print "分区号: ", infor[0], "| 首地址: ", infor[1]
            print "空闲分区表情况:"
            plist.output()
            print "---------------------------------------------------------------"
            print "释放操作: "
            plist.release(p.fno)
            print "空闲分区表情况:"
            plist.output()
            done = True

        elif plist.checkEmpty() >= usize:
            print "进行紧凑操作."
            print "紧凑后空闲空间: ", plist.resize()
            print "空闲分区表情况:"
            plist.output()
            done = False

        else:
            print "No enough space."
        # }

    print "---------------------------------------------------------------"

Running()
