#　　　　　　　时间管理器使用指南

>    　　　　　　　　　　　　　　　与投入的时间相比，聪明、勤奋之类的因素实在是微不足道。  
>　　　
>   　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　*via*《与时间做朋友》  
  　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　李笑来

##软件原理
　　手机发送的每条短信均带有发送时间，我们可以在做每一项事情前向<code>移动</code>\\<code>联通</code>\\<code>电信</code>发送短信的方法，来记录自己一天的生活轨迹
##软件的使用方法
###建立记录  
　　建立记录之前前需要先在手机上安装搜狗输入法[2]，在电脑上安装豌豆荚[3]，安装完成后直接给`10086`\\<code>10010</code>\\<code>10000</code>发短信就成。	 
　　比如，移动用户上完自习后要记录一下，就给10086发条短信：
	
	。上自习？
	
　　或者，上完自习准备出去玩会儿，就发
	
	。上自习？出去玩
	
　　`？`和它左右两侧的文字合称为记录，发条短信，就是记录一下。记录完成后用豌豆荚把短信内容导出，运行程序根据提示进行处理就能得到一份属于自己的时间报表，就这么简单~  
  
　　不过，话说如果现在是13：20，准备开始写数学作业，但是突然发现把12：05开始的`写读书笔记`，跟13：10开始的`休息`忘了记录了咋办？  
 　　答：这么发短信就成
	
	。，12.05？写读书笔记！，13.10？休息！？写数学作业
	  
　　导入记录的时系统会自动读出时间并把记录补充完整。  
　　：）  

---
###详细说明
1.	`。`
	*	`。`用于将我们的生活记录与普通短息区分开
	*	只有开头为`。`的短信才会被系统作为记录进行读取
2.	`？`
	*	`？`代表我们发送短信的那个时间节点，`?`之前的内容是我们发短信之前所做的事情，之后是发完短信后我们准备去做的事。
	*	每条记录中有且只能由一个`？`。记录中之前所做的事与准备要做的事都可以空掉不写，后一条记录中所写的之前所做的事会自动替换掉前条记录中准备做的事
	*	示例	
	
			。吃饭？洗碗
	
	    *	`。`标识记录的开始，发短信前所做的事情是`吃饭`，发完短信后准备要做的事为`洗碗`	
3.	`，`
	*	当我们因为各种原因而忘记做记录时，使用补充记录功能可以方便的进行补记
	*	`,`代表之后的内容属于补充的记录，具体使用方法见下  
4.	`！`  
        *   `！`代表补充记录的结束，每一个`,`都必须与一个`！`相对应，否则系统会因为找不到结束的地方而报错
		*	示例
		
			。，13.04去食堂？吃饭！？刷碗	
	*   示例解释
    	*   13：04之前所做的事情为`去食堂`，准备做的事是`吃饭`，现在（发短信时）准备要做的事情为`刷碗`
    *	说明
	*	`,`代表补充记录的开始，随后添加4位24小时制的数字表示记录发生时间（如`09.21`,`13.05`,`23.59`之类的），小时与分钟间使用小数点`.`隔开，时间之后是要补充的记录内容，补充完毕后用`！`收尾，随后是正常的记录内容  
				
	       	注意：距离现在一天之内的记录都可以使用『，』与『!』进行补充
	*	示例：  
				
	        。，12.30？上网！，13.00？吃饭！,13.50？上自习！，15.30？休息！，16.00？做数学一模拟题！？吃饭
		
	
		
5.	记录备注
	*	如果对事件需要进行备注的话，直接将备注内容在事项后用`[]`括起来即可
	*	示例  
				
	        。吃饭[班级聚会]？洗碗[你好，我叫平田真杯具男]
		
	*	`班级聚会`与`你好，我叫平田真杯具男`就是备注内容  

