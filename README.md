# esp32-temperature

This project aim is to understand integration between IOT sensor with Cloud.  Here ESP32 reading AnalogDevice TMP36GZ temperature, construct HTTP message with sensor data and sending to 
InfluxDB HTTP endpoint. Time series data can be viewed  chronograph.

To store sensors time series data JetsonNano board used to running InfluxDB(TICK).  This allows jetsonNano to apply any AI integration on sensor data easily. 

https://github.com/RaspiRepo/jetsonNano-Tick

