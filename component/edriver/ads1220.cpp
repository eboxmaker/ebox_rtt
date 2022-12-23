#include "ads1220.h"
#include "ebox.h"


void Ads1220Interface::print(AdsConfig_t *cfg)
{
    
    uart1.println();
    uart1.print(cfg->value,BIN);
    uart1.printf("\t0X%08X\r\n",cfg->value);

}



void Ads1220App::begin()
{
    AdsConfig_t temp_cfg;
    Ads1220Interface::begin();
    ch_table[0] = ADS_CH_1;
    ch_table[1] = ADS_CH_2;
    
    
    ch_index = 0;
    
    count = 0;
    //寄存器0
    //
    cfg.bit.mux = 0;//
    cfg.bit.gain = ADS_GAIN_16;
    cfg.bit.pga_bypass = 0;//0:开启,1:关闭
    
    //寄存器1
    //
    cfg.bit.dr = ADS_SPEED_20;//20sps速度模式。
    cfg.bit.mode = 0;//正常模式
    cfg.bit.cm = 0;//1：连续转换
    cfg.bit.ts = 0;//1：启用温度传感器
    cfg.bit.bcs = 0;//电流源关断

    //寄存器2
    //内部基准源，滤波器
    cfg.bit.vref = 0;//00:2.048v内部基准
    cfg.bit.filter = B10;//同时抑制50,60HZ


    write_cfg(&cfg);
    temp_cfg =  read_cfg();
    if(temp_cfg.value != cfg.value)
    {
        uart1.printf("config failed\r\n");
    }
    set_mux(ch_table[ch_index]);
    start();
    
    //遍历第一遍。保证后期读的第一个数据是正确的
    for(int i = 0 ; i < AVERAGE_COUNT * ADS_CH_NUMS * 5; i++)
    {
        loop();
    }
    
}
float Ads1220App::hex_to_voltage(int32_t hex)
{
    no_interrupts();
    float temp = hex * 4.096 / 16777216.0;
    interrupts();
    return temp;
}


int32_t Ads1220App::read_average(uint8_t index)
{
    //抛弃第一个数据,第一个数据为刚切换完通道的数据，
    //误差可能会偏大
    int64_t sum = 0;
    for(int i = 0; i < AVERAGE_COUNT; i++)
    {
        sum += buf[index][i];

    }
    sum = sum / (AVERAGE_COUNT);
    return sum;
}
float Ads1220App::read_vol_average(uint8_t index)
{
    float vol;
    int32_t temp = read_average(index);
    vol = hex_to_voltage(temp);
    vol = vol / pow(2,(float)cfg.bit.gain);
//    uart1.printf("read_vol_average(%d):%d,%0.6f\r\n",index,temp,vol);
    return vol;
}

void Ads1220App::loop()
{
    static uint32_t last_mark = 0;
    uint32_t diff;
    int32_t value;
    if(avaliable()){
        diff = millis() - last_mark;
        last_mark = millis();
        no_interrupts();
        value = read();
        buf[ch_index][count++] = value;
//        uart1.printf("time:%d  mux:%d  raw:%d\r\n",  diff,ch_index,value);
        if(count >= AVERAGE_COUNT){
            count = 0;
            ch_index = (ch_index + 1) % ADS_CH_NUMS;

            set_mux(ch_table[ch_index]);
//            uart1.printf(" mux:%02d(%d)\n",cfg.bit.mux,ch_index);
       }
       start();
        interrupts();
    }
    else {
        diff = millis() - last_mark;
        if(diff > 100){
            start();
//            uart1.printf(" 重启AD\n");
        }

    }

}








Ads1220Interface::Ads1220Interface(Gpio *cs, Gpio *drdy, Spi *spi)
{
    this->drdy = drdy;
    this->cs = cs;
    this->spi = spi;

    cfg.value = 0;  
    
}
void Ads1220Interface::begin()
{
    spiConfig.dev_num = cs->id;
    spiConfig.mode = Spi::MODE1;
    spiConfig.prescaler = Spi::DIV16;
    spiConfig.bit_order = Spi::MSB;

    spi->begin(&spiConfig);
    cs->mode(OUTPUT_PP);
    cs->set();
    drdy->mode(INPUT_PU);
    reset();
    offset = 0;

}


