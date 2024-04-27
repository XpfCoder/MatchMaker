/*********************************************************************
	File name:	UMConverter.h
	
	
	Purpose:	
*********************************************************************/
#include "../MatchMaker/securec.h"

#ifndef __UNICODE_MBCS_CONVERTER_H__
#define __UNICODE_MBCS_CONVERTER_H__

/*
#t ���ֽڱ�����գ������

star_clʹ�ãգ��������룬��ĳЩ����£���Ҫ�������ֱ����ַ�����ת����UMConverterʵ������һ���ܡ�
��_T��ʾ���ţ�������������ö������������ε��ַ��������ֱ��뷽ʽ��
L��ʾ�������ε��ַ����ǣգ��������뷽ʽ��
����star_clʹ�ãգ��������룬����ַ���ʹ��_T��ʾ��
����ʹ��_tstring��_tchar��ʾĬ�ϱ�����ʽ���ַ����Լ��ַ���ʹ��std::string��char��ʾ��ȷ�Ķ��ַ����롣

*/

#include <windows.h>
#include <wchar.h>


#if (defined UNICODE) || (defined _UNICODE)
	#define STR_ASSIGNMENT(dest, source) do{UMConverter con(source);dest=con.GetUnicode();}while(false)
#else
	#define STR_ASSIGNMENT(dest, source) dest = source
#endif

#if (defined UNICODE) || (defined _UNICODE)
	#define STR_ASSIGNMENT_UTF8(dest, source) do{UUtf8Converter con(source);dest=con.GetUnicode();}while(false)
#else
	#define STR_ASSIGNMENT_UTF8(dest, source) dest = source
#endif

#define  GB18030 54936
// unicode��multibyte��ת����
class UMConverter
{
protected:
	bool		_IsMBCS;
	char*		_MBCSString;
	wchar_t*	_UnicodeString;

public:
	// �����������͵��ַ����Ĺ��캯��
	UMConverter( const wchar_t *str );
	UMConverter( const char *str);

	~UMConverter( void );

	// ���multibyte���ַ���
	const char *GetMBCS( void ) const { return( _MBCSString ); };
	// ���unicode���ַ���
	const wchar_t *GetUnicode( void ) const { return( _UnicodeString ); };

	static const char* GetMBFromUTF8(const char* utf8){ return utf8; };

protected:
	UMConverter( void );
private:
	// disable
	UMConverter(const UMConverter&);
	const UMConverter& operator=(const UMConverter&);
};

class UUtf8Converter
{
protected:
	bool		_IsUtf8;
	char*		_Utf8String;
	wchar_t*	_UnicodeString;

public:
	// �����������͵��ַ����Ĺ��캯��
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	UUtf8Converter( const wchar_t *str,bool realConvert=false );//true:������SUPPORT_UTF8_MODE����ת����ʽ��
	UUtf8Converter( const char *str,bool realConvert=false);//true:��utf-8��ʽ xml�ļ�ʱ��������SUPPORT_UTF8_MODE����ת����ʽ��
#else
	UUtf8Converter( const char *str );
	UUtf8Converter( const wchar_t *str );
#endif
	~UUtf8Converter( void );

	// ���multibyte���ַ���
	const char *GetUtf8( void ) const { return( _Utf8String ); };
	// ���unicode���ַ���
	const wchar_t *GetUnicode( void ) const { return( _UnicodeString ); };

protected:
	UUtf8Converter( void );
private:
	// disable
	UUtf8Converter(const UUtf8Converter&);
	const UUtf8Converter& operator=(const UUtf8Converter&);
};


class MUtf8Converter
{
protected:
	bool			_IsUtf8;
	char*		_MBCSString;
	char*		_Utf8String;	

public:
	// �����������͵��ַ����Ĺ��캯��
	MUtf8Converter( const char *str, bool isUtf8 = true);
	~MUtf8Converter( void );

