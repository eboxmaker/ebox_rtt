#include "ADS1120.h"
#include "SPI.h"

#define CSDELAY 5


float ADS1120::voltage(float adc,float Vref,int gain)
{
    float v = Vref * (adc / 32767.0) / gain;
    return v;
}


ADS1120::ADS1120(Gpio *cs_pin,Gpio *drdy_pin,Spi *_spi)
{
    cs = cs_pin;
    drdy = drdy_pin;
    spi = _spi;
}

void ADS1120::writeRegister(uint8_t address, uint8_t value)
{
    cs->reset();
    delay_us(CSDELAY);
    spi->write(CMD_WREG | (address << 2));
    spi->write(value);
    delay_us(CSDELAY);
    cs->set();
}

uint8_t ADS1120::readRegister(uint8_t address)
{
    cs->reset();
    delay_us(CSDELAY);
    spi->write(CMD_RREG | (address << 2));
    uint8_t data = spi->read();
    delay_us(CSDELAY);
    cs->set();
    return data;
}

void ADS1120::begin()
{
  // Set pins up
    
    // Configure chip select as an output
    cs->mode(OUTPUT_PP_PU);
    
    // Configure the SPI interface (CPOL=0, CPHA=1)
    cfg.dev_num = cs->id;
    cfg.bit_order = Spi::MSB;
    cfg.mode = Spi::MODE1;
    cfg.prescaler = Spi::DIV8;
    spi->begin(&cfg);
    
  
    // Configure DRDY as as input
    drdy->mode(INPUT_PU);

    cs->reset(); // Set CS Low
    delay_us(1);              
    reset();                           
    delay_ms(20);
//    ; // Delay a minimum of 50 us + 32 * tclk

//    startSync();                        // Send start/sync for continuous conversion mode
//    delay_us(1);               // Delay a minimum of td(SCCS)
    cs->set(); // Clear CS to high
}

bool ADS1120::isDataReady()
{
  if (drdy->read() == HIGH)
  {
    return false;
  }
  return true;
}


int ADS1120::readADC()
{
  cs->reset(); // Take CS low
  uint32_t adcVal =spi->read();
  adcVal = (adcVal << 8) | spi->read();
//    rt_kprintf("raw:0x%08x\n",adcVal);
  cs->set();
  return adcVal;
}

byte *ADS1120::readADC_Array()
{
  cs->reset(); // Take CS low
  static byte dataarray[2];
  for (int x = 0; x < 2; x++)
  {
    dataarray[x] = spi->read();
  }
  cs->set();
  return dataarray;
}

//Single Conversion read modes
int ADS1120::readADC_Single()
{
  cs->reset(); // Take CS low
  spi->transfer(0x08);
  while (drdy->read() == HIGH)
  {
    // Espera a que DRDY se ponga en nivel bajo. Esto es un riesgo porque pude quedar bloqueado el codigo aca.
    // Se deberia poner un timeout configurable en el metodo de begin y devolver un error si no responde 
  }

  int adcVal = spi->read();
  adcVal = (adcVal << 8) | spi->read();
  cs->set();
  return adcVal;
}

byte *ADS1120::readADC_SingleArray()
{
  cs->reset(); // Take CS low
  spi->transfer(0x08);
  while (drdy->read() == HIGH)
  {
    // Espera a que DRDY se ponga en nivel bajo. Esto es un riesgo porque pude quedar bloqueado el codigo aca.
    // Se deberia poner un timeout configurable en el metodo de begin y devolver un error si no responde
  }

  static byte dataarray[2];
  for (int x = 0; x < 2; x++)
  {
    dataarray[x] = spi->read();
  }
  cs->set();
  return dataarray;
}

void ADS1120::sendCommand(uint8_t command)
{
  cs->reset();
  delay_us(2);
  cs->set();
  delay_us(2);
  cs->reset();
  delay_us(2);
  spi->write(command);
  delay_us(200);
  cs->set();
}

