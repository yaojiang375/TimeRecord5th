#include "content_check.h"

Content_Check::Content_Check()
{

}

int Content_Check::check(globeset globe, QString content)
{

        content.remove(0,1);
        if(content[0]==globe.AddFlag[0])
        {
            content.remove(0,1);
            return AddCheck(globe,content);
        }
        else
        {
            return STDCheck(globe,content);
        }


}

int Content_Check::STDCheck(globeset globe, QString content)
{
    if(content.indexOf(globe.LeftRem)!=-1)//检测多余的备注符
    {
        if(content.indexOf(globe.ReghtRem)!=-1)
        {
            content.remove(content.indexOf(globe.LeftRem),content.indexOf(globe.ReghtRem)-content.indexOf(globe.LeftRem));
            return STDCheck(globe,content);
        }
        else
        {
            return 1;// 多余的左备注符号
        }
    }
    else
    {
        if(content.indexOf(globe.MidThing)!=-1)
        {
            content.remove(content.indexOf(globe.MidThing),1);
            if(content.indexOf(globe.MidThing)!=-1)
            {
                qDebug()<<"Wrong=3,"<<content;
                return 3;//检测到多于一个『？』
            }
            else
            {
                return 0;//检测通过return 1;// 多余的左备注符号return 2;//未检测到『？』return 3;//检测到多于一个『？』return 4;//在补充标识符后未检测到补充事件结束标识符『！』return 5;//补充标识符后没有对应的时间。时间标准格式：12.35return 6;//补充事项后没有正文内容
            }
        }
        else
        {
            return 2;//未检测到『？』
        }
    }
}

int Content_Check::AddCheck(globeset globe, QString content)
{
    if(content.indexOf(globe.AddEndFlag)!=-1)
    {
        QString a;

        a=content.mid(0,content.indexOf(globe.AddEndFlag));
        content.remove(0,a.length()+1);
        qDebug()<<"a="<<a;
        if(a[0].isNumber()&&a[1].isNumber()&&a[3].isNumber()&&a[4].isNumber()&&(!a[2].isNumber()))
        {
            a.remove(0,5);
            if(STDCheck(globe,a)==0)
            {
                if(content.length()!=0)
                {
                    content.insert(0,globe.RecFlag);
                    return check(globe,content);
                }
                else
                {
                    return 6;//补充事项后没有正文内容
                }
            }
            else
            {
                return STDCheck(globe,a);
            }
        }
        else
        {
            return 5;//补充标识符后没有对应的时间。时间标准格式：12.35
        }
    }
    else
    {
        return 4;//在补充标识符后未检测到补充事件结束标识符『！』return 5;//补充标识符后没有对应的时间。时间标准格式：12.35return 6;//补充事项后没有正文内容
    }
}
