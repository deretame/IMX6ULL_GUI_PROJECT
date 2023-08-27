#include "include/config.h"
#include "include/disp_manager.h"
#include "include/encoding_manager.h"
#include "include/fonts_manager.h"
#include "include/input_manager.h"
#include "include/pic_operation.h"
#include "include/picfmt_manager.h"
#include "include/render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

/* digitpic <freetype_file> */
int main(int argc, char ** argv)
{
    int iError;

    /* ��ʼ������ģ��: ����ͨ��"��׼���"Ҳ����ͨ��"����"��ӡ������Ϣ
     * ��Ϊ�������Ͼ�Ҫ�õ�DBG_PRINTF����, �����ȳ�ʼ������ģ��
     */

    /* ע�����ͨ�� */
    DebugInit();

    /* ��ʼ������ͨ�� */
    InitDebugChanel();

    if (argc != 2)
    {
        DBG_PRINTF("Usage:\n");
        DBG_PRINTF("%s <freetype_file>\n", argv[0]);
        return 0;
    }

    /* ע����ʾ�豸 */
    DisplayInit();
    /* ���ܿ�֧�ֶ����ʾ�豸: ѡ��ͳ�ʼ��ָ������ʾ�豸 */
    SelectAndInitDefaultDispDev("fb");

    /*
     * VideoMem: Ϊ�ӿ���ʾ�ٶ�,�����������ڴ��й������ʾ��ҳ�������,
                 (����ڴ��ΪVideoMem)
     *           ��ʾʱ�ٰ�VideoMem�е����ݸ��Ƶ��豸���Դ���
     * �����ĺ�����Ƿ���Ķ��ٸ�VideoMem
     * ������ȡΪ0, ����ζ�����е���ʾ���ݶ�������ʾʱ���ֳ�����,Ȼ��д���Դ�
     */
    AllocVideoMem(5);

    /* ע�������豸 */
    InputInit();
    /* �������������豸�ĳ�ʼ������ */
    AllInputDevicesInit();

    /* ע�����ģ�� */
    EncodingInit();

    /* ע���ֿ�ģ�� */
    iError = FontsInit();
    if (iError)
    {
        DBG_PRINTF("FontsInit error!\n");
    }

    /* ����freetype�ֿ����õ��ļ�������ߴ� */
    iError = SetFontsDetail("freetype", argv[1], 24);
    if (iError)
    {
        DBG_PRINTF("SetFontsDetail error!\n");
    }

    /* ע��ͼƬ�ļ�����ģ�� */
    PicFmtsInit();

    /* ע��ҳ�� */
    PagesInit();

    /* ������ҳ�� */
    Page("main")->Run(NULL);

    return 0;
}
