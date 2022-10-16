// Project AIDA
// Created by Long Duong on 6/7/22.
// Purpose: 
//

#ifndef LUSCBACKEND_BACKEND_H
#define LUSCBACKEND_BACKEND_H

#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

///// C-vector //////
void *c_malloc(size_t size);
void c_free(void *in);

////// CEntityType ///////
typedef enum CEntityType {
    event,
    reminder
} CEntityType;

////// CCGColor ///////
typedef double CCGFloat;

///// Forward decl ///////
typedef void *CEventStoreHandle;
typedef void *CCalendarHandle;
typedef enum CSpan {
    this_event, future_events
} CSpan;
typedef void* CNSPredicate;
typedef void* CDateHandle;

typedef enum DTCalendarComponent : long {
    era = 1 << 0,
    year = 1 << 2,
    yearForWeekOfYear = 1 << 3,
    quarter = 1 << 4,
    month = 1 << 5,
    weekOfYear = 1 << 6,
    weekOfMonth = 1 << 7,
    weekday = 1 << 8,
    weekdayOrdinal = 1 << 9,
    day = 1 << 10,
    hour = 1 << 11,
    minute = 1 << 12,
    second = 1 << 13,
    nanosecond = 1 << 14,
    calendar = 1 << 15,
    timeZone = 1 << 16
} DTCalendarComponent;

typedef struct DTDateComponents {
    int era, year, yearForWeekOfYear, quarter, month;
    bool isLeapMonth;
    int weekOfMonth, weekOfYear, weekday, weekdayOrdinal, day;
    int hour, minute, second, nanosecond;
} DTDateComponents;
extern DTDateComponents DTDateDefault;


//////// DTCalendar /////////
typedef void *CDTCalendarHandle;
typedef struct CDTCalendarClass {
    CDTCalendarHandle (*_init)();
    void (*_incref)(CDTCalendarHandle);
    CDTCalendarHandle (*_copy)(CDTCalendarHandle);
    long (*_deinit)(CDTCalendarHandle);

    int (*_component)(CDTCalendarHandle, DTCalendarComponent, CDateHandle date);
    // DTDateComponents (*_date_components_from)(CDTCalendarHandle, DTCalendarComponent, CDateHandle from);
    // DTDateComponents (*_date_components_from_to)(CDTCalendarHandle, DTCalendarComponent, CDateHandle from, CDateHandle to);

    void (*_timezone_ident)(CDTCalendarHandle, char *ident);
    int (*_seconds_from_gmt)(CDTCalendarHandle);
    CDateHandle (*_start_of_day)(CDTCalendarHandle, CDateHandle date);

//    void (*_enumerate_dates)(
//            CDateHandle start_after,
//            DTDateComponents matching,
//            int direction,
//            void (*)(CDateHandle, bool exact_match) );
//
//    CDateHandle (*_next_date)(
//            CDateHandle after,
//            DTDateComponents matching,
//            int direction);
    CDateHandle (*_date_from_components)(CDTCalendarHandle, DTDateComponents date_components);
} CDTCalendarClass;

////// Date /////////
typedef double CTimeInterval;

typedef struct CDateClass {
    CDateHandle (*_init)();
    void (*_incref)(CDateHandle);
    CDateHandle (*_copy)(CDateHandle);
    long (*_deinit)(CDateHandle);

    CDateHandle (*_init_from_time_interval_since_now)(CTimeInterval);
    CDateHandle (*_init_from_time_interval_since_reference_date)(CTimeInterval);
    CDateHandle (*_init_from_time_interval_since_1970)(CTimeInterval);

    CDateHandle (*_distant_future)();
    CDateHandle (*_distant_past)();

    CTimeInterval (*_time_interval_since_reference_date)(CDateHandle);
    CTimeInterval (*_time_interval_since_1970)(CDateHandle);

    void (*_formatted)(CDateHandle, char *out_str);

    ///// from `Calendar` struct
} CDateClass;

////// Event ///////
typedef void *CEventHandle;
typedef struct CEventClass {
    CEventHandle (*_init)(CEventStoreHandle);
    void (*_incref)(CEventHandle);
    CEventHandle (*_copy)(CEventHandle);
    long (*_deinit)(CEventHandle);

    int (*_compare_start_dates)(CEventHandle, CEventHandle); // return -1, 0, 1
    void (*_event_ident)(CEventHandle, char *out_ident);

    CDateHandle (*_get_start_date)(CEventHandle);
    void (*_set_start_date)(CEventHandle, CDateHandle);
    CDateHandle (*_get_end_date)(CEventHandle);
    void (*_set_end_date)(CEventHandle, CDateHandle);

    bool (*_get_is_all_day)(CEventHandle);
    void (*_set_is_all_day)(CEventHandle, bool is_all_day);
    CDateHandle (*_occurrence_date)(CEventHandle);
    bool (*_is_detached)(CEventHandle);

    /// inherited from CalendarItem
    void (*_calitem_ident)(CEventHandle, char *out_ident);
    CCalendarHandle (*_get_calendar)(CEventHandle);
    void (*_set_calendar)(CEventHandle, CCalendarHandle);
    void (*_get_title)(CEventHandle, char *out_title);
    void (*_set_title)(CEventHandle, const char* in_title);
    CDateHandle (*_creation_date)(CEventHandle);
    bool (*_has_notes)(CEventHandle);
    int (*_get_notes)(CEventHandle, char *out_note, const int max_length);
    void (*_set_notes)(CEventHandle, const char* in_out);
} CEventClass;

