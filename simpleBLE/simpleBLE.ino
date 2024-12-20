/*
* Group Name: The Alligaitors
* Course Name: BMEN 3030 - Bioinstrumentation
* 
* Connects the Arduino MKR WiFi 1010 to Bluetooth where the output
* from the (3) FlexiForce pressure sensors was transmitted, in addition
* to printing the output to the Serial Monitor once a device pairs. 
*/

#include <ArduinoBLE.h>

#define SENSOR_PIN1 A1
#define SENSOR_PIN2 A2
#define SENSOR_PIN3 A3

// Initialize a service for the sensor characteristics
BLEService sensorService("19b10010-e8f2-537e-4f6c-d104768a1214");

// Initialize characteristics for each of the sensors
BLEUnsignedCharCharacteristic sensor1("19b10011-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEUnsignedCharCharacteristic sensor2("19b10012-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEUnsignedCharCharacteristic sensor3("19b10014-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);

// Initilize a calibration factor to multiply sensor readings by
const int cf = 80;

void setup() 
{
  Serial.begin(9600);

  pinMode(SENSOR_PIN1, INPUT);
  pinMode(SENSOR_PIN2, INPUT);
  pinMode(SENSOR_PIN3, INPUT);
  
  // Wait for the Serial Monitor to get opened before beginning the bulk of setup
  while (!Serial);

  if (!BLE.begin()) 
  {
    Serial.println("BLE failed to start");
    while (1);
  }

  BLE.setLocalName("MKR Wifi 1010");
  BLE.setAdvertisedService(sensorService);
  sensorService.addCharacteristic(sensor1);
  sensorService.addCharacteristic(sensor2);
  sensorService.addCharacteristic(sensor3);
  BLE.addService(sensorService);
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() 
{
  BLEDevice central = BLE.central(); // wait for a Bluetooth® Low Energy central
  float reading1, reading2, reading3 = 0; 
  float v1, v2, v3 = 0;
  
  if (central)
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address()); // print the central's BT address

    while (central.connected())
    {
      reading1 = analogRead(SENSOR_PIN1);
      v1 = (reading1 * 5.0) / 1023.0; // Convert first FlexiForce sensor's reading to voltage
      v1 = v1 * cf ;
      sensor1.setValue(v1);

      reading2 = analogRead(SENSOR_PIN2); 
      v2 = (reading2 * 5.0) / 1023.0; // Convert second FlexiForce sensor's reading to voltage
      v2 = v2 * cf ;
      sensor2.setValue(v2);

      reading3 = analogRead(SENSOR_PIN3); 
      v3 = (reading3 * 5.0) / 1023.0; // Convert third FlexiForce sensor's reading to voltage
      v3 = v3 * cf ;
      sensor3.setValue(v3);

    // Print out each sensors' respective data in the Serial Monitor (every 700ms)
      Serial.println("");
      Serial.print("Sensor"); Serial.print("\t"); Serial.print("1"); Serial.print("\t"); Serial.print("2"); Serial.print("\t"); Serial.print("3");
      Serial.println("");
      Serial.print("\t"); Serial.print(v1); Serial.print("\t"); Serial.print(v2); Serial.print("\t"); Serial.print(v3); 
      Serial.println("");
      delay(700);
    }
  }
  
  BLE.poll();
  delay(1000);
}


