/** Import der Libraries */
#include <Connectivity.h>
#include "DHT.h"   
             
#define DHTPIN 2          // 2 ist die Pinnummer des DHT11 Signalpins
#define DHTTYPE DHT11     // Hier wählt man zwischen DHT11 und DHT22

//Definition der sog. Items. Wir verwenden Item 1 für die Temperatur und Item 2 für die Luftfeuchtigkeit.
//1 <-> Temperature -> Double
const int ITEM_TEMP = 1;
//2 -> Luftfeuchtigkeit  -> Double 
const int ITEM_MOISTURE = 2;

// Das Object über welches wir auf den Sensor zugreifen       
DHT dht(DHTPIN, DHTTYPE);
// Die Objekte für die serielle Kommunikation mit dem Modul
Connectivity connectivity;
Message message;

void setup() {
  connectivity.begin(&Serial1);
  // Wird benötigt, um später die Sensorwerte zu lesen.
  dht.begin();
}

void loop() {
  // Alle zwei Sekunden lesen wir die Werte und schicken sie an die Cloud
  delay(2000);                   
                                    
  double feuchtigkeit = (double) dht.readHumidity();    // Lesen der Luftfeuchtigkeit
  double temperatur = (double) dht.readTemperature(); // Lesen der Temperatur
  
  //Wir überprüfen, ob die Sensorwerte richtig gelesen wurden
  if (isnan(feuchtigkeit)){ 
    //Wenn nicht, dann senden wir einen STATUS_ERROR für das entsprechende Item
    connectivity.send_item_status(ITEM_MOISTURE, STATUS_ERROR);
  }else{
    //Wenn alles in Ordnung ist übermitteln wir den Wert mit dem STATUS_OK Statuscode.
    connectivity.send_state_change_double(ITEM_MOISTURE, feuchtigkeit, STATUS_OK);
  }

  // Und das gleiche nochmal für die Temperatur
  if (isnan(temperatur)) {
    connectivity.send_item_status(ITEM_TEMP, STATUS_ERROR);
  }else{
    connectivity.send_state_change_double(ITEM_TEMP, temperatur, STATUS_OK);
  }
}
