/*
* Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
* Description: OS���������������ӿں���(���豸����ʵ��)
*/
#include "hilink_open_sys_adapter.h"
#include "hilink_sec_random.h"
//#include "hilink_log.h" //Quectel larson.li �ṩ��SDK��δ������ͷ�ļ�

/*
 * IoT OS �����߳̽ӿ�
 * ����ֵ: ����0 �ɹ�, ���� ʧ��
 */
int HILINK_CreateTask(const void *handle, TaskInitParam *initParam)
{
    return 0;
}

/*
 * IoT OS ɾ���߳̽ӿ�
 * ��������handle ɾ���߳̾���
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_DeleteTask(const void *handle)
{
    return 0;
}

/*
 * IoT OS ������
 * �������� HiLinkMutex *mutex  ���ش�������
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_MutexCreate(HiLinkMutex *mutex)
{
    return 0;
}

/*
 * IoT OS ����
 * ��������HiLinkMutex *mutex ��������Դ
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_MutexLock(HiLinkMutex *mutex)
{
    return 0;
}

/*
 * IoT OS �����ӿں���
 * ��������HiLinkMutex *mutex
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_MutexUnlock(HiLinkMutex *mutex)
{
    return 0;
}

/*
 * IoT OS ɾ����
 * ��������HiLinkMutex *mexut  ɾ����
 * ����0 �ɹ�, ���� ʧ��
 */
int HILINK_MutexDestroy(HiLinkMutex *mutex)
{
    return 0;
}

/*
 * �����ź���
 * handle��ָ���ź����Ķ�����countָ���ź���ֵ�Ĵ�С
 * ����0��ʾ�ɹ�, ����ֵ��ʾʧ��
 */
int HILINK_SemaphoreCreate(HiLinkSemaphore *handle, int count)
{
    return 0;
}

/*
 * ���ź�����ֵ��1
 * msָ�����ź���Ϊ0�޷���1��������������ʱ������
 * ����0��ʾ�ɹ�, ����ֵ��ʾʧ��
 */
int HILINK_SemaphoreTimedWait(HiLinkSemaphore handle, int ms)
{
    return 0;
}

/*
 * ���ź�����ֵ��1
 * ����0��ʾ�ɹ�, ����ֵ��ʾʧ��
 */
int HILINK_SemaphorePost(HiLinkSemaphore handle)
{
    return 0;
}

/* �����ź��� */
void HILINK_SemaphoreDestroy(HiLinkSemaphore handle)
{
    return;
}

/*
 * IoT OS ��ȡ��ǰʱ��
 * ����������ʾ��ǰ�豸����������ʱ�� ��λ����
 * ����0 �ɹ�������ʧ��
 */
int HILINK_GetCurrTime(unsigned long *ms)
{
    return 0;
}

/*
 * ��ȡ��ǰ�����̵߳��̺߳�
 * ע��:freeRTOS��mico2.0���ص��ǵ�ǰ�̵߳�Handle
 */
unsigned long HILINK_GetCurrentTaskId(void)
{
    return 0;
}
#if 0
/* ���̷�������Ҫ����Ӳ������Դ�����������㣬�����ݰ�ȫ���ṩ�ķ��������Ż� */
int ThirdRandomEntropy(unsigned int *entropyValue)
{
    *entropyValue = 88888; /* 88888��ʾ����Ӳ������Դֵ�����Ҹ���ʵ�������滻 */
    return 0;
}
#endif
/*
 * ��ȡ������������ʵ�ָýӿ�
 * ����ģ����Ӳ����Դ����Ҫ����HILINK_RegisterRandomEntropyע��Ӳ������Դ��Ȼ������HILINK_SecRandom���ذ�ȫ������
 * ����ģ��û��Ӳ����Դ���򳧼�����������������ͬʱ��Ҫ����Ϊ����
 */
int HILINK_Rand(void)
{
    /* �ο�ʵ�� */
    //HILINK_RegisterRandomEntropy(ThirdRandomEntropy);
    unsigned int securityRandom = 0;
	/*
    if (HILINK_SecRandom(&securityRandom) != 0) {
        //hilink_error("get security random number error!\n\r"); //Quectel larson.li SDK��δ����hilink_error
    }*/
    return (int)securityRandom;
}