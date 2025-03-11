#ifndef __WIN_MANAGE_H
#define __WIN_MANAGE_H
#include "mType.h"
#include "userDef.h"
#include "memPool.h"

#define uiLock()
#define uiUnlock()
typedef uint32 resID;
#define INVALID_RES_ID 0x7fffffff
#define strToResId(addr) ((resID)(addr))
#define resIdIsStr(id) (((id) & 0x80000000) == 0)
#define resIdToStrAddr(id) ((char *)(id))
#define resIdIsImage(id) (((id) & 0xff000000) == 0x80000000)

#define ALIGNMENT_LEFT (1 << 0)
#define ALIGNMENT_CENTER (1 << 1)
#define ALIGNMENT_RIGHT (1 << 2)
typedef uint8 charFont;
#define DEFAULT_FONT 0;

#define WIN_INVALID (1 << 0)
#define WIN_WIDGET (1 << 1)
#define WIN_VISIBLE (1 << 2)
#define WIN_ABS_POS (1 << 3) // create window by absolute position
#define WIN_FRAME (1 << 4)
#define WIN_NOT_ZOOM (1 << 5)
#define WIN_TOUCH_SUPPORT (1 << 6)

typedef enum
{
	// 窗口基础消息
	MSG_WIN_CREATE = 0,		// 窗口创建完成
	MSG_WIN_DESTROY,		// 窗口销毁请求
	MSG_WIN_CHILE_DESTROY,	// 子窗口销毁通知（疑似拼写错误，应为CHILD）
	MSG_WIN_WIDGET_DESTROY, // 控件销毁通知
	MSG_WIN_PARENT_DEAL,	// 父窗口处理请求

	// 绘制相关消息
	MSG_WIN_INVALID, // 窗口区域失效（需要重绘）
	MSG_WIN_PAINT,	 // 窗口绘制请求

	// 资源管理消息
	MSG_WIN_CHANGE_RESID,		// 更改资源ID
	MSG_WIN_CHG_ITEM_SEL_RESID, // 更改选中项资源ID
	MSG_WIN_INVALID_RESID,		// 资源ID无效通知
	MSG_WIN_UPDATE_RESID,		// 更新资源ID

	// 窗口状态控制
	MSG_WIN_VISIBLE_SET,	 // 设置窗口可见性
	MSG_WIN_VISIBLE_GET,	 // 获取窗口可见状态
	MSG_WIN_CHANGE_BG_COLOR, // 修改背景色
	MSG_WIN_CHANGE_FG_COLOR, // 修改前景色
	MSG_WIN_CHANGE_STRINFOR, // 修改字符串显示参数

	// 进度条专用消息
	MSG_WIN_PROGRESS_RATE,		// 设置进度百分比
	MSG_WIN_PROGRESS_DIRECTION, // 设置进度方向

	// 控件状态消息
	MSG_WIN_SELECT_INFOR,	   // 设置选中状态基础参数
	MSG_WIN_UNSELECT_INFOR,	   // 设置未选中状态基础参数
	MSG_WIN_SELECT_INFOR_EX,   // 设置选中状态扩展参数
	MSG_WIN_UNSELECT_INFOR_EX, // 设置未选中状态扩展参数

	// 触摸交互消息
	MSG_WIN_TOUCH,			 // 触摸事件通知
	MSG_WIN_TOUCH_GET_INFOR, // 获取触摸信息

	// 控件操作消息
	MSG_WIN_SELECT,		// 选中控件
	MSG_WIN_UNSELECT,	// 取消选中控件
	MSG_WIN_GET_WIDGET, // 获取控件句柄
	MSG_WIN_ADD_WIDGET, // 添加新控件

	// 控件属性操作
	MSG_WIDGET_GET_ID,			 // 获取控件ID
	MSG_WIDGET_SET_ID,			 // 设置控件ID
	MSG_WIDGET_NEXT,			 // 切换到下一个控件
	MSG_WIDGET_PRE,				 // 切换到上一个控件
	MSG_WIDGET_CUR,				 // 获取当前控件
	MSG_WIDGET_SELECT,			 // 控件被选中事件
	MSG_WIDGET_CHANGE_ALL_RESID, // 批量修改控件资源ID

	// 对话框消息
	MSG_DIALOG_INIT,  // 对话框初始化
	MSG_DIALOG_CLOSE, // 对话框关闭

	MSG_MAX, // 消息类型总数（用于校验）
} msgId;