void Ads1220Interface::start()
{
    uint8_t cmd = ADS_CMD_START;

    spi->take(&spiConfig);
    cs->reset();
    spi->write(cmd);
    cs->set();
    spi->release();
    delay_us(200);
}
void Ads1220Interface::self_calibration()
{
    uint8_t temp_mux = cfg.bit.mux;
    cfg.bit.mux = ADS_CH_TEST;
    write_cfg(&cfg);
    delay_ms(50);
    offset = read();
    uart1.printf("ADC校准偏差:%d\n",offset);
    cfg.bit.mux = temp_mux;
    write_cfg(&cfg);
    delay_ms(50);

}
void Ads1220Interface::set_gain(uint8_t gain)
{
    uint8_t temp_mux = cfg.bit.mux;
    limit(gain,(uint8_t)0,(uint8_t)7);
    cfg.bit.gain = gain;
    cfg.bit.mux = ADS_CH_TEST;
    write_cfg(&cfg);
    delay_ms(50);
    offset = read();
    uart1.printf("ADC校准偏差:%d\n",offset);
    cfg.bit.mux = temp_mux;
    write_cfg(&cfg);
    delay_ms(50);

}
void Ads1220Interface::set_mux(uint8_t mux)
{

    limit(mux,(uint8_t)0,(uint8_t)15);
    if(mux != cfg.bit.mux)
    {
        cfg.bit.mux = mux;
//        write_cfg(&cfg);
        write_reg(0,&cfg.byte[0],1);

    }
}




bool Ads1220Interface::self_test()
{
    AdsConfig_t temp_cfg;
    write_cfg(&cfg);
    temp_cfg = read_cfg();
    if(cfg.value == temp_cfg.value)
    {
        return true;
    }
    return false;
}
bool Ads1220Interface::avaliable()
{
    
    //如果引脚为高电平，说明没有转换完成，返回false
    return drdy->read()?false:true;
}

int32_t Ads1220Interface::read()
{
    int32_t value = 0;
    uint32_t last = millis();

    //读取相应通道值
    spi->take(&spiConfig);
    cs->reset();
    spi->write(ADS_CMD_RDATA);
    value |= spi->read() << 16;
    value |= spi->read() << 8;
    value |= (spi->read() & 0XF0);
    cs->set();
    spi->release();
    value = (value<<8)/256;//将得到的数据进行补码转换
//    uart1.printf("mux:%02d-%02d   raw:\t%d\r\n",  cfg.bit.mux,value);
    return value;
}
void Ads1220Interface::write_reg(uint8_t addr,uint8_t *p,uint8_t len)
{
    uint8_t cmd = ADS_CMD_WREG;
    addr &= B11;
    len -= 1;
    len &= B11;
    cmd |= addr << 2;
    cmd |= len ;


    spi->take(&spiConfig);
    cs->reset();
    spi->write(cmd);
    spi->write_buf(p,len+1);    
    cs->set();    
   spi->release();
  
}
void Ads1220Interface::read_reg(uint8_t addr,uint8_t *p,uint8_t len)
{
    uint8_t cmd = ADS_CMD_RREG;
    addr &= B11;
    len -= 1;
    len &= B11;
    cmd |= addr << 2;
    cmd |= len ;   

    spi->take(&spiConfig);
    cs->reset();
    spi->write(cmd);
    spi->read_buf(p,len+1);  
    cs->set();    
    spi->release();
    

}
void Ads1220Interface::write_cfg(AdsConfig_t *cfg)
{
    write_reg(0,cfg->byte,4);
    delay_us(200);
}
AdsConfig_t Ads1220Interface::read_cfg()
{
    AdsConfig_t temp;
    read_reg(0,temp.byte,4);
    return temp;
}

void Ads1220Interface::reset()
{
    uint8_t cmd = ADS_CMD_RESET;

    spi->take(&spiConfig);
    cs->reset();
    spi->write(cmd);
    delay_ms(20);
    cs->set();
    spi->release();

}
