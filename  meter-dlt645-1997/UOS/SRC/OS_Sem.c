#define OS_SEM_C
#include "OS_Core.h"

void OS_Sem_Clr()
{
  OS_ASSERT(OS_SEM_NUM<=32);
  OS_memset((void *)&OS_Sem,0,sizeof(OS_Sem),(void *)&OS_Sem,sizeof(OS_Sem));
#if OS_USE_CS==1
  OS_SET_STRUCT_SUM(OS_Sem);
#endif  
}

//创建信号量,信号量的ID由应用程序自己固定
//Val为信号量的初始值，1表示资源被占用，0表示资源已释放
//创建成功返回1，失败返回0
OS_INT8U OS_Sem_Init(OS_INT8U ID,OS_INT8U Val)
{
#if OS_USE_CS==1
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_SUM(OS_Sem);
  OS_ASSERT(1==Re);
#endif
  
  if(ID>OS_SEM_NUM-1)
    return 0;
  
  OS_Sem.Sem_Num++;
  if(Val==0)
    OS_CLR_BIT(OS_Sem.Sem,ID);
  else
    OS_SET_BIT(OS_Sem.Sem,ID);
  
#if OS_USE_CS==1
  OS_SET_STRUCT_SUM(OS_Sem);//设置校验和
#endif
  
  return 1; 
}

//Post信号量,成功返回1，失败返回0
OS_INT8U OS_Sem_Post(OS_INT8U ID)
{
#if OS_USE_CS==1
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_SUM(OS_Sem);
  OS_ASSERT(1==Re);
#endif
  
  if(ID>=OS_SEM_NUM)
  {
    OS_ASSERT(0);
    return 0;
  }
  
  OS_CLR_BIT(OS_Sem.Sem,ID);
  //不是该任务握资源，则认为是错误的释放信号量
  //本系统中的信号量都是用于互斥访问，因此信号量的申请和释放在每个任务中都是成对出现的
  if(OS_GET_BIT(OS_Sem.Task_Hold_Flag[Task_Info.Cur_Task_ID],ID)==0)
  {
    OS_ASSERT(0);  
    return 0;
  }
  
  OS_CLR_BIT(OS_Sem.Task_Hold_Flag[Task_Info.Cur_Task_ID],ID);//清除任务占有信号量标志  
#if OS_USE_CS==1
  OS_SET_STRUCT_SUM(OS_Sem);
#endif
  //OS_TimeDly_Ms(10);
  
  return 1;
}
#if OS_SEM_LOCK_CHK==1
//查询当前任务的申请信号量是否锁死,发生了锁死返回1，否则返回0
OS_INT8U OS_Check_Task_Sem_Lock(OS_INT8U Task_ID)
{
  OS_INT32U Wait_Flag;
  OS_INT8U i;
  
  Wait_Flag=OS_Sem.Task_Wait_Flag[Task_ID];  
  while(1)
  {
    for(i=0;i<Task_Info.Cur_Task_Num;i++)
    {
      if((OS_Sem.Task_Hold_Flag[i] & Wait_Flag)!=0)//当前等待的信号量就是任务i的占有的信号量
      {
         Wait_Flag=OS_Sem.Task_Wait_Flag[i];//替换等待信号量
         //该任务等待的信号量就是Task_ID占有的信号量，形成了一个锁死的圈
         if((Wait_Flag & OS_Sem.Task_Hold_Flag[Task_ID])!=0)
           return 1;
         else if(Wait_Flag==0)//没有等待信号量了，当然能形成圈
           return 0;
         else  //重新从第0号任务查起
           break;
      }
    }
    //没有一个任务占有的信号量中有Wait_Flag说明无法形成一个圈，不会死锁
    if(i==Task_Info.Cur_Task_Num)
      return 0;
  }
}
#endif

//申请信号量,成功返回1，失败返回0或者不返回 
OS_INT8U OS_Sem_Pend(OS_INT8U ID)
{
#if OS_USE_CS==1
  OS_INT8U Re;
  
  Re=OS_CHECK_STRUCT_SUM(OS_Sem);
  OS_ASSERT(1==Re);
#endif
  
  if(ID>=OS_SEM_NUM)
  {
    OS_ASSERT(0);
    OS_Debug_Print("OS:Task %d pend sem %d ID error\r\n",Task_Info.Cur_Task_ID,ID);
    return 0;
  }
  
#if OS_SEM_LOCK_CHK==1
  OS_SET_BIT(OS_Sem.Task_Wait_Flag[Task_Info.Cur_Task_ID],ID);//设置任务等待信号量标志
  OS_SET_STRUCT_SUM(OS_Sem);
  //在等待不到信号量的情况下，每隔OS_SEM_CHK_LOCK_TIME S检查一次是否陷入死锁状态
  while(1)
  {
    OS_Waitfor_Sec(OS_GET_BIT(OS_Sem.Sem,ID)==0,OS_SEM_CHK_LOCK_TIME);
    //等待1S后仍然申请不到信号量，则需要查询是否有死锁现象
    if(OS_GET_BIT(OS_Sem.Sem,ID)!=0)
    {
      if(OS_Check_Task_Sem_Lock(Task_Info.Cur_Task_ID))//当前任务申请的信号量导致锁死
      {
        OS_ASSERT(0);
        OS_Debug_Print("OS:Task %d pend sem %d locked!\r\n",Task_Info.Cur_Task_ID,ID);
        return 0;
      }
    }
    else//已经申请到
    {
      OS_CLR_BIT(OS_Sem.Task_Wait_Flag[Task_Info.Cur_Task_ID],ID);//清除任务等待信号量标志
      OS_SET_BIT(OS_Sem.Task_Hold_Flag[Task_Info.Cur_Task_ID],ID);//设置任务占有信号量标志
      break;
    }
  }
#else
  OS_Waitfor(OS_GET_BIT(OS_Sem.Sem,ID)==0);
#endif
  
  OS_SET_BIT(OS_Sem.Sem,ID);
   
#if OS_USE_CS==1
  OS_SET_STRUCT_SUM(OS_Sem);
#endif   
   return 1;  
}


#undef OS_SEM_C

