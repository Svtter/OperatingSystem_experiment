#!/usr/bin/env python

print int("20", 8)

print oct( int("128", 10) + int("011", 8))

print oct(8)


a = [0, 1, 2, 3, 4]
print a

class Test:
    def __init__(self):
        self.a = 1

a = Test()
b = a
b.a += 1
print a.a, b.a

