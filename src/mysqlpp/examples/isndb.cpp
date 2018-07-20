#include "isndb.h"

ISNDB::ISNDB()
{
	try{
		con= mysqlpp::Connection("imreward", "localhost", "immysql", "im123456");
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		exit(-1);
	}
}

// select from ISNDB according to address
mysqlpp::StoreQueryResult ISNDB::ISNSqlSelectAA(string tablename, vector<string> field, string condition, string cvalue)
{
	// form the query sentence
	int fieldSize= field.size();

	try{

		mysqlpp::Query query= con.query();
		//according tablename in different way
		if(fieldSize == 1)
		{
			query<< "select %0"" from %1"" where %2""= %3q";
			query.parse();
			mysqlpp::StoreQueryResult dataTmp = query.store(field[0], tablename, condition, cvalue);
			return dataTmp;
		}
		else if(fieldSize == 2)
		{
			query<< "select %0"", %1"" from %2"" where %2""= %3q";
			query.parse();
			mysqlpp::StoreQueryResult dataTmp = query.store(field[0], field[1], tablename, condition, cvalue);
			return dataTmp;
		}
		else if(fieldSize == 3)
		{
			query<< "select %0"", %1"", %2"" from %3"" where address = %4q";
			query.parse();
			mysqlpp::StoreQueryResult dataTmp = query.store(field[0], field[1], field[2], tablename, condition, cvalue);
			return dataTmp;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		exit(-1);
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions; e.g. type mismatch populating 'stock'
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
		exit(-1);
	}

}

// update ISNDB with condition
mysqlpp::SimpleResult ISNDB::ISNSqlUpdate(string tablename, vector<string> field, vector<string> values, string condition)
{
	// form the query sentence
	if(field.size()!=values.size())
	{
		cout<< "Error in update " << endl;
		exit(-1);
	}

	int fieldSize= field.size();

	try{

		mysqlpp::Query query= con.query();
		//according tablename in different way
		if(fieldSize == 1)
		{
			query<< "update %0"" set %1""= %2q where address = %3q";
			query.parse();
			mysqlpp::SimpleResult dataTmp = query.execute(tablename, field[0], values[0], condition);
			return dataTmp;
		}
		else if(fieldSize == 2)
		{
			query<< "update %0"" set %1""= %2q, %3""= %4q where address = %5q";
			query.parse();
			mysqlpp::SimpleResult dataTmp = query.execute(tablename, field[0], values[0], field[1], values[1], condition);
			return dataTmp;
		}
		else if(fieldSize == 3)
		{
			query<< "update %0"" set %1""= %2q, %3""= %4q, %5""= %6q where address = %7q";
			query.parse();
			mysqlpp::SimpleResult dataTmp = query.execute(tablename,
														  field[0], values[0],
														  field[1], values[1],
														  field[2], values[2],
														  condition);
			return dataTmp;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions; e.g. type mismatch populating 'stock'
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
	}
}
// for ttc+= 1
mysqlpp::SimpleResult ISNDB::ISNSqlAddOne(string tablename, vector<string> field, string condition)
{
	try{

		mysqlpp::Query query= con.query();
		//according tablename in different way
		query<< "update %0"" set %1""=%1""+ 1 where address = %2q";
		query.parse();
		mysqlpp::SimpleResult dataTmp = query.execute(tablename, field[0], condition);
		return dataTmp;
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		exit(-1);
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions; e.g. type mismatch populating 'stock'
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
		exit(-1);
	}
}
// insert ISNDB with condition
int ISNDB::ISNSqlInsert(string tablename, vector<string> values)
{
	try{
		if(tablename=="clubinfo")
		{
			mysqlpp::Query query= con.query();
			query << "insert clubinfo(address, ttc) values(%0q, %1q)";
			query.parse();
			mysqlpp::SimpleResult dataTmp = query.execute(values[0], values[1]);
			return dataTmp.insert_id();
		}
		else if(tablename=="memberinfo")
		{
			mysqlpp::Query query= con.query();
			query << "insert memberinfo(address, club_id, father, tc, balance) values(%0q, %1q, %2q, %3q, %4q)";
			query.parse();
			mysqlpp::SimpleResult dataTmp = query.execute(values[0], values[1], values[2], values[3], values[4]);
			return dataTmp.insert_id();
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		exit(-1);
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions; e.g. type mismatch populating 'stock'
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
		exit(-1);
	}
	
	return 0;
}