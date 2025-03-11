/****************************************************************************
**
**                              CONFIGURE
** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
**** **                      CONFIGURE
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : config.h
* Author      : Mark.Douglas
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*
* History     :
* 2016-09-22  :
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

typedef enum
{
    // 基础时间设置
    CONFIG_ID_YEAR = 0, // 年
    CONFIG_ID_MONTH,    // 月
    CONFIG_ID_MDAY,     // 日
    CONFIG_ID_WDAY,     // 星期
    CONFIG_ID_HOUR,     // 时
    CONFIG_ID_MIN,      // 分
    CONFIG_ID_SEC,      // 秒

    // 系统设置
    CONFIG_ID_LANGUAGE,   // 系统语言
    CONFIG_ID_LCD_BRIGHT, // LCD屏幕亮度
    CONFIG_ID_AUTOOFF,    // 自动关机设置
    CONFIG_ID_SCREENSAVE, // 屏幕保护（10号配置项）

    // 设备功能设置
    CONFIG_ID_FREQUNCY,   // 频率设置（存在拼写错误，应为FREQUENCY）
    CONFIG_ID_ROTATE,     // 屏幕旋转
    CONFIG_ID_FILLIGHT,   // 补光灯设置
    CONFIG_ID_RESOLUTION, // 分辨率设置

    // 拍摄相关
    CONFIG_ID_TIMEPHOTO,     // 定时拍照
    CONFIG_ID_TIMESTAMP,     // 时间戳
    CONFIG_ID_MOTIONDECTION, // 移动侦测（存在拼写错误，应为DETECTION）
    CONFIG_ID_PARKMODE,      // 停车模式

    // 传感器设置
    CONFIG_ID_GSENSOR,  // G-Sensor重力传感器
    CONFIG_ID_KEYSOUND, // 按键音
    CONFIG_ID_IR_LED,   // 红外LED（20号配置项）

    // 高级功能
    CONFIG_ID_LOOPTIME, // 循环录像时长
    CONFIG_ID_AUDIOREC, // 音频录制开关
    CONFIG_ID_EV,       // 曝光补偿
    CONFIG_ID_WBLANCE,  // 白平衡（存在拼写错误，应为BALANCE）

    // 预览设置
    CONFIG_ID_PRESLUTION,  // 预览分辨率（PREVIEW RESOLUTION）
    CONFIG_ID_PFASTVIEW,   // 快速预览
    CONFIG_ID_PTIMESTRAMP, // 预览时间戳（存在拼写错误，应为TIMESTAMP）
    CONFIG_ID_PEV,         // 预览曝光

    // 媒体设置
    CONFIG_ID_VOLUME,    // 音量
    CONFIG_ID_THUMBNAIL, // 缩略图（30号配置项）

    // 传感器模式
    CONFIG_ID_GSENSORMODE, // G-Sensor灵敏度模式
    CONFIG_ID_FORMAT,      // 格式化设置
    CONFIG_ID_DEFUALT,     // 恢复默认（存在拼写错误，应为DEFAULT）

    // 特效与滤镜
    CONFIG_ID_VIDEORECEFFECT, // 视频录制特效
    CONFIG_ID_PS_LENS,        // 镜头滤镜（存在拼写错误，应为PS_LENS）

    // 高级拍摄
    CONFIG_ID_CONTINUOUS_SHOOTING, // 连拍模式

    CONFIG_ID_MAX // 配置项总数标记（用于遍历）
} CONFIG_ID_E;

#define _DEBG_CUT_DATA_RAW_ 0x01
#define _DEBG_CUT_DATA_YUV_ 0x02
#define _DEBG_CUT_BEFOR_CSI_ 0x10
#define _DEBG_CUT_AFTER_CSI_ 0x20

#define _DEBG_CUT_DATA_DIS_ 0
#define _DEBG_CUT_CSI_RAW_ (_DEBG_CUT_BEFOR_CSI_ | _DEBG_CUT_DATA_RAW_)
#define _DEBG_CUT_CSI_YUV_ (_DEBG_CUT_BEFOR_CSI_ | _DEBG_CUT_DATA_YUV_)
#define _DEBG_CUT_POST_YUV_ (_DEBG_CUT_AFTER_CSI_ | _DEBG_CUT_DATA_YUV_)
#define _DEBG_CUT_POST_YUV_INAVI_ (0x100 | _DEBG_CUT_POST_YUV_)
#define _DEBG_CUT_POST_YUV_LCD_ (0x200 | _DEBG_CUT_POST_YUV_)
#define _DEBG_CUT_SDR_DATA_ _DEBG_CUT_DATA_DIS_ //_DEBG_CUT_DATA_DIS_//_DEBG_CUT_POST_YUV_LCD_ //control to cut source data or dma data
void sdram_data_capture_task(u8 *fname);
/*******************************************************************************
 * Function Name  : userConfigSave
 * Description    : save user configure value to spi flash
 * Input          : none
 * Output         : None
 * Return         : s32_t
 *                    0 : always
 *******************************************************************************/
extern s32_t userConfigSave(void);
/*******************************************************************************
 * Function Name  : userConfigReset
 * Description    : reset or default user configure value
 * Input          : none
 * Output         : None
 * Return         : s32_t
 *                    0 : always
 *******************************************************************************/
extern s32_t userConfigReset(void);
/*******************************************************************************
 * Function Name  : userConfigInitial
 * Description    : initial user configure value
 * Input          : none
 * Output         : None
 * Return         : s32_t
 *                      0 : always
 *******************************************************************************/
extern s32_t userConfigInitial(void);
/*******************************************************************************
 * Function Name  : configSet
 * Description    : set configure value
 * Input          : u8_t configId : configure id
 *                  u32_t value   : configure value
 * Output         : None
 * Return         : none
 *******************************************************************************/
extern void configSet(u8_t configId, u32_t value);

/*******************************************************************************
 * Function Name  : userCofigGetValue
 * Description    : get configure value in configure table
 * Input          : u8_t configId : configure id
 * Output         : None
 * Return         : u32_t : configure value
 *******************************************************************************/
extern u32_t configGet(u8_t configId);

int configLanguage(void);
void configSys(uint32 configId);
void configSystem(void);
INT32U configValue2Int(INT8U config_id);

#endif