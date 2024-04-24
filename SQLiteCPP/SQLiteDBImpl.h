#pragma once
#include "SQLiteCpp.h"
#include <memory>
#include <vector>


class SQLiteDBImpl
{
public:
	explicit SQLiteDBImpl();
	virtual ~SQLiteDBImpl();
	SQLiteDBImpl(const SQLiteDBImpl &instance) = delete;
	SQLiteDBImpl &operator =(const SQLiteDBImpl &instance) = delete;
	/* @ brief: initial some params
	*  @ const std::string & dbFile - where is the .db file. such as: "c:/sqlite/demo.db"
	*  @ const int openType - 1:read_only, 2 - read_write, 4 - create, if this param cannot be found the definition, it will reset 1;
	*  @ const int busyTimeOut - busy time out, default is 0;
	*  @ const std::string & vfs - // std::string& outStrErr
	*  @ std::string& outStrErr - error msg. outStrErr will record the error information if initial database occur error
	*  @ return - int
			0 - success
			1 - dbFile is empty
			2 - dbFile is not exist
			3 - internal error, create _pdatabase object failed
			4,5 - initial error, check the param [outStrErr] to get error information
	*/
	int initV2(const std::string dbFile, std::string& outStrErr, const int openType = SQLite::OPEN_READONLY, const int busyTimeOut = 0, const std::string& vfs = "");

	// call sqlite_open 
	int init(const std::string& dbFile, std::string& outStrErr, const int openType = SQLite::OPEN_READONLY);

	/* @ brief: call this function to release resource before quitting
	*  @ return - int
			0 - success , default result is 0;
	*
	*/
	int uninit();


	/* @ brief: check if the table_me exists
	*  @ const std::string & tablName - table name
	*  @ return - int
			0 - table is contained in the database
			1 - false, the database doesnt have that table
			2 - failed, the param [tablName] is empty
			3 - failed, internal error（_pdatabase is nullptr）
	*
	*/
	int tableExist(const std::string& tablName);

	int tableColumnExists(const std::string& tablName, const std::string& columnName);



	/* @ brief: Get a single value result with an easy to use shortcut
	*  @ const std::string& strQuery - sql string to query, for example, "SELECT value FROM test WHERE id=2"
	*  @ std::string strResult - the query's result
	*  @ std::string outStrErr - error msg
	*  @ return - int
			0 - querry success
			1 - failed, the param [strQuery] is empty
			2 - failed, internal error(_pdatabase is nullptr)
	*
	*/
	int getSingleValue(const std::string& strQuery, std::string& strResult, std::string& outStrErr);


	/* @ brief: query the database's name of current operation
	*  @ std::string & strDBFileName - return the name of db file's name
	*  @ std::string & outStrErr - save error msg if sth gets wrong
	*  @ return - int
			0 - success
			1 - failed, internal error, the object of the db file is nullptr
	*/
	int getDBFileName(std::string& strDBFileName, std::string& outStrErr);



	// -------------------------------------------------------------------------------

	/* @ brief: the following function has the these functioms:  create and drop tables, insert and update a row.
					and it will not work with querying data
	*  @ const std::string & inStrDBSql - sql string
	*  @ std::string & outStrErr - exceptional string
	*  @ int& outResultRow - Return the number of rows modified by those SQL statements (INSERT, UPDATE or DELETE only)
	*  @ return - int
			0 - success
			-2 - failed, there is no database file from initialising
			-3 - falied, [inStrDBSql] is empty
			-4 - failed, internal error, the object to communicate with database file was created failed
	*/
	int excDBSql(const std::string& inStrDBSql, int& outResultRow, std::string& outStrErr);



	/* @ brief: query data only
	*  @ const std::string & inStrQuerySql - query string of sql
	*  @ std::string & outStrException - if an excpetion occurs, you could check the param to get error's detail
	*  @ return - int
			0 - success
	*/
	int initStatement(const std::string& inStrQuerySql, std::string& outStrException, const std::string& statementName = std::string("default"));


