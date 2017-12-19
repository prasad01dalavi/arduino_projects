#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

 

#define resistance_R50 100000

#define resistance_R500 1000000

#define resistance_V2 10000

#define caliberation_V2 1.1

#define range50_mul (resistance_R50 / resistance_V2) * caliberation_V2

#define range500_mul (resistance_R500 / resistance_V2) * caliberation_V2

#define resistance_Ri 10

#define resistance_Cr 100000

#define resistance_Rb 100000

#define resistance_Re 10

#define resistance_R2 1000

 

int adc_value = 0;

int voltage_peak_value = 0;

int discharge_voltage_V0 = 0;

int discharge_voltage_V1 = 0;

float voltage_average_value = 0;

float dc_voltage_V0 = 0;

float ac_voltage_V0 = 0;

float dc_voltage_V1 = 0;

float dc_voltage_V2 = 0;

float ac_voltage_V1 = 0;

float dc_current_I0 = 0;

float dc_current_I1 = 0;

float ac_current_I0 = 0;

float dc_power = 0;

float ac_power = 0;

float npn_pnp_hfe = 0;

float capacitance = 0;

unsigned long resistance;

unsigned long sample_count = 0;

unsigned long discharge_time_T0 = 0;

unsigned long discharge_time_T1 = 0;

char fn0 = 6;

char fn1 = 7;

char fn2 = 8;

char rn0 = 9;

char rn1 = 10;

char rn2 = 13;

char function_select [4];

char range_select [4];

 

void setup()

{

  lcd.begin(16, 2);

  lcd.print("     DIGITAL");
  lcd.setCursor(0,1);
  lcd.print("    VOLTMETER");

  delay(3000);

//measured ac voltage will be rms voltage. i.e. Vm*0.707=Vrms 

  pinMode(fn0, INPUT);

  pinMode(fn1, INPUT);

  pinMode(fn2, INPUT);

  pinMode(rn0, INPUT);

  pinMode(rn1, INPUT);

  pinMode(rn2, INPUT);

}

 

void loop()

{

  function_select [0] = digitalRead(fn0) + '0';

  function_select [1] = digitalRead(fn1) + '0';

  function_select [2] = digitalRead(fn2) + '0';

  function_select [3] = '\0';

  range_select [0] = digitalRead(rn0) + '0';

  range_select [1] = digitalRead(rn1) + '0';

  range_select [2] = digitalRead(rn2) + '0';

  range_select [3] = '\0';

 

  //=============================== VOLTAGE ========================================//

 

  voltage_peak_value = 0;

  for(sample_count = 0; sample_count < 5000; sample_count ++)

  {

      adc_value = analogRead(A0);

      if(voltage_peak_value < adc_value)

          voltage_peak_value = adc_value;

      else;

      delayMicroseconds(10);

  }

  dc_voltage_V0 = voltage_peak_value * 0.00488;

  ac_voltage_V0 = dc_voltage_V0 / 1.414;

 

  if (  0 == strncmp (range_select, "01", 2) )

  {

      lcd.clear();

      lcd.setCursor(0, 0);

      lcd.print("[R] ");

     

      if (range_select [2] == '0')

          lcd.print(" DCV");

      else

          lcd.print(" ACV");

         

      lcd.setCursor(0, 1);

      lcd.print("0-5 ");

     

      if (range_select [2] == '0')

          lcd.print(dc_voltage_V0);

      else  

          lcd.print(ac_voltage_V0);

      delay(500);

  }

  else;

 

  if (  0 == strncmp (range_select, "10", 2) )

  { 

      lcd.clear();

      lcd.setCursor(0, 0);

      lcd.print("[R] ");

     

      if (range_select [2] == '0')

          lcd.print(" DCV");

      else

          lcd.print(" ACV");

         

      lcd.setCursor(0, 1);

      lcd.print("5-50 ");     

      if (range_select [2] == '0')

          lcd.print(dc_voltage_V0 * range50_mul);

      else  

          lcd.print(ac_voltage_V0 * range50_mul);

      delay(500);

  }

  else;

 

  if (  0 == strncmp (range_select, "11", 2) )

  {

      lcd.clear();

      lcd.setCursor(0, 0);

      lcd.print("[R] ");

     

      if (range_select [2] == '0')

          lcd.print(" DCV");

      else

          lcd.print(" ACV");

         

      lcd.setCursor(0, 1);

      lcd.print("50-500 "); 

     

      if (range_select [2] == '0')

          lcd.print(dc_voltage_V0 * range500_mul);

      else  

          lcd.print(ac_voltage_V0 * range500_mul);

      delay(500);

  }

  else;

 

  //=================================================================================//

}

