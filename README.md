# StepperSR
Esta librería permite controlar múltiples motores paso a paso utilizando registros de desplazamiento
Para poder usarla, debes tener Un registro de desplazamiento compatible como el CD4094 o el 74HC595 y conectar el pin de datos, reloj y latch.

Si usas muchos registros de desplazamiento, puedes concatenarlos para tener hasta 8 registros en cascada (16 motores paso a paso). La cantidad maxima de registros concatenados puede ampliarse modificando una variable en el codigo, pero no lo recomiendo, ya que mientras mas registros haya, menor sera la velocidad de actualización.
