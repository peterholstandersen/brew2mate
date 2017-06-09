#!/usr/bin/python

import sys

sample = 200
count = 0


for line in sys.stdin:
    count = count + 1
    if count % sample == 1:
        print line,
    
