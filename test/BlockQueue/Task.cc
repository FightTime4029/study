#pragma once
#include <iostream>
#include <functional>
#include <cstdio>
#include <string>
class Task
{
public:
    using fun_t = std::function<int(int,int,char)>; // == typedef function<int(int,int)> fun_t  函数指针
    Task()
    {}
     Task(int x,int y,char op,fun_t callback):_x(x),_y(y),_op(op),_callback(callback)
    {}
    std::string operator()()
    {
        int ret = _callback(_x,_y,_op);
        char buffer[1024];
        snprintf(buffer,sizeof buffer,"%d %c %d = %d",_x,_op,_y,ret);
        return buffer;
    }

    std::string toTaskstring()
    {
        char buffer[1024];
        snprintf(buffer,sizeof buffer,"%d %c %d",_x,_op,_y);
        return buffer;
    }
private:
    int _x;
    int _y;
    char _op;
    fun_t _callback;
};

class saveTask
{
public:
    typedef std::function<void(const std::string &)> func_t;
    saveTask()
    {}

    saveTask(const std::string& message,func_t func):_message(message),_func(func)
    {}

    void operator()()
    {
        _func(_message);
    }

 private:
   std::string _message;
   func_t _func;
};