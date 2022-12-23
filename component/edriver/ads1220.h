#ifndef __ADS1220_H
#define __ADS1220_H
#include "ebox_core.h"


#define CONFIG_DEFAULT = 0x008B;
#define CONFIG_TEMPERATURE = 0x049B;

// Bit masks
#define PIN_BITMASK = 0x7000;
#define PGA_BITMASK = 0x0E00;
typedef union
{
    struct
    {
        uint32_t pga_bypass: 1;
        uint32_t gain: 3;
        uint32_t mux: 4;

        uint32_t bcs: 1;
        uint32_t ts: 1;
        uint32_t cm: 1;
        uint32_t mode: 2;
        uint32_t dr: 3;
        
        
        uint32_t idac: 3;
        uint32_t psw: 1;
        uint32_t filter: 2;
        uint32_t vref: 2;
        
        uint32_t : 1;
        uint32_t drdym: 1;
        uint32_t i2mux: 3;
        uint32_t i1mux: 3;
        
        
        
    } bit;
    uint8_t byte[4];
    uint32_t value;
} AdsConfig_t;


#define ADC_AIN0 4
#define ADC_AIN1 5
#define ADC_AIN2 6
#define ADC_AIN3 7

#define PGA6144 0
#define PGA4096 1
#define PGA2048 2
#define PGA1024 3
#define PGA512  4
#define PGA256_1  5
#define PGA256_2  6
#define PGA256_3  7

//0000： AINP = AIN0， AINN = AIN1（默认设置）
//0001： AINP = AIN0， AINN = AIN2
//0010： AINP = AIN0， AINN = AIN3
//0011： AINP = AIN1， AINN = AIN2
//0100： AINP = AIN1， AINN = AIN3
//0101： AINP = AIN2， AINN = AIN3
//0110： AINP = AIN1， AINN = AIN0
//0111： AINP = AIN3， AINN = AIN2
//1000： AINP = AIN0， AINN = AVSS
//1001： AINP = AIN1， AINN = AVSS
//1010： AINP = AIN2， AINN = AVSS
//1011： AINP = AIN3， AINN = AVSS
//1100： (V(REFPx) – V(REFNx)) / 4 监视（旁路 PGA）
//1101： (AVDD – AVSS) / 4 监视（旁路 PGA）
//1110： AINP 和 AINN 短接至 (AVDD + AVSS) / 2
//1111： 保留


#define ADS_CMD_RESET       B00000110
#define ADS_CMD_START       B00001000
#define ADS_CMD_POWERDOWN   B00000010
#define ADS_CMD_RDATA       B00010000
#define ADS_CMD_RREG        B00100000
#define ADS_CMD_WREG        B01000000



#define ADS_SPEED_20 0
#define ADS_SPEED_45 1
#define ADS_SPEED_90 2
#define ADS_SPEED_175 3
#define ADS_SPEED_330 4
#define ADS_SPEED_600 5
#define ADS_SPEED_1000 6

#define ADS_GAIN_1      0
#define ADS_GAIN_2      1
#define ADS_GAIN_4      2
#define ADS_GAIN_8      3
#define ADS_GAIN_16     4
#define ADS_GAIN_32     5
#define ADS_GAIN_64     6
#define ADS_GAIN_128    7


//本主板使用了两对查分模式
#define ADS_CH_1 0
#define ADS_CH_2 B0101
//#define ADS_CH_3 B1011
#define ADS_CH_TEST B1110

#define ADS_CH_NUMS 2
#define AVERAGE_COUNT 3


#define TEMP_RATIO 0.00041//41uf/度
class Ads1220Interface
{
    
public:
    AdsConfig_t cfg;
public:
    Ads1220Interface(Gpio *cs,Gpio *drdy, Spi *spi);
    void        begin();

    void        write_cfg(AdsConfig_t *cfg);
    AdsConfig_t read_cfg();
    void        set_gain(uint8_t gain);
    void        self_calibration();
    void        set_mux(uint8_t mux);
    void        start();
    int32_t     read();
    bool        self_test();
    void        reset();
    bool        avaliable();
    void        print(AdsConfig_t *cfg);
    int32_t    offset;

private:
    Gpio    *cs;
    Gpio    *drdy;
    Spi     *spi;
    Spi::Config_t spiConfig;

    void        write_reg(uint8_t addr,uint8_t *p,uint8_t len);
    void        read_reg(uint8_t addr,uint8_t *p,uint8_t len);


};

class Ads1220App:public Ads1220Interface
{
public:
    Ads1220App(Gpio *cs,Gpio *drdy, Spi *spi):Ads1220Interface(cs,drdy,spi){};
    
    void    begin();
        
    int32_t     read_average(uint8_t index);
    float      read_vol_average(uint8_t index);
    float      hex_to_voltage(int32_t hex);
    void        loop();
        
        
        
public:
    int32_t    buf[ADS_CH_NUMS][AVERAGE_COUNT];
    uint8_t     ch_table[ADS_CH_NUMS];
    uint8_t     ch_index;
    uint8_t     count;
    uint8_t     state;

};

#endif
