#!/bin/bash

v=100
if [ $# != 2 ]; then 
	echo "ERROR EN EL NUMERO DE ARGUMENTOS"
else
    
    mkdir bonobuses
  
    for (( i=1; i <= $v; i++ )) 
    do
    	g++ $1 -o prog
    	./prog $2 > bonobus_$i.txt
    	mv bonobus_$i.txt bonobuses
    done
    
    
   
fi
    	
    	
