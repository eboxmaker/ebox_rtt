#include "modbus_addr.h"
#include "config.h"
#include "crc.h"
#include "runValue.h"

#include "apps.h"
#include "power.h"

uint16_t hreg_z0_len = 0;
uint16_t hreg_z1_len = 0;
uint16_t hreg_z2_len = 0;
uint16_t hreg_z3_len = 0;
uint16_t hreg_z4_len = 0;
uint16_t ireg_z1_len = 0;
uint16_t ireg_z2_len = 0;

#define HREG_Z0_START 0
#define HREG_Z1_START 1
#define HREG_Z2_START 50
#define HREG_Z3_START 100
#define HREG_Z4_START 200

#define IREG_RUN_ADR_START 0
#define IREG_READ_ONLY_ADR_START 200

//ModbusSerial object
ModbusSerial mb;

bool hreg_writed_flag = false;
uint8_t sample_mode;

/*******************************
dev     uart        pwr     ctr
pc      lpuart1     PE2     PC13
bat     uart2       PA4     PA0
gps     uart3       PB1
4g      uart1       PA8
*******************************/

void modbus_callback_no_para()
{

}
    
void modbus_callback(uint8_t fun,uint16_t regStart,uint8_t regCntOrValue)
{

    if(regStart < 80)
    {
    
    }
    if((fun == 16 || fun == 6))
    {
//        UART.printf("fun:%d,f1:[%d],f2:[%d]",fun,regStart,regCntOrValue);


        hreg_writed_flag = true;
    }

    //work mode cmd reg
    if(fun == 6 && regStart == REG_MODE)
    {
        hreg_writed_flag = false;
        if(mb.Hreg(REG_MODE) < 2)
        {
            mb.Hreg(2,mb.Hreg(REG_MODE));
        }
        set_sample_mode(mb.Hreg(REG_MODE));
    }
    
    //cal cmd reg
    if(fun == 6 && regStart == REG_CH)
    {
        hreg_writed_flag = false;
        cal_set_ch(mb.Hreg(REG_CH));
    }
    if(fun == 6 && regStart == REG_SAVE)
    {
        hreg_writed_flag = false;
        cal_save(mb.Hreg(REG_SAVE));
    }
    if(fun == 16 && regStart == REG_ADD_DATA && regCntOrValue == 2)
    {
        hreg_writed_flag = false;
        float val = hreg_to_float(REG_ADD_DATA);
        cal_add_data(val);
    }
    
    //test cmd reg
    if(fun == 6 && regStart == REG_TEST_CH)
    {
        hreg_writed_flag = false;
        test_set_ch(mb.Hreg(REG_TEST_CH));
    }

}



void set_sample_mode(uint8_t mode)
{
    if(mode > 3 || mode < 0) 
    {
        goto end;
    }
    //stop current task
    switch(sample_mode)
    {
        case WORK_REGU:
            app_regu_delete();break;
        case WORK_CONT:
            app_cont_delete();break;
        case WORK_TEST:
            app_test_delete();break;
        case WORK_CAL:
            app_cal1_delete();break;
            break;
    }

    sample_mode = mode;
    
    switch(mode)
    {
        case WORK_REGU:
            app_regu_create();break;
        case WORK_CONT:
            app_cont_create();break;
        case WORK_TEST:
            app_test_create();break;
        case WORK_CAL:
            app_cal1_create();break;
        default:
            sample_mode = WORK_CONT;
            break;
    }
    end:
    mb.Hreg(REG_MODE,sample_mode);
}


/*******************************
dev     uart        pwr     ctr
pc      uart1       PE2     PC13
bat     uart2       PA4     PE3
gps     uart3       PB1
4g      lpuart1     PA8
*******************************/

void mb_init()
{
    
//    PA0.mode(OUTPUT_PP_PN);
//    PA0.set();
//    delay_ms(1);
//    uart2.begin(115200,RxIt);

    mb.config(&uart1,115200,&PC13);

    // Set the Slave ID (1-247)
    mb.setSlaveId(config.modbus_id);  
    mb.attach(modbus_callback);
    
    init_mb_addr_table();
    
    //read para to frame
    config.para_to_frame();
    dev.para_to_buf();
    
    mb_frame_to_hreg();
    mb_update_ireg_read_only();


    sample_mode = config.sampleMode;
    mb.Hreg(REG_MODE,sample_mode);

}

