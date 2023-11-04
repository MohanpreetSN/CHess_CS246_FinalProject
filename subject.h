#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>

#include "boardState.h"
#include "observer.h"

class Subject {
    std::vector<Observer *> observers;

protected:
    BoardState bst;

public:
    void attach(Observer *o);
    void detach(Observer *o);
    void notifyObservers();
    const BoardState &getState() const;
    virtual ~Subject() = 0;
};

#endif
