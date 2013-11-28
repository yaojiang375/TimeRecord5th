#时间管理器使用指南
##编写动机
*	去年这个时候读到了李笑来先生的《把时间当作朋友》[1] 他在书中提到：绝大多数人都能认识到金钱的宝贵，为了更好的利用资源，大家往往会对自己的收支状况进行记账。但实际上，时间是一种比金钱更珍贵的资源

>	尽管“千金散去”确实可能“还复来”，但你确实知道也不得不承认“时光匆匆似流水它一去”就千真万确地“不复回”

*	为了能方便的记录下时间的开销，所以，花了些时间，做出了这个软件
*	当然，考研的同学，冲击一万小时定律的同学，都可以用这个软件来生成属于自己的时间财报

>	毕竟，成功的核心素质坚持不懈，其本质就是时间的投入

##软件原理
*	众所周知，手机发送的每条短信带有发送时间，也就是说，如果我们持续的在做每一项事情前都发送一条短信的话，把所有短信记录合并起来，我们就可以得到自己一天的生活轨迹
*	而且，我们亲爱的运营商同学还会经常提醒我们:
>	您可以直接回复以下数字查询或办理其他业务，收发短信**免费**

*	也就是说，我们可以通过不断向中国`移动`/`联通`/`电信`发送短信的办法，来记录我们自己的生活
*	所以，我们所面对的问题就变成了  
	
		怎么才能把短信时间从记录中抽取出来，并组合成我们所需要的形式？
	
*	请看：
##软件的使用方法
###建立记录

以下假定手机为安卓智能机，输入法是搜狗手机输入法 [2] ，电脑端已安装豌豆荚 [3]

####语法格式
1.	记录标识
	*	记录标识符用于系统判断短信是否为我们所使用的记录
	*	默认为`。`写在记录的开始处。只有开头为`。`的短信才会被系统作为记录进行读取
2.	事件分割
	*	用于声明时间节点的前一事件与后一事件
	*	默认为`？`
	*	示例	
		
			。吃饭？洗碗
	
	*	`。`标识记录的开始，`？`前指记录点之前所做的事情	`吃饭`，`？`后面记录节点之后准备要做的事`洗碗`	
3.	补充记录
	*	语法功能中只有简单的记录标识符显然是不够的，因为我们总会因为各种原因忘掉记录
	*	所以，我们引入了补充标识符，一旦忘掉记录，我们就可以用补充标识进行补记
	*	补记标识符默认为 `,`  
4.	补充结束标识
	*	每一个补充标记都必须与一个补记结束标记相对应，否则系统会自动报错，补充结束标记默认为`！`
	*	示例
		
			。，13.04去食堂？喝馄饨！吃饭？洗碗	
	*	说明
	*	`,`代表补充记录的开始，随后要添加5位数的时间，小时与分钟间使用小数点`.`隔开，然后是`事件名`*可选*，事件分隔符`？`*必须*，`事件名`*可选*，补充记录结束符`！`*必须*。结束符之后是正常的记录内容  
			
	        注意:在同一时间段内如果出现两个不同的事件名的话，后一事件名会自动替换掉前一事项名 
		
		
5.	记录备注
	*	如果对事件需要进行备注的话，直接将备注内容用`[]`括起来即可
	*	示例  
				
	        。吃饭[班级聚会]？洗碗[你好，我叫平田真杯具男]
		
	*	`班级聚会`与`你好，我叫平田真杯具男`就是备注内容
	

	
####操作示例  
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
	*	ok，事项分类完毕~PS：支持层级分类，支持使用`Ctrl`与`Shift`进行多选
				
	        提示：
			可以把事项理解为文件，事项分类理解成电脑里的文件夹
			总目录中的『分类』中显示的就是文件的地址   
		
 	*	导出Excel表格	
 		直接点击按钮，选择导出即可
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E5%AF%BC%E5%87%BA%E4%B8%BAexcel.JPG)
		*	在打开文件时会提示说文件类型不一致，忽略掉提示，点击确认就行
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E7%9B%B4%E6%8E%A5%E7%82%B9%E7%A1%AE%E5%AE%9A.JPG)
		*	OK，祝开心~
	

	




[1]:  http://book.douban.com/subject/3609132/
[2]: http://shouji.sogou.com/?r=pinyin
[3]: http://www.wandoujia.com/
