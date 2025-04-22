
#include <memory>
#include "sensesp.h"
#include "sensesp/sensors/analog_input.h"
#include "sensesp/sensors/digital_input.h"
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/system/lambda_consumer.h"
#include "sensesp_app_builder.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#define LCD_MOSI 23  // ESP32 D23
#define LCD_SCLK 18  // ESP32 D18
#define LCD_CS   15  // ESP32 D15
#define LCD_DC   2   // ESP32 D2
#define LCD_RST  4   // ESP32 D4
#define LCD_BLK  32  // ESP32 D32

SPIClass mySPI(VSPI);  // VSPI hardware SPI bus (you can also use HSPI if needed)
Adafruit_ST7789 lcd = Adafruit_ST7789(&mySPI, LCD_CS, LCD_DC, LCD_RST);






using namespace sensesp;
void setup() {
  SetupLogging(ESP_LOG_DEBUG);
  mySPI.begin(LCD_SCLK, -1, LCD_MOSI, LCD_CS); // SCLK, MISO (not used), MOSI, SS/CS
  lcd.init(135, 240);        // Width, Height
  lcd.setRotation(0);        // Rotate screen if needed
  lcd.fillScreen(ST77XX_BLACK);  // Clear screen to black
  lcd.setCursor(0, 0);       // Set cursor position
  lcd.setTextColor(ST77XX_WHITE);
  lcd.setTextSize(2);
  lcd.print("Hello, ideaspark");
  SensESPAppBuilder builder;
  sensesp_app = (&builder)
                    ->set_hostname("my-sensesp-project")
                    ->get_app();
  const unsigned int kAnalogInputReadInterval = 500;

  const uint8_t kAnalogInputPin0 = 36;
  const uint8_t kAnalogInputPin1 = 39;
  const uint8_t kAnalogInputPin2 = 34;
  const uint8_t kAnalogInputPin3 = 35;
  
  
  // ONE
  // ONE sensor for raw ADC (scale = 1.0)
  auto analog_input_raw0 = std::make_shared<AnalogInput>(
    kAnalogInputPin0, kAnalogInputReadInterval, "", 1.0);
  // ONE sensor for voltage (scale = 3.3 / 4095.0)
  auto analog_input_voltage0 = std::make_shared<AnalogInput>(
    kAnalogInputPin0, kAnalogInputReadInterval, "", 3.3 / 4095.0);
  // Debug print the raw ADC value
  analog_input_raw0->attach([analog_input_raw0]() {
    debugD("Analog input raw 0 value: %f", analog_input_raw0->get());
  });
  //TWO
  // ONE sensor for raw ADC (scale = 1.0)
  auto analog_input_raw1 = std::make_shared<AnalogInput>(
    kAnalogInputPin1, kAnalogInputReadInterval, "", 1.0);
  // ONE sensor for voltage (scale = 3.3 / 4095.0)
  auto analog_input_voltage1 = std::make_shared<AnalogInput>(
    kAnalogInputPin1, kAnalogInputReadInterval, "", 3.3 / 4095.0);
  // Debug print the raw ADC value
  analog_input_raw1->attach([analog_input_raw1]() {
    debugD("Analog input raw 1 value: %f", analog_input_raw1->get());
  });

//THREE
  // ONE sensor for raw ADC (scale = 1.0)
  auto analog_input_raw2 = std::make_shared<AnalogInput>(
    kAnalogInputPin2, kAnalogInputReadInterval, "", 1.0);
  // ONE sensor for voltage (scale = 3.3 / 4095.0)
  auto analog_input_voltage2 = std::make_shared<AnalogInput>(
    kAnalogInputPin2, kAnalogInputReadInterval, "", 3.3 / 4095.0);
  // Debug print the raw ADC value
  analog_input_raw2->attach([analog_input_raw2]() {
    debugD("Analog input raw 2 value: %f", analog_input_raw2->get());
  });

  //FOUR
  // ONE sensor for raw ADC (scale = 1.0)
  auto analog_input_raw3 = std::make_shared<AnalogInput>(
    kAnalogInputPin3, kAnalogInputReadInterval, "", 1.0);
  // ONE sensor for voltage (scale = 3.3 / 4095.0)
  auto analog_input_voltage3 = std::make_shared<AnalogInput>(
    kAnalogInputPin3, kAnalogInputReadInterval, "", 3.3 / 4095.0);
  // Debug print the raw ADC value
  analog_input_raw3->attach([analog_input_raw3]() {
    debugD("Analog input raw 3 value: %f", analog_input_raw3->get());
  });



  // --- Signal K Outputs ---
  //RAW ADC
  //ONE
  // RAW ADC Output
  auto adc_metadata0 = std::make_shared<SKMetadata>("", "Analog input raw ADC value");
  auto adc_sk_output0 = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.adc.0",
    "/Sensors/Analog Input/ADC 0 Value",
    adc_metadata0
  );
  ConfigItem(adc_sk_output0)
    ->set_title("Analog Input ADC Raw Value SK Output Path")
    ->set_description("Publishes raw 0–4095 ADC reading")
    ->set_sort_order(100);
  analog_input_raw0->connect_to(adc_sk_output0);

