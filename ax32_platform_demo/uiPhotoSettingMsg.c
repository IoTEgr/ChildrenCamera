#include "uiPhotoSettingRes.c"
#include "userInterface.h"

#define currentPage() (currentMenu->pPage[currentMenu->curPage])
#define getItem(n) (currentPage().pItem[n])
static menu *currentMenu = NULL;
static menuItem *currentItem = NULL;
static bool photoSettingOpened = false;
static u8_t currentPageIndex = 1;
static u8 *menuitem_bk_buf = NULL;

extern MENU(PSmovie);

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
void configIdChange()
{
    switch (currentPageIndex)
    {
    case 1:
        currentItem->configId = CONFIG_ID_PS_LENS;
        break;
    case 2:
        currentItem->configId = CONFIG_ID_PRESLUTION;
        break;
    case 3:
        currentItem->configId = CONFIG_ID_CONTINUOUS_SHOOTING;
        break;
    case 4:
        currentItem->configId = CONFIG_ID_TIMEPHOTO;
        break;
    }
}
void getItemPosition(winHandle handle)
{
    uint32 itemNum = 0, config, configId;
    if (currentPageIndex == 5)
    {
        itemManageSetCharInfor(winItem(handle, ITEM_SELECT_ID), 0, ALIGNMENT_LEFT, R_COLOR_WHITE);
        return;
    }
    else
    {
        itemManageSetCharInfor(winItem(handle, ITEM_SELECT_ID), 0, ALIGNMENT_CENTER, R_COLOR_WHITE);
        switch (currentPageIndex)
        {
        case 1:
            configId = CONFIG_ID_PS_LENS;
            break;
        case 2:
            configId = CONFIG_ID_PRESLUTION;
            break;
        case 3:
            configId = CONFIG_ID_CONTINUOUS_SHOOTING;
            break;
        case 4:
            configId = CONFIG_ID_TIMEPHOTO;
            break;
        }
    }
    config = configGet(configId);
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
    configIdChange();
    itemManageSetCurItem(winItem(handle, ITEM_SELECT_ID), itemNum);
    winSetResid(winItem(handle, ITEM_MODE_ID), currentPage().selectImage);
}
void pageChange(winHandle handle)
{
    currentMenu->curPage = currentPageIndex - 1;
    itemManageUpdateRes(winItem(handle, ITEM_SELECT_ID), currentPage().itemSum, ((itemManageObj *)uiHandleToPtr(handle))->currentRes);
    getItemPosition(handle);

    switch (currentPageIndex)
    {
    case 1:
    {
        winSetResid(winItem(handle, ITEM_TITTLE_ID), R_ID_STR_IoTEgr_001);
        winSetResid(winItem(handle, ITEM_ICON_FILTERNT_ID), R_ID_ICON_FILTERNTON);
        winSetResid(winItem(handle, ITEM_ICON_IMAGENT_ID), R_ID_ICON_IMAGENT);
        winSetResid(winItem(handle, ITEM_ICON_MPHOTOSNT_ID), R_ID_ICON_MPHOTOSNT);
        winSetResid(winItem(handle, ITEM_ICON_SELFIENT_ID), R_ID_ICON_SELFIENT);
        winSetResid(winItem(handle, ITEM_ICON_SETTINGSNT_ID), R_ID_ICON_SETTINGSNT);
    }
    break;
    case 2:
    {
        winSetResid(winItem(handle, ITEM_TITTLE_ID), R_ID_STR_IoTEgr_002);
        winSetResid(winItem(handle, ITEM_ICON_FILTERNT_ID), R_ID_ICON_FILTERNT);
        winSetResid(winItem(handle, ITEM_ICON_IMAGENT_ID), R_ID_ICON_IMAGENTON);
        winSetResid(winItem(handle, ITEM_ICON_MPHOTOSNT_ID), R_ID_ICON_MPHOTOSNT);
        winSetResid(winItem(handle, ITEM_ICON_SELFIENT_ID), R_ID_ICON_SELFIENT);
        winSetResid(winItem(handle, ITEM_ICON_SETTINGSNT_ID), R_ID_ICON_SETTINGSNT);
    }
    break;
    case 3:
    {
        winSetResid(winItem(handle, ITEM_TITTLE_ID), R_ID_STR_IoTEgr_003);
        winSetResid(winItem(handle, ITEM_ICON_FILTERNT_ID), R_ID_ICON_FILTERNT);
        winSetResid(winItem(handle, ITEM_ICON_IMAGENT_ID), R_ID_ICON_IMAGENT);
        winSetResid(winItem(handle, ITEM_ICON_MPHOTOSNT_ID), R_ID_ICON_MPHOTOSNTON);
        winSetResid(winItem(handle, ITEM_ICON_SELFIENT_ID), R_ID_ICON_SELFIENT);
        winSetResid(winItem(handle, ITEM_ICON_SETTINGSNT_ID), R_ID_ICON_SETTINGSNT);
    }
    break;
    case 4:
    {
        winSetResid(winItem(handle, ITEM_TITTLE_ID), R_ID_STR_IoTEgr_004);
        winSetResid(winItem(handle, ITEM_ICON_FILTERNT_ID), R_ID_ICON_FILTERNT);
        winSetResid(winItem(handle, ITEM_ICON_IMAGENT_ID), R_ID_ICON_IMAGENT);
        winSetResid(winItem(handle, ITEM_ICON_MPHOTOSNT_ID), R_ID_ICON_MPHOTOSNT);
        winSetResid(winItem(handle, ITEM_ICON_SELFIENT_ID), R_ID_ICON_SELFIENTON);
        winSetResid(winItem(handle, ITEM_ICON_SETTINGSNT_ID), R_ID_ICON_SETTINGSNT);
    }
    break;
    case 5:
    {
        winSetResid(winItem(handle, ITEM_TITTLE_ID), R_ID_STR_SET_SETTING);
        winSetResid(winItem(handle, ITEM_ICON_FILTERNT_ID), R_ID_ICON_FILTERNT);
        winSetResid(winItem(handle, ITEM_ICON_IMAGENT_ID), R_ID_ICON_IMAGENT);
        winSetResid(winItem(handle, ITEM_ICON_MPHOTOSNT_ID), R_ID_ICON_MPHOTOSNT);
        winSetResid(winItem(handle, ITEM_ICON_SELFIENT_ID), R_ID_ICON_SELFIENT);
        winSetResid(winItem(handle, ITEM_ICON_SETTINGSNT_ID), R_ID_ICON_SETTINGSNTON);
    }
    break;
    }
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
        if (currentPageIndex == 5)
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
        currentItem = &getItem(itemManageGetCurrentItem(winItem(handle, ITEM_SELECT_ID)));
        configIdChange();
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
        currentItem = &getItem(itemManageGetCurrentItem(winItem(handle, ITEM_SELECT_ID)));
        configIdChange();
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
        if (currentPageIndex > 1)
        {
            if (currentPageIndex == 5)
            {
                winDestroy(&handle);
                uiOpenWindow(&photoSettingWindow, 2, &MENU(PSmovie), 4);
                return 0;
            }
            currentPageIndex--;
        }
        else
            currentPageIndex = 5;
        if (currentPageIndex == 5)
        {
            winDestroy(&handle);
            uiOpenWindow(&photoSettingWindow, 2, &MENU(PSmovie), 5);
            return 0;
        }
        pageChange(handle);
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
        if (currentPageIndex < 5)
        {
            if (currentPageIndex == 4)
            {
                winDestroy(&handle);
                uiOpenWindow(&photoSettingWindow, 2, &MENU(PSmovie), 5);
                return 0;
            }
            currentPageIndex++;
        }
        else
        {
            winDestroy(&handle);
            uiOpenWindow(&photoSettingWindow, 2, &MENU(PSmovie), 1);
            return 0;
        }
        pageChange(handle);
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
    if (parame[1] == 5)
    {
        currentPageIndex = 5;
        currentMenu->curPage = 5;
        currentItem = &getItem(0);
        itemManageSetItemHeight(winItem(handle, ITEM_SELECT_ID), R1h(35));
        itemManageCreateItem(winItem(handle, ITEM_SELECT_ID), itemCreateMenuItemEx, NULL, currentPage().itemSum);
        itemManageSetResInforFuncEx(winItem(handle, ITEM_SELECT_ID), getItemResInforEx);
        itemManageSetSelectColor(winItem(handle, ITEM_SELECT_ID), /*R_COLOR_BLUE1*/ R_COLOR_YELLOW);
        itemManageSetUnselectColor(winItem(handle, ITEM_SELECT_ID), /*R_COLOR_GRAY*/ R_COLOR_TRANSFER);
        getItemPosition(handle);
    }
    else
    {
        currentPageIndex = parame[1];
        currentMenu->curPage = parame[1] - 1;
        currentItem = &getItem(0);
        itemManageSetItemHeight(winItem(handle, ITEM_SELECT_ID), R1h(35));
        itemManageCreateItem(winItem(handle, ITEM_SELECT_ID), itemCreateMenuItem, getItemResInfor, currentPage().itemSum);
        // itemManageSetResInforFuncEx(winItem(handle, ITEM_SELECT_ID), );
        itemManageSetSelectColor(winItem(handle, ITEM_SELECT_ID), /*R_COLOR_BLUE1*/ R_COLOR_YELLOW);
        itemManageSetUnselectColor(winItem(handle, ITEM_SELECT_ID), /*R_COLOR_GRAY*/ R_COLOR_TRANSFER);
        getItemPosition(handle);
    }
    pageChange(handle);
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
