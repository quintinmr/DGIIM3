#!/bin/bash

if [ $# != 2 ] then 
	echo "ERROR EN EL NUMERO DE ARGUMENTOS"
else
    mkdir bonobuses
    cd bonobuses
    for i in {1..$2}; do
    	./$1 > bonobus_$i.txt
    done
fi
    	
    	
