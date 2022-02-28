#include "actionlist.h"

ActionList* ActionList::action_list = 0;

/*создание только одного экземпляр класса с глобальной точкой доступа*/
ActionList* ActionList::getSingletonPtr()
{
    if (!action_list)
        action_list = new ActionList();
    return action_list;
}

/*добавление команды в список*/
bool ActionList::setAction(Action *action)
{
    mutex.lock();
    if (is_allowed) {
        if (actions.size() < MAX_ACTIONS) {
            actions.push_front(action);
            mutex.unlock();
            return true;
        } else {
            mutex.unlock();
            delete action;
            return false;
        }
    }
    mutex.unlock();
    delete action;
    return false;
}

/*извлечение команды из списка*/
Action* ActionList::getAction()
{
    mutex.lock();
    if (actions.size() > 0) {
        Action *action = actions.back();
        actions.pop_back();
        mutex.unlock();
        return action;
    } else {
        mutex.unlock();
        return 0;
    }
}

/*очстка списка команд*/
void ActionList::clear()
{
    mutex.lock();
    actions.clear();
    mutex.unlock();
}

/*установка разрешения на добавление команд в список*/
void ActionList::setPermission(bool allow)
{
    mutex.lock();

    is_allowed = allow;

    if (!is_allowed)
        actions.clear();

    mutex.unlock();
}
