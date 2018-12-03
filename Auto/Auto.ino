//Librerías para el uso del display
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Se definen los pines que se conectan a la placa como constantes
#define pinInfrarrojo 2 //Conectado a la salida del infrarrojo
#define pinDipSw4 3 //Conectado a la salida 4 del dip switch
#define pinDipSw3 4 //Conectado a la salida 3 del dip switch
#define pinDipSw2 5 //Conectado a la salida 2 del dip switch
#define pinDipSw1 6 //Conectado a la salida 1 del dip switch
#define tonePin 8 //Conectado a la bocina
#define pinTono 8 //Conectado a la bocina
#define ledVerde 10 //Conectado a un led verde
#define ledRojo 11 //Conectado a un led rojo
#define pinLuz 12 //Conectado al transistor usado como sensor de luz con fotorresistencia

//Arreglo que contiene los pines del dip switch
int dipSwitch[] = {pinDipSw1,pinDipSw2,pinDipSw3,pinDipSw4};

//Clave del Dip Switch para verificación inicial
bool claveDipSwitch[] = {HIGH, LOW, LOW, HIGH}; //1001

//Claves para las estaciones de radio
bool claveRadio[] = {HIGH,HIGH,HIGH,HIGH}; //Por defecto en 1111, posteriormente se leerá
bool radioNavidad[] = {LOW,LOW,LOW,HIGH}; //0001
bool radioRock[] = {LOW,LOW,HIGH,LOW}; //0010
bool radioClasica[] = {LOW,HIGH,LOW,LOW}; //0100
bool radioStarWars[] = {HIGH,LOW,LOW,LOW}; //1000
bool apagarRadio[] = {LOW,LOW,LOW,LOW}; //0000

void setup() {
  //Inicia comunicación con el monitor serial
  Serial.begin(9600);
  
  //Inicializar pines como entrada o salida
  pinMode(pinInfrarrojo,INPUT); //Sensor infrarrojo
  pinMode(pinLuz,INPUT); //Sensor con transistor
  pinMode(ledVerde, OUTPUT); //Led verde
  pinMode(ledRojo, OUTPUT); //Led rojo

  //Pines del dipSwitch
  pinMode(pinDipSw1,INPUT); 
  pinMode(pinDipSw2,INPUT);
  pinMode(pinDipSw3,INPUT);
  pinMode(pinDipSw4,INPUT);
  
  pinMode(pinTono,OUTPUT); //Pin de la bocina
  
  // Indicar a la libreria que se tiene conectada una pantalla de 16x2
  lcd.begin(16, 2);

}