void ADS1120::writeRegisterMasked(uint8_t value, uint8_t mask, uint8_t address)
{
  // Escribe un valor en el registro, aplicando la mascara para tocar unicamente los bits necesarios.
  // No realiza el corrimiento de bits (shift), hay que pasarle ya el valor corrido a la posicion correcta
  
  // Leo el contenido actual del registro
  uint8_t register_contents = readRegister(address);

  // Cambio bit aa bit la mascara (queda 1 en los bits que no hay que tocar y 0 en los bits a modificar)
  // Se realiza un AND co el contenido actual del registro.  Quedan "0" en la parte a modificar
  register_contents = register_contents & ~mask;

  // se realiza un OR con el valor a cargar en el registro.  Ojo, valor debe estar en el posicion (shitf) correcta
  register_contents = register_contents | value;

  // Escribo nuevamente el registro
  writeRegister(address, register_contents);
}

void ADS1120::setMultiplexer(uint8_t value)
{
  /* Set multiplexer

  | Value | AINp | AINn |
  | ----- | ---- | ---- |
  | 0x00  | AIN0 | AIN1 |
  | 0X01  | AIN0 | AIN2 |
  | 0X02  | AIN0 | AIN3 |
  | 0X03  | AIN1 | AIN2 |
  | 0X04  | AIN1 | AIN3 |
  | 0X05  | AIN2 | AIN3 |
  | 0X06  | AIN1 | AIN0 |
  | 0X07  | AIN3 | AIN2 |
  | 0X08  | AIN0 | AVSS |
  | 0X09  | AIN1 | AVSS |
  | 0X0A  | AIN2 | AVSS |
  | 0X0B  | AIN3 | AVSS |
  | 0X0C  |  REF/4 MON  |
  | 0X0D  | APWR/4 MON  |
  | 0X0E  |   SHORTED   |
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x0E)
  {
    value = 0x00;
  }
  value = value << 4; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_MUX, CONFIG_REG0_ADDRESS);
}

void ADS1120::setGain(uint8_t gain)
{
  /* Sets ADC gain. Possible values are 1, 2, 4, 8, 16, 32, 64, 128. */
  uint8_t value = 0x00;
  switch (gain)
  {
  case 1:
    value = 0x00;
    break;
  case 2:
    value = 0x01;
    break;
  case 4:
    value = 0x02;
    break;
  case 8:
    value = 0x03;
    break;
  case 16:
    value = 0x04;
    break;
  case 32:
    value = 0x05;
    break;
  case 64:
    value = 0x06;
    break;
  case 128:
    value = 0x07;
    break;
  default:
    value = 0x00;
    break;
  }
  value = value << 1; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_GAIN, CONFIG_REG0_ADDRESS);
}

void ADS1120::setPGAbypass(bool value)
{
  /* Bypasses the PGA if true.
     PGA can only be disabled for gains 1, 2, 4.
  */
  writeRegisterMasked(value, REG_MASK_PGA_BYPASS, CONFIG_REG0_ADDRESS);
}

void ADS1120::setDataRate(uint8_t value)
{
  /* Sets the data rate for the ADC. See table 18 in datasheet for datarates
     in various operating modes. */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x07)
  {
    value = 0x00;
  }
  value = value << 5; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_DATARATE, CONFIG_REG1_ADDRESS);
}

void ADS1120::setOpMode(uint8_t value)
{
  /* Sets the ADC operating mode:
     0 - Normal mode
     1 - Duty-cycle mode
     2 - Turbo mode
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x02)
  {
    value = 0x00;
  }
  value = value << 3; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_OP_MODE, CONFIG_REG1_ADDRESS);
}

void ADS1120::setConversionMode(uint8_t value)
{
  /* Sets the ADC conversion mode.
     0 - Single shot mode
     1 - continuous conversion mode
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x01)
  {
    value = 0x00;
  }
  value = value << 2; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_CONV_MODE, CONFIG_REG1_ADDRESS);
}

void ADS1120::setTemperatureMode(uint8_t value)
{
  /* Controls the state of the internal temperature sensor.
     0 - Disables temperature sensor
     1 - Enables temperature sensor
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x01)
  {
    value = 0x00;
  }
  value = value << 1; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_TEMP_MODE, CONFIG_REG1_ADDRESS);
}

void ADS1120::setBurnoutCurrentSources(bool value)
{
  /* Turns the 10uA burn-out current sources on or off. */
  writeRegisterMasked(value, REG_MASK_BURNOUT_SOURCES, CONFIG_REG1_ADDRESS);
}