void init_mb_addr_table()
{
    hreg_z0_len = 1;//flag 
    hreg_z1_len = config.frame_z1_len / 2;//
    hreg_z2_len = config.frame_z2_len / 2;
    hreg_z3_len = config.frame_z3_len / 2; 
    hreg_z4_len = config.frame_z4_len / 2; 
    for(int i = 0; i < hreg_z0_len; i++)
    {
        mb.addHreg(HREG_Z0_START + i);
    }
    for(int i = 0; i < hreg_z1_len; i++)
    {
        mb.addHreg(HREG_Z1_START + i);
    }
    for(int i = 0; i < hreg_z2_len; i++)
    {
        mb.addHreg(HREG_Z2_START + i);
    }   
    for(int i = 0; i < hreg_z3_len; i++)
    {
        mb.addHreg(HREG_Z3_START + i);
        mb.addHreg(HREG_Z3_START + i,i);
    }
    //z4 reg count = 50;
    for(int i = 0; i < 50; i++)
    {
        mb.addHreg(HREG_Z4_START + i);
        mb.addHreg(HREG_Z4_START + i,i);
    }
    mbDebug("z0:%d --- %d\n",HREG_Z0_START,HREG_Z0_START+hreg_z0_len - 1);
    mbDebug("z1:%d --- %d\n",HREG_Z1_START,HREG_Z1_START+hreg_z1_len - 1);
    mbDebug("z2:%d --- %d\n",HREG_Z2_START,HREG_Z2_START+hreg_z2_len - 1);
    mbDebug("z3:%d --- %d\n",HREG_Z3_START,HREG_Z3_START+hreg_z3_len - 1);
    mbDebug("z4:%d --- %d\n",HREG_Z4_START,HREG_Z4_START+hreg_z4_len - 1);

//    
//    hreg_zone3_len = dev.para_buf_zone3_len/2;
//    for(int i = 0; i < hreg_zone3_len; i++)
//    {
//        mb.addHreg(HREG_PROTECTED_ADDR_START + i);
//    }
//    
//    dev.run_para_to_buf();//更新一次run_buf_len；
//    ireg_run_len = dev.run_buf_len/2;
    ireg_z1_len = run.len / 2;
    ireg_z2_len = dev.len / 2;
    for(int i = 0; i < ireg_z1_len; i++)
    {
        mb.addIreg(i);
        mb.Ireg(i,i);
    }
    for(int i = 0; i < ireg_z2_len; i++)
    {
        mb.addIreg(i + IREG_READ_ONLY_ADR_START);
    }
    
}



void mb_run_to_ireg()
{
    uint16_t *ptr16;

    for(int i = 0; i < run.len/2; i++)
    {
        ptr16 = (uint16_t *)&run.buf[i * 2];
        mb.Ireg(i,*ptr16);
    }
}
    
void mb_update_ireg_read_only()
{
    uint16_t *ptr16;

    for(int i = 0 ; i < dev.len/2; i++)
    {
        ptr16 = (uint16_t *)&dev.buf[i * 2];
        mb.Ireg(i + IREG_READ_ONLY_ADR_START,*ptr16);
    }
}

