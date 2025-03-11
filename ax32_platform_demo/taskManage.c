#include "application.h"

static sysTask *taskArray[TASK_MAX];
static sysTask *curTask;
static taskID curTaskId;
static taskID nextTaskId;
static taskID preTaskId;

void taskInit(void)
{
	int i;
	for (i = 0; i < TASK_MAX; i++)
	{
		taskArray[i] = NULL;
	}
	curTask = NULL;
	curTaskId = TASK_POWER_ON;
	nextTaskId = TASK_MAX;
	preTaskId = TASK_MAX;
}
void taskInfor(int n)
{
	int i;
	deg_Printf("task infor[%d]:\n", n);
	for (i = 0; i < TASK_MAX; i++)
	{
		if (taskArray[i])
			deg_Printf("task [%s] normal\n", taskArray[i]->name);
		else
			deg_Printf("task [%d] abnormal\n", i);
	}
}
void taskRegister(taskID id, sysTask *task)
{
	if (id < TASK_MAX)
	{
		if (taskArray[id])
			deg_Printf("waring: task[%s]already registered\n", taskArray[id]->name);
		taskArray[id] = task;
		deg_Printf("task[%s] register\n", taskArray[id]->name);
	}
	else
	{
		deg_Printf("task id[%d] too large,max id:%d\n", id, TASK_MAX);
	}
}
taskID taskPrevious(void)
{

	return preTaskId;
}
taskID taskCurrent(void)
{

	return curTaskId;
}
taskID taskNext(void)
{

	return nextTaskId;
}

void taskStart(taskID id, uint32 arg)
{
	if (id < TASK_MAX)
	{
		if (nextTaskId == TASK_POWER_OFF)
		{
			deg_Printf("task [%s] has highest priority,task [%d] was ignored\n", taskArray[TASK_POWER_OFF]->name, id);
			return;
		}
		if (taskArray[id])
		{
			taskArray[id]->arg = arg;
			preTaskId = curTaskId;
			nextTaskId = id;
			deg_Printf("will start task [%s]\n", taskArray[id]->name);
		}
		else
		{
			if (id == TASK_USB_UPDATE)
			{
				nextTaskId = id;
				deg_Printf("start usb update\n");
			}
			else
			{
				deg_Printf("start task [%d] not registered\n", id);
			}
		}
	}
}
void taskChange(void)
{
	/*switch(curTaskId)
	{

		case TASK_VIDEO_RECORD:
			taskStart(TASK_PHOTO_ENCODE,0);
			break;
		case TASK_PHOTO_ENCODE:
			taskStart(TASK_PLAY_BACK,0);
			break;
		case TASK_PLAY_BACK:
			taskStart(TASK_AUDIO_PLAYER,0);
			break;
		case TASK_AUDIO_PLAYER:
			taskStart(TASK_VIDEO_RECORD,0);
			break;

		default:
			deg_Printf("task change,start default video task\n");
			taskStart(TASK_VIDEO_RECORD,0);
			break;

	}*/
}
void taskService(void)
{
	uint32 n = 0;
	while (1) // 主调度循环
	{
		// USB升级任务具有最高优先级
		if (nextTaskId == TASK_USB_UPDATE)
			return;

		//---------- 任务关闭流程 ----------
		if (curTask)
		{
			deg_Printf("task [%s] closed\n", curTask->name);
			if (curTask->taskClose) // 执行任务关闭回调
				curTask->taskClose(curTask->arg);
			winDestroyDeskTopChildWin(); // 销毁所有桌面子窗口
		}

		//---------- 任务切换准备 ----------
		uiHeapInfor(n++);				// 打印内存状态(调试用)
		curTaskId = nextTaskId;			// 切换当前任务ID
		nextTaskId = TASK_MAX;			// 重置下一任务标识
		curTask = taskArray[curTaskId]; // 获取新任务对象

		// 异常处理：当任务未注册时强制切换到关机任务
		if (curTask == NULL)
		{
			deg_Printf("task [%d] not registered,start [%s] task\n",
					   curTaskId, taskArray[TASK_POWER_OFF]->name);
			curTaskId = TASK_POWER_OFF;
			curTask = taskArray[curTaskId];
		}

		//---------- 新任务启动 ----------
		deg_Printf("task [%s] open\n", curTask->name);
		if (curTask->taskOpen) // 执行任务初始化回调
			curTask->taskOpen(curTask->arg);

		// 非关机任务需要启动UI服务
		if (curTaskId != TASK_POWER_OFF)
		{
			drawUIService(true); // 启动UI绘制服务
		}

		//---------- 任务运行循环 ----------
		while (1)
		{
			if (nextTaskId < TASK_MAX) // 检测到新任务请求则跳出
				break;

			systemService();		  // 执行系统基础服务
			if (curTask->taskService) // 执行任务主循环回调
				curTask->taskService(curTask->arg);
		}
	}
}
