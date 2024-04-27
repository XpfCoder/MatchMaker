#pragma once

#ifndef __SHARE_DATE_TIME_H__
#define __SHARE_DATE_TIME_H__

#include "LogUtility.h"
#include <stdio.h>
//#include "tick.h"

namespace  TimeUtil
{
	const static int ONEWEEK_SECONDS	= 604800;
    const static int ONEDAY_SECONDS     = 86400;	///< 1�������
    const static int ONE_HOUR_SECONDS   = 60 * 60;
    const static int ONE_MINUTE_SECONDS = 60;    
    
    const static time_t MIN_TIME_T_VALUE = time_t(0);       ///��Чʱ��ֵ
    const static time_t MAX_TIME_T_VALUE = 0x7FFFFFFFL;     ///��Чʱ��ֵ  
	const static time_t MAX_TIME_T_VALUE_1 = 0x7FFFFFFFL-ONEDAY_SECONDS; ///��Чʱ��ֵ   
    
	//ʱ������Ӧ��ʱ��
	const static int WEEKDAY_AT_ZERO_TIME = 4;
	const static int HOUR_AT_ZERO_TIME = 8;	
	const static int MIN_AT_ZERO_TIME = 0;
	const static int SEC_AT_ZERO_TIME = 0;
	
	//ʱ�������뵱��0�������
	const static int SECONDS_OFFSET = HOUR_AT_ZERO_TIME * ONE_HOUR_SECONDS;

	/**
	 @brief ���ַ����ж�ȡ���ڣ����ڸ�ʽΪYYYY-MM-DD���ڲ�����֤�Ϸ���
	 @param src Դ�ַ���
	 @param out_tm ��ȡ�����ڣ�����tm_hour��tm_min��tm_secΪ0��tm_yday��tm_wday�Ѿ���mktime��ֵ
	 @param out_t ��ȡ������
	 @return �Ƿ�ɹ�
	 */
	inline bool ReadDateStr(const char* src, tm* out_tm, time_t* out_t);
	/**
	 @brief ���ַ����ж�ȡʱ�䣬ʱ���ʽΪHH:MM:SS������HH�Ǳ���ģ�:MM��:SS��ѡ���ڲ�����֤�Ϸ���
	 @param str Դ�ַ���
	 @param out_tm ��ȡ��ʱ�䣬ֻ��tm_hour��tm_min��tm_sec��Ч��
	 @param out_t ��ȡ��ʱ���Ӧ������
	 @return �Ƿ�ɹ�
	 */
	inline bool ReadTimeStr(const char* src, tm* out_tm, time_t* out_t);
	/**
	 @brief ���ַ����ж�ȡ���ڣ����ڸ�ʽΪYYYY-MM-DD HH:MM:SS���ڲ�����֤�Ϸ���
	 @param src Դ�ַ���
	 @param out_tm ��ȡ��ʱ��
	 @param out_t ��ȡ��ʱ��
	 @return �Ƿ�ɹ�
	 */
	inline bool ReadDateTimeStr(const char* src, tm* out_tm, time_t* out_t);

	/**
	 @brief ��time_tת��Ϊmysql��ʱ���ַ�����ʽ��YY-MM-DD HH:MM:SS
	 @param t Դʱ��
	 @return inline�ַ���
	 @deprecated
	 */
	inline const char* TimeToSqlStr(time_t t);

	/**
	 @brief ��time_tת��Ϊmysql��ʱ���ַ�����ʽ��YY-MM-DD HH:MM:SS
	 @param t Դʱ��
	 @return 
	 */
	inline std::string TimeToSqlStr_s(const time_t& t);

	/**
	 @brief ���ص�ǰʱ���mysql��ʱ���ַ�����ʽ��YY-MM-DD HH:MM:SS
	 @return inline �ַ���
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
	 *	YYYY��MM��DD��
	 */
	inline const char* TimeToDateStr2( time_t t );

	/*
	 *	YYYY��MM��DD��HHʱMM��
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
	 @brief	��ѯָ��ʱ���������ڵ�0��
	 @param	t Դʱ��
	 @param	out_tm ����0�㣬����hour��min��sec�Ѿ�����Ϊ0
	 @param out_t ����0��
	 @return true�ɹ���falseʧ��
	 */
	inline bool GetDayStart(time_t t, tm* out_tm, time_t* out_t);
	/**
	 @brief ����ָ��ʱ�������·�1�ŵ�0��
	 @param 
	 @param 
	 @param 
	 @return 
	 */
	inline time_t GetMonthStart(time_t t);

