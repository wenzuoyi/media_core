# media_core
## Introduction
This is a Library which is used for mutilmedia developping.It mainly contains four libraries, InputMediaSource,MediaCoreUtils,MediaHandler and OutputMediaSource. In fact, InputMediaSource is mainly used for capture or input mutilmedia-device, just like micphone capturing, desktop capturing, startdard-protocol reading, custom-protocol reading.MediaCoreUtils is mainly provide various for mutilmedia algorithm, such as screen calculating, videoformat converting, audioformat converting, text-encoding converting. MediaHandler is a handler module for video or audio processing, it contain av synchronization, audio-column calculation. OutputMediaSource is used for audio-video output, it mainly contain output to screen, file, network protocol. You can use the 4 modules to desktop your appliction.Before you use my opensouce libraries, you should pay attention that this libraries is base on GPL, so all your usage behavior is bound by GPL. Moreover, this libraries is desktop base on C++11, so every your change should use C++11 above.
## How to compile
The libraries is base on C++11 on Windows. So you should choose the libraries which support C++11 above, such as visual studio 2015 above. In addition, in my source code, I have use other third-party libraries, so you should download these binary libraries or compile these.Here is the list which I used.
1. DirectX 11
2. Boost(1.67)
3. SDL(2.0)
4. ffmpeg
After you download or compile them , you may get the binary libraries and their header files.Note, you should register these libraries in your os environment variables. here , you shoud know register DirectX environment variables you shoud named DirectX, and Boost you should named BOOST, and SDL you should named SDL 
after you finish that, you may compile my code successly! 



