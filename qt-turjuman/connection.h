#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject> 
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QList>
#include <QMap>

class Connection : public QObject{
	Q_OBJECT

public:
	explicit Connection(QObject *parent = 0);
	~Connection();
	QStringList translateEnglishWord(QString word);
	QStringList translateRussianWord(QString word);
	QMap<QString, QString> translateWordDouble(
		QString from, QString word);
	QMap<QString, QString>  translateWordDouble(
		QString from, QString to, QString word);
	QStringList translateWord(QString from, QString word);
	QStringList translateWord(QString from, QString to, QString word);

private:
	QSqlDatabase database;

protected:
	// Returns a proper query.
	QString formatQuery(QString from, QString to, bool exact);
};


#endif