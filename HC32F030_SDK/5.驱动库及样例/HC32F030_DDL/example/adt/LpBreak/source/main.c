/******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2017-06-20 LiuHL    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adt.h"
#include "lpm.h"
#include "gpio.h"
#include "flash.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/


/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
int32_t main(void)
{
    en_adt_unit_t             enAdt;
    uint16_t                  u16Period;
    uint16_t                  u16CompareA;
    uint16_t                  u16CompareB;
    en_flash_waitcycle_t      enFlashWait;
    stc_sysctrl_clk_config_t  stcClkConfig;
    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM4ACfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM4BCfg;
    stc_gpio_config_t         stcTIM4APort;
    stc_gpio_config_t         stcTIM4BPort;
    stc_gpio_config_t         stcButtonPort;
    
    stc_lpm_config_t stcLpmCfg;
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM4ACfg);
    DDL_ZERO_STRUCT(stcAdtTIM4BCfg);
    DDL_ZERO_STRUCT(stcLpmCfg);
    DDL_ZERO_STRUCT(stcTIM4APort);
    DDL_ZERO_STRUCT(stcTIM4BPort);
    
    enFlashWait = FlashWaitCycle1;                      //读等待周期设置为1（当HCLK大于24MHz时必须至少为1）
    Flash_WaitCycle(enFlashWait);                       // Flash 等待1个周期
    
    stcClkConfig.enClkSrc    = SysctrlClkXTH;           //使用外部高速晶振,32M
    stcClkConfig.enHClkDiv   = SysctrlHclkDiv1;         // HCLK = SystemClk/1
    stcClkConfig.enPClkDiv   = SysctrlPclkDiv1;         // PCLK = HCLK/1
    Sysctrl_SetXTHFreq(SysctrlXthFreq20_32MHz);         //设置外部高速频率为20~32M
    Sysctrl_ClkInit(&stcClkConfig);
    

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //端口外设时钟使能
    

    //PA08设置为TIM4_CHA
    //Gpio_ClrAnalogMode(GpioPortA, GpioPin8);
    stcTIM4APort.enDir  = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin8, &stcTIM4APort);
    Gpio_SetAfMode(GpioPortA,GpioPin8,GpioAf6);
    
    //PA07设置为TIM4_CHB
    //Gpio_ClrAnalogMode(GpioPortA, GpioPin7);
    stcTIM4BPort.enDir  = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin7, &stcTIM4BPort);
    Gpio_SetAfMode(GpioPortA,GpioPin7,GpioAf7);
    
    //PD04设置为GPIO<--SW2控制脚
    //Gpio_ClrAnalogMode(Button_PORT, Button_PIN);
    stcButtonPort.enDrv  = GpioDrvH;
    stcButtonPort.enDir  = GpioDirIn;
    Gpio_Init(GpioPortD, GpioPin4, &stcButtonPort);

    if (Ok != Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE))//ADT外设时钟使能
    {
        return Error;
    }
    
    enAdt = AdtTIM4;
    
    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div4;
    Adt_Init(enAdt, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置

    u16Period = 0xC000;
    Adt_SetPeriod(enAdt, u16Period);                         //周期设置
    
    u16CompareA = 0x6000;
    Adt_SetCompareValue(enAdt, AdtCompareA, u16CompareA);  //通用比较基准值寄存器A设置
    
    u16CompareB = 0x6000;
    Adt_SetCompareValue(enAdt, AdtCompareB, u16CompareB);  //通用比较基准值寄存器B设置
    
    stcAdtTIM4ACfg.enCap = AdtCHxCompareOutput;            //比较输出
    stcAdtTIM4ACfg.bOutEn = TRUE;                          //CHA输出使能
    stcAdtTIM4ACfg.enPerc = AdtCHxPeriodLow;               //计数值与周期匹配时CHA电平输出低
    stcAdtTIM4ACfg.enCmpc = AdtCHxCompareHigh;             //计数值与比较值A匹配时，CHA电平输出高
    stcAdtTIM4ACfg.enStaStp = AdtCHxStateSelSS;            //CHA起始结束电平由STACA与STPCA控制
    stcAdtTIM4ACfg.enStaOut = AdtCHxPortOutLow;            //CHA起始电平为低
    stcAdtTIM4ACfg.enStpOut = AdtCHxPortOutHigh;           //CHA结束电平为高
    stcAdtTIM4ACfg.enDisSel = AdtCHxDisSel2;               //刹车模式选2(LP 刹车)
    stcAdtTIM4ACfg.enDisVal = AdtTIMxDisValLow;
    Adt_CHxXPortConfig(enAdt, AdtCHxA, &stcAdtTIM4ACfg);   //端口CHA配置
    
    stcAdtTIM4BCfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM4BCfg.bOutEn = TRUE;
    stcAdtTIM4BCfg.enPerc = AdtCHxPeriodInv;               //计数值与周期匹配时CHA电平翻转
    stcAdtTIM4BCfg.enCmpc = AdtCHxCompareInv;              //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM4BCfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIM4BCfg.enStaOut = AdtCHxPortOutLow;
    stcAdtTIM4BCfg.enStpOut = AdtCHxPortOutHigh;
    stcAdtTIM4BCfg.enDisSel = AdtCHxDisSel2;
    stcAdtTIM4BCfg.enDisVal = AdtTIMxDisValLow;
    Adt_CHxXPortConfig(enAdt, AdtCHxB, &stcAdtTIM4BCfg);   //端口CHB配置
    
    Adt_StartCount(enAdt);

#if 1
    //按下user按键(PD04)，则进入低功耗模式，否则在等待
    //注：若芯片处于低功耗模式，则芯片无法使用SWD进行调式和下载功能。
    //如需要再次下载程序，需要将芯片复位，且不按user按键
    while (1 == Gpio_GetInputIO(GpioPortD,GpioPin4));
    
    //Lpm Cfg
    stcLpmCfg.enSEVONPEND   = SevPndDisable;
    stcLpmCfg.enSLEEPDEEP   = SlpDpEnable;
    stcLpmCfg.enSLEEPONEXIT = SlpExtDisable;
    Lpm_Config(&stcLpmCfg);

    Lpm_GotoLpmMode();
#endif
    while(1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