void mb_frame_to_hreg()
{
    uint16_t *ptr16;
    int index = 0;
//    hreg_len =  config.frame_len / 2;

    for(int i = 0; i < hreg_z0_len; i++)
    {
        int addr = HREG_Z0_START + i;
        ptr16 = (uint16_t *)&config.frame[index * 2];
        mb.Hreg(addr,*ptr16);
        index++;
    }
    for(int i = 0; i < hreg_z1_len; i++)
    {
        int addr = HREG_Z1_START + i;
        ptr16 = (uint16_t *)&config.frame[index * 2];
        mb.Hreg(addr,*ptr16);
        index++;
    }
    for(int i = 0; i < hreg_z2_len; i++)
    {
        int addr = HREG_Z2_START + i;
        ptr16 = (uint16_t *)&config.frame[index * 2];
        mb.Hreg(addr,*ptr16);
        index++;
    }
    for(int i = 0; i < hreg_z3_len; i++)
    {
        int addr = HREG_Z3_START + i;
        ptr16 = (uint16_t *)&config.frame[index * 2];
        mb.Hreg(addr,*ptr16);
        index++;
    }
    for(int i = 0; i < hreg_z4_len; i++)
    {
        int addr = HREG_Z4_START + i;
        ptr16 = (uint16_t *)&config.frame[index * 2];
        mb.Hreg(addr,*ptr16);
        index++;
    }
    mb.Hreg(2,config.sampleMode);
//    print_buf(config.frame,config.frame_len);
}
uint16_t mb_to_frame()
{
    DataU16_t crcValue;
    DataU16_t XUint16;
    uint16_t index = 0;
//    UART.printf("-------------\n");
    for(int i = 0; i < hreg_z0_len; i++)
    {
        int addr = HREG_Z0_START + i;
        XUint16.value = mb.Hreg(addr);
        config.frame[index++] = XUint16.byte[0];
        config.frame[index++] = XUint16.byte[1];
//        UART.printf("Hreg(%03d)=0X%04X\n",addr,XUint16.value);
    }
    
//        UART.printf("-------------\n");
    for(int i = 0; i < hreg_z1_len; i++)
    {
        int addr = HREG_Z1_START + i;
        XUint16.value = mb.Hreg(addr);
        config.frame[index++] = XUint16.byte[0];
        config.frame[index++] = XUint16.byte[1];
//        UART.printf("Hreg(%03d)=0X%04X\n",addr,XUint16.value);
    }
    
//    UART.printf("-------------\n");
    for(int i = 0; i < hreg_z2_len; i++)
    {
        int addr = HREG_Z2_START + i;
        XUint16.value = mb.Hreg(addr);
        config.frame[index++] = XUint16.byte[0];
        config.frame[index++] = XUint16.byte[1];
    }
    
    //mqtt zone
    for(int i = 0; i < hreg_z3_len; i++)
    {
        int addr = HREG_Z3_START + i;
        XUint16.value = mb.Hreg(addr);
        config.frame[index++] = XUint16.byte[0];
        config.frame[index++] = XUint16.byte[1];
    }

    //calibration zone only save 16 regs;
    for(int i = 0; i < 16; i++)
    {
        int addr = HREG_Z4_START + i;
        XUint16.value = mb.Hreg(addr);
        config.frame[index++] = XUint16.byte[0];
        config.frame[index++] = XUint16.byte[1];
//        UART.printf("Hreg(%03d)=0X%04X\n",addr,XUint16.value);
    }

    crcValue.value = crc16(config.frame,index);

    config.frame[index * 2  ] = crcValue.byte[0];
    config.frame[index * 2 + 1 ] = crcValue.byte[1];
    index += 2;
    
    return index;    
}


void mb_task()
{
    static uint32_t last = millis();
    bool flag = 0;
    mb.task();
    if(hreg_writed_flag)
    {

        hreg_writed_flag = false;
//      UART.printf("fun:%d,regstart:%d,cnt:%d\n",fun,regStart,regCnt);
        mb_to_frame();
//      print_buf(config.frame,config.frame_len);
        if(sample_mode < 2)
        {
            if(config.sampleMode != mb.Hreg(2))
                flag = 1;
        }
        config.frame_to_para();
        config.para_limit();
        config.para_to_frame();
        mb_frame_to_hreg();
        
        dev.para_to_buf();
        mb_update_ireg_read_only();

        config.save();
        
        if(flag == 1)
        {
            flag = 0;
            set_sample_mode(config.sampleMode);
        }

        if(!config.gps_ctr)
        {
            power_off(PwrGps);
        }
        else
        {
            power_on(PwrGps);
        }
    
    }
    
    if(millis() - last > 500)
    {
        last = millis();
        run.r0_hex = adc.read_hex();;
        run.r1_gain = adc.read_gain();
        run.r2_voltage = adc.read_voltage();
        run.r4_rpl = adc.rpl->get();
        run.para_to_buf();
        mb_run_to_ireg();
    }
}




float hreg_to_float(uint16_t reg_start)
{
    DataFloat_t data;
    data.byte[0] = mb.Hreg(reg_start)%256;
    data.byte[1] = mb.Hreg(reg_start)/256;
    data.byte[2] = mb.Hreg(reg_start + 1)%256;
    data.byte[3] = mb.Hreg(reg_start + 1)/256;
    return data.value;
}

void float_to_hreg(uint16_t reg_start,float value)
{
    DataFloat_t data;
    data.value = value;
    mb.Hreg(reg_start,data.byte[0] + (data.byte[1] << 8));
    mb.Hreg(reg_start+1,data.byte[2] + (data.byte[3] << 8));
    return ;
}

void float_to_Ireg(uint16_t reg_start,float value)
{
    DataFloat_t data;
    data.value = value;
    mb.Ireg(reg_start,data.byte[0] + (data.byte[1] << 8));
    mb.Ireg(reg_start+1,data.byte[2] + (data.byte[3] << 8));
    return ;
}
