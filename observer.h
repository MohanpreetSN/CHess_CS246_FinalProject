#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include "boardState.h"

class Observer {
public:
    virtual void notify(const BoardState& bs) = 0;
    virtual ~Observer() = default;
};
#endif
