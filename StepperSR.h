int datos;     //Creamos la variable interna del pin de datos
int reloj;    //Creamos la variable interna del pin de reloj
int enable;     //Creamos la variable interna del pin de enable
int registros;    //Creamos la variable interna que indica el numero de registros de desplazamientos concatenados
byte Salida[8];   //Creamos el array con los datos de salida. Este array permite que se conecten hasta 8 registros de desplazamiento, pero si necesita, puede ampliar en 1 el array por cada registro extra que requiera.
const byte bits[8] = {  //Declaramos un array con los bytes que se pueden escribir en cada registro. Estos bytes se usaran al momento de modificar bytes de salida.
 
                0b10000000,
                0b01000000,
                0b00100000,
                0b00010000,
                0b00001000,
                0b00000100,
                0b00000010,
                0b00000001,
                      };

void InitSR(int _datos,int _reloj, int _enable, int _registros)
{
  
  datos=_datos;     //
  reloj=_reloj;     //
  enable=_enable;   // Pasamos los datos de los parametros del constructor a las variables internas de la funcion.
  registros=_registros;   //

  pinMode(datos,OUTPUT);  //
  pinMode(reloj,OUTPUT);  //Colocamos los pines que conectan el registro de desplazamiento como salidas.
  pinMode(enable,OUTPUT); //
  
  for(int i=0; i<registros; i++)      //
   {              //
    digitalWrite(enable, LOW);                    //
    shiftOut(datos, reloj, LSBFIRST, 0b00000000); // Colocamos todas las salidas de los registros a 0 para evitar valores aleatorios. Y4 con el bucle for lo repetimos tantas veces como registros de desplazamiento tengamos presentes.
    digitalWrite(enable, HIGH);                   //
   }              //
  
}

void RefreshMem (int _pin, int _estado)   //Funcion para escribir datos de salida en los arrays de memoria, sin enviar los datos a los registros de salida. (en el parametro "estado" solo puedes poner "1" y "0").
{
  int pin=_pin;         // Pasamos los datos de los parametros del constructor a las variables internas de la funcion.
  int estado=_estado;       //

  int RegDeAcceso=0;        //Esta variable indica a que registro se accede. Estos se enumeran de 0 a infinito XD(eso ultimo es broma). Siendo el registro 0, el primero de todos los registros.
  int PinDelReg=pin;        //Esta variable indica que pin del registro al que se accede, se debe modificar luego de realizado el calculo.

  RegDeAcceso=(pin)/8;        //Es la operacion que calcula que registro al que se debe acceder.
  PinDelReg=PinDelReg-8*RegDeAcceso;

  if(estado == 1)
   {
    Salida[RegDeAcceso] = Salida[RegDeAcceso] | bits[PinDelReg];  //Si el parametro de la funcion es 1, modificara el byte, con un 1 en el bit que corresponde al pin del registro de desplazamiento indicado.
   }
  else if (estado == 0)
   {
    Salida[RegDeAcceso] = Salida[RegDeAcceso] & ~bits[PinDelReg]; //Si el parametro de la funcion es 0, modificara el byte, con un 0 en el bit que corresponde al pin del registro de desplazamiento indicado.
   }

}

void PrntSR()  //Funcion para imprimir en los registros de desplazamiento los datos almacenados en los arrays de memoria.
{
  digitalWrite(enable, LOW);
  
  int count=registros-1;
  for(int i=0; i<registros; i++)
  { 
     shiftOut(datos, reloj, LSBFIRST, Salida[count]);
   count--;
    }

  digitalWrite(enable, HIGH);
}

class StepperSR 
{

	private:
	int PasosRestantes;
	int PasoActual;
	int PasosPorRev;
	long WaitMicroseconds;
	void Out(int);
	void ExDelayMicroseconds(long);
	int Mode;
	int Pin1;
	int Pin2;
	int Pin3;
	int Pin4;
	
	public:
	StepperSR(int,int,int,int,int);
	void Step(int);
	void SetSpeed(float);
};

