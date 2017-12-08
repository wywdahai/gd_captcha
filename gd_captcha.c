/* gd_captcha.c
** auth wang.yanwei@rytong.com
**/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gd.h"
#include "gdfontt.h"
#include "gdfonts.h"
#include "gdfontmb.h"
#include "gdfontl.h"
#include "gdfontg.h"
#include <string.h>

/************************************************************************
**function    : myrand
**description 产生[a,b]随机数
**autor       ：wang.yanwei@rytong.com
**input_param : a-min,b-max
**output_param: 
**return      : 随机数
*************************************************************************/
int myrand(int a, int b)
{
    return (rand() % (b-a+1))+ a;
}

/************************************************************************
**function    : randColor
**description ：生成随机颜色
**autor       ：wang.yanwei@rytong.com
**input_param : gdImagePtr-
**output_param: 
**return      : 颜色编码
*************************************************************************/
int randColor(gdImagePtr im)
{
    return gdImageColorAllocate(im, myrand(0,255), myrand(0,255),myrand(0,255));
}

/************************************************************************
**function    : createImg
**description ：生成image
**autor       ：wang.yanwei@rytong.com
**input_param : veriryCode-验证码
**output_param: retImgContent-图形验证码图片串，retVerifyCode-验证码
**return      : 图形验证码图片串长度
*************************************************************************/
int  createImg(const char * veriryCode, char* retImgContent, char * retVerifyCode)
{
    //最大支持64字节
    if(strlen(veriryCode) > 64)
    {
        return 0;
    }

    /* output images */
    gdImagePtr im_out = 0;
    int i;
    int brect[8];
    int x, y;
    char *err;
    
    double sz = 15.;
    char *defFont = "fonts/AGENCYB.TTF";  /* User supplied font */
    char fontsArray[][64] = {
        "fonts/AGENCYB.TTF","fonts/AGENCYR.TTF","fonts/arial.ttf","fonts/arialbd.ttf",
        "fonts/arialbi.ttf","fonts/ariali.ttf","fonts/ARIALN.TTF", "fonts/ARIALNB.TTF",
        "fonts/ARIALNBI.TTF", "fonts/ARIALNI.TTF","fonts/ARIALUNI.TTF","fonts/ariblk.ttf",
        "fonts/BAUHS93.TTF","fonts/FORTE.TTF","fonts/HARLOWSI.TTF","fonts/HARNGTON.TTF",
        "fonts/LATINWD.TTF","fonts/LCALLIG.TTF","fonts/MAGNETOB.TTF","fonts/MATURASC.TTF"};
    char sVerifyCode[64] = {0};
    char littles[] = "abcdefghigklmnopqrstuvwsyzABCDEFGHIGKLMNOPQRSTUVWSYZ0123456789";

    //设置时间种子
    srand((unsigned)time(NULL));

    if(!strlen(veriryCode))
    {
        sprintf(sVerifyCode, "%c%c%c%c",littles[myrand(0,61)],littles[myrand(0,61)],littles[myrand(0,61)],littles[myrand(0,61)]);
    }
    else
    {
        memcpy(sVerifyCode, veriryCode, strlen(veriryCode));
    }

    //1.创建画布(根据验证码字体大小和长度)
    err = gdImageStringFT(NULL,&brect[0],0,defFont,sz,0.,0,0,sVerifyCode);
    if (err) {fprintf(stderr,err); return 0;}

    int imgx = strlen(sVerifyCode) * 15;
    int imgy = 32;
    im_out = gdImageCreate(imgx,imgy);

    //2.设定字体大小及颜色
    char ftTmp[4] = {0}; 
    x = 3 - brect[6];
    y = 3 - brect[7]; 

    int backgroundColor =  randColor(im_out);       

    for (i = 0; i < strlen(sVerifyCode); i++)
    {
        memset(ftTmp,0x00,sizeof(ftTmp));
        sprintf(ftTmp,"%c", sVerifyCode[i]);
        int ii = 4;
        if ((i + 1) % 2 == 0)//控制验证码不在同一高度  
        {
            ii = 2;
        }
        
        int fontIndex = myrand(0,19);
        int fontColor = randColor(im_out);
        //防止背景色与字体颜色相同
        while (backgroundColor == fontColor)
        {
            fontColor = randColor(im_out);
        }
        char * ftStyle = fontsArray[fontIndex];
        err = gdImageStringFT(im_out,&brect[0],fontColor,ftStyle,sz,0.0,x+i*12,y+ii,ftTmp);
        if (err) {fprintf(stderr,err); return 0;}  
    }
    
    //3.填充背景颜色  
    gdImageFill (im_out, 8, 8, backgroundColor);

    //4.画干扰点
    // for (i=0; i <10 ; i++) { 
    //     gdImageLine(im_out, myrand(0,imgx),myrand(0,imgx),myrand(0,imgy),myrand(0,imgy),randColor(im_out));
    // }

    //5.画噪点   
    for(i=0;i<100;i++){   
        gdImageSetPixel(im_out, myrand(0,imgx), myrand(0,imgy), randColor(im_out));   
    }
    //6.输出图片
    int size;
    char * imgStr = gdImagePngPtr(im_out,&size);

    memcpy(retVerifyCode, sVerifyCode, strlen(sVerifyCode));
    memcpy(retImgContent, imgStr, size);

    free(imgStr);
 
    //8.销毁画布
    gdImageDestroy(im_out);

    return size;
}


