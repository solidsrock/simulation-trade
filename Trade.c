#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

static unsigned int iFlag=0;
static unsigned long iSed=0;
/* 账户结构体 */
typedef struct stCustomer
{
    int iNo;
    long lAmt;
    pthread_mutex_t mtx;
} CUSTOMER;
CUSTOMER arrPerson[100];

long g_lSum=0;

/* 随机数生成器 */
int GetRandom(int iBeg, int iEnd)
{
    iSed++;
    iSed = iSed%1000000;
    srand(time(NULL)*1000+iSed);
    return (rand()%(iEnd-iBeg))+iBeg;
}

/* 初始化100个账户 */
int InitAll()
{
    int iSum = sizeof(arrPerson)/sizeof(arrPerson[0]);
    int iIndex = 0;
    for (iIndex=0; iIndex<iSum; iIndex++)
    {
        memset(&arrPerson[iIndex],0x00,sizeof(arrPerson[iIndex]));
        arrPerson[iIndex].iNo = iIndex;
        arrPerson[iIndex].lAmt = 1000;
        pthread_mutex_init(&arrPerson[iIndex].mtx,NULL);
    }
    return 0;
}

int DestroyAll()
{
    int iSum = sizeof(arrPerson)/sizeof(arrPerson[0]);
    int iIndex = 0;
    for (iIndex=0; iIndex<iSum; iIndex++)
    {
        memset(&arrPerson[iIndex],0x00,sizeof(arrPerson[iIndex]));
        arrPerson[iIndex].iNo = 0;
        arrPerson[iIndex].lAmt = 0;
        pthread_mutex_destroy(&arrPerson[iIndex].mtx);
    }
    printf("Destroy Ok!\n");
    return 0;
}
long TotalMoney()
{
    int i = 0;
    long lSum = 0;
    for(i=0;i<100;i++)
    {
        lSum+=arrPerson[i].lAmt;
    }
    return lSum;
}

/* 线程调用的事件处理 */
void* thread_trade()
{
    int iAcct1, iAcct2;
    long lTradeAmt=0;
    long lTemp1 = 0;
    long lTemp2 = 0;

    /* 循环的交易 */
    while(iFlag)
    {
        /*printf("线程[%u]发起交易\n",(unsigned int)pthread_self());*/
        /* 先锁住两个账户对象 */
        if( (iAcct1 = GetRandom(0,100)) && 0==pthread_mutex_trylock(&arrPerson[iAcct1].mtx))
        {
            /*lTradeAmt = GetRandom(1,1000)*(time(NULL)%2==1?1:-1);*/
            lTradeAmt = GetRandom(1,1000);
            /*账户1扣钱*/
            if(arrPerson[iAcct1].lAmt<lTradeAmt)
            {
                /* 解锁两个账户对象 */
                /*  printf("[%d]当前[%ld][%ld]has not enough money!\n",iAcct1,arrPerson[iAcct1].lAmt,lTradeAmt);*/
                printf("%u account[%d] current[%ld] sub amt[%ld] is has not enough money,and loop again\n",(unsigned int)pthread_self(),iAcct1,arrPerson[iAcct1].lAmt,lTradeAmt);
                pthread_mutex_unlock(&arrPerson[iAcct1].mtx);
                continue;
            }
            if((iAcct2=GetRandom(0,100)) && 0==pthread_mutex_trylock(&arrPerson[iAcct2].mtx) && iAcct2!=iAcct1)
            {
                lTemp1 = arrPerson[iAcct1].lAmt; 
                arrPerson[iAcct1].lAmt -= lTradeAmt;
                g_lSum -= lTradeAmt;

                lTemp2 = arrPerson[iAcct2].lAmt; 
                arrPerson[iAcct2].lAmt += lTradeAmt;
                g_lSum += lTradeAmt;
                /* printf("thread[%u]account[%d]current[%ld]trade amt[%ld]left[%ld]\n",pthread_self(),iAcct1,lTemp,lTradeAmt,arrPerson[iAcct1].lAmt);*/
                printf("account[%d]current[%ld]sub amt[%ld]left[%ld]\naccount[%d]current[%ld]add amt[%ld] left[%ld]\n",iAcct1,lTemp1,lTradeAmt,arrPerson[iAcct1].lAmt,iAcct2,lTemp2,lTradeAmt,arrPerson[iAcct2].lAmt);
                /* 1,000 微秒 = 1毫秒  1,000,000 微秒 = 1秒 */
                sleep(GetRandom(1,10));
                printf("bank have [%ld]\n",TotalMoney());
                pthread_mutex_unlock(&arrPerson[iAcct1].mtx);
                pthread_mutex_unlock(&arrPerson[iAcct2].mtx);
            }
            else
            {
                pthread_mutex_unlock(&arrPerson[iAcct1].mtx);
                pthread_mutex_unlock(&arrPerson[iAcct2].mtx);
            }
        }
    }
    printf("thread [%u] exit!\n", (unsigned int)pthread_self());
    pthread_exit(NULL);
}

void Stop(int iSigno)
{
    printf("Receive Ctrl+c!\n");
    iFlag = 0;
}

int main()
{
    int iCnt = 0;
    pthread_t td[100];
    InitAll();
    g_lSum = 1000;
    /* 循环创建100个线程 */
    iFlag =1;
    signal(SIGINT,Stop);
    for (iCnt=0; iCnt<100; iCnt++)
    {
        pthread_create(&td[iCnt],NULL,thread_trade,NULL);
    }
    for (iCnt=0; iCnt<100; iCnt++)
    {
        pthread_join(td[iCnt], NULL);
    }
    return 0;
}