void loop() {
  //Mensaje de inicio al encender el auto
  inicio();
  //Para encender, se requieren dos verificaciones
  //Verificar huella con infrarrojo
  do{
    tonoIncorrecto();
    lcd.clear();
    lcd.home();
    lcd.print("Coloque su dedo");
    lcd.setCursor(0,1);
    lcd.print("sobre el lector");
    delay(3500);
  }while(verificarHuella()==false);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Huella");
  lcd.setCursor(3,1);
  lcd.print("verificada");
  tonoCorrecto();
  delay(5000);
  
  //Verificar clave de dip switch

  do{
    tonoIncorrecto();
    lcd.clear();
    lcd.home();
    lcd.print("Ingrese el");
    lcd.setCursor(0,1);
    lcd.print("codigo correcto");
    delay(3500);
  }while(verificarClave(claveDipSwitch)==false);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Clave");
  lcd.setCursor(4,1);
  lcd.print("correcta");
  tonoCorrecto();
  delay(5000);

  //Mensajes intermedios al LCD
  tone(tonePin, 196,100);
  delay(200);
  tone(tonePin, 220,100);
  delay(400);
  tone(tonePin, 246.9,400);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("ENCENDIENDO...");
  lcd.setCursor(3,1);
  lcd.print("BUEN VIAJE");
  delay(5000);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("USE CINTURON");
  lcd.setCursor(2,1);
  lcd.print("DE SEGURIDAD");
  delay(5000);

  //Activar Radio, se requiere elegir estación o apagar
  do{
    tone(pinTono, 1000,100);
    delay(150);
    lcd.clear();
    lcd.home();
    lcd.print("Ingrese una");
    lcd.setCursor(0,1);
    lcd.print("estacion radio");
    delay(4500);
    lcd.clear();
    lcd.home();
    //Se muestra en display las opciones disponibles
    lcd.print("0001 Navidad | 0100 Clasica"); //16+68
    lcd.setCursor(0,1);
    lcd.print("0010 Rock    | 1000 StarWars"); //16+68
    delay(4000);
    //Se recorre la pantalla del display para visualizar mejor las opciones de la radio
    for ( int i = 0; i < 15; i++ ) {
      lcd.scrollDisplayLeft();
      delay (400);
    }
    delay(4000);
    lcd.clear();
    lcd.home();
    lcd.print("0000 Apaga Radio");
    delay(3000);
  }while( verificarClave(radioNavidad)==false && verificarClave(radioRock)==false && verificarClave(radioClasica)==false && verificarClave(radioStarWars)==false && verificarClave(apagarRadio)==false );
  
  //Menú que reproduce una canción según la elección del usuario
  do{
    switch( claveActual() ){
    case 1:
      lcd.clear();
      lcd.home();
      lcd.print("Reproduciendo:");
      lcd.setCursor(4,1);    
      lcd.print("Navidad");
      playNavidad();
      break;
    case 10:
      lcd.clear();
      lcd.home();
      lcd.print("Reproduciendo:");
      lcd.setCursor(6,1);    
      lcd.print("Rock");
      playRock();
      break;
    case 100:
      lcd.clear();
      lcd.home();
      lcd.print("Reproduciendo:");
      lcd.setCursor(4,1);    
      lcd.print("Clasica");
      playClasica();
      break;
    case 1000:
      lcd.clear();
      lcd.home();
      lcd.print("Reproduciendo:");
      lcd.setCursor(4,1);    
      lcd.print("StarWars");
      playStarWars();
      break;
    default:
      lcd.home();
      lcd.clear();
      delay(1500);
      break;
    }
  }while(claveActual() != 0); //Sale del ciclo hasta que se apague la radio

  //Mensaje de radio apagada
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("RADIO");
  lcd.setCursor(4,1);    
  lcd.print("APAGADA");
  tonoIncorrecto();
  delay(5000);

  //Entra al ciclo mientras los faros estén apagados
  while(digitalRead(pinLuz) == LOW){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("BUEN VIAJE");
    delay(2500);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("USE CINTURON");
    lcd.setCursor(2,1);
    lcd.print("DE SEGURIDAD");
    delay(2500);  
  }
  //Sale del ciclo cuando se hace de noche y los faros se encienden
  //Los faros se activan con el sensor de luz con el transistor
  while(digitalRead(pinLuz) == HIGH){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("FAROS");
    lcd.setCursor(3,1);
    lcd.print("ENCENDIDOS");
    delay(3500);
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("MANEJE");
    lcd.setCursor(1,1);
    lcd.print("CON PRECAUCION");
    delay(3500);
  }
  //Se termina el viaje y se prepara para apagar
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("HA LLEGADO A");
  lcd.setCursor(3,1);
  lcd.print("SU DESTINO");
  delay(7000);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("APAGANDO...");
  tone(tonePin, 55, 300);
  delay(350);
  tone(tonePin, 45, 5000);
  delay(6000);
  lcd.clear();
  lcd.setCursor(2,0);
  //Despedida
  tone(tonePin, 2500, 150);
  delay(350);      
  tone(tonePin, 2500, 150);
  lcd.print("HASTA LUEGO");
  //Espera 1 minuto y luego el programa se reinicia
  delay(60000);
  cuentaRegresiva();
}

