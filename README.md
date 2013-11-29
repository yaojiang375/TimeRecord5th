#　　　　　　　时间管理器使用指南

>    　　　　　　　　　　　　　　　与投入的时间相比，聪明、勤奋之类的因素实在是微不足道。  
>　　　
>   　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　*via*《把时间做朋友》  
＞  　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　李笑来

##软件原理
　　手机发送的每条短信均带有发送时间，我们可以在做每一项事情前向`中国`<code>移动</code>\\<code>联通</code>\\<code>电信</code>发送一条短信的方法，来记录自己一天的生活轨迹
##软件的使用方法
###建立记录

以下假定手机为安卓智能机，输入法是搜狗手机输入法 [2] ，电脑端已安装豌豆荚 [3]

####语法格式
1.	记录标识符
	*	记录标识符用于将我们的生活记录与普通短息区分开
	*	默认为`。`，写在每条记录的开始处。只有开头为`。`的短信才会被系统作为记录进行读取
2.	断点标记符
	*	断点标记符默认为`？`，象征我们发送短信的那个时间节点，标记符之前的内容是我们发短信之前所做的事情，之后的内容是发完短信后我们准备去做的事项。
	*	每条记录中有且只能由一个断电标记。记录中之前所做的事与准备要做的事都可以空掉不写，后一条记录中所写的之前所做的事会自动替换掉前条记录中准备做的事，参考，数轴~
	*	示例	
	
			。吃饭？洗碗
	
	    *	`。`标识记录的开始，发短信前所做的事情是`吃饭`，发完短信后准备要做的事为`洗碗`	
3.	补充记录
	*	当我们因为各种原因而忘记做记录时，使用补充记录功能可以方便的进行补记
	*	默认为 `,`  
	*	使用说明见下
4.	补充结束标识
	*	每一个补充标记都必须与一个补记结束标记相对应，否则系统会自动报错，补充结束标记默认为`！`
	*	示例
		
			。，13.04去食堂？吃饭！？刷碗	
	*   示例解释
    	*   13：04之前所做的事情为`去食堂`，准备做的事是`吃饭`，现在（发短信时）准备要做的事情为`刷碗`
    *	说明
	*	`,`代表补充记录的开始，随后要添加4位数的时间(24小时制)，小时与分钟间使用小数点`.`隔开，然后是正常记录内容，补充记录结束符`！`，结束符之后是正常的记录内容  
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
	*	ok，事项分类完毕~PS：事件分类支持层级分类，支持拖放操作，支持使用`Ctrl`与`Shift`进行多选
				
	        好吧，其实我就是把事项做为文件，事项分类做为成电脑里的文件夹进行处理的
			总目录中的『分类』中显示的就是文件在电脑里的地址括弧笑   
		
 	*	导出Excel表格	
 		直接点击按钮，选择导出即可
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E5%AF%BC%E5%87%BA%E4%B8%BAexcel.JPG)
		*	在打开文件时会提示说文件类型不一致，忽略掉提示，点击确认就行
		![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E7%9B%B4%E6%8E%A5%E7%82%B9%E7%A1%AE%E5%AE%9A.JPG)
		
		*	附上：
		*	Excel打开后示例图
        ![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/%E7%A4%BA%E4%BE%8B.JPG)
        *   简单处理之后
            *  ![](http://timerecord-timerecord.stor.sinaapp.com/%E7%A4%BA%E4%BE%8B%E5%9B%BE%E7%89%87/Excel%E7%AE%80%E5%8D%95%E5%A4%84%E7%90%86%E4%B9%8B%E5%90%8E.JPG)
		*	Over，祝开心~	

	




[1]:  http://book.douban.com/subject/3609132/
[2]: http://shouji.sogou.com/?r=pinyin
[3]: http://www.wandoujia.com/
