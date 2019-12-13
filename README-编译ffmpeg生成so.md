
## 环境
Ubuntu 18.04.1 LTS

# 编译 ffmpeg
1. 下载 ffmpeg
wget https://launchpad.net/ubuntu/+archive/primary/+sourcefiles/ffmpeg/7:4.1.4-1build2/ffmpeg_4.1.4.orig.tar.xz
tar -xvJf ffmpeg_4.1.4.orig.tar.xz 

2. 下载 ndk
wget https://dl.google.com/android/repository/android-ndk-r19c-linux-x86_64.zip
unzip android-ndk-r19c-linux-x86_64.zip

cd ffmpeg_4.1.4

./configure --enable-shared --disable-x86asm --prefix=/home/major/ffmpeg/android

3. 创建编译脚本
vim build_android.sh // 创建文件
```sh
#!/bin/bash
make clean
#填写你具体的ndk解压目录
export NDK=/home/major/ffmpeg/android-ndk-r19c
export SYSROOT=$NDK/platforms/android-16/arch-arm/
#arm-linux-androideabi-4.6 要确保NDK下这个目录存在，还有ndk需要与 ubuntu的版本位数对应上，我这里都是64位
export TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
export CPU=arm
#编译后的文件会放置在 当前路径下的android/arm／下
export PREFIX=$(pwd)/android/$CPU
export ADDI_CFLAGS="-marm"

#./configure 即为ffmpeg 根目录下的可执行文件configure
#你可以在ffmpeg根目录下使用./configure --hellp 查看 ./configure后可填入的参数。

./configure --target-os=linux \
    --prefix=$PREFIX --arch=arm \
    --disable-doc \
    --enable-static \    #编译静态库
    --disable-shared \   #禁止编译动态库
    --disable-yasm \
    --disable-symver \
    --enable-gpl \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-ffserver \
    --disable-doc \
    --disable-symver \
    --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
    --enable-cross-compile \
    --sysroot=$SYSROOT \
    --extra-cflags="-Os -fpic $ADDI_CFLAGS" \
    --extra-ldflags="$ADDI_CFLAGS" \
    $ADDITIONAL_CONFIGURE_FLAG
make clean
make
make install
```

4. 执行脚本

sudo apt install make

./build_android.sh



#编译问题
* -bash: ./build_arm.sh: /bin/bash^M: 解释器错误: 没有那个文件或目录
在 windows 编译的文本在 unix 不能识别

解决方法：
1. 在 ubuntu 修改权限
chmod 777 build_arm.sh
2. vim build_arm.sh
3. 查看格式 :set ff，获取到的格式信息为 fileformat=dos 或 fileformat=unix
4. 修改格式 :set ff=unix
5. 保存退出





sh 多行注释
:<<!
要注释的内容
！


vim 调到指定行
:n


---
* 最简单的基于FFmpeg的移动端例子：Android HelloWorld
https://blog.csdn.net/leixiaohua1020/article/details/47008825

* https://github.com/bravobit/FFmpeg-Android

