typedef uint8 uiColor;
#define DEFAULT_COLOR 0xfb
#define INVALID_COLOR 0xff
typedef void *winHandle;
// #define INVALID_HANDLE      0xffff
#define INVALID_HANDLE NULL
typedef struct _uiRect
{
	int16 x0;			  // 矩形左上角X坐标
	int16 x1;			  // 矩形右下角X坐标（需满足 x1 >= x0）
	int16 y0;			  // 矩形左上角Y坐标
	int16 y1;			  // 矩形右下角Y坐标（需满足 y1 >= y0）
	struct _uiRect *next; // 链表指针，用于连接多个矩形区域
} uiRect;
typedef struct
{
	msgId id;
	winHandle hWin;
	winHandle hSrc;
	union
	{
		void *p;
		uint32 v;
	} parameter;
} winMsg;
typedef void (*winCB)(winMsg *msg);

typedef struct _winObj // 所有窗口对象的基类结构体
{
	uiRect rect;		// 窗口屏幕坐标范围 (x0,y0)-(x1,y1)
	uiRect invalidRect; // 无效区域链表头（需要重绘的区域）
	uiColor bgColor;	// 窗口背景颜色（DEFAULT_COLOR 为默认值）
	uint16 style;		// 窗口样式标志位（WIN_VISIBLE | WIN_TOUCH_SUPPORT 等）
	char *name;			// 窗口名称（允许为NULL）
	winCB cb;			// 窗口消息处理回调函数

	// 窗口层级关系
	winHandle parent; // 父窗口句柄（INVALID_HANDLE 表示无父窗口）
	winHandle child;  // 子窗口链表头
	winHandle next;	  // 同级窗口/控件链表（用于挂载widget）
} winObj;
#define WIN_SIZE sizeof(winObj)

#define TOUCH_NONE 0
#define TOUCH_PRESS 1
#define TOUCH_OVER 2
#define TOUCH_RELEASE 3

typedef struct _touchInfor
{
	uiRect touchArea;
	winHandle touchWin;
	winHandle touchHandle;
	uint32 touchID;
	uint32 touchItem;
	uint32 touchState;
} touchInfor;

typedef struct _widgetCreateInfor widgetCreateInfor;
typedef winHandle (*widgetCreateFun)(widgetCreateInfor *infor, winHandle parent, winCB cb);
typedef struct _widgetCreateInfor
{
	// 核心创建函数指针（如按钮/图标等控件的具体创建方法）
	widgetCreateFun widgetCreate;

	// 基础控件参数
	uint16 id;		 // 控件唯一标识符（用于消息传递）
	int16 x0;		 // 控件相对父窗口的X起始坐标
	int16 y0;		 // 控件相对父窗口的Y起始坐标
	uint16 width;	 // 控件显示宽度（像素）
	uint16 height;	 // 控件显示高度（像素）
	uint16 style;	 // 控件样式标志位（如可见性、触摸支持等）
	uiColor bgColor; // 默认背景颜色（使用DEFAULT_COLOR表示继承父窗口）

	// 图像资源参数
	resID image;	  // 图片资源标识（INVALID_RES_ID表示无图片）
	uint8 imageAlign; // 图片对齐方式（ALIGNMENT_LEFT/CENTER/RIGHT）

	// 文字显示参数
	resID str;		   // 字符串资源标识（地址或资源ID）
	uint8 strAlign;	   // 文字对齐方式
	uiColor fontColor; // 文字颜色
	charFont font;	   // 字体类型（DEFAULT_FONT为系统默认）
	// bool  strIsASCII;  // 预留字段：字符编码类型标识
	void *private; // 私有数据指针（用于扩展功能）

	// 选中状态样式参数（S后缀表示Selected状态）
	uint8 strAlignS;	// 选中时文字对齐方式
	charFont fontS;		// 选中时字体类型
	uiColor fontColorS; // 选中时文字颜色
	uiColor bgColorS;	// 选中时背景颜色
	uiColor rimColorS;	// 选中时边框颜色

	// 通用边框颜色
	uiColor rimColor; // 默认边框颜色（INVALID_COLOR表示无边框）
} widgetCreateInfor;