StepperSR::StepperSR(int PasosPorRev_, int Pin1_, int Pin2_, int Pin3_, int Pin4_)
{
	Pin1=Pin1_;
	Pin2=Pin2_;
	Pin3=Pin3_;
	Pin4=Pin4_;
	Mode=0;
	PasosPorRev=PasosPorRev_;
	WaitMicroseconds=41666;
	PasosRestantes=0;
	PasoActual=1;
	Out(1);
}

StepperSR::StepperSR(int PasosPorRev_, int Pin1_, int Pin2_)
{
	Pin1=Pin1_;
	Pin2=Pin2_;
	Mode=1;
	PasosPorRev=PasosPorRev_;
	WaitMicroseconds=41666;
	PasosRestantes=0;
	PasoActual=1;
	Out(1);
}

void StepperSR::Step(int Pasos_)
{
	PasosRestantes = Pasos_;
	if (Pasos_ > 0)
	{
		while (PasosRestantes > 0)
		{
			if (PasoActual < 3)
			{
				PasosRestantes--;
				PasoActual++;
				Out(PasoActual);
				ExDelayMicroseconds(WaitMicroseconds);
			}
			else
			{
				PasoActual = 0;
				Out(PasoActual);
				PasosRestantes--;
				ExDelayMicroseconds(WaitMicroseconds);
			}
			
		}
	}
	else if (Pasos_ < 0)
	{
		while (PasosRestantes < 0)
		{
			if (PasoActual > 0)
			{
				PasosRestantes++;
				PasoActual--;
				Out(PasoActual);
				ExDelayMicroseconds(WaitMicroseconds);
			}
			else
			{
				PasoActual = 3;
				Out(PasoActual);
				PasosRestantes++;
				ExDelayMicroseconds(WaitMicroseconds);
				
			}
			
		}
	}
	else
	{	}
	
}

void StepperSR::SetSpeed(float RPM_)
{
	WaitMicroseconds = (1000000 / ((RPM_ / 60) * PasosPorRev));
}

void StepperSR::Out(int Out_)
{
if(Mode==0)
 {
	if (Out_ == 0) {
		RefreshMem(Pin1, HIGH);
		RefreshMem(Pin2, LOW);
		RefreshMem(Pin3, HIGH);
		RefreshMem(Pin4, LOW);
    PrntSR();
	}
	else if ((Out_ == 1)) {
		RefreshMem(Pin1, HIGH);
		RefreshMem(Pin2, LOW);
		RefreshMem(Pin3, LOW);
		RefreshMem(Pin4, HIGH);
    PrntSR();
	}
	else if ((Out_ == 2)) {
		RefreshMem(Pin1, LOW);
		RefreshMem(Pin2, HIGH);
		RefreshMem(Pin3, LOW);
		RefreshMem(Pin4, HIGH);
    PrntSR();
	}
	else if ((Out_ == 3)) {
		RefreshMem(Pin1, LOW);
		RefreshMem(Pin2, HIGH);
		RefreshMem(Pin3, HIGH);
		RefreshMem(Pin4, LOW);
    PrntSR();
	}
 }
else if(Mode==1)
 {
	if (Out_ == 0)
			{
		RefreshMem(Pin1, HIGH);
		RefreshMem(Pin2, HIGH);
    		PrntSR();
		}
	else if ((Out_ == 1)) 
		{
		RefreshMem(Pin1, HIGH);
		RefreshMem(Pin2, LOW);
    		PrntSR();
		}
	else if ((Out_ == 2))
		{
		RefreshMem(Pin1, LOW);
		RefreshMem(Pin2, LOW);
   		PrntSR();
			}
	else if ((Out_ == 3))
		{
		RefreshMem(Pin1, LOW);
		RefreshMem(Pin2, HIGH);
    		PrntSR();
		}
 }
	
}
void StepperSR::ExDelayMicroseconds(long time_)
{
	int Rep=time_/1000;
	int final=time_-(Rep*1000);
	
	for(int i=0; i<Rep; i++)
	{
		delayMicroseconds(1000);
	}
	
	delayMicroseconds(final);
}