	//�õ�����µĵڼ���
	inline int GetDayIndexInMonth(time_t t);
	// 
	inline int GetYear(time_t t);
	inline int GetMonthNum(time_t t);
	inline time_t GetLastMonthStart(time_t t);
	inline time_t GetNextMonthStart(time_t t);
    /******************************************************************* 
     * @brief:	  ����ָ��ʱ���������ڵ�0��
     * @function: GetDayStart
     * @access:   public inline 
     * @return:   time_t - ��������Чʱ�䣬�򷵻�MAX_TIME_T_VALUE
     * @param: 	  time_t t
     *
     ******************************************************************/
    inline time_t GetDayStart(time_t t);

	 /******************************************************************* 
     * @brief:	  ����ָ��ʱ���������ڵ�23:59:59��
     * @function: GetDayEnd
     * @access:   public inline 
     * @return:   time_t - ��������Чʱ�䣬�򷵻�MAX_TIME_T_VALUE
     * @param: 	  time_t t
     *
     ******************************************************************/
    inline time_t GetDayEnd(time_t t);

    /******************************************************************* 
     * @brief:	  �ж�����ʱ���Ƿ���ͬһ��
     * @function: IsSameDay
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t1
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsSameDay(time_t t1, time_t t2);
	
    /******************************************************************* 
     * @brief:	  �ж�����ʱ���Ƿ���ͬһСʱ
     * @function: IsSameHour
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t1
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsSameHour(time_t t1, time_t t2);
    /******************************************************************* 
     * @brief:	  �ж�����ʱ���Ƿ���ͬһ��
     * @function: IsSameWeek
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t2
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsSameWeek(time_t t1, time_t t2);

	 /******************************************************************* 
     * @brief:	  �ж�����ʱ���Ƿ���ͬһ�� (����Ƚ����⣬�ǳ���ͬһ�ܣ�����ƫ��ʱ��)
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
     * @brief:	  �ж�����ʱ���Ƿ���ͬһ��
     * @function: IsSameMonth
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t2
     * @param: 	  time_t t2
     *
     ******************************************************************/
    inline bool IsSameMonth(time_t t1, time_t t2);

    /******************************************************************* 
     * @brief:	  ����HH:MM��ʽ�ַ���
     * @function: ToHourMinuteStr
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t
     * @param: 	  std::string & str
     *
     ******************************************************************/
    inline bool ToHourMinuteStr(time_t t, std::string &str);

	/******************************************************************* 
	* @brief:	  ����ĳʱ���ǵڼ��죨1970.1.1�ǵ�0�죩
	* @function:  GetDayNum
	* @access:   public inline 
	* @return:   int
	* @param: 	  time_t t

	*
	******************************************************************/
	inline int GetDayNum(time_t t);
	inline time_t GetTimeOfDayNum(int num);

	/******************************************************************* 
	* @brief:	  ��start��end�������˼���t����(t=[0,1,2,...,23])
	* @function:  GetDayNum
	* @access:   public inline 
	* @return:   int
	* @param: 		time_t	start	��ʼʱ��
	* @param:		time_t	end		����ʱ��
	* @param:		int		t		����ʱ�̣�[0,1,2, ... ,23]
	
	*
	******************************************************************/
	inline int GetNumContainedT(time_t start,time_t end,int t);

	/******************************************************************* 
	* @brief:	  ��������һ��ʱ�̣���t���������t�����Ҵ�������h (h=[0,1,2,...,23])
	* @function:  TimeEarlierThanT
	* @access:   public inline 
	* @return:   int
	* @param: 		time_t	t		
	* @param:		int		h		

	*
	******************************************************************/
	inline time_t TimeEarlierThanT(time_t t,int h);

	/***************************************
	 *@brief   GetHour ��ȡʱ���Сʱֵ [0,1....23]
	 *@return int
	 *@param time_t time
	****************************************/
	inline int GetHour(time_t time);

	/*************************************
	*@brief GetMinute ���ʱ��[0,59]
	*@return int
	*@param time_t time
	**************************************/
	inline int GetMinute(time_t time);

	/***************************************
	 *@brief   GetMonthDayCountByMonthIndex ͨ���·ݵõ�ʱ�� [0,11]
	 *@return int
	 *@param time_t time
	****************************************/
	inline int GetMonthDayCountByMonthIndex(int year,int month_index);

	inline time_t GetWeekStart(time_t t);

	inline int GetWeekIndex(time_t t);

	inline struct tm GettmInfoByWeek(int week_num);

	inline int GetWeekNum(time_t t);

