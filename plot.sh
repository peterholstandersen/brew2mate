#!/bin/bash

cat $@ |\
    awk '{ print $8, $9, $10, $11; }' |\
    grep actual |\
    sed 's/actual\[.\]=//g' |\
    awk 'BEGIN{ count = 0;}{ print count++, $0;}' > /tmp/to_plot

gnuplot > /tmp/graph.png <<EOF
set terminal png
plot "/tmp/to_plot" using 2 with line title "ambient",\
     "/tmp/to_plot" using 3 with line title "side1",\
     "/tmp/to_plot" using 5 with line title "side2",\
     "/tmp/to_plot" using 4 with line title "tank"
EOF

eog /tmp/graph.png

