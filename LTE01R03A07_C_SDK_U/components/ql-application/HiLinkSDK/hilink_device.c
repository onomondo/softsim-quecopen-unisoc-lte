/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: HiLink��Ʒ����ʵ��Դ�ļ�
 * Notes: ���ļ��еĽӿ���Ҫ�����ṩ������������ʹ�ã�Ϊ��ǰ�����ݣ������Ͻӿ��ݲ������±����淶����.
 */
#include "hilink_device.h"
#include <stdlib.h>
#include "hilink.h"
//#include "hilink_log.h"
#include "hilink_bt_api.h"
//#include "ql_api_dev.h"
#include "ql_log.h"
//#include "ql_fs.h"

#define HILINK_DEV_LOG_LEVEL                QL_LOG_LEVEL_INFO
#define HILINK_DEV_LOG(msg, ...)			QL_LOG(HILINK_DEV_LOG_LEVEL, "hilink_dev", msg, ##__VA_ARGS__)

/* �豸���Ͷ��� */
typedef struct {
    const char *sn;     /* �豸Ψһ��ʶ������sn�ţ����ȷ�Χ��0,40] */
    const char *prodId; /* �豸HiLink��֤�ţ����ȷ�Χ��0,5] */
    const char *model;  /* �豸�ͺţ����ȷ�Χ��0,32] */
    const char *dev_t;  /* �豸���ͣ����ȷ�Χ��0,4] */
    const char *manu;   /* �豸�����̣����ȷ�Χ��0,4] */
    const char *mac;    /* �豸MAC��ַ���̶�32�ֽ� */
    const char *btMac;    /* ����MAC��ַ���̶�32�ֽ� */
    const char *hiv;    /* �豸HilinkЭ���汾�����ȷ�Χ��0,32] */
    const char *fwv;    /* �豸�̼��汾�����ȷ�Χ[0,64] */
    const char *hwv;    /* �豸Ӳ���汾�����ȷ�Χ[0,64] */
    const char *swv;    /* �豸�����汾�����ȷ�Χ[0,64] */
    int prot_t;         /* �豸Э�����ͣ�ȡֵ��Χ[1,3] */
} dev_info_t;

/* �豸����Ӣ�����ͳ���Ӣ��������֮�Ͳ��ܳ���17�ֽ� */
typedef struct {
    const char *devTypeName; /* �豸����Ӣ������ */
    const char *manuName;    /* ����Ӣ������ */
} DevNameEn;

/* �������Ͷ��� */
typedef struct {
    const char *st;     /* �������ͣ����ȷ�Χ��0,32] */
    const char *svc_id; /* ����ID�����ȷ�Χ��0,64] */
} svc_info_t;

/* �豸��Ϣ���� */
dev_info_t dev_info = {
    "Device SN",
    PRODUCT_ID,
    DEVICE_MODEL,
    DEVICE_TYPE,
    MANUAFACTURER,
    "Device Mac",
    "Device BtMac",
    "1.0.0",
    "1.0.0",
    "1.0.0",
    "1.0.0",
    PROTOCOL_TYPE
};

/* �豸���ƶ���, ��ȷ���豸����Ӣ�����ͳ���Ӣ��������֮�Ͳ�����17�ֽ� */
DevNameEn g_devNameEn = {
    DEVICE_TYPE_NAME,
    MANUAFACTURER_NAME
};

/* ������Ϣ���� */
int gSvcNum = 1;
svc_info_t gSvcInfo[] = {
    { "binarySwitch", "switch" }
};

/* AC���� */
unsigned char A_C[48] = {
    0x49, 0x3F, 0x45, 0x4A, 0x3A, 0x72, 0x38, 0x7B, 0x36, 0x32, 0x50, 0x3C, 0x49, 0x39, 0x62, 0x38,
    0x72, 0xCB, 0x6D, 0xC5, 0xAE, 0xE5, 0x4A, 0x82, 0xD3, 0xE5, 0x6D, 0xF5, 0x36, 0x82, 0x62, 0xEB,
    0x89, 0x30, 0x6C, 0x88, 0x32, 0x56, 0x23, 0xFD, 0xB8, 0x67, 0x90, 0xA7, 0x7B, 0x61, 0x1E, 0xAE
};

/* BI���� */
char *bi_rsacipher = "";

static HILINK_BT_DevInfo g_btDevInfo;

/* ��ȡ���� AC ����  */
unsigned char *hilink_get_auto_ac(void)
{
    return A_C;
}

/* ��ȡ���� BI ���� */
char *hilink_get_auto_bi_rsa_cipher(void)
{
    return bi_rsacipher;
}

