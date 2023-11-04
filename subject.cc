#include "subject.h"
void Subject::attach(Observer *o) { observers.emplace_back(o); }

void Subject::notifyObservers() {
    for (auto &ob : observers) ob->notify(bst);
}

void Subject::detach(Observer *o) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == o) {
            observers.erase(it);
            break;
        }
    }
}

const BoardState &Subject::getState() const { return bst; }

Subject::~Subject() {}