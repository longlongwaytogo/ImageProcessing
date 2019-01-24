## 关键字：
前景检测算法  背景建模  背景分割器 目标跟踪
## 前言

opencv实现的背景建模方法有很多，早期的opencv版本modules/video/src下有acmmm2003(2.3.1版本)、codebook(2.3.1版本)、gmg(2.4.8版本)、gaussmix、gassmix2方法。这两天下了个opencv3.0beta版本，video模块中只有gaussmix、gassmix2被留了下来，其他的方法都被丢到legacy模块去了，legacy模块一般是放即将废弃或老旧的算法的，这也说明了mog、mog2方法依旧老当益壮。(3.0版本还多了个KNN的背景建模算法，暂时没有多余的时间去研究)
网上有很多相关的高斯背景建模原理介绍，就不再多说，这里主要记录下这两天对mog2代码的理解。


在OpenCV3中有三种背景分割器：K-Nearest（KNN）、Mixture of Gaussians（MOG2）、Geometric Multigid（GMG）
目标跟踪，背景分割器：KNN、MOG2和GMG

BackgroundSubtractor类是专门用于视频分析的，即BackgroundSubtractor类会对每帧的环境进行“学习”。BackgroundSubtractor类常用来对不同帧进行比较，并存储以前的帧，可按时间推移方法来提高运动分析的结果。


## 常见算法：
- gmm： 混合高斯分布（Gaussian Mixture Model），是背景建模中的经典算法，
- mog： OpenCV将其封装为BackgroundSubtractorMOG
- mog2：自适应的高斯混合模型（Adaptive GMM，Gaussian Mixture Model）,是改进版GMM，opencv把它封装为BackgroundSubtractorMOG2算法类
- gmg： 
- KNN
--------------------- 
作者：abc20002929 
来源：CSDN 
原文：https://blog.csdn.net/abc20002929/article/details/43247425 
版权声明：本文为博主原创文章，转载请附上博文链接！



在很多基础应用中背景检出都是一个非常重要的步骤。例如顾客统计,使用一个静态摄像头来记录进入和离开房间的人数,或者是交通摄像头,需要提取交通工具的信息等。在所有的这些例子中,首先要将人或车单独提取出来。 
技术上来说,我们需要从静止的背景中提取移动的前景。如果你有一张背景(仅有背景不含前景)图像,比如没有顾客的房间,没有交通工具的道路等,那就好办了。我们只需要在新的图像中减去背景就可以得到前景对象了。但是在大多数情况下,我们没有这样的(背景)图像,所以我们需要从我们有的图像中提取背景。如果图像中的交通工具还有影子的话,那这个工作就更难了,因为影子也在移动,仅仅使用减法会把影子也当成前景。真是一件很复杂的事情。 
1:BackgroundSubtractorMOG 
这是一个以混合高斯模型为基础的前景/背景分割算法。它是 P.KadewTraKuPong和 R.Bowden 在 2001 年提出的。它使用 K(K=3 或 5)个高斯分布混合对背景像素进行建模。使用这些颜色(在整个视频中)存在时间的长短作为混合的权重。背景的颜色一般持续的时间最长,而且更加静止。一个像素怎么会有分布呢?在 x,y 平面上一个像素就是一个像素没有分布,但是我们现在讲的背景建模是基于时间序列的,因此每一个像素点所在的位置在整个时间序列中就会有很多值,从而构成一个分布。

在编写代码时,我们需要使用函数: cv2.createBackgroundSubtractorMOG()创建一个背景对象。这个函数有些可选参数,比如要进行建模场景的时间长度,高斯混合成分的数量,阈值等。将他们全部设置为默认值。然后在整个视频中我们是需要使用 backgroundsubtractor.apply() 就可以得到前景的掩模了。

2:BackgroundSubtractorMOG2 
这个也是以高斯混合模型为基础的背景/前景分割算法。它是以 2004 年和 2006 年 Z.Zivkovic 的两篇文章为基础的。这个算法的一个特点是它为每一个像素选择一个合适数目的高斯分布。(上一个方法中我们使用是 K 高斯分布)。这样就会对由于亮度等发生变化引起的场景变化产生更好的适应。 
和前面一样我们需要创建一个背景对象。但在这里我们我们可以选择是否检测阴影。如果detectShadows = T rue(默认值),它就会检测并将影子标记出来,但是这样做会降低处理速度。影子会被标记为灰色。

