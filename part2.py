#!/usr/bin/env python
# coding: UTF-8

__author__  = "svtter"
__time__    = "2015年 07月 08日 星期三 03:36:18 EDT"


MAX_MEMNO = 5
MAX_OUTMEM = 5

# page
class Page:

    def __init__(self, no, sym, memno, outmem, change):
        self.no = no
        self.sym = sym
        self.memno = memno
        self.outmem = outmem
        self.change = change

    def genRand(self):
        pass


    def isChange(self):
        return self.change

    def output(self):
        print ("no:", self.no, "sym:", self.sym, "memno: ", self.memno, "outmem: ",
            self.outmem, "change: ", self.change)



# Page list
class Page_List:
    MAX_MEM = 4

    def __init__(self):
        self.plist = []
        self.freeMem = []

    def isFull(self):
        return len(self.plist) == self.MAX_MEM

    def read(self):
        "Use data were given."
        page = [Page(0, 1, 5, "011", 1),
                Page(1, 1, 8, "012", 1),
                Page(2, 1, 9, "013", 0),
                Page(3, 1, 1, "021", 0),
                Page(4, 0, -1, "022", 0),
                Page(5, 0, -1, "023", 0),
                Page(6, 0, -1, "121", 0),
                ]
        self.plist += page

    def checkPage(self, no):
        for x in self.plist:
            if x.no == no and x.sym == 1:
                return True
        return False

    def getMemNo(self, pageNo):
        """
        check page no first

        -1 means cannot find
        """
        for x in self.plist:
            if x.no == pageNo and x.sym == 1:
                return x.memno
        return -1

    def output(self):
        for x in self.plist:
            x.output()

    def getOutPageFIFO(self):
        for x in self.plist:
            if x.sym == 1:
                x.sym = 0
                p = x
                x.change = 0
                self.freeMem.append(x.memno)
                break
        return p

    def getInPageFIFO(self, no):
        self.plist[no].sym = 1
        self.plist[no].memno = self.freeMem.pop(0)
        return self.plist[no]

    def changePage(self, no):
        self.plist[no].change = 1

    def debug(self):
        print ("About Page List:")
        self.read()
        self.output()

        #print "****"
        #p = self.getOutPageFIFO()
        #p.output()
        #print "****"
        #self.output()

        print "-----------------------------------------"



class Command:

    def __init__(self, operation, pageNo, unitNo):
        self.pageNo = pageNo
        self.unitNo = unitNo
        self.operation = operation

    def getUnitNo(self):
        try:
            return int(self.unitNo, 8)
        except:
            return -1

    def checkValid(self, MEM_SIZE):
        try:
            int(self.unitNo, 8)
        except:
            return False
        else:
            return int(self.unitNo) < int(MEM_SIZE)

    def output(self):
        print ("pageNo: ", self.pageNo, "UnitNo: ", self.unitNo)


# Command List
class Command_List:
    def __init__(self):
        self.clist = []

    def read(self):
        command = [Command("+", 0, "070"),
                   Command("+", 1, "050"),
                   Command("*", 2, "015"),
                   Command("存", 3, "021"),
                   Command("取", 0, "057"),
                   Command("-", 6, "040"),
                   Command("移位", 4, "053"),
                   Command("+", 5, "023"),
                   Command("存", 1, "037"),
                   Command("取", 2, "076"),
                   Command("+", 4, "001"),
                   Command("取", 6, "084"),
                   ]
        self.clist += command

    def output(self):
        for x in self.clist:
            x.output()

    def empty(self):
        if len(self.clist) == 0:
            return True
        return False

    def getCommand(self):
        return self.clist[0]

    def finishCommand(self):
        self.clist.pop(0)

    def debug(self):
        print ("About Command List:")
        self.read()
        self.output()
        print "-----------------------------------------"


# int
MEM_SIZE = "128"

def Running_Part1():
    clist = Command_List()
    plist = Page_List()
    clist.debug()
    plist.debug()

    count = 1

    while(not clist.empty()):
        c = clist.getCommand()
        print "指令", count, ":"

        if c.checkValid(MEM_SIZE):
            if plist.checkPage(c.pageNo):
                res = plist.getMemNo(c.pageNo) * int(MEM_SIZE, 10) + \
                    int(c.unitNo, 8)

                print "绝对地址:" + oct(res)
            else:
                print "该页不在内存."
        else:
            print "地址非法."

        count += 1
        clist.finishCommand()

def Running_Part2():
    clist = Command_List()
    plist = Page_List()
    clist.debug()
    plist.debug()

    count = 1

    while not clist.empty():
        c = clist.getCommand()
        print "指令", count, ":"

        if c.checkValid(MEM_SIZE):
            suc = False
            while not suc:
                if plist.checkPage(c.pageNo):
                    res = plist.getMemNo(c.pageNo) * int(MEM_SIZE, 10) + \
                        int(c.unitNo, 8)

                    if c.operation == "存":
                        plist.changePage(c.pageNo)
                        print "* Change MEM."

                    if res < 0:
                        import ipdb; ipdb.set_trace()  # XXX BREAKPOINT
                        c, plist

                    print "绝对地址:" + oct(res)
                    suc = True

                else:
                    print "该页不在内存."
                    p = plist.getOutPageFIFO()
                    if p.isChange():
                        print "OUT J"
                    p = plist.getInPageFIFO(c.pageNo)
                    print "* 调入页:"
                    p.output()
        else:
            print "访问地址非法."

        count += 1
        clist.finishCommand()
        print "--------------------------------------------------------------"


# Running main
#Running_Part1()
Running_Part2()

