<div align="center"><img src="logo.png"  width="300" height="200">

</br>

# **MEMORIA PRÁCTICA 3 - INTELIGENCIA ARTIFICIAL**
## Búsqueda con Adversario (Juegos)
### El Parchís (con un twist) </div>

> **Alumno**: Quintín Mesa Romero

>  **Curso**: 3º DGIIM
  
>  **DNI**: 78006011Q
  
>  **Correo**: quintinmr@correo.ugr.es

</br>
</br>

## Introducción

En el presente documento se ha elaborado un análisis del problema que se aborda en la práctica en el que se ha especificado cuál es el objetivo del mismo y se ha especificado, sin entrar demasiado en detalle, cómo se ha abordado. A continuación, se ha descrito, ahora sí con más detalle, la solución que se ha planteado para resolver el problema.

## Análisis del Problema

En esta práctica se nos pide desarrollar un programa que implemente un algoritmo MINMAX o Poda Alpha-Beta con objeto de derrotar a 3 Ninjas jugando al famoso juego del parchís. Además, se nos pide definir una heuristica adecuada que, al incorporarla al algoritmo de Poda que hemos implementado, logremos haber creado un buen jugador inteligente de parchís.

Para la elaboración del algoritmo de poda alpha-beta, he seguido las directrices establecidas por la profesora durante la resolución de problemas de esta índole.

La heurística que he definido para la práctica otorga un valor a los nodos hoja del árbol con base a cómo de bueno es para el jugador que nos representa el estado de las fichas en el tablero.

## Descripción de la solución planteada

**Algoritmo Poda Alpha-Beta**: el algoritmo que se ha implementado nos devuelve, a parte de la valoración minmax del juego, gracias a los pasos por referencia de los parámetros, la siguiente acción a realizar por el jugador: la ficha que tenemos que mover, y cómo. Destaco que se ha empleado un iterador de la clase ParchisBros para recorrer los hijos del estado actual del tablero y aplicar la poda a los mismos. El desarrollo del algoritmo en pseudicódigo sería más o menos el siguiente:

'''

    funcion podaAlphaBeta (estado (nodo), jugador, profundidad, profundidad maxima, color ficha, ficha, dado, alpha, beta)

    si profundidad = profundidad maxima o si la partida ha terminado
        devolver el valor de la heuristica del nodo
    si jugador es el jugador actual
        para cada hijo del nodo actual
            alpha := max(alpha, podaAlphaBeta(hijo,jugador,profundidad+1,profundidad maxima, color, ficha, dado, alpha, beta))

            actualizamos valores de color, ficha y dado con funciones del iterador.

            si beta <= alpha
                break (poda beta)

        devolver alpha

    si no
        para cada hijo del nodo actual
            beta := min(beta, podaAlphaBeta(hijo, jugador, profundidad+1, profundidad maxima, color, ficha, dado, alpha, beta))
            si beta <= alpha
                break (poda alpha)

        devolver beta


'''

Se han ejecutado los test proporcionados por el bot de Telegram para probar la poda alpha-beta. Se han ejecutado los siguientes comandos:

* ./build/ParchisGame --p1 AI 0 J1 --p2 AI 0 J2
* ./build/ParchisGame --p1 AI 0 J1 --p2 Ninja 1 J2
* ./build/ParchisGame --p1 Ninja 1 J1 --p2 AI 0 J2
* ./build/ParchisGame --p1 AI 0 J1 --p2 Ninja 2 J2
* ./build/ParchisGame --p1 Ninja 2 J1 --p2 AI 0 J2
* ./build/ParchisGame --p1 AI 0 J1 --p2 Ninja 3 J2
* ./build/ParchisGame --p1 Ninja 3 J1 --p2 AI 0 J2

En todas las ejecuciones se consigue el esquema esperado según el bot.

**Heurística empleada**: 

* *Heurística*: La heurística que he definimos posee una estructura simétrica entre jugador y adversario pues se basa en beneficiar y perjudicar en valor de heurística a movimientos del jugador y del adversario en función de si dichos movimientos acercan al jugador a la victoria o lo alejan y lo mismo con el adversario pero al contrario, es decir, si un movimiento beneficia al jugador, perjudica al adversario en la misma medida, y si perjudica al jugador, beneficia al adversario.

Se han tenido en consideración varias cosas a la hora de determinar el valor de la heurística:
    
* La distancia de las fichas a la meta se ha de tener en cuenta: cuanto más cerca estén de la meta, más valor tiene esa ficha frente a otra que esté más lejos.

* El número de fichas que tengamos en la meta frente a las que tenga el adversario.

* Si tenemos fichas en la casa, las fichas que estén en terreno de juego corren más peligro, por tanto, se da menos valor a las que estén en casa. De la misma forma, las fichas que estén en zona segura, tendrá más valor que otras.

* Si el jugador tiene dados especiales, se obtienen las fichas con las que los puedo aplicar y doy más valor a esas fichas con las que puede usar los dados frente a otras. En el caso de que el adversario tenga dados especiales, perjudico dichas fichas.


Esta es la única heurística que he implementado. Logra vencer al Ninja 1 tanto con Jugador 1 como con Jugador 2 y al Ninja 2 con Jugador 1.

## Comentarios

> Pese a todo el tiempo que hemos tenido, he de reconocer que me he puesto a trabajarla en serio en los últimos días antes de la entrega. Se que podría haber mejorado mucho más la heurística pero la carga de trabajo que hemos tenido y los exámenes que tenemos limitan mucho el tiempo que he podido dedicar, no solo a IA, sino a todas las asignaturas en general. Creo que es lo malo del DGIIM: es tal la carga de trabajo que tenemos y la cantidad de parciales, etc. que hacen que no podamos dedicarle el tiempo que requiere a cada cosa, vamos picando de un lado y de otro salvando entregas y exámenes y al final, no logramos los resultados que en verdad podríamos a ser capaces de alcanzar.






