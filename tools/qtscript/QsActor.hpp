#ifndef _QS_ACTOR_HPP_
#define _QS_ACTOR_HPP_
/*
 * QtScript Actor component
 *
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Denis Zalevskiy <denis.zalevskiy@jolla.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 */

#include <QObject>
#include <QVariant>
#include <QString>
#include <QScriptValue>
#include <QThread>
#include <QEvent>
#include <QWaitCondition>
#include <QMutex>
#include <QStringList>
#include <QDeclarativeEngine>
namespace QsExecute
{

class Event : public QEvent
{
public:
    enum Type {
        LoadScript = QEvent::User,
        ProcessMessage,
        QuitThread,
        LoadException
    };

    Event();
    Event(QString const&);
    virtual ~Event();

    QString src_;

protected:
    Event(Type t);

};

class Message : public Event
{
public:
    Message(QVariant const&, QScriptValue const&);
    virtual ~Message() {}

    QVariant data_;
    QScriptValue cb_;
};

class EngineException : public Event
{
public:
    EngineException(QScriptEngine const&);
    virtual ~EngineException() {}

    QVariant exception_;
    QStringList backtrace_;
};

class Actor;

class Engine;
class MessageContext : public QObject
{
    Q_OBJECT;
public:
    MessageContext(Engine *, QScriptValue);
    virtual ~MessageContext();

    Q_INVOKABLE void reply(QScriptValue);
private:
    Engine *engine_;
    QScriptValue cb_;
};

class Actor;
class Engine : public QObject
{
    Q_OBJECT;
public:
    Engine(Actor*);
    virtual ~Engine();

    void run();
    virtual bool event(QEvent *);

    void reply(QVariant const &, QScriptValue const &);

signals:
    void onQuit();

private:
    void load(QString const&);
    void processMessage(Message*);
    void toActor(Event*);

    Actor *actor_;
    QScriptEngine *engine_;
    QScriptValue handler_;
    QWaitCondition cond_;
    QMutex mutex_;
};

class WorkerThread : public QThread
{
    Q_OBJECT;
public:
    WorkerThread(Actor *parent, QString const &src);
    virtual ~WorkerThread();

    void run();
    void sendMessage(QScriptValue, QScriptValue);

private:
    Actor *actor_;
    QWaitCondition cond_;
    QMutex mutex_;
    QScopedPointer<Engine> engine_;
};


class Actor : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(QString source READ source WRITE setSource);
public:
    Actor(QObject *parent = nullptr);
    virtual ~Actor();

    QString source() const;
    void setSource(QString const &);

    Q_INVOKABLE void sendMessage(QScriptValue, QScriptValue);
    virtual bool event(QEvent *);

signals:
    void message(QScriptValue message);
    void error(QVariant const& error);

protected:
    QString src_;
    void reply(Message*);
    QDeclarativeEngine *engine();

private:

    QScopedPointer<WorkerThread> worker_;
};

}

#endif // _QS_ACTOR_HPP_