#include "uiPhotoSettingRes.c"
#include "userInterface.h"

#define currentPage() (currentMenu->pPage[currentMenu->curPage])
#define getItem(n) (currentPage().pItem[n])
#define TOTAL_PAGE 5
extern MENU(PSmovie);

static menu *currentMenu = NULL;
static menuItem *currentItem = NULL;
static bool photoSettingOpened = false;
static u8_t photoSettingCurrentPageIndex = 1;
static u8 *menuitem_bk_buf = NULL;
static u8 photoSettingPageIconFlag[TOTAL_PAGE] = {0};
static const struct
{
    uint32_t titleRes;
    uint32_t activeIconRes;
    uint16_t activeItemID;
    uint32_t configId;
} photoSettingPageConfig[] = {
    [0] = {0}, // 占位符
    {R_ID_STR_IoTEgr_001, R_ID_ICON_FILTERNTON, ITEM_ICON_FILTERNT_ID, CONFIG_ID_PS_LENS},
    {R_ID_STR_IoTEgr_002, R_ID_ICON_IMAGENTON, ITEM_ICON_IMAGENT_ID, CONFIG_ID_PRESLUTION},
    {R_ID_STR_IoTEgr_003, R_ID_ICON_MPHOTOSNTON, ITEM_ICON_MPHOTOSNT_ID, CONFIG_ID_CONTINUOUS_SHOOTING},
    {R_ID_STR_IoTEgr_004, R_ID_ICON_SELFIENTON, ITEM_ICON_SELFIENT_ID, CONFIG_ID_TIMEPHOTO},
    {R_ID_STR_SET_SETTING, R_ID_ICON_SETTINGSNTON, ITEM_ICON_SETTINGSNT_ID, CONFIG_ID_MAX},
};

bool photoSettingIsOpen(void)
{
    return photoSettingOpened;
}
static uint32 getOptionResInfor(uint32 item, uint32 *image, uint32 *str)
{
    if (image)
        *image = currentItem->pOption[item].image;
    if (str)
        *str = currentItem->pOption[item].str;
}
void photoSettingConfigIdChange(winHandle handle)
{
    currentItem = &getItem(itemManageGetCurrentItem(winItem(handle, ITEM_SELECT_ID)));
    currentItem->configId = photoSettingPageConfig[photoSettingCurrentPageIndex].configId;
}
void photoSettingGetItemPosition(winHandle handle)
{
    uint32 itemNum = 0, config;
    config = configGet(photoSettingPageConfig[photoSettingCurrentPageIndex].configId);
    itemNum = 0;
    while (itemNum < currentPage().itemSum)
    {
        if (currentPage().pItem[itemNum].str == config)
            break;
        itemNum++;
    }
    if (itemNum >= currentPage().itemSum)
        itemNum = 0;
    currentItem = &getItem(itemNum);
    itemManageSetCurItem(winItem(handle, ITEM_SELECT_ID), itemNum);
    winSetResid(winItem(handle, ITEM_MODE_ID), currentPage().selectImage);
}
void photoSettingPageChange(winHandle handle)
{
    winDestroy(&handle);
    uiOpenWindow(&photoSettingWindow, 2, &MENU(PSmovie), photoSettingCurrentPageIndex);
}
static uint32 getItemResInfor(uint32 item, uint32 *image, uint32 *str)
{
    if (image)
        *image = getItem(item).image;
    if (str)
        *str = getItem(item).str;
}

