#include "application.h"

static msgDealFunc taskMsgDeal[EVENT_MAX];
static msgDealFunc unshieldedMsgDeal[EVENT_MAX];
extern msgDealInfor sysRecommendMsgDeal[];

void msgFuncRegister(msgDealInfor *infor)
{
	uint32 i;
	// 清空任务消息处理函数数组
	for (i = 0; i < EVENT_MAX; i++)
		taskMsgDeal[i] = NULL;

	// 注册系统推荐消息处理函数
	i = 0;
	for (;;)
	{
		// 遇到越界消息ID或空函数指针时停止注册
		if (sysRecommendMsgDeal[i].msgId >= EVENT_MAX || sysRecommendMsgDeal[i].func == NULL)
			break;
		// 将消息ID与处理函数绑定：array[msgID] = handler
		taskMsgDeal[sysRecommendMsgDeal[i].msgId] = sysRecommendMsgDeal[i].func;
		i++;
	}

	if (infor == NULL)
		return;

	// 注册用户自定义消息处理函数（可覆盖系统推荐）
	i = 0;
	for (;;)
	{
		// 防御性检查：确保消息ID合法且函数有效
		if (infor[i].msgId >= EVENT_MAX || infor[i].func == NULL)
			break;
		// 绑定用户自定义处理函数
		taskMsgDeal[infor[i].msgId] = infor[i].func;
		i++;
	}
}

void unshieldedMsgFuncRegister(msgDealInfor *infor)
{
	uint32 i;
	// 初始化无屏蔽消息处理数组（无系统默认值）
	for (i = 0; i < EVENT_MAX; i++)
		unshieldedMsgDeal[i] = NULL;

	if (infor == NULL)
		return;

	// 直接注册用户提供的紧急消息处理函数
	i = 0;
	for (;;)
	{
		// 终止条件：非法消息ID或空函数指针
		if (infor[i].msgId >= EVENT_MAX || infor[i].func == NULL)
			break;
		// 绑定紧急消息处理函数
		unshieldedMsgDeal[infor[i].msgId] = infor[i].func;
		i++;
	}
}

void msgDeal(void)
{
	MSG_T *msgid;
	INT8U err;
	uint32 msgType;
	uint32 msgData;
	msgid = XMsgQPend(SysCtrl.sysQ, &err); // check system event
	if (err == X_ERR_NONE)
	{
		msgType = getType(msgid);
		// debg("msgType=%d\n",msgType);
		if (msgType < EVENT_MAX)
		{
			msgData = getValue(msgid);
			if (msgType != KEY_EVENT_POWEROFF && msgType >= KEY_EVENT_START && msgType < KEY_EVENT_END && msgData == KEY_PRESSED)
			{
				/*
								if(msgType==KEY_EVENT_OK)
								{
									if(!(!menuIsOpen()&&(taskCurrent()==TASK_PHOTO_ENCODE||taskCurrent()==TASK_PLAY_BACK||taskCurrent()==TASK_AUDIO_PLAY)))
									{
										deamon_keysound_play();
									}
								}
								else
								{
									if(!(!menuIsOpen()&&(taskCurrent()==TASK_PLAY_BACK||taskCurrent()==TASK_AUDIO_PLAY)))
									{
										deamon_keysound_play();
									}
								}
				*/
				if (!(!menuIsOpen() && (taskCurrent() == TASK_PLAY_BACK)) && (msgType != KEY_EVENT_POWEROFF) && (MEDIA_STAT_START != task_video_Get_Status()) && !((TASK_AUDIO_PLAYER == taskCurrent()) && (MP3_DAC_START == mp3_play_sta() || MP3_DAC_PAUSE == mp3_play_sta())) && (taskCurrent() != TASK_USB_DEVICE) && (taskCurrent() != TASK_PHOTO_ENCODE))
				{
					deamon_keysound_play();
				}
			}
			if (taskMsgDeal[msgType]) // 为什么通过消息类型就能找到处理函数？消息类型就是消息的顺序，上面初始化了，把消息类型和处理函数一一对应起来了，为什么相同消息可以有不同的处理函数，因为任务开始会打开ui打开ui会调用msgFuncRegister，把消息类型和处理函数一一对应起来了，所以相同消息可以有不同的处理函数
				taskMsgDeal[msgType](winGetCurrent(), 1, &msgData);
			if (unshieldedMsgDeal[msgType])
				unshieldedMsgDeal[msgType](winGetCurrent(), 1, &msgData);
		}
	}
}

void msgDealByType(uint32 msgType, void *handle, uint32 parameNum, uint32 *parame)
{
	if (taskMsgDeal[msgType])
		taskMsgDeal[msgType](handle, parameNum, parame);
	if (unshieldedMsgDeal[msgType])
		unshieldedMsgDeal[msgType](handle, parameNum, parame);
}
void msgDealByMsgInfor(msgDealInfor *infor, uint32 msgType, void *handle, uint32 parameNum, uint32 *parame)
{
	uint32 i;
	if (infor == NULL)
		return;
	i = 0;
	for (;;)
	{
		if (infor[i].msgId >= EVENT_MAX || infor[i].func == NULL)
			break;
		if (infor[i].msgId == msgType)
		{
			if (infor[i].func)
				infor[i].func(handle, parameNum, parame);
			break;
		}

		i++;
	}
}