	// ���multibyte���ַ���
	const char* GetUtf8( void ) const { return( _Utf8String ); };
	// ���unicode���ַ���
	const char* GetMBCS( void ) const { return( _MBCSString ); };

protected:
	MUtf8Converter( void );
private:
	// disable
	MUtf8Converter(const MUtf8Converter&);
	const MUtf8Converter& operator=(const MUtf8Converter&);
};

class MUtf8RealConverter
{
protected:
	bool			_IsUtf8;
	char*		_MBCSString;
	char*		_Utf8String;	

public:
	// �����������͵��ַ����Ĺ��캯��
	MUtf8RealConverter( const char *str, bool isUtf8 = true);
	~MUtf8RealConverter( void );

	// ���multibyte���ַ���
	const char* GetUtf8( void ) const { return( _Utf8String ); };
	// ���unicode���ַ���
	const char* GetMBCS( void ) const { return( _MBCSString ); };

protected:
	MUtf8RealConverter( void );
private:
	// disable
	MUtf8RealConverter(const MUtf8RealConverter&);
	const MUtf8RealConverter& operator=(const MUtf8RealConverter&);
};
typedef MUtf8RealConverter AlwaysUUtf8Converter;


class QQMusicUtf8Converter
{
protected:
	bool		_IsUtf8;
	char*		_Utf8String;
	wchar_t*	_UnicodeString;

public:
	// �����������͵��ַ����Ĺ��캯��
	QQMusicUtf8Converter( const wchar_t *str);
	QQMusicUtf8Converter( const char *str);
	~QQMusicUtf8Converter( void );

	// ���utf8���ַ���
	const char *GetUtf8( void ) const { return( _Utf8String ); };
	// ���unicode���ַ���
	const wchar_t *GetUnicode( void ) const { return( _UnicodeString ); };

protected:
	QQMusicUtf8Converter( void );
private:
	// disable
	QQMusicUtf8Converter(const QQMusicUtf8Converter&);
	const QQMusicUtf8Converter& operator=(const QQMusicUtf8Converter&);
};



#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#pragma warning(disable:4267)
inline UMConverter::UMConverter( const char *str )
: _IsMBCS( true )
, _UnicodeString( NULL )
, _MBCSString(NULL)
{
	if ( str == NULL )
	{
		_MBCSString = NULL;
		return;
	}
	int len = ::MultiByteToWideChar( GB18030, 0, str, strlen( str ) + 1,	NULL, 0 );
	_UnicodeString = new wchar_t[ len ];
	::MultiByteToWideChar( GB18030, 0, str, strlen( str ) + 1, _UnicodeString, len );

	_MBCSString = const_cast< char * >( str );
}

inline UMConverter::UMConverter( const wchar_t *str )
: _IsMBCS( false )
, _MBCSString( NULL )
{
	if ( str == NULL )
	{
		_UnicodeString = NULL;
		return;
	}
	int len = ::WideCharToMultiByte( GB18030, 0, str, wcslen( str ) + 1,	NULL, 0, NULL, NULL );
	_MBCSString = new char[ len ];
	::WideCharToMultiByte( GB18030, 0, str, wcslen( str ) + 1, _MBCSString, len, NULL, NULL );

	_UnicodeString = const_cast< wchar_t * >( str );
}

inline UMConverter::~UMConverter( void )
{
	if ( _IsMBCS )
	{
		if ( _UnicodeString != NULL )
		{
			delete []_UnicodeString;
			_UnicodeString = NULL;
		}
	}
	else
	{
		if ( _MBCSString != NULL )
		{
			delete []_MBCSString;
			_MBCSString = NULL;
		}
	}
}



#else//this is need for compile. 

inline UMConverter::UMConverter( const char *str )
: _IsMBCS( true )
, _UnicodeString( NULL )
{
	if(str == 0)
		return;

	_MBCSString = new char[strlen(str)+1];
	_MBCSString[strlen(str)] = 0;
	strcpy(_MBCSString, str);
}

