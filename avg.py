#!/usr/bin/python

import sys

data = []

for line in sys.stdin:
    line = line.strip()
    split = line.split(" ")
    data.append(tuple((split)))

avg = 50
    
# i iterates over the tupples
for i in range(0, len(data)):
    # j iterates over the elements in the tupple
    sum = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    count = [0, 0, 0, 0, 0, 0]
    for j in [1, 2, 3, 4, 5]:
        for x in range(0, avg):
            index = i-x
            if index < 0 or index > len(data)-1:
                continue
            sum[j] = sum[j] + float(data[index][j])
            count[j] = count[j] + 1
    print data[i][0],
    for j in [1, 2, 3, 4, 5]:
        print sum[j] / count[j],
    print
        


