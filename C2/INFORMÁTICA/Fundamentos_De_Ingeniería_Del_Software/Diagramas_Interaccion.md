# ELABORACIÓN DE UN MODELO DE INTERACCIÓN DE OBJETOS

Patrones GRAPS
> 
    Experto en Información: 

        Ante el problema de cuál es el principio general para asignar   
        responsabilidades a los objetos --> Asignar responsabilidad a la clase que 
        contiene la información necesaria para llevarla a cabo.

    Creador:

        ¿Quién debería ser el responable de la creación de una instancia de alguna 
        clase?
        Asignar a la clase B la responsabilidad de crear instancias de A cuando:

        - B agrega objetos de A
        - B contiene objetos de A
        - B registra objetos de A
        - B utiliza objetos de A
        - B tiene datos de inicialización de A

    Bajo Acoplamiento:

        ¿Cómo soportar bajas dependencias, bajo impacto del cambio e incremento de 
        la reutilización?
        Asignar una responsabilidad de manera que el acoplamiento permanezca bajo.

    Alta Cohesión:

        ¿Cómo mantener la 
        complejidad manejable?
        Asignar una responsabilidad de manera que la cohesión permanezca alta. No 
        es lo mismo un módulo que implemente muchas funciones que otro que 
        implementa pocas pero relacionadas entre sí.
        (Se tiende a una cohesión moderada, más que alta)

    Controlador:

        ¿Quién debe ser responsable de gestionar un evento de entrada al sistema 
        (generado por un actor externo y asociado con operaciones del sistema)?
        Asignar la responsabilidad de recibir o manejar un mensaje del sistema a 
        una clase que represente:

        - El sistema global, dispositivo o subsistema (controlador de fachada).
        - El escenario de caso de uso en el que tiene lugar el evento del sistema 
        (controlador de caso de uso).


## Procedimiento para la elaboración de un modelo de interacción de objetos

Son necesarios los **contratos** y del **mapa conceptual** del sistema.

El **modelo conceptual** sirve para ver cómo se relacionan los objetos en una operación.

Las **postcondiciones, excepciones y salidas** tienen que satisfacerse en el diagrama de comunicacion.

Mínimo se tienen que aplicar los patrones:

- Controlador
- Creador
- Experto en información

### PASOS

1. Tener delante el diagrama conceptual y el contrado de la operación. En el contrato de la operación aparecen los conceptos a tener en cuenta para poder diseñar el diagrama.
2. Seleccionar en el modelo conceptual la parte del mismo en la que aparecen los conceptos relacionados con los objetos que intervienen en la interacción.
3. Representar las relaciones del controlador con los objetos que intervienen en la interacción:

    3.1 Elegimos el controlador: la operación (o evento) que tenemos en el contrato se dirige hacia una clase, que puede ser el sistema, por ejemplo.

    3.2 Aplicar patrón experto en información: Nos preguntamos, ¿qué objetos necesita conocer directamente el controlador?

    3.3 Representamos las relaciones de dichos objetos con el controlador.

    3.4 Asignar responsabilidades a objetos:

        3.4.1 Para cada objeto nos debemos preguntar: De todo lo que se dice en el contrato, ¿de qué es responsable? La respuesta es aplicar el patrón de diseño experto en información y creador. Ejemplo: el sistema en la operación de "definirProyecto" (ejemplo de clase), es responsable de verificar las excepciones (experto en información) y de cumplor con la primera postcondición (el creador(crear el proyecto)).

        En definitiva, tenemos que poner las cabeceras de las funciones que realiza cada clase hacia otra con la que se relaciona.

        3.4.2 Poner las flechitas que indican la dirección de los mensajes.

4. Establecer tipos de enlaces entre objetos: Estereotipo de visibilidad:

    > Visibilidad es la capacidad de un objeto de "ver" o tener una referencia a otro objeto

    <b>
        
        Visibilidad de atributo (B es un atributo de A)--> "<<A>>"

        Visibilidad de parámetro (B es un parámetro de un método de A) --> "<<P>>"

        Visibilidad local (B es un objeto local en un método de A) --> "<<L>>"

        Visibilidad global (B es de algún modo visible globalmente) --> "<<G>>"

        
    </b>

    Para determinar el tipo de visibilidad entre un objeto A y un objeto B, debemos responder a estas preguntas:

        - ¿El objeto de la clase A conoce al objeto de la clase B solo para esta operación?

            - SÍ --> Parámetro o local

                - ¿El objeto de la clase B ha entrado como parámetro de la operación?

                    - SÍ --> <<P>>
                    - NO --> <<L>>

            - NO --> global o asociación

                - ¿El objeto de la clase B se necesita conocer fuera del ámbito 
                del objeto de la clase A?

                    - SÍ --> <<G>>
                    - NO --> <<A>>


>  Consideraciones:
<b>
    
    1. Cuando el sistema accede a un multiobjeto, accede a todos los elemenntos del sistema.
    2. Si el que accede es una clase, solo accede a los que se relacionan con ella.
    3. El estereotipo "{new}" indica que se crea un nuevo enlace con el objeto (o con cada objeto de un multiobjeto) con el que esta relacionado.
    4. Establecer dobles enlaces si es necesario.

</b>

