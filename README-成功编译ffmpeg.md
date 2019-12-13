## 环境
FFmpeg 4.1.3
Ubuntu 18.04
Android Studio 3.4.1

需要安装 make
```
sudo apt install make
```

1. 打开 Ubuntu 在~目录创建 ffmpeg，进入该目录
```
sudo su

apt-get update
apt-get install yasm
apt-get install pkg-config
```
2. 下载 NDK，ffmpeg
```
wget https://dl.google.com/android/repository/android-ndk-r17c-linux-x86_64.zip

unzip android-ndk-r17c-linux-x86_64.zip

wget https://ffmpeg.org/releases/ffmpeg-4.1.3.tar.bz2

tar -xvf ffmpeg-4.1.3.tar.bz2
```
3. 创建编译脚本
cd ffmpeg-4.1.3
vim build_android.sh
```
#!/bin/bash
clear

function build_android
{
	./configure \
		--prefix=$PREFIX \
		--enable-shared \
		--disable-static \
		--disable-ffmpeg \
		--disable-ffplay \
		--disable-ffprobe \
		--target-os=android \
		--disable-doc \
		--enable-cross-compile \
		--cross-prefix=$CROSS_PREFIX \
		--arch=$ARCH \
		--sysroot=$SYSROOT \
		--extra-cflags="-I$NDK/sysroot/usr/include/arm-linux-androideabi -isysroot $NDK/sysroot -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -marm -march=armv7-a" \
		--enable-neon
	make clean all
	make -j4
	make install
}

NDK=/home/major/ffmpeg/android-ndk-r17c #填写自己ndk的解压路径

#armv7-a
API=19
ARCH=arm
CPU=armv7-a
PREFIX=$(pwd)/android/$CPU
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
SYSROOT=$NDK/platforms/android-$API/arch-$ARCH
CROSS_PREFIX=$TOOLCHAIN/bin/arm-linux-androideabi-

build_android

```
4. 执行脚本
```
chmod 777 build_android.sh
./build_android.sh
```

5. 编译完成后查看 ffmpeg-4.1.3 目录下的 android 目录
拷贝 ~/ffmpeg/ffmpeg-4.1.3/android/armv7-a 下的 include 和 lib 目录到 AS



## 参考文档
*[FFmpeg 4.1.3 + Ubuntu 18.04 + Android Studio 3.4.1编译](https://blog.csdn.net/wuppa/article/details/91879902)
*参考编译脚本 [在ubuntu18.4平台上编译FFmpeg-3.4 for Android](https://blog.csdn.net/long_24k_os/article/details/80542089)
*[Ubuntu18.04下编译FFmpeg for Android](https://blog.csdn.net/n_fly/article/details/102951930)