#ifndef __MENU_H
#define __MENU_H

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))
typedef void (*menuItemProc)(winHandle handle, uint32 parameNum, uint32 *parame);
// 菜单选项结构：包含图标和文字资源ID
typedef struct
{
	uint32 image; // 图标资源ID
	uint32 str;	  // 文字资源ID
} menuOption;

// 菜单项结构：代表单个可配置的菜单条目
typedef struct
{
	uint32 configId;	 // 配置项ID（用于系统配置）
	uint32 image;		 // 默认图标资源ID
	uint32 str;			 // 菜单文字资源ID
	uint32 optionSum;	 // 子选项数量
	menuOption *pOption; // 指向子选项数组的指针
} menuItem;

// 菜单页结构：组织多个菜单项
typedef struct
{
	uint32 unselectImage; // 未选中状态图标
	uint32 selectImage;	  // 选中状态图标
	uint32 str;			  // 页面标题文字
	uint32 itemSum;		  // 当前页菜单项数量
	menuItem *pItem;	  // 菜单项数组指针
} menuPage;

// 菜单主体结构：管理多个页面
typedef struct
{
	uint32 curPage;	 // 当前显示页索引
	uint32 pageSum;	 // 总页数
	menuPage *pPage; // 页面数组指针
} menu;
// menu(整个菜单)
// └── pPage → [menuPage1, menuPage2...](页面列表)
//     ├── pItem → [menuItem1, menuItem2...](菜单项列表)
//     │   └── pOption → [menuOption1, menuOption2...](子选项列表)
//     └── ...

#define MENU_OPTION_START(name) menuOption menuOption##name[] = {
#define MENU_OPTION_IMAGE_STR(image, str) {image, str},
#define MENU_OPTION_IMAGE(image) {image, (uint32)0},
#define MENU_OPTION_STR(str) {(uint32)0, str},
#define MENU_OPTION_END() \
	}                     \
	;

#define MENU_ITME_START(name) menuItem menuItem##name[] = {
#define MENU_ITEM_OPTIONS(name, id, image, str) {id, image, str, ARRAY_LEN(menuOption##name), menuOption##name},
#define MENU_ITEM_NO_ID(name, image, str) {(uint32)0, image, str, ARRAY_LEN(menuOption##name), menuOption##name},
#define MENU_ITEM_PROC(itemProc, image, str) {(uint32)0, image, str, (uint32)0, (menuOption *)itemProc},
#define MENU_ITME_END() \
	}                   \
	;

#define MENU_PAGE_START(name) menuPage menuPage##name[] = {
#define MENU_PAGE_ITEMS(name, unselectImage, selectImage, str) {unselectImage, selectImage, str, ARRAY_LEN(menuItem##name), menuItem##name},
#define MENU_PAGE_END() \
	}                   \
	;

#define MENU_DEFINE(name) menu menu##name = {(uint32)0, ARRAY_LEN(menuPage##name), menuPage##name};
#define MENU(name) (menu##name)

#endif
