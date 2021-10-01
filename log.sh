#!/bin/sh

for i in 1 2 3 4 5 6 7 8 9
do
    tmp="cli id: $i"
    log="log$i.txt"
    echo $tmp;
    cat log.txt | grep "$tmp" > $log;
done
