#include "content_check.h"

Content_Check::Content_Check()
{

}

int Content_Check::check(globeset globe,std::string content)
{

        content.erase(0,1);
        if(content[0]==globe.AddFlag[0])
        {
            content.erase(0,1);
            return AddCheck(globe,content);
        }
        else
        {
            return STDCheck(globe,content);
        }
}

int Content_Check::STDCheck(globeset globe,std::string content)
{
    std::string::size_type LeftPos =    content.find(globe.LeftRem.toStdString());
    if(LeftPos!=std::string::npos)//检测多余的备注符
    {
        std::string::size_type RightPos =    content.find(globe.ReghtRem.toStdString());
        if(RightPos !=std::string::npos)
        {
            content.erase(LeftPos,LeftPos-RightPos+1);//右括号也要删掉
            return STDCheck(globe,content);
        }
        else
        {
            return 1;// 多余的左备注符号
        }
    }
    else
    {
        std::string::size_type MidThingPos=content.find(globe.MidThing.toStdString());
        if(MidThingPos!=std::string::npos)
        {
            content.erase(MidThingPos,1);
            if(content.find(globe.MidThing.toStdString())!=std::string::npos)
            {
                //qDebug()<<"Wrong=3,"<<content;
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

int Content_Check::AddCheck(globeset globe,std::string content)
{
    std::string::size_type AddEndPos =content.find(globe.AddEndFlag.toStdString());
    if(AddEndPos!=std::string::npos)
    {
        std::string a;

        a=content.substr(0,AddEndPos);
        content.erase(0,a.length()+1);
        //qDebug()<<"a="<<a;
        if(isdigit(a[0])&&isdigit(a[1])&&isdigit(a[3])&&isdigit(a[4])&&(!isdigit(a[2])))
        {
            a.erase(0,5);
            if(STDCheck(globe,a)==0)
            {
                if(content.length()!=0)
                {
                    content.insert(0,globe.RecFlag.toStdString());
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
