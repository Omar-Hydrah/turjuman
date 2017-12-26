#include "connection.h"

Connection::Connection(QObject *parent) : QObject(parent){
	database = QSqlDatabase::addDatabase("QSQLITE");
	database.setHostName("localhost");
	database.setDatabaseName("dictionary.db");

	if(database.open()){
		// QMessageBox::information(NULL, "Database message", "Connection established");
		qDebug() << "Connection established";
	}else{
		QMessageBox::information(NULL, "Database message", "Database connection failed");
		qDebug() << "Database connection failed";
		exit(1);
	}
}

Connection::~Connection(){
	database.close();
}


// Not used.
QStringList Connection::translateEnglishWord(QString word){
	QSqlQuery query;
	// QString sql = "select russian from words where english = 'boy'";
	// query.prepare("select russian from words where english like ?");
	// query.addBindValue(word);
	query.prepare("select russian from words where english like (:word)");
	query.bindValue(":word", word + "%");

	QStringList result;
	// To prevent having more than a 100 words.
	int counter = 0;

	if(query.exec()){
		while(query.next() && counter <= 100){
			result << query.value(0).toString();
			counter++;
		}
	}else{
		// query.exec() returns false if the query fails, 
		// then query.lastError becomes available. 
		QSqlError error = query.lastError();
        // qDebug() << "SqlError: " << query.lastError();
        qDebug() << error.text();
        // qDebug() << "SqlError " occured.
	}

	return result;
}

// Not used.
QStringList Connection::translateRussianWord(QString word){
	QSqlQuery query;
	query.prepare("select english from words where russian like (:word)");
	query.bindValue(":word", word + "%");

	QStringList result;
	// To prevent having more than 100 results.
	int counter = 0;

	if(query.exec()){
		// qDebug() << "Translation of " << word << ":";
		while(query.next() && counter <= 100){

			result << query.value(0).toString();
			counter++;
		}
	}else{
		QSqlError error = query.lastError();
		qDebug() << error.text();
	}

	return result;
}

// Assuming we're only using english and russian.
QStringList Connection::translateWord(QString from, QString word){
	if(from == "russian"){
		return translateWord("russian", "english", word);
	}else{
		return translateWord("english", "russian", word);
	}
}

// Receives a formatted sql query, and returns a list of words.
QStringList Connection::translateWord(QString from, QString to, QString word){
	// Sample sql query: "select english from words where russian = word";
	QString sql = Connection::formatQuery(from, to, true);

	QSqlQuery query;
	query.prepare(sql);
	query.bindValue(":word", word);

	QStringList words;

	// qDebug() << "SQL Statement: " << sql;

	if(query.exec()){
		// To avoid having more than 100 words.
		int counter = 0;
		while(query.next() && counter <= 100){
			words << query.value(0).toString();
			counter++;
		}
	}else{
		QSqlError error = query.lastError();
		qDebug() << "error: " << error.text();
	}

	return words;
}

// Assuming we're only using english and russian
QMap<QString, QString> Connection::translateWordDouble(
	QString from, QString word)
{
	if(from == "russian"){
		return translateWordDouble("russian", "english", word);
	}else{
		return translateWordDouble("english", "russian", word);
	}
}

// Returns a list of words with their meaning.
QMap<QString, QString> Connection::translateWordDouble(
		QString from, QString to, QString word)
{
	// Sample sql query: "select * from english where russian like word% limit 100";
	QString sql = formatQuery(from, to, false);

	QSqlQuery query;
	query.prepare(sql);
	query.bindValue(":word", word + "%");

	QMap<QString, QString>  words;


	if(query.exec()){
		// To prevent having more than 100 words in the result.
		int counter = 0;
		while(query.next() && counter <= 100){
            // QMap<QString, QString> word;
            words.insert(query.value("english").toString(),
            	query.value("russian").toString());

            counter++;
		}

	}else{
		QSqlError error = query.lastError();
		qDebug() << "query error: " << error.text();
		QMessageBox::information(NULL, "Query Error", error.text());
	}
	// qDebug() << words.size() << " words found";
	return words;
}

// Formats a query to be used in this->translateWord()
// Exact will decide returning two columns or one column from the query.
// An "exact" search will return only a list of words that match the searched word.
// The opposite, a "similar" search, will return a list of similar words.
// It appends "%" to the word in the query, to get similar words.
// This separation because search for "home" will return meaning for "homeless" as well.
QString Connection::formatQuery(QString from, QString to, bool exact){
	QString sql;

	// Preparing sql statement happens in Connection::translateWord 
	if(from == "english" && to == "russian"){
		if(exact){
			sql = "select russian from words where english = (:word)";
		}else{
			sql = "select english, russian from words where english like (:word)";
		}

	}else if(from == "russian" && to == "english"){
		if(exact){
			sql = "select english from words where russian = (:word)";

		}else{
			sql = "select english, russian from words where russian like (:word)";
		}

	}else{
		return "";
	}

	return sql + " limit 100" ;
}
