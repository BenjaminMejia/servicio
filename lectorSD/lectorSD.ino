#include <SD.h>

File myFile;
String cadena="";

void setup(){
  Serial.begin(9600);
  Serial.print("Iniciando SD ...");
  if (!SD.begin(4)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
  if(!SD.exists("datalog.csv")){
        myFile = SD.open("datalog.csv", FILE_WRITE);
        if (myFile) {
          Serial.println("Archivo nuevo, Escribiendo encabezado(fila 1)");
          myFile.close();
        } else {
          Serial.println("Error creando el archivo datalog.csv");
        }
  }
  
}

void loop(){
  GuardarSD();
  delay(1000);
  leerSD();
  delay(3000);
  Serial.println("Ciclo terminado");
  delay(5000);
}

void GuardarSD(){
  myFile = SD.open("datalog.csv", FILE_WRITE);//abrimos  el archivo
  
  if (myFile) { 
    Serial.print("Escribiendo SD: ");
        //int sensor1 = analogRead(0);
        //int sensor2 = analogRead(1);
        //int sensor3 = analogRead(2);
        /*myFile.print("Tiempo(ms)=");
        myFile.print(millis());
        myFile.print(", sensor1=");
        myFile.print(sensor1);
        myFile.print(", sensor2=");
        myFile.print(sensor2);
        myFile.print(", sensor3=");*/
    myFile.println("esta,es,una,prueba");
        
    myFile.close(); //cerramos el archivo  
    Serial.println("Fin escritura");
  
  } else {
    Serial.println("Error al abrir el archivo para escribir");
  }
  delay(1000);
}

void leerSD(){
  myFile = SD.open("datalog.csv",FILE_READ);//abrimos  el archivo 
  if (myFile){
    bool line=false;
    myFile.seek(myFile.size()-1); //Ubicacion en posicion anterior a ultimo caracter
    while (myFile.available()){
      if(line==false){ //Primero leer en reversa para buscar salto de linea
        char caracter=myFile.read();
        //Serial.println(caracter);
        if(caracter==',') //Cuando encuentra salto de linea cambia estado
          {
            line=true;
          }else{
            myFile.seek(myFile.position()-2);
          }
      }
      if(line==true){//Empieza a leer normalmente de izquierda a derecha
          char caracter=myFile.read();
         //Serial.println(caracter);
          cadena=cadena+caracter;
          if(caracter=='\n'){ //La cadena termina en este caracter
            break;
          }      
      }
    }
    myFile.close(); //cerramos el archivo
    delay(300);            
      Serial.println("*****************");
      Serial.println("datos.txt: ");
      Serial.print("cadena Leida: ");
      Serial.println(cadena);    
  } 
  else {
    Serial.println("Error al abrir el archivo para leer");
  }
  cadena="";
}