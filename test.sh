#!/bin/bash
SOURCE="p1.c"
EXECUTABLE="p1"
FLAGS="-std=gnu99 -Wall -Wextra -pedantic"

if [ -f $EXECUTABLE ]; then
	rm $EXECUTABLE
fi


echo "COMPILATION ---------------------------------------"
#preklad c zdrojaku
mpicc $FLAGS $SOURCE -o $EXECUTABLE
echo "---------------------------------------------------"

if [ -f $EXECUTABLE ]; then
	#pocet cisel bud zadam nebo 10 :)
	if [ $# -lt 1 ];then 
	    numbers=10;
	else
	    numbers=$1;
	fi;
	#vyrobeni souboru s random cisly
	dd if=/dev/random bs=1 count=$numbers of=numbers
	#spusteni
	((numbers++))
	mpirun -np $numbers $EXECUTABLE
	#./$EXECUTABLE
fi

#uklid
if [ -f $EXECUTABLE ]; then
	rm $EXECUTABLE numbers
fi