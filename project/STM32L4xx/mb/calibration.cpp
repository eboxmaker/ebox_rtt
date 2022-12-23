#include "calibration.h"
#include "bsp_ebox.h"
ConfigCalibration config_cal;


const float cal_table[] = 
{
//49.8587,
//-0.00292572,
//48.5376,
//-0.0190457,
//66.4994,
//-0.00177332,
//64.8444,
//0.0462423,
49.9532	,
0.0293668	,
48.6793	,
-0.0106854	,
66.5468	,
-0.0184479	,
65.0438	,
0.0617783	,
};



void ConfigCalibration::begin()
{
    
//49.8587
//-0.00292572
//48.5376
//-0.0190457
//66.4994
//-0.00177332
//64.8444
//0.0462423
    int i = 0;
    for(i = 0; i < 4; i++)
    {
        k[i] = cal_table[2*i];
        b[i] = cal_table[2*i+1];
    }

    
    len = i * 8;
    para_to_buf();
}

int ConfigCalibration::para_to_buf()
{
    DataFloat_t XFloat;    
    uint16_t counter = 0;

    for(int i = 0; i < 4; i++)
    {
        XFloat.value = k[i];
        for(int j = 0; j < 4; j++)
            buf[counter++] = XFloat.byte[j];
        XFloat.value = b[i];
        for(int j = 0; j < 4; j++)
            buf[counter++] = XFloat.byte[j];
    }
    
//    print_buf(buf,len);
    return counter;
}

int ConfigCalibration::buf_to_para()
{
    DataFloat_t XFloat;    
    uint16_t counter = 0;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
            XFloat.byte[j] = buf[counter++];
        k[i] = XFloat.value;
        for(int j = 0; j < 4; j++)
            XFloat.byte[j] = buf[counter++];
        b[i] = XFloat.value;
    }   
//    print_buf(buf,len);
    return counter;
}


void ConfigCalibration::print(Uart &uart)
{
    uart.printf("===========ConfigCalibration===========\n");
    for(int i = 0; i < 4; i++)
    {
        uart.printf("k[%d]:%0.6f\n",i,k[i]);      
        uart.printf("b[%d]:%0.6f\n",i,b[i]);    
    }     
    uart.printf("================================\n");
}


