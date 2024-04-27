#pragma once

#ifndef __SHARE_DATE_TIME_H__
#define __SHARE_DATE_TIME_H__

#include "LogUtility.h"
#include <stdio.h>
//#include "tick.h"

namespace  TimeUtil
{
	const static int ONEWEEK_SECONDS	= 604800;
    const static int ONEDAY_SECONDS     = 86400;	///< 1天的秒数
    const static int ONE_HOUR_SECONDS   = 60 * 60;
    const static int ONE_MINUTE_SECONDS = 60;    
    
    const static time_t MIN_TIME_T_VALUE = time_t(0);       ///无效时间值
    const static time_t MAX_TIME_T_VALUE = 0x7FFFFFFFL;     ///无效时间值  
	const static time_t MAX_TIME_T_VALUE_1 = 0x7FFFFFFFL-ONEDAY_SECONDS; ///无效时间值   
    
	//时间零点对应的时刻
	const static int WEEKDAY_AT_ZERO_TIME = 4;
	const static int HOUR_AT_ZERO_TIME = 8;	
	const static int MIN_AT_ZERO_TIME = 0;
	const static int SEC_AT_ZERO_TIME = 0;
	
	//时间零点距离当天0点的秒数
	const static int SECONDS_OFFSET = HOUR_AT_ZERO_TIME * ONE_HOUR_SECONDS;

	/**
	 @brief 从字符串中读取日期，日期格式为YYYY-MM-DD，内部会验证合法性
	 @param src 源字符串
	 @param out_tm 提取的日期，其中tm_hour、tm_min、tm_sec为0，tm_yday、tm_wday已经用mktime赋值
	 @param out_t 提取的日期
	 @return 是否成功
	 */
	inline bool ReadDateStr(const char* src, tm* out_tm, time_t* out_t);
	/**
	 @brief 从字符串中读取时间，时间格式为HH:MM:SS，其中HH是必须的，:MM和:SS可选，内部会验证合法性
	 @param str 源字符串
	 @param out_tm 提取的时间，只有tm_hour、tm_min、tm_sec有效，
	 @param out_t 提取的时间对应的秒数
	 @return 是否成功
	 */
	inline bool ReadTimeStr(const char* src, tm* out_tm, time_t* out_t);
	/**
	 @brief 从字符串中读取日期，日期格式为YYYY-MM-DD HH:MM:SS，内部会验证合法性
	 @param src 源字符串
	 @param out_tm 提取的时间
	 @param out_t 提取的时间
	 @return 是否成功
	 */
	inline bool ReadDateTimeStr(const char* src, tm* out_tm, time_t* out_t);

	/**
	 @brief 将time_t转化为mysql的时间字符串格式：YY-MM-DD HH:MM:SS
	 @param t 源时间
	 @return inline字符串
	 @deprecated
	 */
	inline const char* TimeToSqlStr(time_t t);

	/**
	 @brief 将time_t转化为mysql的时间字符串格式：YY-MM-DD HH:MM:SS
	 @param t 源时间
	 @return 
	 */
	inline std::string TimeToSqlStr_s(const time_t& t);

	/**
	 @brief 返回当前时间的mysql的时间字符串格式：YY-MM-DD HH:MM:SS
	 @return inline 字符串
	 */
	inline const char*  CurTimeToSqlStr();

	/*
	 *	YYYY-MM-DD
	 */
	inline const char* TimeToDateStr(time_t t);
	/*
	 *	YYYY-MM-DD_HH
	 */
	inline const char* TimeToDateHourStr(time_t t);

	/*
	 *	YYYY年MM月DD日
	 */
	inline const char* TimeToDateStr2( time_t t );

	/*
	 *	YYYY年MM月DD日HH时MM分
	 */
	inline const char* TimeToDateStr3( time_t t );
		/*
	 *	YYYY.MM.DD
	 */
	inline const char* TimeToDateStr4(time_t t);
	/*
	 *	MM.DD
	 */
	inline const char* TimeToDateStr5(time_t t);
	/*
	 *	HH:MM:SS
	 */
	inline const char* TimeToTimeStr(time_t t);

