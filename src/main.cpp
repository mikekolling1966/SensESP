#include <memory>

#include "sensesp.h"
#include "sensesp/sensors/analog_input.h"
#include "sensesp/sensors/digital_input.h"
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/system/lambda_consumer.h"
#include "sensesp_app_builder.h"

using namespace sensesp;

void setup() {
  SetupLogging(ESP_LOG_DEBUG);

  SensESPAppBuilder builder;
  sensesp_app = (&builder)
                    ->set_hostname("my-sensesp-project")
                    ->get_app();

  const uint8_t kAnalogInputPin = 36;
  const unsigned int kAnalogInputReadInterval = 500;

  // ONE sensor for raw ADC (scale = 1.0)
  auto analog_input_raw = std::make_shared<AnalogInput>(
    kAnalogInputPin, kAnalogInputReadInterval, "", 1.0);

  // ONE sensor for voltage (scale = 3.3 / 4095.0)
  auto analog_input_voltage = std::make_shared<AnalogInput>(
    kAnalogInputPin, kAnalogInputReadInterval, "", 3.3 / 4095.0);

  // Debug print the raw ADC value
  analog_input_raw->attach([analog_input_raw]() {
    debugD("Analog input raw value: %f", analog_input_raw->get());
  });

  // Digital output toggler on GPIO 15
  const uint8_t kDigitalOutputPin = 15;
  const unsigned int kDigitalOutputInterval = 650;
  pinMode(kDigitalOutputPin, OUTPUT);
  event_loop()->onRepeat(kDigitalOutputInterval, [kDigitalOutputPin]() {
    digitalWrite(kDigitalOutputPin, !digitalRead(kDigitalOutputPin));
  });

  // Digital input 1 (GPIO 14)
  const uint8_t kDigitalInput1Pin = 14;
  auto digital_input1 = std::make_shared<DigitalInputChange>(
    kDigitalInput1Pin, INPUT_PULLUP, CHANGE);

  auto digital_input1_consumer = std::make_shared<LambdaConsumer<bool>>(
    [](bool input) { debugD("Digital input value changed: %d", input); });

  digital_input1->connect_to(digital_input1_consumer);

  // Digital input 2 (GPIO 13)
  const uint8_t kDigitalInput2Pin = 13;
  const unsigned int kDigitalInput2Interval = 1000;

  pinMode(kDigitalInput2Pin, INPUT_PULLUP);

  auto digital_input2 = std::make_shared<RepeatSensor<bool>>(
    kDigitalInput2Interval,
    [kDigitalInput2Pin]() { return digitalRead(kDigitalInput2Pin); });

  // --- Signal K Outputs ---

  // RAW ADC Output
  auto adc_metadata = std::make_shared<SKMetadata>("", "Analog input raw ADC value");
  auto adc_sk_output = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.adc",
    "/Sensors/Analog Input/ADC Value",
    adc_metadata
  );

  ConfigItem(adc_sk_output)
    ->set_title("Analog Input ADC Raw Value SK Output Path")
    ->set_description("Publishes raw 0–4095 ADC reading")
    ->set_sort_order(100);

  analog_input_raw->connect_to(adc_sk_output);

  // Voltage Output
  auto voltage_metadata = std::make_shared<SKMetadata>("V", "Analog input voltage");
  auto voltage_sk_output = std::make_shared<SKOutput<float>>(
    "sensors.analog_input.voltage",
    "/Sensors/Analog Input/Voltage",
    voltage_metadata
  );

  ConfigItem(voltage_sk_output)
    ->set_title("Analog Input Voltage SK Output Path")
    ->set_description("Publishes scaled 0.0–3.3V reading")
    ->set_sort_order(110);

  analog_input_voltage->connect_to(voltage_sk_output);

  // Digital input 2 Output
  auto di2_metadata = std::make_shared<SKMetadata>("", "Digital input 2 value");
  auto di2_sk_output = std::make_shared<SKOutput<bool>>(
    "sensors.digital_input2.value",
    "/Sensors/Digital Input 2/Value",
    di2_metadata
  );

  ConfigItem(di2_sk_output)
    ->set_title("Digital Input 2 SK Output Path")
    ->set_sort_order(200);

  digital_input2->connect_to(di2_sk_output);

  while (true) {
    loop();
  }
}

void loop() { event_loop()->tick(); }
