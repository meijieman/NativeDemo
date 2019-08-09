## 使用调试工具 addr2line 定位 so 异常位置
1. 当发生异常，会有如下日志 
``` 
08-09 13:53:10.460 140-140/? I/DEBUG: backtrace:
08-09 13:53:10.460 140-140/? I/DEBUG:     #00  pc 00021fa8  /system/lib/libc.so (tgkill+12)
08-09 13:53:10.460 140-140/? I/DEBUG:     #01  pc 00013069  /system/lib/libc.so (pthread_kill+48)
08-09 13:53:10.460 140-140/? I/DEBUG:     #02  pc 0001327d  /system/lib/libc.so (raise+10)
08-09 13:53:10.460 140-140/? I/DEBUG:     #03  pc 00011fb1  /system/lib/libc.so
08-09 13:53:10.460 140-140/? I/DEBUG:     #04  pc 0002185c  /system/lib/libc.so (abort+4)
08-09 13:53:10.460 140-140/? I/DEBUG:     #05  pc 00045b9b  /system/lib/libdvm.so (dvmAbort+78)
08-09 13:53:10.460 140-140/? I/DEBUG:     #06  pc 0000d6e8  /system/lib/libc.so
08-09 13:53:10.460 140-140/? I/DEBUG:     #07  pc 0000ee50  /system/lib/libc.so (pthread_exit+80)
08-09 13:53:10.460 140-140/? I/DEBUG:     #08  pc 0000929d  /data/app-lib/com.major.demo-2/libnative-lib.so (Java_com_major_demo_MainActivity_runThread+120)
```
2. 在发生崩溃的 so 库打开终端，如上面日志 libnative-lib.so
   发生崩溃，打开终端到该位置
   `..\NativeDemo\app\src\main\jniLibs\armeabi-v7a`

addr2line.exe 工具位置
`..\sdk\ndk-bundle\toolchains\arm-linux-androideabi-4.9\prebuilt\windows-x86_64\bin\arm-linux-androideabi-addr2line.exe`

3. 获取日志中崩溃地址`0000929d`，执行命令 `addr2line.exe -C -f -e 
   .\libnative-lib.so 0000929d` 得到结果如下
```
posix_run(void*)
C:\Users\Administrator\Desktop\NativeDemo\app\src\main\cpp\aaa/native-lib.cpp:83
```
说明错误在 native-lib.cpp:83