#define createFrameWin(x0, y0, width, height, bgColor, style) {frameWinCreateIndirect, INVALID_WIDGET_ID, x0, y0, width, height, style, bgColor, INVALID_RES_ID, 0, INVALID_RES_ID, 0, INVALID_COLOR, 0, NULL, 0, 0, INVALID_COLOR, bgColor, INVALID_COLOR, INVALID_COLOR}
#define createItemManage(id, x0, y0, width, height, bgColor) {itemManageCreateIndirect, id, x0, y0, width, height, 0, bgColor, INVALID_RES_ID, 0, INVALID_RES_ID, 0, INVALID_COLOR, 0, NULL, 0, 0, INVALID_COLOR, bgColor, INVALID_COLOR, INVALID_COLOR}
#define createButton(id, x0, y0, width, height, image, imageAlign, str, strAlign, fontColor, font) {buttonCreateIndirect, id, x0, y0, width, height, 0, INVALID_COLOR, image, imageAlign, str, strAlign, fontColor, font, NULL, strAlign, font, fontColor, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR}
#define createImageIcon(id, x0, y0, width, height, image, imageAlign) {imageIconCreateIndirect, id, x0, y0, width, height, 0, INVALID_COLOR, image, imageAlign, INVALID_RES_ID, 0, INVALID_COLOR, 0, NULL, 0, 0, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR}
#define createRect(id, x0, y0, width, height, fgColor) {rectCreateIndirect, id, x0, y0, width, height, 0, INVALID_COLOR, INVALID_RES_ID, 0, INVALID_RES_ID, 0, fgColor, 0, NULL, 0, 0, fgColor, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR}
// font: R_FONT_DEFAULT R_FONT_NUM2 R_FONT_NUM3 R_FONT_NUM4 R_FONT_NUM5
#define createStringIcon(id, x0, y0, width, height, str, strAlign, fontColor, font) {stringIconCreateIndirect, id, x0, y0, width, height, 0, INVALID_COLOR, INVALID_RES_ID, 0, str, strAlign, fontColor, font, NULL, strAlign, font, fontColor, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR}
#define createStringRim(id, x0, y0, width, height, str, strAlign, fontColor, font, rimColor) {stringIconCreateIndirect, id, x0, y0, width, height, 0, INVALID_COLOR, INVALID_RES_ID, 0, str, strAlign, fontColor, font, NULL, strAlign, font, fontColor, INVALID_COLOR, INVALID_COLOR, rimColor}
#define createProgressBar(id, x0, y0, width, height, bgColor, fgColor) {progressBarCreateIndirect, id, x0, y0, width, height, 0, bgColor, INVALID_RES_ID, 0, INVALID_RES_ID, 0, fgColor, 0, NULL, 0, 0, fgColor, bgColor, INVALID_COLOR, INVALID_COLOR}