inline UMConverter::UMConverter( const wchar_t *str )
: _IsMBCS( false )
, _MBCSString( NULL )
{
//	if(str == 0)
//		return;
//
//	_UnicodeString = new wchar_t[strlen(str)+1];
//	_UnicodeString[strlen(str)] = 0;
//	strcpy(_UnicodeString, str);
}

inline UMConverter::~UMConverter( void )
{
	if ( _IsMBCS )
	{
		if ( _MBCSString != NULL )
		{
			delete []_MBCSString;
			_MBCSString = NULL;
		}
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)


inline UUtf8Converter::UUtf8Converter( const char *str ,bool realConvert)
: _IsUtf8( true )
, _UnicodeString( NULL )
, _Utf8String(NULL)
{
	int len = 0;
	if ( str == NULL )
	{
		_Utf8String = NULL;
		return;
	}
	if(realConvert)
	{
		len = ::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, NULL, 0 );	
	}
	else
	{
	#ifdef SUPPORT_UTF8_MODE
		len = ::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, NULL, 0 );
	#else
		len = ::MultiByteToWideChar( GB18030, 0, str, strlen( str ) + 1, NULL, 0 );
	#endif
	}
	_UnicodeString = new wchar_t[ len ];

	if(realConvert)
	{
		::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, _UnicodeString, len );
	}
	else
	{
	#ifdef SUPPORT_UTF8_MODE
		::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, _UnicodeString, len );
	#else
		::MultiByteToWideChar( GB18030, 0, str, strlen( str ) + 1, _UnicodeString, len );
	#endif
	}

	_Utf8String = const_cast< char * >( str );
}

inline UUtf8Converter::UUtf8Converter( const wchar_t *str ,bool realConvert)
: _IsUtf8( false )
, _Utf8String( NULL )
{
	int len = 0;
	if ( str == NULL )
	{
		_UnicodeString = NULL;
		return;
	}
	if(realConvert)
	{
		len = ::WideCharToMultiByte( CP_UTF8, 0, str, wcslen( str ) + 1,	NULL, 0, NULL, NULL );
	}
	else
	{
	#ifdef SUPPORT_UTF8_MODE
		len = ::WideCharToMultiByte( CP_UTF8, 0, str, wcslen( str ) + 1,	NULL, 0, NULL, NULL );
	#else
		len = ::WideCharToMultiByte( GB18030, 0, str, wcslen( str ) + 1,	NULL, 0, NULL, NULL );
	#endif
	}
	_Utf8String = new char[ len ];

	if(realConvert)
	{
		::WideCharToMultiByte( CP_UTF8, 0, str, wcslen( str ) + 1, _Utf8String, len, NULL, NULL );
	}
	else
	{
	#ifdef SUPPORT_UTF8_MODE
		::WideCharToMultiByte( CP_UTF8, 0, str, wcslen( str ) + 1, _Utf8String, len, NULL, NULL );
	#else
		::WideCharToMultiByte( GB18030, 0, str, wcslen( str ) + 1, _Utf8String, len, NULL, NULL );
	#endif
	}
	_UnicodeString = const_cast< wchar_t * >( str );
}

inline UUtf8Converter::~UUtf8Converter( void )
{
	if ( _IsUtf8 )
	{
		if ( _UnicodeString != NULL )
		{
			delete []_UnicodeString;
			_UnicodeString = NULL;
		}
	}
	else
	{
		if ( _Utf8String != NULL )
		{
			delete []_Utf8String;
			_Utf8String = NULL;
		}
	}
}



#else//this is need for compile.

inline UUtf8Converter::UUtf8Converter( const char *str )
: _IsUtf8( true )
, _UnicodeString( NULL )
{
	if(str == 0)
		return;

	_Utf8String = new char[strlen(str)+1];
	_Utf8String[strlen(str)] = 0;
	strcpy(_Utf8String, str);
}