void ADS1120::setVoltageRef(uint8_t value)
{
  /* Sets the voltage reference used by the ADC.
     0 - Internal 2.048 V
     1 - External on REFP0 and REFN0 inputs
     2 - External on AIN0/REFP1 and AIN3/REFN1 inputs
     3 - Use analog supply as reference
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x03)
  {
    value = 0x00;
  }
  value = value << 6; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_VOLTAGE_REF, CONFIG_REG2_ADDRESS);
}

void ADS1120::setFIR(uint8_t value)
{
  /* Controls the FIR filter on the ADC.
     0 - No 50 or 60 Hz rejection
     1 - Both 50 and 60 Hz rejection
     2 - 50 Hz rejection
     3 - 60 Hz rejection
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x03)
  {
    value = 0x00;
  }
  value = value << 4; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_FIR_CONF, CONFIG_REG2_ADDRESS);
}

void ADS1120::setPowerSwitch(uint8_t value)
{
  /* Configures behavior of low-side switch between AIN3/REFN1 and AVSS.
     0 - Always open
     1 - Automatically closes when START/SYNC command is sent and opens when
         POWERDOWN command is issues.
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x01)
  {
    value = 0x00;
  }
  value = value << 3; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_PWR_SWITCH, CONFIG_REG2_ADDRESS);
}

void ADS1120::setIDACcurrent(uint8_t value)
{
  /* Set current for both IDAC1 and IDAC2 excitation sources.
     0 - Off
     1 - 10 uA
     2 - 50 uA
     3 - 100 uA
     4 - 250 uA
     5 - 500 uA
     6 - 1000 uA
     7 - 1500 uA
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x07)
  {
    value = 0x00;
  }
  writeRegisterMasked(value, REG_MASK_IDAC_CURRENT, CONFIG_REG2_ADDRESS);
}

void ADS1120::setIDAC1routing(uint8_t value)
{
  /* Selects where IDAC1 is routed to.
     0 - Disabled
     1 - AIN0/REFP1
     2 - AIN1
     3 - AIN2
     4 - AIN3/REFN1
     5 - REFP0
     6 - REFN0
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x06)
  {
    value = 0x00;
  }
  value = value << 5; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_IDAC1_ROUTING, CONFIG_REG3_ADDRESS);
}

void ADS1120::setIDAC2routing(uint8_t value)
{
  /* Selects where IDAC2 is routed to.
     0 - Disabled
     1 - AIN0/REFP1
     2 - AIN1
     3 - AIN2
     4 - AIN3/REFN1
     5 - REFP0
     6 - REFN0
  */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x06)
  {
    value = 0x00;
  }
  value = value << 2; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_IDAC2_ROUTING, CONFIG_REG3_ADDRESS);
}

void ADS1120::setDRDYmode(uint8_t value)
{
  /* Controls the behavior of the DOUT/DRDY pin when new data are ready.
     0 - Only the dedicated DRDY pin is used  (Default)
     1 - Data ready indicated on DOUT/DRDY and DRDY
 */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  if (value > 0x01)
  {
    value = 0x00;
  }
  value = value << 1; // Shift to match with mask
  writeRegisterMasked(value, REG_MASK_DRDY_MODE, CONFIG_REG3_ADDRESS);
}

void ADS1120::reset()
{
  sendCommand(CMD_RESET);
}

void ADS1120::startSync()
{
  sendCommand(CMD_START_SYNC);
}

void ADS1120::powerDown()
{
  sendCommand(CMD_PWRDWN);
}

void ADS1120::rdata()
{
  sendCommand(CMD_RDATA);
}

#include "bsp_ebox.h"

