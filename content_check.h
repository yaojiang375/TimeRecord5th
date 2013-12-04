#ifndef CONTENT_CHECK_H
#define CONTENT_CHECK_H
#include <QString>
#include <globe.h>
#include <string>//用STD重写
#include <ctype.h>
/*************************************************
 *错误代码：
 *
 *return 0;//检测通过
 *return 1;// 多余的左备注符号
 *return 2;//未检测到『？』
 *return 3;//检测到多于一个『？』
 *return 4;//在补充标识符后未检测到补充事件结束标识符『！』
 *return 5;//补充标识符后没有对应的时间。时间标准格式：12.35
 *return 6;//补充事项后没有正文内容
 *
 */
class Content_Check
{
public:
    Content_Check();
    int check(globeset globe,std::string content);
    int STDCheck(globeset globe,std::string content);
    int AddCheck(globeset globe,std::string content);

};

#endif // CONTENT_CHECK_H