/////// CEKSource ////////
typedef enum CEKSourceType : int {
    local,
    exchange,
    calDAV,
    mobileMe,
    subscribed,
    birthdays
} CEKSourceType;

typedef void* CEKSourceHandle;
typedef struct CEKSourceClass {
    void (*_incref)(CEKSourceHandle);
    CEKSourceHandle (*_copy)(CEKSourceHandle);
    long (*_deinit)(CEKSourceHandle);
    CEKSourceType (*_sourceType)(CEKSourceHandle);
    void (*_source_ident)(CEKSourceHandle, char* out_ident);
    void (*_title)(CEKSourceHandle, char* out_title);
} CEKSourceClass;

////// Calendar //////

typedef struct CCalendarClass {
    CCalendarHandle (*_init)(CEntityType, CEventStoreHandle);
    void (*_incref)(CCalendarHandle);
    CCalendarHandle (*_copy)(CCalendarHandle);
    long (*_deinit)(CCalendarHandle);

    bool (*_allows_content_modifications)(CCalendarHandle);
    bool (*_is_immutable)(CCalendarHandle);

    void (*_get_cg_color)(CCalendarHandle, CCGFloat *, CCGFloat *, CCGFloat *);
    void (*_get_title)(CCalendarHandle, char *);
    void (*_get_cal_ident)(CCalendarHandle, char *);

    void (*_set_cg_color)(CCalendarHandle, CCGFloat, CCGFloat, CCGFloat);
    void (*_set_title)(CCalendarHandle, const char *);
    void (*_set_source)(CCalendarHandle, CEKSourceHandle); // This function merely borrows the incoming CEKSourceHandle
} CCalendarClass;

////// CEventStore ///////

typedef struct CEventStoreClass {
    CEventStoreHandle (*_init)(void);
    void (*_incref)(CEventStoreHandle);
    CEventStoreHandle (*_copy)(CEventStoreHandle);
    long (*_deinit)(CEventStoreHandle);

    // returns 1 if out buffer too small. on success return 0
    int (*_event_store_ident)(CEventStoreHandle, char *out_buffer,
                              int max_length); // max_length = allocated - 1 (bc null terminated)
    int (*_request_access_to)(
            CEventStoreHandle,
            CEntityType entity,
            char *err_msg);
    void (*_get_sources)(CEventStoreHandle, CEKSourceHandle **, int* len);

    int (*_commit)(CEventStoreHandle, char *err_msg);
    void (*_reset)(CEventStoreHandle);
    void (*_refresh_sources_if_needed)(CEventStoreHandle);
    void (*_calendars_for_entity)(CEventStoreHandle, CEntityType, CCalendarHandle **calendars, int *len);
    CCalendarHandle (*_calendar_with_ident)(CEventStoreHandle, const char *ident);
    int (*_save_calendar)(CEventStoreHandle, CCalendarHandle, bool commit, char *out_err_msg);
    int (*_remove_calendar)(CEventStoreHandle, CCalendarHandle, bool commit, char *out_err_msg);
    CEventHandle (*_event_with_ident)(CEventStoreHandle, const char *ident);
    int (*_remove)(CEventStoreHandle, CEventHandle, CSpan, char *err_out);
    int (*_save)(CEventStoreHandle, CEventHandle, CSpan, char *err_out);
    CNSPredicate
    (*_predicate_for_events)(CEventStoreHandle, CDateHandle start, CDateHandle end, const CCalendarHandle *cals,
                             int len);
    void (*_deinit_predicate)(CNSPredicate);
    void (*_events_matching)(CEventStoreHandle, CNSPredicate, CEventHandle **events, int *len);
} CEventStoreClass;



extern CEventStoreClass es_cls;
extern CCalendarClass cal_cls;
extern CEventClass ev_cls;
extern CDateClass dt_cls;
extern CDTCalendarClass dtcal_cls;
extern CEKSourceClass eksrc_cls;

int c_handoff(int argc, char** argv);
void receive_classes(
        CEventStoreClass,
        CCalendarClass,
        CEventClass,
        CDateClass,
        CDTCalendarClass,
        CEKSourceClass);

#ifdef __cplusplus
}
#endif

#endif //LUSCBACKEND_BACKEND_H
