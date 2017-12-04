#include <Wire.h>
#include <Adafruit_MCP4725.h>

// Voltage Step test sequence
// array format = voltage, time in mS
int numberOfTestCycles  = 5;
float testSequence[][2] = {{11, 1000},
                           { 7,  200},
                           { 9, 1500}};
float finalVoltage = 0;

// Hardware constants:                           
float driverGain    = 4;   //the gain of the output amplifier from the ADC
float refADCvoltage = 5;   //the full scale reference voltage of the ADC
int   bitsADC       = 12;  //resolution of the ADC in bits

float countsPerVolt = pow(2,bitsADC) / refADCvoltage / driverGain;
Adafruit_MCP4725 dac;

void setup(void) 
{
  Serial.begin(9600);
  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x62);
}

void loop(void)
{
  for(int cycle = 0; cycle < numberOfTestCycles; cycle++)
  {
    Serial.print("Test Cycle ");
    Serial.print(cycle + 1);
    Serial.print(" of ");
    Serial.println(numberOfTestCycles);
    for( int i = 0; i < (sizeof(testSequence) / sizeof(float) / 2); i++)
    {
      setOutputVoltage(testSequence[i][0]);
      delay(testSequence[i][1]);
    }
  }
  Serial.println("Finished");
  setOutputVoltage(finalVoltage);
  while(1);
}

void setOutputVoltage(float v)
{
  long countsADC = v * countsPerVolt;
  
  dac.setVoltage(countsADC, false);
  Serial.print("  Volts: ");
  Serial.print(v);
  Serial.print(", Counts: ");
  Serial.println(countsADC);
}