3:BackgroundSubtractorGMG 
此算法结合了静态背景图像估计和每个像素的贝叶斯分割。这是 2012年Andrew_B.Godbehere, Akihiro_Matsukawa 和 Ken_Goldberg 在文章中提出的。它使用前面很少的图像(默认为前 120 帧)进行背景建模。使用了概率前景估计算法(使用贝叶斯估计鉴定前景)。这是一种自适应的估计,新观察到的 
对象比旧的对象具有更高的权重,从而对光照变化产生适应。一些形态学操作如开运算闭运算等被用来除去不需要的噪音。在前几帧图像中你会得到一个黑色窗口。 
对结果进行形态学开运算对与去除噪声很有帮助。

https://blog.csdn.net/App_12062011/article/details/51843036


## 图形抠图


图像抠图技术（Image Matting）在现代影视技术中有重要应用。这里所谓的“抠图”，英文Matting其实是“融合”的意思。所以如果翻译成图像融合技术其实也是恰当的的，但如果我们从“抠取”这个角度来看，Image Matting更类似于一种图像分割方法，只不过：1）它是一种超精细的图像分割技术；2）它要分割的内容通常是将前景从背景中分割（而广义的图像分割则还包括同等地位目标之间的分离）。

作者：白马负金羁 
来源：CSDN 
原文：https://blog.csdn.net/baimafujinji/article/details/72863106 
版权声明：本文为博主原创文章，转载请附上博文链接！

Matting是计算机视觉的基本问题，不过也是比较难的问题，它与分割问题有所不同。核心问题是解决I = αF + (1-α)B

[自然抠图算法：以经典的贝叶斯抠图为例（Bayesian Matting）](https://blog.csdn.net/baimafujinji/article/details/72863106?utm_source=gold_browser_extension)


zhihu上有关 image Matting的文章
[天干物燥，小心抠图 —— A journey of matting](https://zhuanlan.zhihu.com/p/27852081)
Poisson Matting(泊松融合)
tri-map  
Bayes Matting 贝叶斯
Learning Based Digital Matting 半监督学习方法
KNN Matting 　KNN抠图属于非局部抠图
Closed form matting 封闭式表面抠图
Deep Learning Methods 基于深度学习，尤其是CNN的方法实现抠图，paper：
-《Natural Image Matting Using Deep CNN》ECCV 2016
-《Deep Automatic Portrait Matting》 ECCV 2016
-《Deep Image Matting》CVPR 2017

[抠图技术及方法简介（Image Matting Overview)](https://blog.csdn.net/qq_36165459/article/details/78549686)

之前接触过语义分割，所以在刚接触图像抠图时以为两者是差不多。语义分割是端到端的，对像素按照语义进行多分类，而抠图就是按照前景和背景进行二分类嘛？实际上这是错误的理解。语义分割重在对每个像素的语义理解，将相同语义的像素分割为同一个部分，得到的结果就是若干个块状，至于块与块之间是否衔接自然则不考虑。抠图只将图片分成前景和背景两块，目的是拿到前景，好的抠图算法会对于头发等细节的处理效果比较精确。分割和抠图的另一个重要区别是分割是返回像素分类结果，分类结果是整型；而抠图返回的是属于前景或背景的概率p，在前景与背景交互区域会产生渐变的效果，使得抠图更加自然。 

作者：桃木子 
来源：CSDN 
原文：https://blog.csdn.net/qq_36165459/article/details/78549686 
版权声明：本文为博主原创文章，转载请附上博文链接！
[论文笔记：Deep Image Matting](https://blog.csdn.net/u012905422/article/details/62226403)

、
### zero density、  black magic 怎么实现动态抠图？ 好像用的是绿幕抠图， 产品是面向广电的虚拟演播室

[zero density 官网](https://www.zerodensity.tv/)
Zero Density正在塑造实时虚拟演播室制作的未来，结合前沿技术，深刻的专业知识和无限的创造力。
Zero Density是一家国际技术公司，致力于为广播，增强现实，现场活动和电子竞技等行业开发创意产品。Zero Density提供具有实时视觉效果的下一级虚拟演播室制作。它为虚幻引擎本机平台Reality Engine提供了先进的实时合成工具及其专有的键控技术Reality Keyer™。Reality是业内最逼真的实时3D虚拟演播室和增强现实平台。

张正友标定：以后研究 A Flexible New Technique for Camera Calibration


[MIT用AI实现3分钟自动抠图，精细到头发丝，秒杀一切PS软件](https://blog.csdn.net/za8KFnpo2/article/details/81978448)



## [自然图像抠图/视频抠像技术梳理（image matting, video matting)](https://blog.csdn.net/jaccen2012/article/details/50208257)

[抠图算法合集](http://www.alphamatting.com/eval_25.php)
[贝叶斯抠图](https://blog.csdn.net/baimafujinji/article/details/72863106?utm_source=gold_browser_extension)