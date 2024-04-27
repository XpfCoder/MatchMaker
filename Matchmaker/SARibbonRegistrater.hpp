#pragma once
#include<map>
#include<string>
//#include<QWidget>

typedef QWidget* (*fun_ptr)();
typedef std::map<const std::string, fun_ptr> create_obj_map;
class LazySingleton  //返回类实例
{
public:
	template<class T>
	static T& instance()
	{
		static T _instance;
		return _instance;
	}
};

class ClassFactory
{
public:
	static QWidget* createObjectByName(const std::string& className)
	{
		create_obj_map::iterator iter = LazySingleton::instance<create_obj_map>().begin();  //获取map迭代器
		create_obj_map::iterator iterEnd = LazySingleton::instance<create_obj_map>().end();

		while (iter != iterEnd)
		{
			if (iter->first == className)
			{
				return iter->second(); //函数指针的调用
			}
			iter++;
		}
		return NULL;
	}
	static void registerClass(const std::string& className, fun_ptr fp)
	{
		LazySingleton::instance<create_obj_map>().insert(std::make_pair(className, fp));
	}
	static ClassFactory& getInstance()
	{
		{
			static ClassFactory classF;
			return classF;
		}
	}
};

//void* ClassFactory::createObjectByName(const std::string& className)
//{
//	create_obj_map::iterator iter = LazySingleton::instance<create_obj_map>().begin();  //获取map迭代器
//	create_obj_map::iterator iterEnd = LazySingleton::instance<create_obj_map>().end();
//
//	while (iter != iterEnd)
//	{
//		if (iter->first == className)
//		{
//			return iter->second(); //函数指针的调用
//		}
//		iter++;
//	}
//	return NULL;
//}
//void ClassFactory::registerClass(const std::string& className, fun_ptr fp)
//{
//	LazySingleton::instance<create_obj_map>().insert(std::make_pair(className, fp));
//}

class Register
{
public:
	Register(const char* className, fun_ptr fp)
	{
		ClassFactory::registerClass(className, fp);
	}
};
//Register::Register(const char* className, fun_ptr fp)
//{
//	
//}
template<typename T>
class RegisterClassName
{
public:
	static void* instance()
	{
		T* t = new T();
		return t;
	}
};
#define REGISTER_SARIBBON(ClassName)	\
Register  g_RegisterAction##ClassName(#ClassName,(fun_ptr)RegisterClassName<ClassName>##::instance);