	/*
	 *	HH:MM
	 */
	inline const char* TimeToHourMin(time_t t);

	/**
	 @brief	查询指定时间所在日期的0点
	 @param	t 源时间
	 @param	out_tm 日期0点，其中hour、min、sec已经设置为0
	 @param out_t 日期0点
	 @return true成功，false失败
	 */
	inline bool GetDayStart(time_t t, tm* out_tm, time_t* out_t);
	/**
	 @brief 返回指定时间所在月份1号的0点
	 @param 
	 @param 
	 @param 
	 @return 
	 */
	inline time_t GetMonthStart(time_t t);

	//得到这个月的第几天
	inline int GetDayIndexInMonth(time_t t);
	// 
	inline int GetYear(time_t t);
	inline int GetMonthNum(time_t t);
	inline time_t GetLastMonthStart(time_t t);
	inline time_t GetNextMonthStart(time_t t);
    /******************************************************************* 
     * @brief:	  返回指定时间所在日期的0点
     * @function: GetDayStart
     * @access:   public inline 
     * @return:   time_t - 如果输出无效时间，则返回MAX_TIME_T_VALUE
     * @param: 	  time_t t
     *
     ******************************************************************/
    inline time_t GetDayStart(time_t t);

	 /******************************************************************* 
     * @brief:	  返回指定时间所在日期的23:59:59点
     * @function: GetDayEnd
     * @access:   public inline 
     * @return:   time_t - 如果输出无效时间，则返回MAX_TIME_T_VALUE
     * @param: 	  time_t t
     *
     ******************************************************************/
    inline time_t GetDayEnd(time_t t);

    /******************************************************************* 
     * @brief:	  判断两个时间是否是同一天
     * @function: IsSameDay
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t1
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsSameDay(time_t t1, time_t t2);
	
    /******************************************************************* 
     * @brief:	  判断两个时间是否是同一小时
     * @function: IsSameHour
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t1
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsSameHour(time_t t1, time_t t2);
    /******************************************************************* 
     * @brief:	  判断两个时间是否在同一周
     * @function: IsSameWeek
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t2
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsSameWeek(time_t t1, time_t t2);

	 /******************************************************************* 
     * @brief:	  判断两个时间是否在同一周 (这个比较特殊，非常规同一周，计算偏移时间)
     * @function: IsSameWeek
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t2
     * @param: 	  time_t t2
	 * @param:	  int    offset_days 
     *
     ******************************************************************/
    inline bool IsSameWeek(time_t t1, time_t t2,int offset_days);
    /******************************************************************* 
     * @brief:	  判断两个时间是否在同一月
     * @function: IsSameMonth
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t2
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsSameMonth(time_t t1, time_t t2);

    /******************************************************************* 
     * @brief:	  返回HH:MM格式字符串
     * @function: ToHourMinuteStr
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t
     * @param: 	  std::string & str
     *
     ******************************************************************/
    inline bool ToHourMinuteStr(time_t t, std::string &str);

	/******************************************************************* 
	* @brief:	  返回某时刻是第几天（1970.1.1是第0天）
	* @function:  GetDayNum
	* @access:   public inline 
	* @return:   int
	* @param: 	  time_t t

	*
	******************************************************************/
	inline int GetDayNum(time_t t);
	inline time_t GetTimeOfDayNum(int num);

	/******************************************************************* 
	* @brief:	  从start到end，经历了几个t点钟(t=[0,1,2,...,23])
	* @function:  GetDayNum
	* @access:   public inline 
	* @return:   int
	* @param: 		time_t	start	起始时间
	* @param:		time_t	end		结束时间
	* @param:		int		t		检查的时刻，[0,1,2, ... ,23]
	
	*
	******************************************************************/
	inline int GetNumContainedT(time_t start,time_t end,int t);