inline UUtf8Converter::UUtf8Converter( const wchar_t *str )
: _IsUtf8( false )
, _Utf8String( NULL )
{
	//if(str == 0)
	//	return;

	//_UnicodeString = new wchar_t[strlen(str)+1];
	//_UnicodeString[strlen(str)] = 0;
	//strcpy(_UnicodeString, str);
}

inline UUtf8Converter::~UUtf8Converter( void )
{
	if ( _IsUtf8 )
	{
		if ( _Utf8String != NULL )
		{
			delete []_Utf8String;
			_Utf8String = NULL;
		}
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)


inline MUtf8Converter::MUtf8Converter( const char *str, bool isUtf8)
: _IsUtf8( isUtf8 )
, _MBCSString( const_cast< char * >( str ) )
, _Utf8String( const_cast< char * >( str ) )
{
	if ( str == NULL )
		return;

//#ifdef SUPPORT_UTF8_MODE
	if( _IsUtf8 )
	{		
		int len = ::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, NULL, 0 );
		wchar_t* unicodeString = new wchar_t[ len ];
		::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, unicodeString, len );

		len = ::WideCharToMultiByte( GB18030, 0, unicodeString, wcslen( unicodeString ) + 1,	NULL, 0, NULL, NULL );
		_MBCSString = new char[ len ];
		::WideCharToMultiByte( GB18030, 0, unicodeString, wcslen( unicodeString ) + 1, _MBCSString, len, NULL, NULL );

		delete[] unicodeString;
	}
	else
	{
		int len = ::MultiByteToWideChar( GB18030, 0, str, strlen( str ) + 1, NULL, 0 );
		wchar_t* unicodeString = new wchar_t[ len ];
		::MultiByteToWideChar( GB18030, 0, str, strlen( str ) + 1, unicodeString, len );

		len = ::WideCharToMultiByte( CP_UTF8, 0, unicodeString, wcslen( unicodeString ) + 1, NULL, 0, NULL, NULL );
		_Utf8String = new char[ len ];
		::WideCharToMultiByte( CP_UTF8, 0, unicodeString, wcslen( unicodeString ) + 1, _Utf8String, len, NULL, NULL );

		delete[] unicodeString;
	}
//#endif
}

inline MUtf8Converter::~MUtf8Converter( void )
{
//#ifdef SUPPORT_UTF8_MODE
	if ( _IsUtf8 )
	{
		if ( _MBCSString != NULL )
		{
			delete []_MBCSString;
			_MBCSString = NULL;
		}
	}
	else
	{
		if ( _Utf8String != NULL )
		{
			delete []_Utf8String;
			_Utf8String = NULL;
		}
	}
//#endif
}

inline MUtf8RealConverter::MUtf8RealConverter( const char *str, bool isUtf8)
: _IsUtf8( isUtf8 )
, _MBCSString( const_cast< char * >( str ) )
, _Utf8String( const_cast< char * >( str ) )
{
	if ( str == NULL )
		return;


	if( _IsUtf8 )
	{		
		int len = ::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, NULL, 0 );
		wchar_t* unicodeString = new wchar_t[ len ];
		::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, unicodeString, len );

		len = ::WideCharToMultiByte( GB18030, 0, unicodeString, wcslen( unicodeString ) + 1,	NULL, 0, NULL, NULL );
		_MBCSString = new char[ len ];
		::WideCharToMultiByte( GB18030, 0, unicodeString, wcslen( unicodeString ) + 1, _MBCSString, len, NULL, NULL );

		delete[] unicodeString;
	}
	else
	{
		int len = ::MultiByteToWideChar( GB18030, 0, str, strlen( str ) + 1, NULL, 0 );
		wchar_t* unicodeString = new wchar_t[ len ];
		::MultiByteToWideChar( GB18030, 0, str, strlen( str ) + 1, unicodeString, len );

		len = ::WideCharToMultiByte( CP_UTF8, 0, unicodeString, wcslen( unicodeString ) + 1, NULL, 0, NULL, NULL );
		_Utf8String = new char[ len ];
		::WideCharToMultiByte( CP_UTF8, 0, unicodeString, wcslen( unicodeString ) + 1, _Utf8String, len, NULL, NULL );

		delete[] unicodeString;
	}

}