	/* @ brief: you must run the function [excQuerySql] , then you could call the following function ,
				it will get the next column after  returnning true;
				Note: if the object do not created, it also returns false
	*  @ return - bool
			true - youg could call [] to get column value
			false - it gets end, or dont initialise the db file and call the [excQuerySql] function
	*/
	bool statementExecQueryStep(const std::string& statementName = std::string("default"));

	/* @ brief: to query the data from the next column
	*  @ std::string & outStrErr - if an error occurs, you could check the param [outStrErr] to get details
	*  @ return - int
			0 - success
			1 - failed, dont initialise the db file and call the [excQuerySql] function firstly
			2 - failed, please to check the param outStrErr to get details
	*/
	int statementExecQueryStep(std::string& outStrErr, const std::string& statementName = std::string("default"));

	bool statementExec(const std::string& statementName = std::string("default"));


	int statementExec(std::string& outStrErr, const std::string& statementName = std::string("default"));

	/* @ brief: Reset the statement to make it ready for a new execution
	*  @ std::string & outStrErr - error msg
	*  @ return - int
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, please check [outStrErr] to get more details
	*/
	int statementReset(std::string& outStrErr, const std::string& statementName = std::string("default"));


	/* @ brief: get column's name
	*  @ const unsigned int & inColumnIndex - index of column
	*  @ std::string & outStrName - column's name
	*  @ std::string & outStrErr - error msg
	*  @ return - int
			0 - success
			1 - failed, it doesnt initialize db file
			2 - an error occurs, please check [outStrErr] to get more details
	*/
	int getTableColumnName(const unsigned int& inColumnIndex, std::string& outStrName, std::string& outStrErr, const std::string& statementName = std::string("default"));


	/* @ brief: get total rows of the table.
	*  @ unsigned int & out_total_row_count - the total row count
	*  @ std::string & outStrErr - error msg
	*  @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - an error occur, please check [outStrErr] to get more details
	*/
	int getTableColumnCount(unsigned int& outTotalColumnCount, std::string& outStrErr, const std::string& statementName = std::string("default"));

	/* @ brief: get name's index
	*  @ const std::string & name -
	*  @ unsigned int& outIndex - the result of index
	*  @ return - int
			0 - success
			1 - falied, it doesnt initialize db file
			2 - failed, name is empty
	*/
	int getIndex(const std::string& name, unsigned int& outIndex, const std::string& statementName = std::string("default"));


	///------------------------ bind start---------------------------------------------
	/* @ brief: Bind an int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
	*  @ const int index -
	*  @ const int value -
	*  std::string& outStrErre -
	*  @ return - int
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [outStrErr] to get more details
	*/
	template<typename BindIndexT>
	int bind(const int index, const BindIndexT value, std::string& outStrErr, const std::string& statementName = std::string("default"))
	{
		int retVal = 0;
		if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
		{
			retVal = 1;
			return retVal;
		}

		try
		{
			m_pStatementList.find(statementName)->second->bind(index, value);
		}
		catch (SQLite::Exception& ex)
		{
			outStrErr = std::string(ex.getErrorStr());
			retVal = 2;
		}

		return retVal;
	}
	// 	int bind(const int index, const int value, std::string& outStrErr);
	// 	int bind(const int index, const unsigned int value, std::string& outStrErr);
	// 	int bind(const int index, const long long value, std::string& outStrErr);
	// 	int bind(const int index, const double value, std::string& outStrErr);
	// 	int bind(const int index, const char* value, std::string& outStrErr);
	// 	int bind(const int index, const std::string& value, std::string& outStrErr);

	int bind(const int index, const void* value, uint32_t valueSize, std::string& outStrErr, const std::string& statementName = std::string("default"));
		/** 202
		* @brief Bind a string value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1).
		*
		* The string can contain null characters as it is binded using its size.
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [outStrErr] to get more details
		*/
		// int bindNoCopy(const int aIndex, const std::string&    aValue, std::string& outStrErr);


		/**  210
		* @brief Bind a text value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* Main usage is with null-terminated literal text (aka in code static strings)
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [outStrErr] to get more details
		*/
	int bindNoCopy(const int index, const std::string& value, std::string& outStrErr, const std::string& statementName = std::string("default"));