/*
 * �޸ķ�����ǰ�ֶ�ֵ
 * svcIdΪ������ID��payloadΪ���յ���Ҫ�޸ĵ�Json��ʽ���ֶ�����ֵ��lenΪpayload�ĳ���
 * ����0��ʾ����״ֵ̬�޸ĳɹ�������Ҫ�ײ��豸�����ϱ�����Hilink Device SDK�ϱ���
 * ����-101��ʾ���ñ��Ĳ�����Ҫ����
 * ����-111��ʾ����״ֵ̬�����޸��У��޸ĳɹ����ײ��豸���������ϱ���
 */
int hilink_put_char_state(const char *svcId, const char *payload, unsigned int len)
{
    return 0;
}

/*
 * ��ȡ�����ֶ�ֵ
 * svcId��ʾ����ID������ʵ�ָú���ʱ����Ҫ��svcId�����жϣ�
 * in��ʾ���յ���Json��ʽ���ֶ�����ֵ��
 * inLen��ʾ���յ���in�ĳ��ȣ�
 * out��ʾ���������ֶ�ֵ���ݵ�ָ��,�ڴ��ɳ��̿��٣�ʹ�����ɺ�����Hilink Device SDK�ͷţ�
 * outLen��ʾ��ȡ����payload�ĳ��ȣ�
 * ����0��ʾ����״̬�ֶ�ֵ��ȡ�ɹ������ط�0��ʾ��ȡ����״̬�ֶ�ֵ���ɹ���
 */
int hilink_get_char_state(const char *svcId, const char *in, unsigned int inLen, char **out, unsigned int *outLen)
{
    return 0;
}

/*
 * ��ȡ�豸sn��
 * ����len��ʾsn�����󳤶�,39�ֽ�
 * ����sn��ʾ�豸sn
 * snָ�����ַ�������Ϊ0ʱ��ʹ���豸mac��ַ��Ϊsn
 */
void HilinkGetDeviceSn(unsigned int len, char *sn)
{
    /* �ڴ˴�����ʵ�ִ���, ��sn��ֵ��*sn�ش� */
    return;
}

/*
 * ��ȡ�豸���ذ汾��
 * ����0��ʾ�汾�Ż�ȡ�ɹ�������������ʾ�汾�Ż�ȡʧ��
 * ע�⣬�˽ӿ�ΪHiLink�ڲ����ú���
 */
int getDeviceVersion(char **firmwareVer, char **softwareVer, char **hardwareVer)
{
    *firmwareVer = FIRMWARE_VER;
    *softwareVer = SOFTWARE_VER;
    *hardwareVer = HARDWARE_VER;
    return 0;
}

/*
 * ��ȡSoftAp����PIN��
 * ����ֵΪ8λ����PIN��, ����-1��ʾʹ��HiLink SDK��Ĭ��PIN��
 * �ýӿ����豸������ʵ��
 */
int HiLinkGetPinCode(void)
{
    /* ����ʱ���������ֿ��������ģ�ֻҪ��8λ���ּ��� */
    return -1;
}

/*
 * ��ѯ��ǰ�豸�����Ա�ʶ
 * ����0Ϊ�������豸������1Ϊ�����豸
 */
int HILINK_IsSensitiveDevice(void)
{
    return 0;
}

/*
 * ֪ͨ�豸��״̬
 * status��ʾ�豸��ǰ��״̬
 * ע�⣬�˺������豸���̸��ݲ�Ʒҵ��ѡ����ʵ��
 */
void hilink_notify_devstatus(int status)
{
    switch (status) {
        case HILINK_M2M_CLOUD_OFFLINE:
            /* �豸���ƶ����ӶϿ������ڴ˴�����ʵ�� */
            break;
        case HILINK_M2M_CLOUD_ONLINE:
            /* �豸�����ƶ˳ɹ������ڴ˴�����ʵ�� */
            break;
        case HILINK_M2M_LONG_OFFLINE:
            /* �豸���ƶ����ӳ�ʱ���Ͽ������ڴ˴�����ʵ�� */
            break;
        case HILINK_M2M_LONG_OFFLINE_REBOOT:
            /* �豸���ƶ����ӳ�ʱ���Ͽ����������������ڴ˴�����ʵ�� */
            break;
        case HILINK_UNINITIALIZED:
            /* HiLink�߳�δ���������ڴ˴�����ʵ�� */
            break;
        case HILINK_LINK_UNDER_AUTO_CONFIG:
            /* �豸��������ģʽ�����ڴ˴�����ʵ�� */
            break;
        case HILINK_LINK_CONFIG_TIMEOUT:
            /* �豸����10���ӳ�ʱ״̬�����ڴ˴�����ʵ�� */
            break;
        case HILINK_LINK_CONNECTTING_WIFI:
            /* �豸��������·���������ڴ˴�����ʵ�� */
            break;
        case HILINK_LINK_CONNECTED_WIFI:
            /* �豸�Ѿ�����·���������ڴ˴�����ʵ�� */
            break;
        case HILINK_M2M_CONNECTTING_CLOUD:
            /* �豸���������ƶˣ����ڴ˴�����ʵ�� */
            break;
        case HILINK_M2M_CLOUD_DISCONNECT:
            /* �豸��·���������ӶϿ������ڴ˴�����ʵ�� */
            break;
        case HILINK_DEVICE_REGISTERED:
            /* �豸��ע�ᣬ���ڴ˴�����ʵ�� */
            break;
        case HILINK_DEVICE_UNREGISTER:
            /* �豸�����������ڴ˴�����ʵ�� */
            break;
        case HILINK_REVOKE_FLAG_SET:
            /* �豸����λ������λ�����ڴ˴�����ʵ�� */
            break;
        case HILINK_NEGO_REG_INFO_FAIL:
            /* �豸Э��������Ϣʧ�� */
            break;
        default:
            break;
    }

    return;
}

