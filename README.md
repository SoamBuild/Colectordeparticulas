# Colectordeparticulas (CAPADA)

La monitorización de partículas aéreas constituye una variable ambiental de gran importancia cuando se evalúa la calidad de vida de un grupo humano que habita en una determinada localización geográfica, es por esta razón que el desarrollo de dispositivos que permitan cuantificar y colectar muestras del material particulado (PM) presente en el aire resulta ser de especial interés para la posterior elaboración de medidas de mitigación y la toma de decisiones que permitan mejorar la salud de las personas. La automatización de dichos dispositivos mediante el uso de microcontroladores y actuadores de bajo costo (en comparación a equipos comerciales presentes en el mercado) permite prescindir de la tarea manual que normalmente debería realizar un especialista del área, siendo este aspecto particularmente útil en zonas extremas, además permite estandarizar de forma más uniforme la colección de las muestras, mediante la minimización del error humano.

# Code

Dentro de este repositorio existen dos ramas, dev_iman y dev

## Dev_iman 
Se basa en el uso de imanes para el movimiento de los recipientes del colector, los imanes cumplen la función de un encoder logrando identificar la ubicación de cada una de estas y logrando a la vez con precisión encontrar el punto indicado para el rellenado de cada recipiente.

## Dev 
Se basa en el uso de las distancias para calcular la posición de cada botella, sin embargo hace uso de un punto de referencia (Home) y de los imanes, además de una matriz que le permite identificar y asegurar la posición de cada botella. Para esto el microcontrolador revisa la posición de cada botella / busca un imán de referencia / revisa la matriz, con esto logra determinar y confirmar la ubicación de cada recipiente para el rellenado, además registra en la matriz el estado de cada botella.

# Electrónica
Para conocer los componentes en detalles del colector **CAPADA**, puedes revisar el documento PIN.MD (recuerda que aún puede estar variando la electrónica)