	/******************************************************************* 
	* @brief:	  返回这样一个时刻：离t最近且早于t，并且处于整点h (h=[0,1,2,...,23])
	* @function:  TimeEarlierThanT
	* @access:   public inline 
	* @return:   int
	* @param: 		time_t	t		
	* @param:		int		h		

	*
	******************************************************************/
	inline time_t TimeEarlierThanT(time_t t,int h);

	/***************************************
	 *@brief   GetHour 获取时间的小时值 [0,1....23]
	 *@return int
	 *@param time_t time
	****************************************/
	inline int GetHour(time_t time);

	/*************************************
	*@brief GetMinute 获得时间[0,59]
	*@return int
	*@param time_t time
	**************************************/
	inline int GetMinute(time_t time);

	/***************************************
	 *@brief   GetMonthDayCountByMonthIndex 通过月份得到时间 [0,11]
	 *@return int
	 *@param time_t time
	****************************************/
	inline int GetMonthDayCountByMonthIndex(int year,int month_index);

	inline time_t GetWeekStart(time_t t);

	inline int GetWeekIndex(time_t t);

	inline struct tm GettmInfoByWeek(int week_num);

	inline int GetWeekNum(time_t t);

	/******************************************************************* 
     * @brief:	  判断两个时间是否在同一周期内
     * @function: IsSameCycle
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t1 现在时间
     * @param: 	  time_t t2 上次记录时间
	 * @param:    int cycle(周期单位"天")
     *
     ******************************************************************/
    inline bool IsSameCycle(time_t t1, time_t t2, int cycle);
	/******************************************************************* 
     * @brief:	  获取下一周期开始时间
     * @function: GetNextCycleStartTime
     * @access:   public inline 
     * @return:   time_t
     * @param: 	  time_t t1 现在时间
	 * @param:    int cycle(周期单位"天")
     *
     ******************************************************************/
	inline time_t GetNextCycleStartTime(time_t t1, int cycle);
	/******************************************************************* 
     * @brief:	  获取当前自定义时间戳 格式：YYYYMM
     * @function: TimeToDateStamp
     * @access:   public inline 
     * @return:   int
     * @param: 	  time_t t 现在时间
     *
     ******************************************************************/
	inline int TimeToDateStamp(time_t t);

	/******************************************************************* 
     * @brief:	  获取当前时间是当月几日
     * @function: GetDaysInMonth
     * @access:   public inline 
     * @return:   int
     * @param: 	  time_t t 时间
     *
     ******************************************************************/
	inline int GetDaysInMonth(time_t t);

