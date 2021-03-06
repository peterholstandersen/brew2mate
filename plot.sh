#!/bin/bash

cat $@ |\
    awk '{ print $7, $8, $9, $10, $11; }' |\
    grep actual |\
    sed 's/actual\[.\]=//g' |\
    sed 's/target_temperature=//g' |\
    awk 'BEGIN{ count = 0;}{ print count++, $0;}' \
    | ./sample.py \
    | ./avg.py \
    > /tmp/to_plot


# "/tmp/to_plot" using 3 with line title "actual[0]",\
# "/tmp/to_plot" using 5 with line title "actual[2]",\

gnuplot > /tmp/graph.png <<FOO
set terminal png
set yrange [10:25]
plot \
     "/tmp/to_plot" using 3 with line title "actual[0]",\
     "/tmp/to_plot" using 4 with line title "actual[1]",\
     "/tmp/to_plot" using 5 with line title "actual[2]",\
     "/tmp/to_plot" using 6 with line title "actual[3]"
FOO

eog /tmp/graph.png

