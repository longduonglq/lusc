// Project AIDA
// Created by Long Duong on 6/7/22.
// Purpose: 
//

#include "backend.h"
#include <memory.h>
#include <stdio.h>

extern "C" int wrapper_handoff(int, char**);
extern "C" int test_handoff();

//extern CEventStoreClass es_cls;
//extern CCalendarClass cal_cls;
//extern CEventClass ev_cls;
//extern CDateClass dt_cls;
//extern CDTCalendarClass dtcal_cls;

void* c_malloc(size_t size) { return malloc(size); }
void c_free(void* in) { return free(in); }

int c_handoff(int argc, char** argv) {
#ifndef LUSC_TESTING
    return wrapper_handoff(argc, argv);
#else
    return test_handoff();
#endif
}

extern DTDateComponents DTDateDefault {
    -1, -1, -1, -1, -1,
    false,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1
};

void receive_classes(
        CEventStoreClass _es_cls,
        CCalendarClass _cal_cls,
        CEventClass _ev_cls,
        CDateClass _dt_cls,
        CDTCalendarClass _dtcal_cls,
        CEKSourceClass _eksrc_cls) {
    es_cls = _es_cls;
    cal_cls = _cal_cls;
    ev_cls = _ev_cls;
    dt_cls = _dt_cls;
    dtcal_cls = _dtcal_cls;
    eksrc_cls = _eksrc_cls;
}