inline MUtf8RealConverter::~MUtf8RealConverter( void )
{

	if ( _IsUtf8 )
	{
		if ( _MBCSString != NULL )
		{
			delete []_MBCSString;
			_MBCSString = NULL;
		}
	}
	else
	{
		if ( _Utf8String != NULL )
		{
			delete []_Utf8String;
			_Utf8String = NULL;
		}
	}

}

#else//this is need for compile.

#include <iconv.h>

inline MUtf8Converter::MUtf8Converter( const char *str, bool isUtf8 )
: _IsUtf8( isUtf8 )
, _MBCSString( const_cast< char * >( str ) )
, _Utf8String( const_cast< char * >( str ) )
{
	if(str == 0)
		return;
	
	if( _IsUtf8 )
	{
		size_t utf8_len = strlen(str);
		size_t gbk_len = utf8_len;
		
		_MBCSString = new char[gbk_len+1];
		//memset(_MBCSString, 0, gbk_len+1);
		memset_s(_MBCSString, gbk_len + 1, 0, gbk_len + 1);

		iconv_t cd = iconv_open("gb18030", "UTF-8//IGNORE");
		if(cd == (iconv_t)-1)
		{
			return;
		}

		char* str_in = _Utf8String;
		char* str_out = _MBCSString;

		if(iconv(cd, &str_in, &utf8_len, &str_out, &gbk_len) == -1)
		{
			str_out[0] = '\0';
		}
		iconv_close(cd);

	}
	else
	{
		size_t gbk_len = strlen(str);
		size_t utf8_len = gbk_len * 3;

		_Utf8String = new char[utf8_len+1];
		//memset(_Utf8String, 0, utf8_len+1);
		memset_s(_Utf8String, utf8_len + 1, 0, utf8_len + 1);

		iconv_t cd = iconv_open("UTF-8", "gb18030//IGNORE");
		if(cd == (iconv_t)-1)
		{
			return;
		}

		char* str_in = _MBCSString;
		char* str_out = _Utf8String;

		if(iconv(cd, &str_in, &gbk_len, &str_out, &utf8_len) == -1)
		{
			str_out[0] = '\0';
		}
		iconv_close(cd);

	}
}

inline MUtf8Converter::~MUtf8Converter( void )
{
	if ( _IsUtf8 )
	{
		if ( _MBCSString != NULL )
		{
			delete []_MBCSString;
			_MBCSString = NULL;
		}
	}
	else
	{
		if ( _Utf8String != NULL )
		{
			delete []_Utf8String;
			_Utf8String = NULL;
		}
	}
}


inline MUtf8RealConverter::MUtf8RealConverter( const char *str, bool isUtf8 )
: _IsUtf8( isUtf8 )
, _MBCSString( const_cast< char * >( str ) )
, _Utf8String( const_cast< char * >( str ) )
{
	if(str == 0)
		return;
	
	if( _IsUtf8 )
	{
		size_t utf8_len = strlen(str);
		size_t gbk_len = utf8_len;

		_MBCSString = new char[gbk_len+1];
		memset_s(_MBCSString, gbk_len + 1, 0, gbk_len+1);
		
		iconv_t cd = iconv_open("gb18030", "UTF-8//IGNORE");
		if(cd == (iconv_t)-1)
		{
			return;
		}

		char* str_in = _Utf8String;
		char* str_out = _MBCSString;

		if(iconv(cd, &str_in, &utf8_len, &str_out, &gbk_len) == -1)
		{
			str_out[0] = '\0';
		}
		iconv_close(cd);

	}
	else
	{
		size_t gbk_len = strlen(str);
		size_t utf8_len = gbk_len * 3;
		
		_Utf8String = new char[utf8_len+1];
		memset_s(_Utf8String, utf8_len + 1, 0, utf8_len+1);

		iconv_t cd = iconv_open("UTF-8", "gb18030//IGNORE");
		if(cd == (iconv_t)-1)
		{
			return;
		}

		char* str_in = _MBCSString;
		char* str_out = _Utf8String;

		if(iconv(cd, &str_in, &gbk_len, &str_out, &utf8_len) == -1)
		{
			str_out[0] = '\0';
		}
		iconv_close(cd);

	}
}

