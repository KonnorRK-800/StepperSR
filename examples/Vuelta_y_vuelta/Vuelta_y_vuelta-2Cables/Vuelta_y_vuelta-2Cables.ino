/*
 Vuelta y vuelta

 Gira 4 motores en sentido horario para luego regresar en sentido antihorario. Esto se repite con los cuatro motores A-B-C-D de forma secuencial.
 Este ejemplo es para conectar el motor con 2 cables.

 Escrito el 28 de Junio de 2022
 por Figueroa Leonel Sebastian, Alias KonnorRK-800.
 
 Este Codigo de ejemplo es de dominio publico.
 
 */


#include <StepperSR.h>

StepperSR myStepperA(200,0,1); //Aqui declaramos los constructores para los motores.
StepperSR myStepperB(200,2,3); //La sintaxis del constructor es: StepperSR nombreInstancia(int Pasos/RevMotor, int Pin1, int Pin2,int Pin3·,int Pin4); (el primer parametro depende de los pasos que tenga que hacer el motor para dar una vuelta)
StepperSR myStepperC(200,4,5);// Los pines que configuramos aqui con los pines del registro de desplazamiento al que iran conectados al motor
StepperSR myStepperD(200,6,7);


void setup()
{
  InitSR(3,4,5,1); //Aqui inicializamos los pines de conexion de los registros de desplazamiento con el Arduino. Sintaxis: InitSR(int PinDatos,Int PinReloj,Int PinEnable,Int NºRegistrosConcatenados);
  myStepperA.SetSpeed(120);//Seleccionamos la velocidad de rotacion de los motores en 60 RPM.
  myStepperB.SetSpeed(120);  
  myStepperC.SetSpeed(120);
  myStepperD.SetSpeed(120);

}

void loop() 
{
myStepperA.Step(200); //El motor avanza 200 pasos en sentido horario.
delay(1000); // Espera un segundo.
myStepperA.Step(-200); //Retrocede de nuevo 200 pasos.

myStepperB.Step(200);// Y aqui se repite lo mismo para el motor B.
delay(1000);
myStepperB.Step(-200);

myStepperC.Step(200);
delay(1000);
myStepperC.Step(-200);

myStepperD.Step(200);
delay(1000);
myStepperD.Step(-200);
}
