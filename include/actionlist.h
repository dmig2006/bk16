#ifndef ACTION_LIST
#define ACTION_LIST

#include <QObject>
#include <QMutex>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <list>

#include "tcpclient.h"				// Support
//Actions----------------------------------------------------------------------
#include "action.h"

#define MAX_ACTIONS 250

class ActionList : public QObject
{
    Q_OBJECT

    std::list<Action*> actions;
    QMutex mutex;
    bool is_allowed;

    static ActionList *action_list;
    ActionList() : is_allowed(false) {}

public:
    static ActionList* getSingletonPtr();
    bool setAction(Action *action);
    Action* getAction();
    void clear();

public slots:
    void setPermission(bool allow);
};

#endif // ACTIONLINE_H
