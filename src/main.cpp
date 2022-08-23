#include <Arduino.h>
#define BLYNK_TEMPLATE_ID "TMPL5l53AZQs"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "578iz_ytMoOen-3xwVawFcKRhCYrjPIa"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "KONTRAKAN UYE";
char pass[] = "KUSANG123";

BlynkTimer timer;

// buat variabel bool untuk parsing data.
bool parsing = false;
String sData, data[10];
float nilaiPh, nilaiTds, voltaseBaterai, arusPv, tekanan, suhu;
int kecepatanMotor, keadaanMotor;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V7)
{
  // Set incoming value from pin V0 to a variable
  kecepatanMotor = param.asInt();
}
BLYNK_WRITE(V8)
{
  // Set incoming value from pin V0 to a variable
  keadaanMotor = param.asInt();
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  Blynk.virtualWrite(V0, millis() / 1000);
  Blynk.virtualWrite(V1, nilaiPh);
  Blynk.virtualWrite(V2, nilaiTds);
  Blynk.virtualWrite(V3, voltaseBaterai);
  Blynk.virtualWrite(V4, arusPv);
  Blynk.virtualWrite(V5, tekanan);
  Blynk.virtualWrite(V6, suhu);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  while (Serial.available())
  {
    // buat variabel nilaiinput, dan masukkan nilai serial.readString kesana
    // String nilaiInput = Serial.readString();
    // print/tampilkan nilai input tadi di serial monitor
    char inChar = Serial.read();
    sData += inChar;
    if (inChar == '$')
      parsing = true;
    if (parsing)
    {
      int q = 0;
      for (int i = 0; i < sData.length(); i++)
      {
        if (sData[i] == '#')
        {
          q++;
          data[q] = "";
        }
        else
          data[q] += sData[i];
      }
      // setelah semua data didapatkan, kita akan print datanya ke serial satu persatu
      nilaiPh = data[1].toFloat();
      nilaiTds = data[2].toFloat();
      voltaseBaterai = data[3].toFloat();
      arusPv = data[4].toFloat();
      tekanan = data[5].toFloat();
      suhu = data[6].toFloat();

      parsing = false;
      sData = "";
    }
  }

  Serial.print("#");
  Serial.print(kecepatanMotor);
  Serial.print("#");
  Serial.print(keadaanMotor);
  Serial.println("#$");

  Blynk.run();
  timer.run();
}
