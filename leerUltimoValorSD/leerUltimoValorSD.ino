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
}

void loop(){
  leerSD();
  delay(3000);
}

void leerSD(){
  Serial.println("abriendo");
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
    Serial.println("Error al abrir el archivo");
  }
  cadena="";
}