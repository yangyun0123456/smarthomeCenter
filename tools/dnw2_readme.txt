使用说明： 
1. 下载附件中的dnw2_win32.zip, 解压得到可执行文件dnw2.exe 
2. 下载dnw2 for win的PC端USB驱动，dnw2_usb_driver.zip,并解压 
3. 打开开发板，按用户手册，进入DNW下载模式。 
4. 用USB线连接开发板和PC，当出现“发现新硬件”提示的对话框时，选择步骤2中解压得到的驱动目录，让Windows自动搜索安装其中的USB驱动。（注1） 
5. 驱动成功安装后，可以在“设备管理器“中"LibUSB-Win32 Devices"下看到一个新的USB设备"MINI2440/QQ2440" 
6. 打开一个命令行窗口，进入dnw2.exe所在目录，执行dnw2.exe <file/to/download>进行下载 

注：1. 如果未弹出该对话框，并且在“设备管理器中”可以看到USB设备"SEC S3C2410X B/D", 说明之前已安装过光盘上提供的USB驱动，现在需要卸载该驱动，然后选择重新安装/更新驱动，按步骤4安装新的USB驱动。 