#define GAIN_MAX 32
Ads1120App::Ads1120App(Gpio *cs_pin,Gpio *drdy_pin,Spi *_spi):ADS1120(cs_pin,drdy_pin,_spi)
{

};
void Ads1120App::begin()
{
    filter = new FilterMoveWindow(3);
    rpl = new Ripple(3);
    rpl->set_threshold(100);//adc hex
    rpl->set_keep(100);//ms
    gain = 1;
    ch = 6;
    ADS1120::begin();
    setVoltageRef(0);       // set Vref = internal referenc 2.048V
	setOpMode(0x00);		 // Set Normal Mode
//	adc.setDataRate(0x05);		 // Set Data rate 101.  In normal mode = 600sps
	setDataRate(0x00);		 // Set Data rate 000.  In normal mode = 20sps
	setConversionMode(0x01); // 1=Continous Mode
//	setConversionMode(0x00);// 0=Single Mode
//	setMultiplexer(0x00);	 // Set AIN0  & AIN1 (differential)
//	setMultiplexer(0x09);
	setMultiplexer(ch);
    setGain(gain);				 //Set gain 1.  Possible values are 1, 2, 4, 8, 16, 32, 64, 128.

    startSync();                        // Send start/sync for continuous conversion mode
  
//    setFIR(1);
//    setIDACcurrent(0);
//    UART.printf("reg0:0X%02X\n",readRegister(0));
//    UART.printf("reg1:0X%02X\n",readRegister(1));
//    UART.printf("reg2:0X%02X\n",readRegister(2));
//    UART.printf("reg3:0X%02X\n",readRegister(3));
    state = 0;
    flag_update = false;
}
bool Ads1120App::is_update()
{
    if(flag_update)
    {
        flag_update = false;
        return true;
    }
    else
    {
        return false;
    }
}
bool Ads1120App::avaliable()
{
    if(auto_state == Okay)
        return true;
    else
        return false;
}

float Ads1120App::read_voltage()
{
    return ADS1120::voltage(hex,2.048,gain);
}

void Ads1120App::set_ch_dc()
{ 
    if(ch != 6)
    {
        ch = 0x06;
        setMultiplexer(ch);
    }        
    flag_update = false;
    state = 0;
    count = 0;
    auto_state = Adjusting;
    filter->clear();
    rpl->reset();
    return;
}
void Ads1120App::set_ch_ac()
{
    if(ch != 5)
    {
        ch = 5;
        setMultiplexer(ch);
    }        
    flag_update = false;
    state = 0;
    count = 0;
    auto_state = Adjusting;
    filter->clear();
    rpl->reset();
    return;
}
void Ads1120App::loop()
{
    if(isDataReady())
    {
        int16_t raw = readADC();
        filter->in(raw);
        rpl->input(raw);
        flag_update = true;
        count++;
        switch(state)
        {
            case 0:
                if(count >= 2)
                {
                    state = 1;
                    auto_state = Adjusting;
                }
                break;
            case 1:
                hex = filter->get();
//                UART.printf("vol=%06.3fuV,hex=[%05d][%06.1f],gain:%02d\t",read_voltage()*1000,raw,hex,gain);
//                UART.printf("percent=%0.3f,auto:%d;",(abs(hex) / 32767),auto_state);
//                UART.printf("rpl=[%4.0f],[%d];",rpl->get(), rpl->is_stable());
//                UART.printf("millis=[%d]\n",millis());
                if((abs(hex) / 32767.0) > 0.99)  
                {
                    if(gain >= 2)
                    {
                        gain /= 2;
                        setGain(gain);				 //Set gain 1.  Possible values are 1, 2, 4, 8, 16, 32, 64, 128.
                        state = 0;
                        count = 0;
                        filter->clear();
                    }
                    else
                    {
                        auto_state = Overflow;
                    }
                }
                else if((abs(hex) / 32767) < 0.4)  
                {
                    if(gain < GAIN_MAX)
                    {
                        gain *= 2;
                        setGain(gain);				 //Set gain 1.  Possible values are 1, 2, 4, 8, 16, 32, 64, 128.
                        state = 0;
                        count = 0;
                        filter->clear();
                    }
                    else
                    {
                        auto_state = Okay;
                    }
                }
                else
                {
                        auto_state = Okay;
                }
                break;
            default:
                break;
        }    
    }
    
    

}

