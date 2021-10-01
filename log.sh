#!/bin/sh
#cat log.txt | grep "cli id: 0" > log0.txt
#cat log.txt | grep "cli id: 1" > log1.txt
#cat log.txt | grep "cli id: 2" > log2.txt
#cat log.txt | grep "cli id: 3" > log3.txt
#cat log.txt | grep "cli id: 4" > log4.txt
#cat log.txt | grep "cli id: 5" > log5.txt
#cat log.txt | grep "cli id: 6" > log6.txt
#cat log.txt | grep "cli id: 7" > log7.txt
#cat log.txt | grep "cli id: 8" > log8.txt
#cat log.txt | grep "cli id: 9" > log9.txt

for i in 1 2 3 4 5 6 7 8 9 10 11 12
do
    tmp="cli id: $i"
    echo $tmp;
    cat log.txt | grep "$tmp" > log$i.txt;
    #cat log.txt | grep "cli id: $i" > log$i.txt
done
