// Project AIDA
// Created by Long Duong on 6/8/22.
// Purpose: 
//

#ifndef LUSCBACKEND_WRAPPER_H
#define LUSCBACKEND_WRAPPER_H

#include "backend.h"
#include <type_traits>
#include <string>
#include <optional>
#include <vector>
#include <boost/outcome.hpp>
#include <exception>
#include <fmt/format.h>

extern "C" int wrapper_handoff(int argc, char** argv);

namespace cx {
    using namespace boost::outcome_v2;
    struct CGColor { CCGFloat red, green, blue; };

    class Date {
    public:
        CDateHandle _date = nullptr;

        Date() = delete;
        Date(CDateHandle _date) : _date { _date } {};
        Date(const Date&);
        Date(Date&&);
        ~Date();
        Date& operator=(Date&& other);

        static Date init_from_time_interval_since_now(CTimeInterval);
        static Date init_from_time_interval_since_reference_date(CTimeInterval);
        static Date init_from_time_interval_since_1970(CTimeInterval);
        static Date distant_future();
        static Date distant_past();

        CTimeInterval time_interval_since_reference_date();
        CTimeInterval time_interval_since_1970();

        std::string formatted();
    };
    class DTCalendar {
    public:
        CDTCalendarHandle _dtcal = nullptr;

        DTCalendar();
        DTCalendar(const DTCalendar&);
        DTCalendar(DTCalendar&&);
        ~DTCalendar();

        int component(DTCalendarComponent, const Date&);
        std::string timezone_ident();
        int seconds_from_gmt();
        Date start_of_day(const Date& date);
        Date date_from_components(DTDateComponents);
    };
    class EKSource {
    public:
        CEKSourceHandle _source = nullptr;

        EKSource() = delete;
        EKSource(const EKSource&);
        EKSource(CEKSourceHandle h) : _source { h } {};
        EKSource(EKSource&&);
        EKSource& operator=(EKSource&& other);
        ~EKSource();
        CEKSourceType source_type();
        std::string source_ident();
        std::string title();
    };
    class EKEventStore;
    class EKCalendar {
    public:
        CCalendarHandle _cal = nullptr;

        EKCalendar() = delete;
        EKCalendar(CEntityType, const EKEventStore& es); // need to use reference to prevent circular referencing
        EKCalendar(CCalendarHandle _cal);
        EKCalendar(const EKCalendar&);
        EKCalendar(EKCalendar&&);
        ~EKCalendar();
        bool allows_content_modification();
        bool is_immutable();
        CGColor get_cg_color();
        void set_cg_color(CGColor);
        std::string get_title();
        void set_title(const char* title);
        void set_source(const EKSource&);
        std::string get_cal_ident();
    };
    class EKEvent {
    public:
        CEventHandle _ev = nullptr;

        EKEvent(const EKEventStore& es);
        EKEvent(const EKEvent&);
        EKEvent(CEventHandle _ev) : _ev{ _ev } {};
        EKEvent(EKEvent&&);
        ~EKEvent();
        int compare_start_dates_with(const EKEvent& other);
        std::string event_identification();
        Date get_start_date();
        void set_start_date(const Date&);
        Date get_end_date();
        void set_end_date(const Date&);
        bool get_is_all_day();
        void set_is_all_day(bool);

        std::string calitem_ident();
        EKCalendar get_calendar();
        void set_calendar(const EKCalendar&);
        std::string get_title();
        void set_title(const char* title);
        Date creation_date();
        bool has_notes();
        result<std::string, int> get_notes();
        void set_notes(const char*);
        result<void, int> append_notes(const char*);
    };
    class NSPredicate {
    public:
        CNSPredicate _pred = nullptr;

        NSPredicate() = delete;
        NSPredicate(const NSPredicate&) = delete;
        NSPredicate(NSPredicate&& other) {
            _pred = other._pred;
            other._pred = nullptr;
        }
        NSPredicate(CNSPredicate _pred) : _pred { _pred } {};
        ~NSPredicate();
    };

    class EKException : std::exception {
    public:
        std::string _msg;
        template <typename ...T>
        EKException(T&& ...args) : _msg {fmt::format(std::forward<T>(args)...)}  {}
        [[nodiscard]] const char * what() const noexcept override { return _msg.c_str(); }
    };

    class EKEventStore {
    public:
        CEventStoreHandle _eventStore = nullptr;

        EKEventStore();
        EKEventStore(const EKEventStore&) = delete;
        EKEventStore(EKEventStore&&);
        ~EKEventStore();

        std::string identification() const;
        void request_access_to_entity(CEntityType);
        std::vector<EKSource> get_sources();
        void commit();
        void reset();
        void refresh_sources_if_needed();
        std::vector<EKCalendar> calendars_for_entity(CEntityType);
        std::optional<EKCalendar> calendar_with_ident(const char* ident);
        void save_calendar(const EKCalendar&, bool commit);
        void remove_calendar(const EKCalendar&, bool commit);
        std::optional<EKEvent> event_with_identifier(const char* ident);
        void remove(const EKEvent&, CSpan);
        void save(const EKEvent&, CSpan);
        NSPredicate predicate_for_events(
                const Date& start, const Date& end,
                std::vector<CCalendarHandle> calendars);
        std::vector<EKEvent> events_matching(const NSPredicate&);
    };
}

#endif //LUSCBACKEND_WRAPPER_H
