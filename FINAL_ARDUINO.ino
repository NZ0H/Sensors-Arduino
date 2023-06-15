// Inclusion de la Bibliothèque pour le Capteur HTU21DF 
#include <Wire.h>
#include "Adafruit_HTU21DF.h"

// Inclusion de la Bibliothèque pour le capteur BMP280
#include <SPI.h>
#include <Adafruit_BMP280.h>
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
Adafruit_BMP280 bmp;

void setup() {
  pinMode(13, INPUT);
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1);
  pinMode(13, INPUT);

  unsigned status;
  status = bmp.begin();

  // Gestion du status du capteur HTU, s'il n'est pas détecté
  if (!htu.begin()) {
    Serial.println("Capteur HTU non trouvé !");
    while (1);
  }

  // Gestion du status du capteur BMP, s'il n'est pas détecté
  if (!status) {
    Serial.println(F("Capteur BMP non trouvé !"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  // Ensemble des variables de mesure
  float temp = htu.readTemperature();
  float rel_hum = htu.readHumidity();
  float pression = bmp.readPressure();

  // Variable température
  float essais1 = temp;
  uint8_t *transmis1;
  transmis1 = (uint8_t *)&essais1;
  uint8_t CaracFTransmis1[4]; // Augmenter la taille pour ajouter le checksum

  // Variable Humidité
  float essais2 = rel_hum;
  uint8_t *transmis2;
  transmis2 = (uint8_t *)&essais2;
  uint8_t CaracFTransmis2[4]; // Augmenter la taille pour ajouter le checksum

  // Variable Pression
  float essais3 = pression;
  uint8_t *transmis3;
  transmis3 = (uint8_t *)&essais3;
  uint8_t CaracFTransmis3[4];

  // Variable boitier TOR
  bool TOR = digitalRead(13);

  // Envoie le résultat du capteur en caractère de 8bits (au total 32) TEMPERATURE
  Serial2.write("Te"); // Envoie la chaîne de caractères "Te" pour identifier le début de la trame
  for (int i = 0; i < 4; i++) {
    CaracFTransmis1[i] = *(transmis1 + i); // Copie le caractère pointé par *(transmis1+i) dans le tableau CaracFTransmis1 celon l'indice " i "
    Serial2.write(CaracFTransmis1[i]); // Envoie le caractère sur port Série
  }

  // Envoie le résultat du capteur en caractère de 8bits (au total 32) HUMIDITE
  for (int i = 0; i < 4; i++) // Mettre 4 pour ne pas ajouter le checksum à l'envoi
  {
    CaracFTransmis2[i] = *(transmis2 + i); // Copie le caractère pointé par *(transmis1+i) dans le tableau CaracFTransmis2 celon l'indice " i "
    Serial2.write(CaracFTransmis2[i]); // Envoie le caractère sur port Série
  }

  // Envoie le résultat du capteur en caractère de 8bits (au total 32) PRESSION
  for (int i = 0; i < 4; i++) // Mettre 4 pour ne pas ajouter le checksum à l'envoi
  {
    CaracFTransmis3[i] = *(transmis3 + i); // Copie le caractère pointé par *(transmis1+i) dans le tableau CaracFTransmis3 celon l'indice " i "
    Serial2.write(CaracFTransmis3[i]); // Envoie le caractère sur port Série
  }

  Serial2.write(TOR); // Envoie le résultat booléen de TOR (0 ou 1)

  // Initialisation variable checksum
  uint32_t result = 0;
  
  // XOR entre 0 et CaracFTransmis1
  for (int i = 0; i < 4; i++) {
    result = (result ^ CaracFTransmis1[i]);
  }

  // XOR entre CaracFTranmis1 et CaracFTransmis2
  for (int i = 0; i < 4; i++) {
    result = result ^ CaracFTransmis2[i];
  }

  // XOR entre le XOR de CaracFTranmis1,CaracFTransmis2 et CaracFTransmis3
  for (int i = 0; i < 4; i++) {
    result = result ^ CaracFTransmis3[i];
  }

  // XOR entre le XOR de CaracFTranmis1,CaracFTransmis2,CaracFTransmis3 et TOR
  result = result^TOR ;

  // Application d'un masque pour inverser les bits de rang pair
  result = result^0x55555555 ;
   
  for (int i = 0; i < 4; i++) {
    uint8_t byteToSend = (result >> (i * 8)) & 0xFF;
    Serial2.write(byteToSend);
  }
  
  Serial2.write(result);
  Serial2.write(13); // uniquement pour que ça s'affiche correctement dans minicom
  Serial2.write(10); // uniquement pour que ça s'affiche correctement dans minicom
}