//TWO
  // RAW ADC Output 1
  auto adc_metadata1 = std::make_shared<SKMetadata>("", "Analog input raw ADC value");
  auto adc_sk_output1 = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.adc.1",
    "/Sensors/Analog Input/ADC 1 Value",
    adc_metadata1
  );
  ConfigItem(adc_sk_output1)
    ->set_title("Analog Input ADC Raw Value SK Output Path")
    ->set_description("Publishes raw 0–4095 ADC reading")
    ->set_sort_order(100);
  analog_input_raw1->connect_to(adc_sk_output1);


//THREE
  // RAW ADC Output 1
  auto adc_metadata2 = std::make_shared<SKMetadata>("", "Analog input raw ADC value");
  auto adc_sk_output2 = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.adc.2",
    "/Sensors/Analog Input/ADC 2 Value",
    adc_metadata2
  );
  ConfigItem(adc_sk_output2)
    ->set_title("Analog Input ADC Raw Value SK Output Path")
    ->set_description("Publishes raw 0–4095 ADC reading")
    ->set_sort_order(100);
  analog_input_raw2->connect_to(adc_sk_output2);
//FOUR
  // RAW ADC Output 1
  auto adc_metadata3 = std::make_shared<SKMetadata>("", "Analog input raw ADC value");
  auto adc_sk_output3 = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.adc.3",
    "/Sensors/Analog Input/ADC 3 Value",
    adc_metadata3
  );
  ConfigItem(adc_sk_output3)
    ->set_title("Analog Input ADC Raw Value SK Output Path")
    ->set_description("Publishes raw 0–4095 ADC reading")
    ->set_sort_order(100);
  analog_input_raw3->connect_to(adc_sk_output3);
//
//

// VOLTAGES
//ONE
  // Voltage Output
  auto voltage_metadata0 = std::make_shared<SKMetadata>("V", "Analog input voltage");
  auto voltage_sk_output0 = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.voltage.0",
    "/Sensors/Analog Input/Voltage",
    voltage_metadata0
  );
  ConfigItem(voltage_sk_output0)
    ->set_title("Analog Input Voltage SK Output Path")
    ->set_description("Publishes scaled 0.0–3.3V reading")
    ->set_sort_order(110);
  analog_input_voltage0->connect_to(voltage_sk_output0);

//TWO
  // Voltage Output
  auto voltage_metadata1 = std::make_shared<SKMetadata>("V", "Analog input voltage");
  auto voltage_sk_output1 = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.voltage.1",
    "/Sensors/Analog Input/Voltage",
    voltage_metadata1
  );
  ConfigItem(voltage_sk_output1)
    ->set_title("Analog Input Voltage SK Output Path")
    ->set_description("Publishes scaled 0.0–3.3V reading")
    ->set_sort_order(110);
  analog_input_voltage1->connect_to(voltage_sk_output1);

//THREE
  // Voltage Output
  auto voltage_metadata2 = std::make_shared<SKMetadata>("V", "Analog input voltage");
  auto voltage_sk_output2 = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.voltage.2",
    "/Sensors/Analog Input/Voltage",
    voltage_metadata2
  );
  ConfigItem(voltage_sk_output2)
    ->set_title("Analog Input Voltage SK Output Path")
    ->set_description("Publishes scaled 0.0–3.3V reading")
    ->set_sort_order(110);
  analog_input_voltage2->connect_to(voltage_sk_output2);
//FOUR
  // Voltage Output
  auto voltage_metadata3 = std::make_shared<SKMetadata>("V", "Analog input voltage");
  auto voltage_sk_output3 = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.voltage.3",
    "/Sensors/Analog Input/Voltage",
    voltage_metadata3
  );
  ConfigItem(voltage_sk_output3)
    ->set_title("Analog Input Voltage SK Output Path")
    ->set_description("Publishes scaled 0.0–3.3V reading")
    ->set_sort_order(110);
  analog_input_voltage3->connect_to(voltage_sk_output3);


  while (true) {
    loop();
  }
}

// ---- END of setup() ----
// Start normal functions here:
bool printed = false;  // Global variable
void app_loop() {
  event_loop()->tick();

}
void loop() {
  while (true) {
    app_loop();
  }
}
