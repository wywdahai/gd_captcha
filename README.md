# gd_captcha
c语言调用gd库生成图形验证码

1、查看已有环境是否有依赖库
rpm -qa|grep png
rpm -qa|grep jpeg
rpm -qa|grep freetype
rpm -qa|grep zlib
rpm -qa|grep gd

2、如果上述已有环境没有依赖库，那么安装相应依赖
例如：
rpm -ivh libpng-1.2.49-1.el6_2.x86_64.rpm

3、编译gd_captcha.so
gcc gd_captcha.c -o gd_captcha -lgd -lpng -lz -ljpeg -lfreetype -lm

调用gd_captcha:erl_img_create(_VerifyCode)生成图形验证码串
