# Test-Ethernet-IPv4

El Arduino Test Ethernet IPv4  (a partir de ahora Test Ethernet) es un proyecto que surgió de la necesidad de poder probar el cableado y funcionamiento de una red Ethernet. Su función principal es la de verificar la conexión mediante un cliente DHCP y posteriores pruebas de ping a la información obtenida. Posteriormente se añadió pruebas de DNS y calidad de la salida a Internet. 

Con estas sencillas pruebas se puede confirmar que por lo menos el cable a probar sincroniza y da enlace, además de probar la correcta configuración del puerto del switch (si se usan VLAN), así como los servicios esenciales de la propia red (DHCP, DNS). 

Esto es algo básico y esencial a probar en una red, pero siempre ha sido necesario disponer de un ordenador, un portátil o un equipo de pruebas de red comercial (muy caros, por cierto). Con el Test Ethernet evitamos necesitar esos equipos y los problemas que suelen dar al realizar las pruebas, como la lentitud de adquisición del DHCP o los problemas que suelen dar las pilas TCP/IP en los sistemas operativos ante conexiones y desconexiones constantes de la interfaz Ethernet. El Arduino Test Ethernet realiza estas pruebas en apenas unos segundos y sin la mediación de ningún sistema operativo.

### Materiales necesarios

A continuación, pongo los materiales necesarios para el montaje de un Test Ethernet. 
Alguna de las piezas puede tener varias configuraciones posibles, se indicarán con un asterisco.

Los dispositivos necesarios son:
#### Pantalla 2.8” TFT LCD HX8347 Shield (puede valer otra como las de 2.4”, pero habría que editar el sketch y probar si es compatible con la librería usada).

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen1.png "")
![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen2.png "")

#### Arduino Uno + Shield Ethernet W5100 *
 
![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen3.jpg "")

*Arduino Uno + Shield Ethernet W5100 puede ser sustituido por un Arduino Ethernet:
 
![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen4.jpg "")
 
Es la unión de dos en uno, ocupando menos espacio, aunque con el inconveniente de ser más caro que los otros dos juntos (dependiendo donde se adquieran) y la necesidad de tener que adquirir un cable FTDI 5V a mayores para poder subir el programa. 

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen5.jpg "")

#### Shield Alimentación LiPo + Batería LiPo *
 
![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen6.jpg "")
![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen7.jpg "")

Si queremos que el equipo sea portátil, necesitaremos alimentarlo de alguna manera. Una opción son las Shield de alimentación, la ventaja es la fácil implantación en el conjunto, así como la posibilidad de ser recargables. La principal desventaja es el precio de la propia Shield.

*Puede ser sustituido por una pila o conjunto de pilas de 9V y un interruptor. Quedaría bien si se pudiese añadir o acoplar a una caja para poder imprimirla en 3D (esto está aún en desarrollo)


#### Pines Hembra Arduino (Opcional)

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen8.jpg "")
 
Para evitar que quede mal ajustada la pantalla con el conector Ethernet se pueden usar unos pines Arduino como extensores a fin de evitar el contacto entre las placas y evitar posibles cortocircuitos.


Además de estos materiales será necesario disponer de un ordenador con un puerto USB así como del IDE Arduino Configurado (recomendable, aunque se puede usar otro, añadiendo las librerías necesarias).

### Montaje

En esto no hay mucha ciencia, es tan sencillo como ir uniendo Shield al Arduino Uno (o Arduino Ethernet) finalizando el sándwich con la Pantalla 2.8”. Siempre evitando que las Shield se lleguen a tocar o que queden forzadas (en ese caso usar los Pines Hembra Arduino, viendo si es necesario en el caso, acortar los pines para que quede más compacto)
Al final tendrá que quedar más o menos como esto:

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen9.jpg "")
 
O si se usa un Arduino Ethernet:
 
![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen10.jpg "")


### Subir programa 

Una vez montado ya solo queda configurar y subir el programa al Arduino. Para ello se recomienda usar la versión del IDE Arduino Configurado, ya que dispone de las librerías modificadas para el correcto funcionamiento del programa. 

Está disponible en: https://1drv.ms/u/s!At1AwrxoRqguw-Y5gXUSvRbdpju0hA

