#ifndef _QSEXECUTE_HPP_
#define _QSEXECUTE_HPP_

#include <QtScript>
#include <QCoreApplication>

#include <stdexcept>

namespace QsExecute
{

class Error : public std::runtime_error
{
public:
    Error(QString const &s);
    virtual ~Error() throw() {}

    QString msg;
};

class JsError : public Error
{
public:
    JsError(QScriptEngine &engine, QString const &file);
    static QString errorMessage(QScriptEngine &engine, QString const &file);
};

typedef QScriptValue (*qscript_file_loader_type)(QString, QScriptEngine &);

qscript_file_loader_type setupEngine
(QCoreApplication &app, QScriptEngine &engine, QScriptValue global);

}

#endif // _QSEXECUTE_HPP_
