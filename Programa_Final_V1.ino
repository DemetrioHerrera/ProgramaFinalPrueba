//GM Robótica
//-----------------Orientación Robot----------------------------
/*En este programa se rienta al robot Mediante el uso de una brújula
  HMC5883L, la cual se comunica al Arduino mediante comunicacion 
  I2C. Y  Se logra el control de los motores mediante funciones.
*/ //20/10/15
//Para encontrar la libreria HMC5883L.h revise el siguente Link:
  //http://bildr.org/2012/02/hmc5883l_arduino/
/*Ubicación Motores{
              ...    ..       
          M1 .   \../  . M2   
            .           .     
            .           .     
             .         .      
              .........       
                  M3          
}*/
// Por Luis Demetrio Herrera Cobos
//¬¬¬¬¬Brújula¬¬¬¬¬¬¬
  #include <Wire.h>  //Incluye las librerias para la brujula:
  #include <HMC5883L.h> //Wire y HMC5883L.
  HMC5883L brujula; //declaramos la variable
  int error = 0; // para detectar algun error en la brujula
  float AnguloInicial = 0; // Deckara Angulo Inicial y R1
  float R1 = 0;     //Como valores flotantes
//¬¬¬¬¬Motores¬¬¬¬¬¬¬
  //Definicion de enables    
  #define E1 10
  #define E2 3
  #define E3 5
  //Salidas del arduino al Motor, Control del motor   
  #define M1PI 12             //In 1 Driver 1
  #define M1PD 26             //In 2 Driver 1
  #define M2PI 44             //In 3 Driver 2
  #define M2PD 42             //In 4 Driver 2
  #define M3PI 8              //In 3 Driver 1
  #define M3PD 7              //In 4 Driver 1
  //Velocidad Base con la que se calibrara 
  #define vel 200
  //Cal(#deMotor)(Dirección)(Calibracion[Porcentajes de 200])
  #define CalM1AR  .75
  #define CalM2AR  .585
  #define CalM3AR  0
      
  #define CalM1AtR  .75
  #define CalM2AtR  .585
  #define CalM3AtR  0
      
  #define CalM1IR  .3
  #define CalM2IR  .3
  #define CalM3IR  .75
      
  #define CalM1DR  .3
  #define CalM2DR  .3
  #define CalM3DR  .75

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||    
void setup() {
  //Definicion de pines para el Motor
    DDRB = 0B11010000;
    DDRE = 0B00101000;
    DDRH = 0B00110000;
    DDRL = 0B10001000;
  //Comandos Brujula
    Wire.begin(); // iniciamos el interfaz I2C
    brujula = HMC5883L(); // iniciamos la instancia de la brujula
    error = brujula.SetScale(1.3); // indicamos la escala que vamos a usar
     if(error != 0){
       Serial.println(brujula.GetErrorText(error));
     }
    error = brujula.SetMeasurementMode(Measurement_Continuous);
    if(error != 0){
      Serial.println(brujula.GetErrorText(error));
   }
   AnguloInicial = PromedioAngulo(); //obtiene el valor inicial de la brujula
   R1 = (180 - AnguloInicial);  // a ese valor le resta 180
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//Motores
  //Control Motor 
    void Motor (int motor, int dir, int velocidad){
      if(motor==1 && dir==1){
      digitalWrite(M1PD,1);
      digitalWrite(M1PI,0);
      analogWrite(E1,velocidad);
      }
      if(motor==1 && dir==0){
        digitalWrite(M1PD,0);
        digitalWrite(M1PI,1);
        analogWrite(E1,velocidad);
      }
      if(motor==2 && dir==1){
        digitalWrite(M2PD,1);
        digitalWrite(M2PI,0);
        analogWrite(E2,velocidad);
      }
      if(motor==2 && dir==0){
        digitalWrite(M2PD,0);
        digitalWrite(M2PI,1);
        analogWrite(E2,velocidad);
      }
      if(motor==3 && dir==1){
        digitalWrite(M3PD,1);
        digitalWrite(M3PI,0);
        analogWrite(E3,velocidad);
      }
      if(motor==3 && dir==0){
        digitalWrite(M3PD,0);
        digitalWrite(M3PI,1);
        analogWrite(E3,velocidad);
      }
    }
  //Direcciónes
    void direccion(int direccion, int velocidad){
      if(direccion == 1){                         //Adelante
        Motor (1, 1, (velocidad*CalM1AR));
        Motor (2, 0, (velocidad*CalM2AR));
        Motor (3, 1, 0);
      }
      else if(direccion == 2){                   //Atras
        Motor (1, 0, (velocidad*CalM1AtR));
        Motor (2, 1, (velocidad*CalM2AtR));
        Motor (3, 0, 0);
      }
      else if(direccion == 3){                   //Derecha
        Motor (1, 1, (velocidad*CalM1DR));
        Motor (2, 1, (velocidad*CalM2DR));
        Motor (3, 0, (velocidad*CalM3DR));
      }
      else if(direccion == 4){                  //Izquierda
        Motor (1, 0, (velocidad*CalM1IR));
        Motor (2, 0, (velocidad*CalM2IR));
        Motor (3, 1, (velocidad*CalM3IR));
      }
    }
  //Alto
    void alto(){
      Motor (1, 1, 0);
      Motor (2, 1, 0);
      Motor (3, 1, 0);
    }
  //Diagonales
    void diagonal(int Diagonal, int velocidad){
      if(Diagonal == 1){         //Derecha Adelante
        Motor (1, 1, velocidad);
        Motor (2, 0, 0);
        Motor (3, 0, velocidad);
      }
      else if(Diagonal == 2){    //Derecha Atras
        Motor (1, 0, 0);
        Motor (2, 1, velocidad);
        Motor (3, 0, velocidad);
      }
      else if(Diagonal == 3){
        Motor (1, 0, velocidad);
        Motor (2, 1, 0);
        Motor (3, 1, velocidad);
      }
      else if(Diagonal == 4){
        Motor(1, 1, 0);
        Motor(2, 0, velocidad);
        Motor(3, 1, velocidad);
      }
    }
  //Giros
    void giro(int Giro, int velocidad){
      if(Giro ==1){
        Motor (1, 1, velocidad);
        Motor (2, 1, velocidad);
        Motor (3, 1, velocidad);
      }
      else if (Giro == 0){
        Motor (1, 0, velocidad);
        Motor (2, 0, velocidad);
        Motor (3, 0, velocidad);
      }
    }
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//Brujula
  float obtenerangulo(){
    MagnetometerRaw raw = brujula.ReadRawAxis(); // recibimos el valor del RAW sin escalar
    MagnetometerScaled scaled = brujula.ReadScaledAxis(); // recibimos el valor escalado con la configuracion eleg
    float angulo = atan2(scaled.YAxis, scaled.XAxis) * (180 / 3.14159265) + 180; // arcotangete x y. sacamos el angulo en grado
    return angulo;
  }
  float PromedioAngulo (){
    float SumaAngulo = (obtenerangulo() + obtenerangulo() + obtenerangulo() + obtenerangulo() + obtenerangulo()+ obtenerangulo() + obtenerangulo() + obtenerangulo() + obtenerangulo() + obtenerangulo());
    float PromedioAngulo = (SumaAngulo / 10);
    return PromedioAngulo;
    }
  float Anguloa (){
    float AnguloActual = 0;
    float Angulo = PromedioAngulo ();
    AnguloActual = (Angulo + R1);
    if (AnguloActual < 0){
      AnguloActual = (AnguloActual + 360);
    }
    return AnguloActual;
  }
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//Orientacion
  void orientacion (){
    float angulo = Anguloa();
    while(angulo < 170 || angulo > 190){
      if(angulo < 169 && angulo > 150){
        giro(1,100);
      }
      else if(angulo < 210 && angulo > 191){
        giro(2,100);
      }
      else if(angulo < 149 && angulo > 10){
        giro(1,200);
      }
      else if(angulo < 211 && angulo >350){
        giro(2,200);
      }
    angulo = Anguloa();
    }
  alto(); 
  } 
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void loop() {
  giro(0,200);
}