	/******************************************************************* 
     * @brief:	  �ж�����ʱ���Ƿ���ͬһ������
     * @function: IsSameCycle
     * @access:   public inline 
     * @return:   bool
     * @param: 	  time_t t1 ����ʱ��
     * @param: 	  time_t t2 �ϴμ�¼ʱ��
	 * @param:    int cycle(���ڵ�λ"��")
     *
     ******************************************************************/
    inline bool IsSameCycle(time_t t1, time_t t2, int cycle);
	/******************************************************************* 
     * @brief:	  ��ȡ��һ���ڿ�ʼʱ��
     * @function: GetNextCycleStartTime
     * @access:   public inline 
     * @return:   time_t
     * @param: 	  time_t t1 ����ʱ��
	 * @param:    int cycle(���ڵ�λ"��")
     *
     ******************************************************************/
	inline time_t GetNextCycleStartTime(time_t t1, int cycle);
	/******************************************************************* 
     * @brief:	  ��ȡ��ǰ�Զ���ʱ��� ��ʽ��YYYYMM
     * @function: TimeToDateStamp
     * @access:   public inline 
     * @return:   int
     * @param: 	  time_t t ����ʱ��
     *
     ******************************************************************/
	inline int TimeToDateStamp(time_t t);

	/******************************************************************* 
     * @brief:	  ��ȡ��ǰʱ���ǵ��¼���
     * @function: GetDaysInMonth
     * @access:   public inline 
     * @return:   int
     * @param: 	  time_t t ʱ��
     *
     ******************************************************************/
	inline int GetDaysInMonth(time_t t);

	    /******************************************************************* 
     * @brief:	  �ж�t2�Ƿ���t1�����ܵ�������������
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
			return "1970-01-01 08:00:00";	///< ����ʱ��
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
			return "1970-01-01 08:00:00";	///< ����ʱ��
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
			safe_sprintf(timestr, "1970-01-01 08:00:00");///< ����ʱ��
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

		//������ĩ
		tm1.tm_wday = (0 == tm1.tm_wday) ? 7 : tm1.tm_wday;

		//��һ�賿�����ڵ�ʱ���ֵ
		time_t differ = (tm1.tm_wday - 1) * ONEDAY_SECONDS +
			tm1.tm_hour * ONE_HOUR_SECONDS +
			tm1.tm_min * ONE_MINUTE_SECONDS +
			tm1.tm_sec;

		time_t weekbegin = t1 - differ;             //��ʱ�����ÿ�ܵĿ�ʼ����һ����0�㣩

		return((t2 >= weekbegin) && (t2 <= t1));
	}
	bool TimeUtil::IsSameWeek(time_t t1, time_t t2, int offset_days)
	{
		if (t1 < t2)
		{
			std::swap(t1, t2);
		}

		struct tm tm1 = safe_localtime(t1);

		//������ĩ


		tm1.tm_wday = (0 == tm1.tm_wday) ? 7 : tm1.tm_wday;
		tm1.tm_wday += offset_days;
		if (tm1.tm_wday > 7)
		{
			tm1.tm_wday = tm1.tm_wday - 7;
		}

		//��һ�賿�����ڵ�ʱ���ֵ
		time_t differ = (tm1.tm_wday - 1) * ONEDAY_SECONDS +
			tm1.tm_hour * ONE_HOUR_SECONDS +
			tm1.tm_min * ONE_MINUTE_SECONDS +
			tm1.tm_sec;

		time_t weekbegin = t1 - differ;             //��ʱ�����ÿ�ܵĿ�ʼ����һ����0�㣩

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

		//������ĩ
		tm1.tm_wday = (0 == tm1.tm_wday) ? 7 : tm1.tm_wday;

		//��һ�賿�����ڵ�ʱ���ֵ
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
		//������ĩ
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

		safe_sprintf(ti, "%d��%02d��%02d��",
			local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
		return ti;
	}

	const char* TimeUtil::TimeToDateStr3(time_t t)
	{
		tm local = safe_localtime(t);

		static char ti[32];

		safe_sprintf(ti, "%d��%02d��%02d��%02dʱ%02d��",
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
		static int monthDay2[] = { 31,29,31,30,31,30,31,31,30,31,30,31 };//����

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
		// 2014.10.6 ��һ��
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
		// 2015.3.14 0:0:0 ��׼ʱ��
		temp_tm.tm_year = 2015 - 1900;
		temp_tm.tm_mon = 3 - 1;
		temp_tm.tm_mday = 14;
		temp_tm.tm_hour = temp_tm.tm_min = temp_tm.tm_sec = 0;//��չƫ��

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
		// 2015.3.14 0:0:0 ��׼ʱ��
		temp_tm.tm_year = 2015 - 1900;
		temp_tm.tm_mon = 3 - 1;
		temp_tm.tm_mday = 14;
		temp_tm.tm_hour = temp_tm.tm_min = temp_tm.tm_sec = 0;//��չƫ��

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
