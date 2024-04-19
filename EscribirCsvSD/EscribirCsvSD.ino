#include <SD.h>

File myFile;
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
                    
  
  } else {
    Serial.println("Error al abrir el archivo");
  }
  delay(1000);
}