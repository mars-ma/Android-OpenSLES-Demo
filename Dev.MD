2017.3.13
FFMPEG 编译集成参考：http://blog.csdn.net/nothingl3/article/details/52774550
注意引用FFMPEG的头文件要外括 extern "C"{},否则会引起编译链接失败

未解问题：
1、opensl 双缓冲数组