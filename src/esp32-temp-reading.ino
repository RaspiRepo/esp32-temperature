#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

#include <WiFi.h>
#include <HTTPClient.h>

uint8_t temprature_sens_read();

const char* ssid = "WiFi-SSID";
const char* password =  "WIFi-Password";
HTTPClient http;   

void setup() {
  Serial.begin(115200);
  setup_WiFi();
}

void setup_WiFi() {

    delay(4000);   //Delay needed before calling the WiFi.begin
    WiFi.begin(ssid, password); 

    //wait for WiFi Access connect success
    while (WiFi.status() != WL_CONNECTED) { 
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to your WiFi network");
}

void loop() {

    //get Anlog ADC from TMP sensor and subtract offset as per Datasheet
    int adcCount = analogRead(36) - 50;

    //convert Adc to C and F
    float tempCels = ((150.0 / 4096.0) * adcCount);
    float tempFrn = (tempCels * 9.0 / 5.0) + 32;
    String stringExternal = String(tempFrn);

    String tem_str = String("Count ") + String(tempFrn) + "f " + String(tempCels) + " c";

    // Convert Internal CPU temperature to F
    double internal = (temprature_sens_read());
    String stringInternl = String(internal);

    //construct string to write InfluxDB HTTP post
    String stringOne = String("temperature,machine=unit21,type=assembly external=" + stringExternal + ",internal=" + stringInternl);
    Serial.println(stringOne);

    //recheck wifi status and post current reading to TICK service HTTP endpoint
    if (WiFi.status()== WL_CONNECTED) {   //Check WiFi connection status

        //write InfluxDB API
        http.begin("http://192.168.0.115:8086/write?db=esp32");  //Specify destination for HTTP request
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header 
        int httpResponseCode = http.POST(stringOne);   //Send the actual POST request

        //Wont get any response other than status code 204 if success 
        if(httpResponseCode == 204){
            ;
        } else{
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }
 
        http.end();  //Free resources

     } else {
        //in case WiFI/Network issue retry after 30 seconds(self rebbot)
        Serial.println("Error in WiFi connection");   
        delay(6 * 5000);
        ESP.restart(); 
     }

    //collect ADC sample every 5 seconds and conevert to temperature
    delay(5000);
}