inline MUtf8RealConverter::~MUtf8RealConverter( void )
{
	if ( _IsUtf8 )
	{
		if ( _MBCSString != NULL )
		{
			delete []_MBCSString;
			_MBCSString = NULL;
		}
	}
	else
	{
		if ( _Utf8String != NULL )
		{
			delete []_Utf8String;
			_Utf8String = NULL;
		}
	}
}
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
inline QQMusicUtf8Converter::QQMusicUtf8Converter( const char *str )
: _IsUtf8( true )
, _UnicodeString( NULL )
, _Utf8String(NULL)
{
	int len = 0;
	if ( str == NULL )
	{
		_Utf8String = NULL;
		return;
	}

	len = ::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, NULL, 0 );	

	_UnicodeString = new wchar_t[ len ];

	::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, _UnicodeString, len );

	_Utf8String = const_cast< char * >( str );
}

inline QQMusicUtf8Converter::QQMusicUtf8Converter( const wchar_t *str)
: _IsUtf8( false )
, _Utf8String( NULL )
{
	int len = 0;
	if ( str == NULL )
	{
		_UnicodeString = NULL;
		return;
	}

	len = ::WideCharToMultiByte( CP_UTF8, 0, str, wcslen( str ) + 1,	NULL, 0, NULL, NULL );

	_Utf8String = new char[ len ];

	::WideCharToMultiByte( CP_UTF8, 0, str, wcslen( str ) + 1, _Utf8String, len, NULL, NULL );

	_UnicodeString = const_cast< wchar_t * >( str );
}

inline QQMusicUtf8Converter::~QQMusicUtf8Converter( void )
{
	if ( _IsUtf8 )
	{
		if ( _UnicodeString != NULL )
		{
			delete []_UnicodeString;
			_UnicodeString = NULL;
		}
	}
	else
	{
		if ( _Utf8String != NULL )
		{
			delete []_Utf8String;
			_Utf8String = NULL;
		}
	}
}



#else//this is need for compile.

inline QQMusicUtf8Converter::QQMusicUtf8Converter( const char *str )
: _IsUtf8( true )
, _UnicodeString( NULL )
{
	if(str == 0)
		return;

	_Utf8String = new char[strlen(str)+1];
	_Utf8String[strlen(str)] = 0;
	strcpy(_Utf8String, str);
}

inline QQMusicUtf8Converter::QQMusicUtf8Converter( const wchar_t *str )
: _IsUtf8( false )
, _Utf8String( NULL )
{
	//if(str == 0)
	//	return;

	//_UnicodeString = new wchar_t[strlen(str)+1];
	//_UnicodeString[strlen(str)] = 0;
	//strcpy(_UnicodeString, str);
}

inline QQMusicUtf8Converter::~QQMusicUtf8Converter( void )
{
	if ( _IsUtf8 )
	{
		if ( _Utf8String != NULL )
		{
			delete []_Utf8String;
			_Utf8String = NULL;
		}
	}
}

#endif


#endif // __H3D_CLIENT_UNICODE_MBCS_CONVERTER_H__
