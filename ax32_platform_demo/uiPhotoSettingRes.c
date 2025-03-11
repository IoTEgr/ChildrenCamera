#include "uiWin.h"

enum
{
    ITEM_SELECT_ID = 0,
    ITEM_MODE_ID,
    ITEM_HIDE_RECT_ID,
    ITEM_RESOLUTION_ID,
    ITEM_SD_ID,
    ITEM_BATERRY_ID,
    ITEM_SYSTIME_ID,
    ITEM_WM_ID,
    ITEM_TITTLE_ID,
    ITEM_TITTLELOGO_ID,
    ITEM_ICON_FILTERNT_ID,
    ITEM_ICON_IMAGENT_ID,
    ITEM_ICON_MPHOTOSNT_ID,
    ITEM_ICON_SELFIENT_ID,
    ITEM_ICON_SETTINGSNT_ID,
};
static widgetCreateInfor photoSettingWin[] =
    {
        createFrameWin(Rx(0), Ry(0), Rw(320), Rh(240), R_COLOR_TRANSFER, WIN_ABS_POS),
        createRect(ITEM_WM_ID, Rx(0 + 40), Ry(0 + 25), Rw(240), Rh(160), R_COLOR_GRAY),
        createStringIcon(ITEM_TITTLE_ID, Rx(10 + 40), Ry(0 + 25), Rw(100), Rh(30), R_ID_STR_IoTEgr_001, ALIGNMENT_LEFT, R_COLOR_WHITE, 0),
        createItemManage(ITEM_SELECT_ID, Rx(10 + 40), Ry(30 + 25), Rw(220), Rh(120), R_COLOR_TRANSFER),
        createImageIcon(ITEM_ICON_FILTERNT_ID, Rx(0 + 40 + 30), Ry(160 + 35), Rw(32), Rh(32), R_ID_ICON_FILTERNTON, ALIGNMENT_CENTER),
        createImageIcon(ITEM_ICON_IMAGENT_ID, Rx(35 + 40 + 30), Ry(160 + 35), Rw(32), Rh(32), R_ID_ICON_IMAGENT, ALIGNMENT_CENTER),
        createImageIcon(ITEM_ICON_MPHOTOSNT_ID, Rx(70 + 40 + 30), Ry(160 + 35), Rw(32), Rh(32), R_ID_ICON_MPHOTOSNT, ALIGNMENT_CENTER),
        createImageIcon(ITEM_ICON_SELFIENT_ID, Rx(105 + 40 + 30), Ry(160 + 35), Rw(32), Rh(32), R_ID_ICON_SELFIENT, ALIGNMENT_CENTER),
        createImageIcon(ITEM_ICON_SETTINGSNT_ID, Rx(140 + 40 + 30), Ry(160 + 35), Rw(32), Rh(32), R_ID_ICON_SETTINGSNT, ALIGNMENT_CENTER),
        widgetEnd(),
};
static void menuResolutionShow(winHandle handle)
{
}

static void menuMDShow(winHandle handle)
{
}
static void menuMonitorShow(winHandle handle)
{
}

static void menuIrLedShow(winHandle handle)
{
}
static void menuSDShow(winHandle handle)
{
}

static void menuMicShow(winHandle handle)
{
}

static void menuBaterryShow(winHandle handle)
{
}

static void itemPoweOnTimeShow(winHandle handle, uint32 sec)
{
}