	    /******************************************************************* 
     * @brief:	  判断t2是否是t1所在周的周六或者周日
     * @function: IsWeekEnd
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t2
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsWeekEnd(time_t t1, time_t t2);
};
inline bool isLeapYear(int year)
{
	if ((year % 4 == 0) && (year % 400 != 0))
	{
		return true;
	}

	return false;
}

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "LogAssert.h"
#include "../../MatchMaker/securec.h"

namespace TimeUtil
{
#define safe_sprintf sprintf_s
#define safe_sscanf sscanf_s

	bool TimeUtil::ReadDateStr(const char* src, tm* out_tm, time_t* out_t)
	{
		if (!src)
			return false;
		tm temp_tm;
		time_t temp_t;
		//memset(&temp_tm, 0, sizeof(temp_tm));
		memset_s(&temp_tm, sizeof(temp_tm), 0, sizeof(temp_tm));
		if (!strongAssert(safe_sscanf(src, "%d-%d-%d", &temp_tm.tm_year, &temp_tm.tm_mon, &temp_tm.tm_mday) == 3))
			return false;
		temp_tm.tm_year -= 1900;
		temp_tm.tm_mon -= 1;
		if (!strongAssert(temp_t = mktime(&temp_tm)))
			return false;
		if (out_tm)
			*out_tm = temp_tm;
		if (out_t)
			*out_t = temp_t;
		return true;
	}

	bool TimeUtil::ReadTimeStr(const char* src, tm* out_tm, time_t* out_t)
	{
		if (!src)
			return false;
		int hour = 0, min = 0, sec = 0;
		if (!strongAssert(safe_sscanf(src, "%d:%d:%d", &hour, &min, &sec) >= 1))
			return false;
		if (hour < 0 || hour > 23 ||
			min < 0 || min > 59 ||
			sec < 0 || sec > 59)
			return false;
		if (out_tm)
		{
			out_tm->tm_hour = hour;
			out_tm->tm_min = min;
			out_tm->tm_sec = sec;
		}
		if (out_t)
			*out_t = hour * 3600 + min * 60 + sec;
		return true;
	}


	bool TimeUtil::ReadDateTimeStr(const char* src, tm* out_tm, time_t* out_t)
	{
		if (!src)
			return false;
		tm temp_tm;
		time_t temp_t;
		memset_s(&temp_tm, sizeof(temp_tm), 0, sizeof(temp_tm));
		//memset(&temp_tm, 0, sizeof(temp_tm));
		if (!strongAssert(safe_sscanf(src, "%d-%d-%d %d:%d:%d", &temp_tm.tm_year, &temp_tm.tm_mon, &temp_tm.tm_mday, &temp_tm.tm_hour, &temp_tm.tm_min, &temp_tm.tm_sec) >= 3))
			return false;
		temp_tm.tm_year -= 1900;
		temp_tm.tm_mon -= 1;
		if (!strongAssert(temp_t = mktime(&temp_tm)))
			return false;
		if (out_tm)
			*out_tm = temp_tm;
		if (out_t)
			*out_t = temp_t;
		return true;
	}

	const char* TimeUtil::TimeToSqlStr(time_t t)
	{
		if (t <= 0)
			return "1970-01-01 08:00:00";

		static char timestr[20];
		//memset(timestr, 0, sizeof(timestr));
		memset_s(timestr, sizeof(timestr), 0, sizeof(timestr));
		struct tm _tm = safe_localtime(t);
		tm* tm = &_tm;
		if (strongAssert(t))
		{
			safe_sprintf(timestr, "%d-%02d-%02d %02d:%02d:%02d",
				tm->tm_year + 1900,
				tm->tm_mon + 1,
				tm->tm_mday,
				tm->tm_hour,
				tm->tm_min,
				tm->tm_sec);
			timestr[19] = 0;
			return timestr;
		}
		else
		{
			return "1970-01-01 08:00:00";	///< 北京时间
		}
	}

	std::string TimeUtil::TimeToSqlStr_s(const time_t& t)
	{
		if (t <= 0)
			return "1970-01-01 08:00:00";

		char timestr[20];
		//memset(timestr, 0, sizeof(timestr));
		memset_s(timestr, sizeof(timestr), 0, sizeof(timestr));
		struct tm _tm = safe_localtime(t);
		tm* tm = &_tm;
		if (strongAssert(t))
		{
			safe_sprintf(timestr, "%d-%02d-%02d %02d:%02d:%02d",
				tm->tm_year + 1900,
				tm->tm_mon + 1,
				tm->tm_mday,
				tm->tm_hour,
				tm->tm_min,
				tm->tm_sec);
			timestr[19] = 0;
			return timestr;
		}
		else
		{
			return "1970-01-01 08:00:00";	///< 北京时间
		}
	}


	const char*  TimeUtil::CurTimeToSqlStr()
	{
		static char			timestr[20] = { 0 };
		static time_t		t = 0;

		time_t cur_time = time(NULL);
		if (cur_time == t) {
			return timestr;
		}

		t = cur_time;
		if (t <= 0)
		{
			safe_sprintf(timestr, "1970-01-01 08:00:00");
			return timestr;
		}

		struct tm _tm = safe_localtime(t);
		tm* tm = &_tm;
		if (strongAssert(t))
		{
			safe_sprintf(timestr, "%d-%02d-%02d %02d:%02d:%02d",
				tm->tm_year + 1900,
				tm->tm_mon + 1,
				tm->tm_mday,
				tm->tm_hour,
				tm->tm_min,
				tm->tm_sec);
			timestr[19] = 0;
			return timestr;
		}
		else
		{
			safe_sprintf(timestr, "1970-01-01 08:00:00");///< 北京时间
			return timestr;
		}
	}

	bool TimeUtil::GetDayStart(time_t t, tm* out_tm, time_t* out_t)
	{
		struct tm _tm = safe_localtime(t);
		tm* tm = &_tm;
		//if (!strongAssert(tm))
		//	return false;
		tm->tm_hour = 0;
		tm->tm_min = 0;
		tm->tm_sec = 0;
		if (out_tm)
			*out_tm = *tm;
		if (out_t)
			*out_t = mktime(tm);
		return true;
	}

	time_t TimeUtil::GetDayStart(time_t t)
	{
		time_t result = MAX_TIME_T_VALUE;
		GetDayStart(t, NULL, &result);
		return(result);
	}

	time_t TimeUtil::GetDayEnd(time_t t)
	{
		time_t result = MAX_TIME_T_VALUE;
		struct tm _tm = safe_localtime(t);
		tm* tm = &_tm;
		if (!strongAssert(tm))
			return result;
		tm->tm_hour = 23;
		tm->tm_min = 59;
		tm->tm_sec = 59;
		result = mktime(tm);
		return result;
	}

	time_t TimeUtil::GetMonthStart(time_t t)
	{
		struct tm _tm = safe_localtime(t);
		tm* tm_t = &_tm;
		if (tm_t != NULL)
		{
			tm_t->tm_mday = 1;
			tm_t->tm_hour = 0;
			tm_t->tm_min = 0;
			tm_t->tm_sec = 0;
			return mktime(tm_t);
		}
		return MAX_TIME_T_VALUE;
	}
	int TimeUtil::GetDayIndexInMonth(time_t t)
	{
		struct tm _tm = safe_localtime(t);
		tm* tm_t = &_tm;
		if (tm_t != NULL)
		{
			return tm_t->tm_mday;
		}
		else
		{
			return 0;
		}
	}
	int TimeUtil::GetMonthNum(time_t t)
	{
		struct tm _tm = safe_localtime(t);
		tm* tm_t = &_tm;
		if (!tm_t)
			return 0;

		return tm_t->tm_mon + 1;
	}
	int TimeUtil::GetYear(time_t t)
	{
		struct tm _tm = safe_localtime(t);
		tm* tm_t = &_tm;
		if (!tm_t)
			return 0;

		return tm_t->tm_year + 1900;
	}
	time_t TimeUtil::GetLastMonthStart(time_t t)
	{
		struct tm _tm = safe_localtime(t);
		tm* tm_t = &_tm;
		if (tm_t != NULL)
		{
			tm_t->tm_mon--;
			if (tm_t->tm_mon < 0)
				tm_t->tm_mon = 11;

			tm_t->tm_mday = 1;
			tm_t->tm_hour = 0;
			tm_t->tm_min = 0;
			tm_t->tm_sec = 0;
			return mktime(tm_t);
		}
		return MAX_TIME_T_VALUE;
	}

	time_t TimeUtil::GetNextMonthStart(time_t t)
	{
		struct tm _tm = safe_localtime(t);
		tm* tm_t = &_tm;
		if (tm_t != NULL)
		{
			tm_t->tm_mon++;
			if (tm_t->tm_mon > 11)
				tm_t->tm_mon = 0;

			tm_t->tm_mday = 1;
			tm_t->tm_hour = 0;
			tm_t->tm_min = 0;
			tm_t->tm_sec = 0;
			return mktime(tm_t);
		}
		return MAX_TIME_T_VALUE;
	}

	bool TimeUtil::IsSameWeek(time_t t1, time_t t2)
	{
		if (t1 < t2)
		{
			std::swap(t1, t2);
		}

		struct tm tm1 = safe_localtime(t1);

		//调整周末
		tm1.tm_wday = (0 == tm1.tm_wday) ? 7 : tm1.tm_wday;

		//周一凌晨到现在的时间差值
		time_t differ = (tm1.tm_wday - 1) * ONEDAY_SECONDS +
			tm1.tm_hour * ONE_HOUR_SECONDS +
			tm1.tm_min * ONE_MINUTE_SECONDS +
			tm1.tm_sec;

		time_t weekbegin = t1 - differ;             //把时间调整每周的开始（周一早上0点）

		return((t2 >= weekbegin) && (t2 <= t1));
	}
	bool TimeUtil::IsSameWeek(time_t t1, time_t t2, int offset_days)
	{
		if (t1 < t2)
		{
			std::swap(t1, t2);
		}

		struct tm tm1 = safe_localtime(t1);

		//调整周末


		tm1.tm_wday = (0 == tm1.tm_wday) ? 7 : tm1.tm_wday;
		tm1.tm_wday += offset_days;
		if (tm1.tm_wday > 7)
		{
			tm1.tm_wday = tm1.tm_wday - 7;
		}

		//周一凌晨到现在的时间差值
		time_t differ = (tm1.tm_wday - 1) * ONEDAY_SECONDS +
			tm1.tm_hour * ONE_HOUR_SECONDS +
			tm1.tm_min * ONE_MINUTE_SECONDS +
			tm1.tm_sec;

		time_t weekbegin = t1 - differ;             //把时间调整每周的开始（周一早上0点）

		return((t2 >= weekbegin) && (t2 <= t1));
	}

	struct tm TimeUtil::GettmInfoByWeek(int week_num)
	{
		static time_t temp = 1412524800;//2014.10.6
		time_t temp_t = temp + week_num * 7 * 3600 * 24;

		return safe_localtime(temp_t);
	}

	time_t TimeUtil::GetWeekStart(time_t t)
	{
		struct tm tm1 = safe_localtime(t);

		//调整周末
		tm1.tm_wday = (0 == tm1.tm_wday) ? 7 : tm1.tm_wday;

		//周一凌晨到现在的时间差值
		time_t differ = (tm1.tm_wday - 1) * ONEDAY_SECONDS +
			tm1.tm_hour * ONE_HOUR_SECONDS +
			tm1.tm_min * ONE_MINUTE_SECONDS +
			tm1.tm_sec;

		time_t weekbegin = t - differ;
		return weekbegin;
	}
	int TimeUtil::GetWeekIndex(time_t t)
	{
		struct tm tm1 = safe_localtime(t);
		//调整周末
		return tm1.tm_wday;
	}
	bool TimeUtil::IsSameMonth(time_t t1, time_t t2)
	{
		return(GetMonthStart(t1) == GetMonthStart(t2));
	}


	bool TimeUtil::IsSameDay(time_t t1, time_t t2)
	{
		return(GetDayStart(t1) == GetDayStart(t2));
	}

	bool TimeUtil::IsSameHour(time_t t1, time_t t2)
	{
		tm tmt1 = safe_localtime(t1);
		tm tmt2 = safe_localtime(t2);
		return tmt1.tm_year == tmt2.tm_year && tmt1.tm_mon == tmt2.tm_mon && tmt1.tm_mday == tmt2.tm_mday && tmt1.tm_hour == tmt2.tm_hour;
	}

	bool TimeUtil::ToHourMinuteStr(time_t t, std::string &str)
	{
		tm tmt = safe_localtime(t);
		str.reserve(10);
		str = "00:00";

		safe_sprintf((char*)str.c_str(), 10, "%02d:%02d", tmt.tm_hour, tmt.tm_min);

		return(false);
	}

	int TimeUtil::GetDayNum(time_t t)
	{
		time_t ret = (t + SECONDS_OFFSET) / ONEDAY_SECONDS;
		return (int)ret;
	}

	time_t TimeUtil::GetTimeOfDayNum(int num)
	{
		time_t tm = (time_t)num * ONEDAY_SECONDS - SECONDS_OFFSET;
		return tm;
	}

	int TimeUtil::GetNumContainedT(time_t start, time_t end, int t)
	{
		if (t < 0 || t>23)
			return 0;

		if (start > end || start < 0 || end < 0)
			return 0;

		time_t start_top = GetDayStart(start);
		time_t end_top = GetDayStart(end);

		int res = (int)(end_top - start_top) / ONEDAY_SECONDS + 1;

		struct tm _tm_start = safe_localtime(start);
		tm* tm_start = &_tm_start;
		if (tm_start && tm_start->tm_hour >= t)
			res--;

		struct tm _tm_end = safe_localtime(end);
		tm* tm_end = &_tm_end;
		if (tm_end && tm_end->tm_hour < t)
			res--;

		return res;
	}

	time_t TimeUtil::TimeEarlierThanT(time_t t, int h)
	{
		struct tm _tt = safe_localtime(t);
		tm* tt = &_tt;
		if (tt->tm_hour >= h)
		{
			tt->tm_hour = h;
			tt->tm_min = 0;
			tt->tm_sec = 0;
			return mktime(tt);
		}
		else
		{
			time_t start_of_day_before_t = GetDayStart(t) - ONEDAY_SECONDS;
			return start_of_day_before_t + h * ONE_HOUR_SECONDS;
		}
	}

	//unsigned int TimeUtil::GetTickDiff(tick_t curr, tick_t base)
	//{
	//    return curr - base;
	//}

	int TimeUtil::GetHour(time_t time)
	{
		tm local = safe_localtime(time);
		return local.tm_hour;
	}
	int TimeUtil::GetMinute(time_t time)
	{
		tm local = safe_localtime(time);
		return local.tm_min;
	}
	const char* TimeUtil::TimeToDateStr(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[16];

		safe_sprintf(ti, "%d-%02d-%02d",
			local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
		return ti;
	}
	const char* TimeUtil::TimeToDateHourStr(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[16];

		safe_sprintf(ti, "%d-%02d-%02d_%02d",
			local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, local.tm_hour);
		return ti;
	}

	const char* TimeUtil::TimeToDateStr2(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[16];

		safe_sprintf(ti, "%d年%02d月%02d日",
			local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
		return ti;
	}

	const char* TimeUtil::TimeToDateStr3(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[32];

		safe_sprintf(ti, "%d年%02d月%02d日%02d时%02d分",
			local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min);
		return ti;
	}

	const char* TimeUtil::TimeToTimeStr(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[16];

		safe_sprintf(ti, "%02d:%02d:%02d",
			local.tm_hour, local.tm_min, local.tm_sec);
		return ti;
	}

	const char* TimeUtil::TimeToHourMin(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[8];

		safe_sprintf(ti, "%02d:%02d",
			local.tm_hour, local.tm_min);
		return ti;
	}

	int TimeUtil::GetMonthDayCountByMonthIndex(int year, int month_index)
	{
		static int monthDay1[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		static int monthDay2[] = { 31,29,31,30,31,30,31,31,30,31,30,31 };//闰年

		if (month_index >= 0 && month_index <= 11)
		{
			if (isLeapYear(year))
			{
				return monthDay2[month_index];
			}
			else
			{
				return monthDay1[month_index];
			}
		}
		else
		{
			return 30;
		}
	}
	// int TimeUtil::GetTimeByWeekNum(int wk)
	// {
	// 	time_t sec = wk * 7 * 60 * 60 * 24;
	// 	tm temp_tm;
	// 	temp_tm.tm_year = 2014 - 1900;
	// 	temp_tm.tm_mon = 10 - 1;
	// 	temp_tm.tm_mday = 6;
	// 	temp_tm.tm_hour = temp_tm.tm_min = temp_tm.tm_sec = 0;
	// 	static time_t temp_t = mktime(&temp_tm);
	// 	return sec+temp_t;
	// 
	// }
	int TimeUtil::GetWeekNum(time_t t)
	{
		tm temp_tm;
		// 2014.10.6 第一周
		temp_tm.tm_year = 2014 - 1900;
		temp_tm.tm_mon = 10 - 1;
		temp_tm.tm_mday = 6;
		temp_tm.tm_hour = temp_tm.tm_min = temp_tm.tm_sec = 0;

		static time_t temp_t = mktime(&temp_tm);

		return (int)(GetWeekStart(t) - temp_t) / ONEWEEK_SECONDS + 1;
	}

	bool TimeUtil::IsSameCycle(time_t t1, time_t t2, int cycle)
	{
		if (cycle <= 0)
			return true;

		tm temp_tm;
		// 2015.3.14 0:0:0 基准时间
		temp_tm.tm_year = 2015 - 1900;
		temp_tm.tm_mon = 3 - 1;
		temp_tm.tm_mday = 14;
		temp_tm.tm_hour = temp_tm.tm_min = temp_tm.tm_sec = 0;//扩展偏移

		static time_t temp_ti = mktime(&temp_tm);

		int64 sec = cycle * ONEDAY_SECONDS;
		int64 delta = (t1 - temp_ti) / sec;
		if ((t1 - temp_ti) % sec > 0)
			delta++;

		int64 alpha = (t2 - temp_ti) / sec;
		if ((t2 - temp_ti) % sec > 0)
			alpha++;

		return delta == alpha ? true : false;
	}

	time_t TimeUtil::GetNextCycleStartTime(time_t t1, int cycle)
	{
		if (cycle <= 0)
			return t1;

		tm temp_tm;
		// 2015.3.14 0:0:0 基准时间
		temp_tm.tm_year = 2015 - 1900;
		temp_tm.tm_mon = 3 - 1;
		temp_tm.tm_mday = 14;
		temp_tm.tm_hour = temp_tm.tm_min = temp_tm.tm_sec = 0;//扩展偏移

		static time_t temp_te = mktime(&temp_tm);

		int64 sec = cycle * ONEDAY_SECONDS;
		int64 delta = (t1 - temp_te) / sec;
		if ((t1 - temp_te) % sec > 0)
			delta++;

		return (delta * sec + temp_te);
	}

	int TimeUtil::TimeToDateStamp(time_t t)
	{
		int year = TimeUtil::GetYear(t);
		int month = TimeUtil::GetMonthNum(t);
		return (year * 100 + month);
	}

	int TimeUtil::GetDaysInMonth(time_t t)
	{
		struct tm _tm_t = safe_localtime(t);
		tm* tm_t = &_tm_t;
		if (!tm_t)
			return 0;

		return tm_t->tm_mday;
	}

	bool TimeUtil::IsWeekEnd(time_t t1, time_t t2)
	{
		if (!IsSameWeek(t1, t2))
			return false;

		struct tm tm2 = safe_localtime(t2);

		if (tm2.tm_wday == 0 || tm2.tm_wday == 6)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const char* TimeUtil::TimeToDateStr4(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[16];

		safe_sprintf(ti, "%d.%02d.%02d",
			local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
		return ti;
	}

	const char* TimeUtil::TimeToDateStr5(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[16];

		safe_sprintf(ti, "%02d.%02d", local.tm_mon + 1, local.tm_mday);
		return ti;
	}

}

#endif //__SHARE_DATE_TIME_H__