#define createStringItem(id, x0, y0, width, height, str, strAlign, fontColor, font, bgColor, strAlignSel, fontColorSel, fontSel, bgColorSel) {stringIconCreateIndirect, id, x0, y0, width, height, 0, bgColor, INVALID_RES_ID, 0, str, strAlign, fontColor, font, NULL, strAlignSel, fontSel, fontColorSel, bgColorSel, INVALID_COLOR, INVALID_COLOR}
#define createWidgetManage(id, minID, maxID, getResFun) {widgetManageCreateIndirect, id, 0, 0, minID, maxID, 0, 0, INVALID_RES_ID, 0, INVALID_RES_ID, 0, INVALID_COLOR, 0, getResFun, 0, 0, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR}

#define widgetEnd() {NULL, INVALID_WIDGET_ID, 0, 0, 0, 0, 0, 0, INVALID_RES_ID, 0, INVALID_RES_ID, 0, INVALID_COLOR, 0, NULL, 0, 0, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR, INVALID_COLOR}

typedef struct
{
	charFont font;
	uint8 strAlign;
	uiColor fontColor;
	uiColor bgColor;
	uiColor rimColor;
} strInfor;
typedef struct
{
	charFont font;
	uint8 strAlign;
	uiColor fontColor;
	uiColor bgColor;
	uiColor rimColor;

	uiColor color;
	resID image;
} resInfor;

void resInforInit(resInfor *infor);
bool winContain(uiRect *parent, uiRect *child);
void winSendMsg(winHandle handle, winMsg *msg);
void winSendMsgNoData(winHandle handle, msgId id);
void winSendMsgToParent(winHandle handle, winMsg *msg);
void winSendMsgToParentNoData(winHandle handle, msgId id);
void winDefaultProc(winMsg *msg);
winHandle winCreate(int16 x0, int16 y0, uint16 width, uint16 height, winHandle parent, winCB cb, uint32 size, uint16 style);
void winDestroy(winHandle *hWin);
int winTouch(uiRect *rect, uint32 touchState);
void winGetTouchInfor(winHandle hWin, touchInfor *infor);
void winSetbgColor(winHandle hWin, uiColor bgColor);
void winSetfgColor(winHandle hWin, uiColor bgColor);
void winSetPorgressRate(winHandle hWin, uint32 rate);
void winResize(winHandle hWin, int16 x0, int16 y0, int16 x1, int16 y1);
void winSetVisible(winHandle hWin, bool visible);
bool winIsVisible(winHandle hWin);
void winSetResid(winHandle hWin, resID id);
void winSetItemSelResid(winHandle hWin, resID id);
void winSetStrInfor(winHandle hWin, charFont font, uint8 strAlign, uiColor fontColor);
void winSetSelectInfor(winHandle hWin, resInfor *infor);
#define winSetInfor winSetUnselectInfor
void winSetUnselectInfor(winHandle hWin, resInfor *infor);
void winChangeNotify(winHandle hWin);
void winUpdate(winHandle hWin);
uint8 winPaint(void);
void winGetRelativePos(winHandle hWin, uiRect *pos);
void winGetPos(winHandle hWin, uiRect *pos);
winHandle winGetCurrent(void);
void winDestroyDeskTopChildWin(void);
void winSetName(winHandle hWin, char *name);
void winInit(void);
void winUninit(void);
void winUpdateAllResId(void);
bool winOverlapping(uiRect *rect1, uiRect *rect2);
void winIntersection(uiRect *out, uiRect *win1, uiRect *win2);

void drawRectangle(uiRect *rect, uiColor color);
void drawRectangleToBuf(u8 *drawBuff, u16 buf_w, u16 buf_h, uiRect *rect, uiColor color);

void drawImage(uiRect *winRect, uiRect *drawRect, resID id, uint8 alignment, uiColor bgColor);
void drawString(uiRect *winRect, uiRect *drawRect, resID id, charFont font, uint8 alignment, uiColor fontColor, uiColor bgColor, uiColor rimColor);
uint32 resGetAddrAndSize(resID id, uint16 *width, uint16 *height);

#include "uiAlloc.h"

#endif
