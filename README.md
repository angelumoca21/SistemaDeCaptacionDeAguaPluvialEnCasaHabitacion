# Sistema de captación de agua pluvial en casa habitación

## **Objetivo**

Diseñar e implementar un sistema de captación de agua pluvial beneficiando a millones de mexicanos que no cuentan con servicio de agua potable, disminuyendo la extracción de agua del subsuelo con apoyo de la tecnología, específicamente con el IoT.

## **Introducción**

Más de 10 millones de mexicanos no cuentan con servicio de agua potable. Sólo en la CDMX al menos 250 mil personas viven sin conexión a la red, y millones más tienen un servicio intermitente.
En la CDMX, el 70% del agua que usamos viene del subsuelo, la extracción desmedida ha provocado que la ciudad se hunda. 
Captar agua de la lluvia y almacenarla reduce la cantidad de agua extraída y permitir que el acuífero se recargue. La CDMX es la tercera ciudad con mayor estrés hídrico en el mundo, esto es porque extraemos mucha más agua de la que se vuelve a recargar. En las zonas donde el abasto por la red es más deficiente, la captación pluvial se vuelve de enorme importancia, evitando la compra de miles de pipas al año, ahorrando grandes cantidades de energía y dinero. El 30% del agua de la red en la Ciudad de México proviene del sistema Lerma-Cutzamala, cuyas aguas deben ser bombeadas 150 km de distancia y 1 km vertical desde sus cuencas fuera del Valle de México. Se sabe que la energía requerida para este proceso de bombeo es parecida al consumo energético total de la ciudad de Puebla.

## **Principios de funcionamiento**

*¿Cómo se conforma un sistema de captación de agua pluvial?*

**1. Área de captación**

Se compone de una superficie impermeabilizada destinada para la cosecha del agua de lluvia por medio de canaletas o dispositivos que recolecten y entreguen el agua captada hasta el inicio de la tubería de conducción.

**2.Separador de primeras lluvias**

La separación de primera lluvia es una técnica sencilla para mejorar la calidad del agua captada. Consiste en dejar pasar el primer volumen de agua que lava el techo antes de comenzar la captación. Conforme pasa más volumen de agua a través del tiempo, tanto el aire como las superficies que están en contacto con el agua estarán cada vez más limpios y el agua cada vez tendrá menor concentración de contaminantes. Su funcionamiento se puede resumir en que el agua cae por el tubo bajante que viene del techo y entra al cuerpo del separador hasta llegar a su volumen total, creando un tapón de agua y permitiendo que la que sigue cayendo se dirija directo a la cisterna. 

**3.Sistema de filtración\***

Una de las características de la captación de agua de lluvia es que ésta se recolecta y no puede ser utilizada para el consumo humano de manera inmediata; únicamente para actividades domésticas como: lavar ropa, los trastes, el aseo de interiores y exteriores, regar el jardín, el servicio del baño, etc. Sin embargo, con un buen tren de filtración puede llegar a ser de consumo humano.

**4.Almacenamiento**

Consiste en el uso de una cisterna para asegurar el abastecimiento por una temporada.

**5.Distribución**

Esto, en el proceso de captación de agua de lluvia, significa que el agua estará lista para ser utilizada, solo con abrir la llave o a donde se vaya a depositar para la actividad que se desee realizar.

![SCAP](https://github.com/angelumoca21/SistemaDeCaptacionDeAguaPluvialEnCasaHabitacion/blob/main/imagenes/Imagen1.png)

## **Material necesario**
- ESP32 y cable de alimentación
- Fuente de voltaje de 12 volts.
- Protoboards.
- Jumpers y cables.
- Sensor de lluvia.
- Sensor ultrasónico.
- Electroválvula.
- Manguera y conectores de manguera.
- Relevadores.
- Sensores flotadores.
- Bomba de agua.
- Resistencias.

## **Herramientas computacionales**
- Máquina virtual con Ubuntu.
- Computadora.
- IDE de Arduino.
- OpenHAB.
- MQTT.

## **Circuito**
![Circuito](https://github.com/angelumoca21/SistemaDeCaptacionDeAguaPluvialEnCasaHabitacion/blob/main/imagenes/Imagen2.png)

## **Lecturas de sensor**
Con el microcontrolador ESP32 se obtienen las mediciones y diversas tomas de decisiones, de igual forma se crearon algunas reglas en OpenHAB para la automatización. 

## **Envío de información**
Se utilizó MQTT como protocolo de comunicación, enviando el estatus del sensor por un tema en específico para su monitoreo desde OpenHAB.

## **Recepción de información**
Con el ESP32 suscrito a un tema en específico y la programación del microcontrolador se reciben los datos para activar o desactivar ciertos actuadores.

## **Automatización**
El sistema automatizado consta de 6 sensores: 
- **Un sensor de lluvia:** ubicado en el techo para controlar cuándo se tiene que abrir la válvula del separador de primeras lluvias.
- **Un sensor de nivel:** en el tinaco que controla el encendido y apagado de la bomba de la cisterna.
- **Un par de sensores de nivel:**  en el separador de primeras lluvias que abrirá y cerrará la válvula para drenarlo y estar listo para las siguientes lluvias.
- **Un par de sensores de flotador**: se encuentran en la cisterna para indicar el nivel mínimo y el máximo.

## **Visualización de datos:** 
Para visualización de los datos se utilizó OpenHAB, se modeló la maqueta teniendo 3 áreas donde los actuadores y sensores trabajan: techo, cisterna y contenedor de primeras lluvias.
![Interfaz]()