	/** blob 216
	* @brief Bind a binary blob value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
	*
	* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
	* @ return - int
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [outStrErr] to get more details
	*/
	// int bindNoCopy(const int aIndex, const void*           apValue, const int aSize, std::string& outStrErr);


	/** 222
	* @brief Bind a NULL value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
	*
	* @see clearBindings() to set all bound parameters to NULL.
	* @ return - int
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [outStrErr] to get more details
	*/
	int bind(const int index, std::string& outStrErr, const std::string& statementName = std::string("default"));



	/**   342
	* @brief Bind an int value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
	* @ return - int
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [outStrErr] to get more details
	*/

	template<typename BindNameT>
	int bind(const std::string& name, const BindNameT value, std::string& outStrErr, const std::string& statementName = std::string("default"))
	{
		int retVal = 0;

		// 1.m_pDatabase is nullptr
		if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
		{
			retVal = 1;
			return retVal;
		}

		try
		{
			m_pStatementList.find(statementName)->second->bind(name, value);
		}
		catch (SQLite::Exception& ex)
		{
			outStrErr = std::string(ex.getErrorStr());
			retVal = 2;
		}

		return retVal;
	}

	int bind(const std::string& name, const void* value, uint32_t valueSize, std::string& outStrErr, const std::string& statementName = std::string("default"));
	// 	int bind(const std::string& name, const int value, std::string& outStrErr);
	// 	int bind(const std::string& name, const unsigned int value, std::string& outStrErr);
	// 	int bind(const std::string& name, const long long value, std::string& outStrErr);
	// 	int bind(const std::string& name, const double value, std::string& outStrErr);
	// 	int bind(const std::string& name, const char* value, std::string& outStrErr);
	// 	int bind(const std::string& name, const std::string& value, std::string& outStrErr);


		/** 419
		* @brief Bind a string value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* The string can contain null characters as it is binded using its size.
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		*/
	int bindNoCopy(const std::string& name, const std::string& value, std::string& outStrErr, const std::string& statementName = std::string("default"));

	// 430 void bindNoCopy(const std::string& aName, const char*        apValue);

	// 439 void bindNoCopy(const std::string& aName, const void*        apValue, const int aSize)


	/** 448
	* @brief Bind a NULL value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
	*
	* @see clearBindings() to set all bound parameters to NULL.
	*/
	int bind(const std::string& name, std::string& outStrErr, const std::string& statementName = std::string("default")); // bind NULL value

	///------------------------ bind end---------------------------------------------


	/* @ brief: clear bindings
	*  @ std::string & outStrErr - error msg
	*  @ return - int
			0 - success
			1 - failed,  it doesnt initialize db file
			2 - failed, an error occurs, please check [outStrErr] to get more details
	*/
	int clearBindinds(std::string& outStrErr, const std::string& statementName = std::string("default"));


	/* @ brief: get the column's data
	*  @ const unsigned int inColumnIndex - which column do you wanna get . such 0, 1, 3, . it starts 0
	*  @ T & out_val - get column's data
	*  @ std::string & outStrErr - if an error occurs, you could check the param to get details
	*  @ return - int
			0 - success
			1 - failed, dont initialise the db file and call the [excQuerySql] function firstly
			2 - failed, an error occured, please check the [outStrErr] to check the details
			3 - failed, it cannot work with this type of T
	*/
	template<class GetColumIndexT>
	int getColumn(const int index, GetColumIndexT& value, std::string& outStrErr, const std::string& statementName = std::string("default"))
	{
		int retVal = 0;

		static int xx_int = 0;
		static double xx_double = 0.0;
		static unsigned int xx_uint = 0;
		static __int64 xx_int64 = 0;

		if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
		{
			retVal = 1;
			return retVal;
		}
		try
		{
			auto val = m_pStatementList.find(statementName)->second->getColumn(index);
			if (isSameType(value, xx_int))
			{
				value = val.getInt();
			}
			else if (isSameType(value, xx_uint))
			{
				value = val.getUInt();
			}
			else if (isSameType(value, xx_int64))
			{
				value = val.getInt64();
			}
			else if (isSameType(value, xx_double))
			{
				value = val.getDouble();
			}
			else
			{
				retVal = 3;
			}
		}
		catch (SQLite::Exception *e)
		{
			outStrErr = e->getErrorStr();
			retVal = 2;
		}

		return retVal;
	}
	// 	int getColumn(const int index, int& value, std::string& outStrErr);
	// 	int getColumn(const int index, unsigned int& value, std::string& outStrErr);
	// 	int getColumn(const int index, long long& value, std::string& outStrErr);
	// 	int getColumn(const int index, double &value, std::string& outStrErr);
	// 	int getColumn(const int index, std::string& value, std::string& outStrErr);
	int getColumnString(const int index, std::string& value, std::string& outStrErr, const std::string& statementName = std::string("default"));

