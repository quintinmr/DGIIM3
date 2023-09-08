# PŔACTICA 2 - IA

## NIVEL 1

### Cambios respecto al nivel 0

1. Ahora tenemos que tener en cuenta más acciones: las del agente más las del sonámbulo.
2. Tenemos 2 objetivos (2 Goal).
3. El operador < tenemos que cambiarlo.
4. Apply (meter detallitos de programación sobre todo para controlar el sonámbulo).


## Nivel 2: Dijkstra agente jugador

Esto es un coste uniforme (tenemos una cola con prioridad y se ordenad respecto al coste acumulado, de tal forma que si vamos cogiendo cada vez el menor coste acumulado, al final tendremos el óptimo).

### Cambios respecto al nivel 0 y 1

1. No es anchura.
2. El estado va a tener lo mismo que en el nivel 1 más zapatillas y bikini, solo del jugador (no del sonámbulo porque no se mueve).
3. El nodo, compuesto por un estado nivel 2, y coste acumulado.
4. Respecto a acciones, las mismas que en 0 (solo se mueve el jugador; se termina cuando el jugador llega al objetivo).
5. El operador < (que se utiliza para ver si un estado es diferente de otro).

## Nivel 3: A* agente sonámbulo

Es A*, que la diferencia que hay con el coste uniforme es que ahora tenemos una heurística. Es pŕacticamente igual que el nivel 1.

### Cambios respecto al nivel 2

1. Acciones (6) las del 1.
2. Objetivo (no me he enterado pero creo que el mismo que el 1) 
3. Operador < (no me he enterado)
4. El nodo tendrá: estado, coste, heurística y suma (coste acumulado + heurística).
5. ¿Qué heurística?


> Heurísticas:
    
**Admisible**: $\forall N h(n) \leq h^{*}(N)$

**Monótona**: $ \forall P, \forall h \in Hijo(P)  h(P) - h(H) \leq C(P,H)$

Admisible $\Rightarrow$ Óptima.

Si tengo un mundo cuadriculado en el que solo me puedo mover hacia arriba, abajo o a los lados. ¿Cuál sería una heurística admisible?

La distancia Manhatan: diferencia en filas y columnas. Es siempre admisible porque el mejor de los casos es el mínimo número de pasos, y dado que el coste es uno mayor que todo movimiento.
En cambio, si me puedo mover en diagonal, puede darse que el coste sea mayor que la diagonal, entonces la distancia manhatan no es admisible para cuando me puedo mover en diagonal.

En los **misteriosos mundos de Belkan**, me puedo mover en diagonal, así que:
Para el **jugador** sí puedo utilizar la distancia Manhatan, pero para el **sonámbulo** no puedo utilizar como heurística admisible la distancia Manhatan porque él sí puede hacer movimientos de. ¿Qué se nos ocurre hacer? Chevychev (distancia del máximo).