static uint32 getItemResInforEx(uint32 item, uint32 *image, uint32 *str, uint32 *selectImage, uint32 *selectStr)
{
    uint32 config;
    menuItem *pItem = &getItem(item);
    if (image)
        *image = pItem->image;
    if (str)
        *str = pItem->str;
    if (pItem->configId == 0 && pItem->optionSum == 0)
    {
        if (selectImage)
            *selectImage = INVALID_RES_ID;
        if (selectStr)
            *selectStr = INVALID_RES_ID;
    }
    else if (pItem->configId == 0)
    {
        if (selectImage)
            *selectImage = INVALID_RES_ID;
        if (selectStr)
            *selectStr = pItem->pOption[0].str;
    }
    else
    {

        if ((pItem->configId == CONFIG_ID_FORMAT) || (pItem->configId == CONFIG_ID_DEFUALT))
        {
            if (selectImage)
                *selectImage = INVALID_RES_ID;
            if (selectStr)
                *selectStr = INVALID_RES_ID;
        }
        else
        {

            config = configGet(pItem->configId);
#if 0 // 20220823
			if(pItem->optionSum==2&&(config==R_ID_STR_COM_OFF||config==R_ID_STR_COM_ON))
			{
				if(selectImage)
				{
					if(config==R_ID_STR_COM_OFF)
						*selectImage=R_ID_ICON_MTOFF;
					else
						*selectImage=R_ID_ICON_MTON2;
				}
				if(selectStr)
				*selectStr=INVALID_RES_ID;
			}
			else
#endif
            {
                if (selectImage)
                    *selectImage = NULL; // R_ID_ICON_MTMORE;
                if (selectStr)
                    *selectStr = config;
            }
        }
    }
}
static int menuItemKeyMsgOk(winHandle handle, uint32 parameNum, uint32 *parame)
{
    menuItem *pItem;
    uint32 config;
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED)
    {
        if (photoSettingCurrentPageIndex == 5)
        {
            pItem = &getItem(itemManageGetCurrentItem(winItem(handle, ITEM_SELECT_ID)));
            if (pItem->optionSum == 0)
                ((menuItemProc)pItem->pOption)(handle, parameNum, parame);
            else if (pItem->optionSum > 1)
            {
                uiOpenWindow(&menuOptionWindow, 1, pItem);
            }
        }
        else
        {
            if (currentItem)
            {
                configSet(currentItem->configId, currentItem->str);
                configSys(currentItem->configId);
                userConfigSave();
            }
            winDestroy(&handle);
        }
    }
    return 0;
}
static int menuItemKeyMsgUp(winHandle handle, uint32 parameNum, uint32 *parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED || keyState == KEY_CONTINUE)
    {
        itemManagePreItem(winItem(handle, ITEM_SELECT_ID));
        photoSettingConfigIdChange(handle);
    }
    return 0;
}
static int menuItemKeyMsgDown(winHandle handle, uint32 parameNum, uint32 *parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED || keyState == KEY_CONTINUE)
    {
        itemManageNextItem(winItem(handle, ITEM_SELECT_ID));
        photoSettingConfigIdChange(handle);
    }
    return 0;
}
static int menuItemKeyMsgMenu(winHandle handle, uint32 parameNum, uint32 *parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED)
    {
        photoSettingCurrentPageIndex = (photoSettingCurrentPageIndex + TOTAL_PAGE - 2) % TOTAL_PAGE + 1;
        photoSettingPageChange(handle);
    }
    return 0;
}
static int menuItemKeyMsgMode(winHandle handle, uint32 parameNum, uint32 *parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED)
    {
        photoSettingCurrentPageIndex = photoSettingCurrentPageIndex % TOTAL_PAGE + 1;
        photoSettingPageChange(handle);
    }
    return 0;
}
static int menuItemSysMsgSD(winHandle handle, uint32 parameNum, uint32 *parame)
{
    menuSDShow(handle);
    return 0;
}
static int menuSysMsgUSB(winHandle handle, uint32 parameNum, uint32 *parame)
{
    menuBaterryShow(handle);
    return 0;
}
static int menuSysMsgBattery(winHandle handle, uint32 parameNum, uint32 *parame)
{
    if (SysCtrl.usb == USB_STAT_NULL)
        menuBaterryShow(handle);
    return 0;
}
static int menuItemOpenWin(winHandle handle, uint32 parameNum, uint32 *parame)
{
    resInfor infor;
    uint32 itemNum = 0, config, i;
    if (parameNum < 1)
    {
        winDestroy(&handle);
        return 0;
    }
    deg_Printf("menuItem Open Win!!!\n");
    currentMenu = (menu *)parame[0];
    photoSettingCurrentPageIndex = parame[1];
    currentMenu->curPage = photoSettingCurrentPageIndex - 1;
    itemManageSetItemHeight(winItem(handle, ITEM_SELECT_ID), R1h(35));
    if (parame[1] == 5)
    {
        itemManageCreateItem(winItem(handle, ITEM_SELECT_ID), itemCreateMenuItemEx, NULL, currentPage().itemSum);
        itemManageSetResInforFuncEx(winItem(handle, ITEM_SELECT_ID), getItemResInforEx);
        itemManageSetCharInfor(winItem(handle, ITEM_SELECT_ID), 0, ALIGNMENT_LEFT, R_COLOR_WHITE);
        itemManageNextItem(winItem(handle, ITEM_SELECT_ID));
    }
    else
    {
        itemManageCreateItem(winItem(handle, ITEM_SELECT_ID), itemCreateMenuItem, getItemResInfor, currentPage().itemSum);
        itemManageSetCharInfor(winItem(handle, ITEM_SELECT_ID), 0, ALIGNMENT_CENTER, R_COLOR_WHITE);
        photoSettingGetItemPosition(handle);
    }
    itemManageSetSelectColor(winItem(handle, ITEM_SELECT_ID), R_COLOR_YELLOW);
    itemManageSetUnselectColor(winItem(handle, ITEM_SELECT_ID), R_COLOR_TRANSFER);
    winSetResid(winItem(handle, ITEM_TITTLE_ID), photoSettingPageConfig[photoSettingCurrentPageIndex].titleRes);
    winSetResid(winItem(handle, photoSettingPageConfig[photoSettingCurrentPageIndex].activeItemID),
                photoSettingPageConfig[photoSettingCurrentPageIndex].activeIconRes);
    return 0;
}
static int menuItemCloseWin(winHandle handle, uint32 parameNum, uint32 *parame)
{
    deg_Printf("menuItem Close Win!!!\n");

    if (NULL != menuitem_bk_buf)
    {
        hal_sysMemFree(menuitem_bk_buf);
        menuitem_bk_buf = NULL;
    }

    currentMenu = NULL;
    photoSettingOpened = false;
    return 0;
}
static int menuItemWinChildClose(winHandle handle, uint32 parameNum, uint32 *parame)
{
    deg_Printf("menuItem WinChild Close!!!\n");
    winSetResid(winItem(handle, ITEM_MODE_ID), currentPage().selectImage);
    menuResolutionShow(handle);
    menuMDShow(handle);
    menuMonitorShow(handle);
    menuIrLedShow(handle);
    menuSDShow(handle);
    menuMicShow(handle);
    menuBaterryShow(handle);
    // itemManageUpdateCurItem(winItem(handle,ITEM_SELECT_ID));
    itemManageUpdateAllitem(winItem(handle, ITEM_SELECT_ID));
#if SMALL_PANEL_SUPPORT == 0
    itemPoweOnTimeShow(handle, SysCtrl.powerOnTime);
#endif
    return 0;
}
static int menuItemSysMsg1s(winHandle handle, uint32 parameNum, uint32 *parame)
{
    if (SysCtrl.usb == USB_STAT_DCIN)
    {

        if (winIsVisible(winItem(handle, ITEM_BATERRY_ID)))
            winSetVisible(winItem(handle, ITEM_BATERRY_ID), false);
        else
        {
            winSetResid(winItem(handle, ITEM_BATERRY_ID), R_ID_ICON_MTBATTERY5);
            winSetVisible(winItem(handle, ITEM_BATERRY_ID), true);
        }
    }
#if SMALL_PANEL_SUPPORT == 0
    itemPoweOnTimeShow(handle, SysCtrl.powerOnTime);
#endif
    return 0;
}
static int menuItemKeyMsgPower(winHandle handle, uint32 parameNum, uint32 *parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED)
    {
        winDestroy(&handle);
    }
    return 0;
}

msgDealInfor photoSettingMsgDeal[] =
    {
        {SYS_OPEN_WINDOW, menuItemOpenWin},
        {SYS_CLOSE_WINDOW, menuItemCloseWin},
        {SYS_CHILE_COLSE, menuItemWinChildClose},
        {KEY_EVENT_OK, menuItemKeyMsgOk},
        {KEY_EVENT_UP, menuItemKeyMsgUp},
        {KEY_EVENT_POWER, menuItemKeyMsgPower},
        {KEY_EVENT_DOWN, menuItemKeyMsgDown},
        {KEY_EVENT_MENU, menuItemKeyMsgMenu},
        {KEY_EVENT_MODE, menuItemKeyMsgMode},
        {SYS_EVENT_SDC, menuItemSysMsgSD},
        {SYS_EVENT_USB, menuSysMsgUSB},
        {SYS_EVENT_BAT, menuSysMsgBattery},
        {SYS_EVENT_1S, menuItemSysMsg1s},
        {EVENT_MAX, NULL},
};
WINDOW(photoSettingWindow, photoSettingMsgDeal, photoSettingWin)
