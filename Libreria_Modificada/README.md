# Atención

Las librerías aquí expuestas son modificaciones de las librerías Ethernet oficiales de Arduino. Lo que quiere decir, que no pueden convivir las dos a la vez en ningún IDE, ya que tecnicamente son lo mismo y dá errores.

Para usar la librería, mover o eliminar el directorio Ethernet de la carpeta libraries del IDE antes de copiar la librería modificada. Una vez subido el sketch, se puede restaurar el directorio original, previa eliminación de la librería modificada.

Actualmente hay dos versiones de la librería Ethernet, la 1.1.2 que solo funciona con las shields W5100 y la nueva 2.0.0 que añade compatibilidad a las shields W5200 y W5500

Actualmente solo funciona el sketch con la versión 1.1.2, ya que en la nueva versión 2.0.0 aún no se es posible realizar Ping ICMP. La librería ya está modificada y probada, pero sin esa función, gran parte de las pruebas no se repueden realizar. Esperando en un futuro poder hacer ICMP y actualizar la librería.