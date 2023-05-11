#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ArduinoBLE.h>

BLEService ledService("180A");
BLEByteCharacteristic switchCharacteristic("2A57",  BLEWrite);

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define Servo1 0
#define Servo2 1
#define Servo3 2
#define Servo4 3
#define Servo5 4
#define Servo6 5
#define Servo7 6
#define Servo8 7
bool primerCiclo;


Adafruit_PWMServoDriver placa1 = Adafruit_PWMServoDriver(0X40);

void setup() {
  Serial.begin(9600);
  placa1.begin();
  placa1.setOscillatorFrequency(27000000);
  placa1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(500);
  Serial.println("SPO-TICO LISTO");
  //En este punto el robot debería estar en posición de echado
  posicionDescanso();
  delay(1000);
  posicionFirme(); 
  
  if (!BLE.begin()) {
    Serial.println("El blutú falló");
     digitalWrite(LED_BUILTIN, HIGH);         
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Soy yo !!! No me ignores");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characteristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Ya estoy por aquí...disponible, como si no tuviera nada que hacer... ");
  
  
}



/*
**********************************************************************
*                       EL LOOP
**********************************************************************
*/
void loop() {

  BLEDevice central = BLE.central();


  if (central) {
    Serial.print("Me conecté a este teléfono-> ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        switch (switchCharacteristic.value()) {   // any value other than 0
          case 01:
            Serial.println(switchCharacteristic.value());
            //Avanza   
            avanza();        
            break;
          case 02:
              Serial.println(switchCharacteristic.value());
             //retrocede
             posicionFirme();
            break;
          case 03:
            Serial.println(switchCharacteristic.value());
           // Gira derecha
           posicionDescanso();
            break;

 
          default:
            Serial.println((switchCharacteristic.value()));
            posicionFirme();
            break;
        }
      }
    }

    // Cuando se desconecta...
    Serial.print(F("Me desconecté... buuuu: "));
    Serial.println(central.address());
   
  }

    
  
 

}

/*
**********************************************************************
*                       Posición 1 - Patas negras horizontales y blancas en posición final
**********************************************************************
*/

void avanza(){
   int velocidad = 8;
  //Lado derecho delantero // 0: posición inicial, 1: posicionFinal, 2: velocidad, 3:motor


  // Inicio de cadencia de dos patas a la vez

  sumar(130, 180, velocidad, 1);
  sumar(100, 150, velocidad, 7);
  sumar(100, 130, velocidad, 0);
  restar(110, 90, velocidad, 6);
  restar(180, 130, velocidad, 1);
  restar(150, 100, velocidad, 7);
  restar(370, 320, velocidad, 5);
  restar(500, 470, velocidad, 3);
  sumar(400, 450, velocidad, 4);
  restar(500, 460, velocidad, 2);
  sumar(320, 370, velocidad, 5);
  sumar(470, 500, velocidad, 3);
  delay(500);
  //todos los muslos para atras
  posicionFirme();
}


void posicionDescanso(){  //todo horizontal
 moverPata(0,325, 0);
 moverPata(6,325, 0);
 
 moverPata(2,275, 0);
 moverPata(4,230, 0);

 moverPata(1,130, 0);// final... la  pata queda casi vertical 
 moverPata(3,500, 0);
 moverPata(5,350, 0);
 moverPata(7,100, 0);
}



void posicionFirme(){
   //Brazos
   Serial.println("Firme");
  moverPata(1,130, 0);// final... la  pata queda casi vertical moverPata(1,300);// echado... en este caso la pata queda horizontal
  moverPata(3,500, 0); //moverPata(3,350);// echado... en este caso la pata queda horizontal
  moverPata(5,370, 0);
  moverPata(7,100, 0);

 //Muslos
  moverPata(0,100, 0);
  moverPata(2,500, 0);
  moverPata(4,400, 0);
  moverPata(6,110, 0);//90
}


void sumar(int posInicial, int posFinal, int velocidad, int motor){
  for(int i=posInicial; i<=posFinal; i=i+velocidad){
    Serial.print("Valor de i: ");
    Serial.println(i);
    moverPata(motor,i, velocidad);
  }
}


void restar(int posInicial, int posFinal, int velocidad, int motor){
for(int i=posInicial; i>=posFinal; i=i-velocidad){
    Serial.print("Valor de i: ");
    Serial.println(i);
    moverPata(motor,i, velocidad);
  }
}




  
/*
**********************************************************************
*                       WALK
**********************************************************************
*/

void moverPata(int pata, int grado, int espera){
  placa1.setPWM(pata, 0, grado);
  Serial.print("Pata: ");
  Serial.print(pata);
  Serial.print(", está en: ");
  Serial.println(grado);
  delay(espera);
}



/*
**********************************************************************
*                       SIT
**********************************************************************
*/

void sit(){
  //Motores grandes
  moverPata(Servo1, 500, 0);
  moverPata(Servo3, 200, 0);
  moverPata(Servo5, 500, 0);
  moverPata(Servo7, 200, 0);

  //Motores pequeños
  moverPata(Servo2, 110, 0);
  moverPata(Servo6, 110, 0);
  moverPata(Servo4, 500, 0);
  moverPata(Servo8, 500, 0);
}