	//按照索引获取blob数据
	int getColumnBlob(const int index, const void*& value, int& blobSize, std::string& outStrErr, const std::string& statementName = std::string("default"));



	template <typename T3, typename U>
	bool isSameType(T3, U)
	{
		return std::is_same<typename std::decay<T3>::type, U>::value;
	}

	template<typename GetColumNameT>
	int getColumn(const char* name, GetColumNameT& value, std::string& outStrErr, const std::string& statementName = std::string("default"))
	{
		int retVal = 0;

		static int xx_int = 0;
		static double xx_double = 0.0;
		static unsigned int xx_uint = 0;
		static __int64 xx_int64 = 0;

		if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
		{
			retVal = 1;
			return retVal;
		}
		try
		{
			auto val = m_pStatementList.find(statementName)->second->getColumn(name);
			if (isSameType<GetColumNameT, int>(value, xx_int))
			{
				value = val.getInt();
			}
			else if (isSameType(value, xx_uint))
			{
				value = val.getUInt();
			}
			else if (isSameType(value, xx_int64))
			{
				value = val.getInt64();
			}
			else if (isSameType(value, xx_double))
			{
				value = val.getDouble();
			}
			// 			else if (isSameType(value, xx_string))
			// 			{
			// 				std::string value = val.getString();
			// 			}
			else
			{
				retVal = 3;
			}
		}
		catch (SQLite::Exception *e)
		{
			outStrErr = e->getErrorStr();
			retVal = 2;
		}

		return retVal;
	}
	// 	int getColumn(const char* name, int& value, std::string& outStrErr);
	// 	int getColumn(const char* name, unsigned int& value, std::string& outStrErr);
	// 	int getColumn(const char* name, long long& value, std::string& outStrErr);
	// 	int getColumn(const char* name, double &value, std::string& outStrErr);
	int getColumnString(const char* name, std::string& value, std::string& outStrErr, const std::string& statementName = std::string("default"));
	
	//按照索引获取blob数据
	int getColumnBlob(const char* name, const void*& value, int& blobSize, std::string& outStrErr, const std::string& statementName = std::string("default"));



		// -------------------------------------------------------------------------------
		//
		// transaction
		//
		// -------------------------------------------------------------------------------
		/* @ brief: Commit the transaction
		*  @ std::string & outStrErr - if an error occurs, check this param to get more details
		*  @ return - int
				0 - success
				1 - failed, do not initialize database file
				2 - failed, an error occurs, please check the [outStrErr] to get more details
		*/
	int transactionCommit(std::string& outStrErr);

	int transactionBegin(std::string& outStrErr);


	bool isExistStatement(const char* name);
	int addStatement(const char* name);
	int removeStatement(const char* name);
	int getAllStatement(std::vector<std::string> & statementList);
private:
	/* @ brief: initialise the internal pointers
	*  @ return - void

	*/
	void initPtr();


private:
	/* @ brief: get column count
	*  @ return - int
			0 - success, it gets columns
			1 - failed, it doesnt initialise
			2 - failed, it doesnt have columns
	*/
	//int preQuery(const unsigned int& columnIndex);

private:

	std::unique_ptr<SQLite::Database >		m_pDatabase = nullptr;
	std::map<std::string, SQLite::Statement *> m_pStatementList;
	//std::unique_ptr<SQLite::Statement>		m_pStatement = nullptr;
	std::unique_ptr<SQLite::Transaction>	m_pTransaction = nullptr;
};

//#include "SQLiteDBImpl.impl"
