# media_core
## Introduction ##
This is a Library which is used for mutilmedia developping.It mainly contains four libraries, InputMediaSource,MediaCoreUtils,MediaHandler and OutputMediaSource. In fact, InputMediaSource is mainly used for capture or input mutilmedia-device, just like micphone capturing, desktop capturing, startdard-protocol reading, custom-protocol reading.MediaCoreUtils is mainly provide various for mutilmedia algorithm, such as screen calculating, videoformat converting, audioformat converting, text-encoding converting. MediaHandler is a handler module for video or audio processing, it contain av synchronization, audio-column calculation. OutputMediaSource is used for audio-video output, it mainly contain output to screen, file, network protocol. You can use the 4 modules to desktop your appliction.Before you use my opensouce libraries, you should pay attention that this libraries is base on GPL, so all your usage behavior is bound by GPL. Moreover, this libraries is desktop base on C++11, so every your change should use C++11 above.
## How to compile ##
The libraries is base on C++11 on Windows. So you should choose the libraries which support C++11 above, such as visual studio 2015 above. In addition, in my source code, I have use other third-party libraries, so you should download these binary libraries or compile these.Here is the list which I used.
1. DirectX 11
2. Boost(1.67)
3. SDL(2.0)
4. ffmpeg
5. POCO
6. LIBYUV

There are two ways to compile this source code. One of ways is just like this. Firstly, you may download all depends Library from this URL(https://wenzuoyi.synology.me:5001/d/f/625678763228708050). From
this URL, you may download all you need libraries just for x86 and x64 windows platform. After you download all there libraries, you should uncompress them, and save them anywhere on your harddisk. After that operation, you should register environment variables for all libraries path. Here is the list of environment variables name.
1. DIRECTX
2. BOOST
3. SDL
4. FFMPEG
5. POCO
6. LIBYUV

note: where you register environment variables for all those libraries path, the value of environment variables shoud be ends with '\'.
If you finish all these steps, you may compile all source code via VS2015 above IDE.
Of course, It strongly recommand you download boost, poco, libyuv, ffmpeg source code by git, and compile it by yourself. If you don't know how to compile it, here is the document just for compiling.
http://wenzuoyi.synology.me:8000/dokuwiki/doku.php。If you finish it, you should register environment variables for visual studio compiling. 
## Contact me ##
If you have any questions during the use, please contact me through email. My email is wenzuoyi@126.com. 