//Función saludo que se ejecuta al encender el coche
void inicio(){
  // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.setCursor(2,0);
  // Imprimir "Bienvenido a" en la primera linea
  tone(tonePin, 2500, 150);
  delay(350);      
  tone(tonePin, 2500, 150);      
  lcd.print("Bienvenido a");
  lcd.setCursor(4,1);
  lcd.print("su auto");
  // Esperar tres segundos
  delay(3000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("ARRANCANDO...");
  tone(tonePin, 45, 300);
  delay(350);
  tone(tonePin, 55, 5000);
  delay(6000);
  lcd.clear();
  lcd.home();
  lcd.print("* Se requiere *");
  lcd.setCursor(0,1);
  lcd.print("autentificacion");
  delay(3000);
  lcd.clear();
}

//Función que verifica si el dedo se mantuvo en el sensor por al menos 4 segundos
bool verificarHuella(){
  float tiempo1=0, tiempo2=0, dTiempo=0;
  int cont = 0;
  
  delay(100);
  //El ciclo se activa al entrar a LOW
  Serial.println(digitalRead(pinInfrarrojo));
  while (digitalRead(pinInfrarrojo) == LOW){
    //Se registra el tiempo desde el primer ciclo
    if (cont == 0)
      tiempo1 = millis();
    lcd.clear();
    //Imprime mensaje y emite sonido
    tone(pinTono, 720);
    lcd.print("No retirar dedo");
    lcd.setCursor(0,1);
    lcd.print("Mantenga por 4 s");
    delay(1500);
    //Registra el número de ciclo
    cont++;
  }
  tiempo2 = millis();
  
  //Termina sonido
  noTone(pinTono);
  //Calcula tiempo transcurrido
  dTiempo = (tiempo2 - tiempo1)/1000.0;
  //Verifica que el tiempo que se mantuvo el dedo una sola vez cumpla >= 4
  //Con retrasos del display y otros delays, el tiempo sensible al usuario es de 4 segundos
  if (cont > 0 && dTiempo >= 4){
    Serial.println(cont);
    Serial.println(dTiempo);
    return true;
  } else return false;
}

//Función que compara la lectura actual del dipSwitch con una clave que recibe como argumento
//Recibe apuntador al arreglo tipo booleano de la clave
bool verificarClave(bool *apClave){
  int contVerif = 0;
  //Ciclo que verifica los cuatro digitos de la clave
  //Recorre el arreglo recibido con apuntador
  //Compara el dígito correspondiente de la clave con su lectura del dipswitch
  //Se suma uno al contador por cada dígito correcto
  for(int i = 0; i < 4 ; i++, apClave++){
    if( digitalRead(dipSwitch[i]) == *apClave )
      contVerif++; 
  }  

  //retorna verdadero si los cuatro dígitos son correctos, falso en otro caso
  return (contVerif == 4);
}

//tono genérico
void tonoIncorrecto(){
  tone(pinTono, 100,100);
  digitalWrite(ledRojo, HIGH);
  delay(200);
  tone(pinTono, 100,100);  
  digitalWrite(ledRojo, LOW);
}

//tono genérico
void tonoCorrecto(){
  tone(pinTono, 1000,100);
  digitalWrite(ledVerde, HIGH);
  delay(150);
  tone(pinTono, 1000,100);  
  digitalWrite(ledVerde, LOW);
}

//Muestra una cuenta regresiva desde 5 en la segunda linea del LCD
void cuentaRegresiva(){
  lcd.setCursor(7,1);
  lcd.print("05");
  delay(1000);
  lcd.setCursor(7,1);
  lcd.print("04");
  delay(1000);
  lcd.setCursor(7,1);
  lcd.print("03");
  delay(1000);
  lcd.setCursor(7,1);
  lcd.print("02");
  delay(1000);
  lcd.setCursor(7,1);
  lcd.print("01");
  delay(1000);    
}

//Función que convierte a enteros, la clave booleana leida de la estación de radio
int claveActual(){
  if (verificarClave(radioNavidad) == true)
    return 1;
  else if (verificarClave(radioRock) == true)
    return 10;
  else if (verificarClave(radioClasica) == true)
    return 100;
  else if (verificarClave(radioStarWars) == true)
    return 1000;
  else if (verificarClave(apagarRadio) == true)
    return 0;
  else return -1;
}

/*
Las siguientes funciones y declaraciones de variables se usan meramente para reproducir
las melodías del programa. 
*/

int tempo = 200;
char notes[] = "eeeeeeegcde fffffeeeeddedg";
int duration[] = {1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};

void playJB(char note, int duration) {
  char notesName[] = { 'c', 'd', 'e', 'f', 'g' };
  int tones[] = { 261, 293, 329, 349, 392 };

  for (int i = 0; i < sizeof(tones); i++) {
    // Bind the note took from the char array to the array notesName
    if (note == notesName[i]) {
      // Bind the notesName to tones
      tone(pinTono, tones[i], duration);
    }
  }
}

void playNavidad(){
  tone(tonePin, 587, 450.0);
    delay(500.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 450.0);
    delay(500.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 450.0);
    delay(500.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 587, 225.0);
    delay(250.0);
    tone(tonePin, 391, 337.5);
    delay(375.0);
    tone(tonePin, 440, 112.5);
    delay(125.0);
    tone(tonePin, 493, 450.0);
    delay(500.0);
    delay(500.0);
    tone(tonePin, 523, 225.0);
    delay(250.0);
    tone(tonePin, 523, 225.0);
    delay(250.0);
    tone(tonePin, 523, 337.5);
    delay(375.0);
    tone(tonePin, 523, 112.5);
    delay(125.0);
    tone(tonePin, 523, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 112.5);
    delay(125.0);
    tone(tonePin, 493, 112.5);
    delay(125.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 440, 225.0);
    delay(250.0);
    tone(tonePin, 440, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 440, 450.0);
    delay(500.0);
    tone(tonePin, 587, 450.0);
    delay(500.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 450.0);
    delay(500.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 450.0);
    delay(500.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 587, 225.0);
    delay(250.0);
    tone(tonePin, 391, 337.5);
    delay(375.0);
    tone(tonePin, 440, 112.5);
    delay(125.0);
    tone(tonePin, 493, 450.0);
    delay(500.0);
    delay(500.0);
    tone(tonePin, 523, 225.0);
    delay(250.0);
    tone(tonePin, 523, 225.0);
    delay(250.0);
    tone(tonePin, 523, 337.5);
    delay(375.0);
    tone(tonePin, 523, 112.5);
    delay(125.0);
    tone(tonePin, 523, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 225.0);
    delay(250.0);
    tone(tonePin, 493, 112.5);
    delay(125.0);
    tone(tonePin, 493, 112.5);
    delay(125.0);
    tone(tonePin, 587, 225.0);
    delay(250.0);
    tone(tonePin, 587, 225.0);
    delay(250.0);
    tone(tonePin, 523, 225.0);
    delay(250.0);
    tone(tonePin, 440, 225.0);
    delay(250.0);
    tone(tonePin, 391, 675.0);
    delay(750.0);

}
void playRock(){
  //Rolling in the Deep - Adele
  tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 391, 106219287.125);
    delay(300);
    tone(tonePin, 440, 106219287.125);
    delay(300);
    tone(tonePin, 391, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 277, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 391, 106219287.125);
    delay(300);
    tone(tonePin, 440, 106219287.125);
    delay(300);
    tone(tonePin, 391, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 277, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
}
void playClasica(){
  //Fur Elise - Beethoven
  tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 622, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 622, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 587, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 523, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 110, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 164, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 261, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 82, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 164, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 415, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 110, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 164, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 523, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 622, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 622, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 587, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 523, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 110, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 164, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 261, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 82, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 164, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 523, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 110, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 164, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 622, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 622, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 587, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 523, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 110, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 164, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 261, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 82, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 164, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 415, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 110, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 523, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 220, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 622, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 622, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 659, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 587, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 523, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 110, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 220, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 261, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 440, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 82, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 493, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 207, 160.7143125);
    delay(178.571458333);
    tone(tonePin, 329, 69.64286875);
    delay(77.3809652778);
}
//star wars
  const int c = 261;
  const int d = 294;
  const int e = 329;
  const int f = 349;
  const int g = 391;
  const int gS = 415;
  const int a = 440;
  const int aS = 455;
  const int b = 466;
  const int cH = 523;
  const int cSH = 554;
  const int dH = 587;
  const int dSH = 622;
  const int eH = 659;
  const int fH = 698;
  const int fSH = 740;
  const int gH = 784;
  const int gSH = 830;
  const int aH = 880;
  const int buzzerPin = 8;
  int counter = 0;

void playStarWars(){
  //starWars
  //Play first section
  firstSection();
 
  //Play second section
  secondSection();
}
void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  if(counter % 2 == 0)
  {
    delay(duration);
  }else
  {
    delay(duration);
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
 
  //Increment counter
  counter++;
}
void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}
 
void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  delay(325);
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  delay(350);
}