---
###操作示例  
 1. 进行记录  
	*	标准记录模式
		*	![标准记录范例](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E6%A0%87%E5%87%86%E8%AE%B0%E5%BD%95%E8%8C%83%E4%BE%8B.png)

	*	补充记录模式
		*	![补充记录范例](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E8%A1%A5%E5%85%85%E8%AE%B0%E5%BD%95%E8%8C%83%E4%BE%8B.png)
		*	![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E8%A1%A5%E5%85%85%E8%AE%B0%E5%BD%95%E8%8C%83%E4%BE%8B2.png)

	*	记录完成后的样子
		*	![记录完成后的样子](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E8%AE%B0%E5%BD%95%E8%8C%83%E4%BE%8B.png)

 2. 由豌豆荚导出软件记录
 	*	
 		*	![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E5%AF%BC%E5%87%BA.JPG)
 3. 运行程序进行处理
 	*	打开文件
 			![打开文件](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E6%89%93%E5%BC%80%E6%96%87%E4%BB%B6.JPG)
 	*	修改错误
 			![按照提示修改错误](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E6%8C%89%E6%8F%90%E7%A4%BA%E4%BF%AE%E6%94%B9%E8%AE%B0%E5%BD%95%E4%B8%AD%E5%AD%98%E5%9C%A8%E7%9A%84%E9%94%99%E8%AF%AF.JPG)	
 	*	修改完成后点击`修改`，然后再点击`验证`，如果文件中没有错误的话会变成下图这个样子，然后点击进入`下一环节`,进入预览模块
 		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E9%AA%8C%E8%AF%81%E5%AE%8C%E6%AF%95%EF%BC%8C%E7%82%B9%E5%87%BB%E8%BF%9B%E5%85%A5%E4%B8%8B%E4%B8%80%E7%8E%AF%E8%8A%82.JPG)	
	*	点击`添加至总记录中`，或者直接点`进入总记录`
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E6%B7%BB%E5%8A%A0%E8%87%B3%E6%80%BB%E8%AE%B0%E5%BD%95.JPG)
	*	进入总记录后如下图所示，可以直接点击导出为Excel格式，也可以先点击`事件分类`，对时间进行分类
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E8%BF%9B%E5%85%A5%E6%80%BB%E8%AE%B0%E5%BD%95.JPG)	
	*	事件分类中默认会有一个`自定义`项目，`自定义`项目下的所有事项会被置空，点击右键可以新建项目，双击修改
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E4%BA%8B%E4%BB%B6%E5%88%86%E7%B1%BB.JPG)
	*	将事项拖入新建的分类中，点击保存返回
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E4%BF%9D%E5%AD%98.JPG)
	*	刷新
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E4%BF%9D%E5%AD%98%E5%90%8E%E5%88%B7%E6%96%B0.JPG)
	*	ok，事项分类完毕~PS：事件分类支持层级分类，支持拖放操作，支持使用`Ctrl`与`Shift`进行多选
				
	        好吧，其实我就是把事项做为文件，事项分类做为成电脑里的文件夹进行处理的
			总目录中的『分类』中显示的就是文件在电脑里的地址括弧笑   
		
 	*	导出Excel表格	
 		直接点击按钮，选择导出
		*	![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E5%AF%BC%E5%87%BA%E4%B8%BAexcel.JPG)
		*	在打开文件时会提示说文件类型不一致，忽略掉提示，点击确认
			*	![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E7%9B%B4%E6%8E%A5%E7%82%B9%E7%A1%AE%E5%AE%9A.JPG)
		
		*	附上：
			*	Excel打开后示例图
        		*	![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E7%A4%BA%E4%BE%8B.JPG)
        	*   简单处理之后
            	*  ![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/Excel%E7%AE%80%E5%8D%95%E5%A4%84%E7%90%86%E4%B9%8B%E5%90%8E.JPG)
		*	然后，让我们开始吧~	

	
软件下载地址：[http://pan.baidu.com/s/1CaCDN](http://pan.baidu.com/s/1CaCDN)  
作者主页　　：[http://hi.baidu.com/yaojiang375](http://hi.baidu.com/yaojiang375)


[1]:  http://book.douban.com/subject/3609132/
[2]: http://shouji.sogou.com/?r=pinyin
[3]: http://www.wandoujia.com/
