# ####主要思路

首先用videoCapture cap一帧帧读取视频；

用isOpened检查cap是否成功读取

建一个变量Mat frame 来存取这一帧视频。

**##1.图片二值化**

通过询问AI 知道两个函数cv::cvtColor和cv::inRange（所以没用上threshold）

cvtColor 用来将图片变成HSV颜色空间

Scalar设置一下红色的上下范围（色度0-10 亮度和透明度80-255）[并没有（160-180）是因为我发现只取0~10色度就可以成功选出红色灯条]

 通过与下方area(contourArea计算contours面积)过滤配合来选出想要的灯条形状  ——————**刚好能过滤掉下面的干扰项**

inrange转化（范围内变白 之外变黑）         				

再通过AI建议用kernel 进行一下闭运算 使白色膨胀；

**##2.找出矩形**

主要函数：cv::findContours  cv::minAreaRect											

****方法1：****用findContours找出白色块 EXTERNAL只识别外部轮廓  CHAIN节省内存						

使用数组contours储存 																	

用minAreaRect函数将contours转化为****小矩形	**												

再建一个RotatedRect组<valid_light_bars>储存合理的contours								

后面检查 正常 bars应该只有两个 检查 如果是两个：

将valid_light_bars里的两个矩形用points()转化为点

再用一个点组allpoints()储存上述所有的点

使用minAreaRect将这个转化为一个**大矩形**

我发现上种解法有些不符合任务要求 所以我换成任务中求最低最高点：



**方法2：** 用points将所得每个小矩形转化为四个端点 设变量 top_pt 和bottom_pt;

	   通过遍历所有点找到最低（y最大）和最高（y最小）点

	因为有时bottom 和top.x 不同 所以设立新变量全部将x坐标设为rRect.center.x 消除x误差	

	找出最低点和最高点 分别进入俩点组  再排序 并入一个点组  最后连线。






**##3.连线**

使用points()将这个大矩形转化为四个端点（易知成顺序）存为点组finalpoints();

用cv::line和for循环连线 同时(i+1) %4 确保首尾相连

设置绿色画笔(0,255,0)

**##4.输出这一帧**

使用函数imshow(frame)

同时配上waitkey(30) 确保能看清

最后release 和destroyAllWindows释放内存

目前已完成  有时绿色框水平会发生轻微抖动 但大概轮廓相同（使用方法2会这样 方法1非常平稳）

其余无太大问题。