/*
 * ʵ��ģ������ǰ���豸����
 * flagΪ0��ʾHiLink SDK �߳̿��Ź�����ģ������; Ϊ1��ʾAPPɾ���豸����ģ������
 * ����0��ʾ�����ɹ�, ϵͳ����������ʹ��Ӳ����; ����1��ʾ�����ɹ�, ϵͳ����������ʹ��������;
 * ���ظ�ֵ��ʾ����ʧ��, ϵͳ��������
 * ע�⣬�˺������豸����ʵ�֣���APPɾ���豸����ģ������ʱ���豸���������ط���0�������ᵼ��ɾ���豸�쳣
 */
int hilink_process_before_restart(int flag)
{
    /* HiLink SDK�߳̿��Ź���ʱ����ģ������ */
    if (flag == HILINK_REBOOT_WATCHDOG) {
        /* ʵ��ģ������ǰ�Ĳ���(��:����ϵͳ״̬��) */
        return -1;
    }

    /* APPɾ���豸����ģ������ */
    if (flag == HILINK_REBOOT_DEVDELETE) {
        /* ʵ��ģ������ǰ�Ĳ���(��:����ϵͳ״̬��) */
        return 0;
    }

    return -1;
}

/*
 * ��ȡ�豸�����룬��֪ͨAPP
 * status��ʾ�Ƿ����͹��ϣ�0��ʾ�����ͣ�1��ʾ���ͣ�code��ʾ������
 * ����0��ʾ�ɹ������ط�0ʧ��
 */
int get_faultDetection_state(int *status, int *code)
{
    /* ���豸����ʵ�֣�������faultDetection���Ե�ǰֵ�������� */
    return 0;
}

/*
 * ��ȡ�豸�����ͺţ����ȹ̶������ֽ�
 * subProdIdΪ�������ͺŵĻ�������lenΪ�������ĳ���
 * ������Ʒ���������ͺţ����������ֽ����ͺţ�����'\0'����, ����0
 * û�ж������ͺţ��򷵻�-1
 * �ýӿ����豸������ʵ��
 */
int HILINK_GetSubProdId(char *subProdId, int len)
{
    return -1;
}


int HILINK_BT_GetDevSurfacePower(char *power)
{
    if (power == NULL) {
        return -1;
    }
    *power = 0xF8;
    return 0;
}

HILINK_BT_DevInfo *HILINK_BT_GetDevInfo(void)
{
    g_btDevInfo.manuName = (char *)g_devNameEn.manuName;
    g_btDevInfo.devName = (char *)g_devNameEn.devTypeName;
    g_btDevInfo.productId = (char *)dev_info.prodId;
    g_btDevInfo.mac = (char *)dev_info.mac;
    return &g_btDevInfo;
}

/* 
 * д�ļ�
 * key:�ļ���
 * value:��Ҫд��������
 * len:��Ҫд�������ݳ���
 * sync:�Ƿ���Ҫͬ��(Ĭ��Ϊ 1)
 * ����ֵ:0 �ɹ� -1 ʧ��
 */
int duet_flash_kv_set(const char *key, const void *value, int len, int sync)
{
	return 0;
}

/*��ȡ�ļ�
 *key:�ļ���
 *buffer:��ȡ���ݵĻ�����
 *buffer_len:��Ҫ��ȡ�����ݳ���
 *����ֵ:0 �ɹ� -1 ʧ��
 */
int duet_flash_kv_get(const char *key, void *buffer, int *buffer_len)
{
	return 0;
}

/*ɾ���ļ�
 *key:�ļ���
 *����ֵ:0 �ɹ� -1 ʧ��
 */
int duet_flash_kv_del(const char *key)
{
    return 0;
}

int GetBtNetworkState(void)
{	
	return 0;	
}

int HILINK_ql_printf(const char *format, ...)
{
	return 0;		
}

int HILINK_BT_GetNetworkState()
{
	return 0;		
}