* No es más que un arduino-1.8.6 configurado en modo portable (simplemente añadido un directorio con el nombre portable en raiz del directorio) y simplificadas las librerías a las mínimas necesarias.

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen20.png "")

Para poder extraer alguna información de las librerías oficiales de Arduino ha sido necesario modificarlas para tal fin. Pero, por el hecho de modificarlas, ya no es compatible con otros programas que usen esas librerías y genera errores. Con lo cual solo quedan dos opciones, o sustituir temporalmente las librerías originales por las librerías modificadas para subir el programa (luego volviendo a restablecer las originales) o usar el “arduino-1.8.6_Test_Ethernet_IPv4”, que ya incluye las librerías y está comprobado su correcto funcionamiento.
En esta guía se centrará en el uso de “arduino-1.8.6_Test_Ethernet_IPv4”, por ser la manera más fácil de actuar.
Para subir el programa solo hay que conectar el Arduino Uno (o Arduino Ethernet con el cable FTDI) al equipo e iniciar el ejecutable “arduino.exe”.
Tras eso nos parecerá la ventana siguiente:

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen11.png "")
 
Lo primero de todo hay que confirmar el tipo de placa que se está usando, así como el puerto COM donde está conectado el Arduino. Para ello hay que ir a “Herramientas -> Placa” y “Herramientas -> Port” y marcar la opción que dispongamos. Suele aparecer un único puerto COMX, ese es el que hay que usar, en caso de que aparezcan varios, el de mayor número suele ser, si no probar con cada uno.

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen12.png "")
![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen13.png "")

Una vez selecciona el dispositivo Arduino correcto y el puerto, procedemos a abrir el programa a cargar. Para ello ir a “Archivo -> Abrir” y buscar el archivo “Test_Ethernet_V5.ino” dentro de la carpeta “Test_Ethernet_V5”.

Lo único que hay que cambiar, si es necesario, sería la dirección de hostname  que se usa para probar el servidor DNS y a la cual se realizarán los pings de prueba de acceso a internet. Por defecto es www.google.com y no es necesario cambiarlo para que funcione. 

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen14.png "")
 
Como se puede ver, también se puede personalizar la dirección mac (opcional), por si se quiere hacer pruebas static lease o tener varios equipos funcionando en una red simultáneamente.
Ahora solo lo que queda por que hacer es pinchar en el icono superior con una flecha a la derecha para comenzar a subir el programa al Arduino.

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen15.png "")
  
Una vez finalizado el proceso, si no hay errores o problemas, debería de aparecer el siguiente mensaje, además de mostrar texto en la pantalla.

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen16.png "")

Ahora en la pantalla debería de ponerse negra con texto en blanco.

### Cómo funciona 

Una vez montado subido el programa y encendido debería de aparecer en la pantalla lo siguiente:
 
![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen17.png "")

En este momento el Arduino está ya funcionando y realizando el test. Es esta situación el Test Arduino está funcionando el cliente DHCP y está esperando respuesta del servidor DHCP. 

Si el cable no está conectado, defectuoso, o el switch está mal configurado o no están funcionando el servicio DHCP, aparecerá la siguiente pantalla. 

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen18.png "")

Es esta se indica que no se ha encontrado servidor DHCP operativo o correcto. Para verificar si el problema es el cable o el servicio DHCP, se puede observar si los pilotos led del conector RJ45 están encendidos. En caso de no estar encendidos (tanto el verde como el amarillo) el problema probablemente esté localizado en el cable o en la boca del Switch. Si en caso contrario están encendidos y no obtiene IP, las causa más probable será o una errónea configuración del switch o del servidor. Tras 30 Segundos se reiniciará la prueba.

Si la prueba es satisfactoria aparecería la siguiente pantalla:

![alt text](https://github.com/GuillermoElectrico/Test-Ethernet-IPv4/blob/master/Documentacion/Imagenes/imagen19.png "")
 
* Los textos en Blanco son elementos de prueba.
* Los elementos en Amarillo son la información obtenida del test.
* Los elementos en Verde son los resultados satisfactorios del test.*
* Los elementos en Rojo son los resultados fallidos del test.*
* Los elementos en Azul son información adicional al test satisfactorio verde. 

*Principalmente los test Verde o Rojo son resultados de un test de Ping ICMP al elemento Amarillo.

Para reiniciar el test, solo habría que pulsar el boton reset de la placa Arduino.

FIN 😉.

