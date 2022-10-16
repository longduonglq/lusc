// Project AIDA
// Created by Long Duong on 6/8/22.
// Purpose: 
//

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "backend.h"
#include "wrapper.h"
#include <iostream>
#include <string.h>
#include <algorithm>

#include <fmt/format.h>
#include <fmt/core.h>

CEventStoreClass es_cls;
CCalendarClass cal_cls;
CEventClass ev_cls;
CDateClass dt_cls;
CDTCalendarClass dtcal_cls;
CEKSourceClass eksrc_cls;

namespace cx {
    EKEventStore::EKEventStore()
        : _eventStore { es_cls._init() }
    {}
//    EKEventStore::EKEventStore(const EKEventStore &other) {
//        if (_eventStore != nullptr) es_cls._deinit(_eventStore);
//        this->_eventStore = es_cls._copy(other._eventStore);
//        assert(this->_eventStore != other._eventStore);
//    }
    EKEventStore::EKEventStore(EKEventStore && other) {
        std::swap(_eventStore, other._eventStore);
        other._eventStore = nullptr;
    }
    EKEventStore::~EKEventStore() {
        if (_eventStore != nullptr) es_cls._deinit(_eventStore);
        _eventStore = nullptr;
    }
    std::string EKEventStore::identification() const {
        char ident[200];
        memset(ident, 0, 200);
        es_cls._event_store_ident(_eventStore, ident, 200);
        return {ident};
    }
    void EKEventStore::request_access_to_entity(CEntityType entity) {
        char err[200];
        memset(err, 0, 200);
        int res = es_cls._request_access_to(_eventStore, entity, err);
        if (res != 0) throw EKException(err);
        else return;
    }
    std::vector<EKSource> EKEventStore::get_sources() {
        CEKSourceHandle* sources = nullptr;
        int len = 0;
        es_cls._get_sources(_eventStore, &sources, &len);
        std::vector<EKSource> srcs;
        srcs.reserve(len);
        std::transform(sources, sources + len, std::back_inserter(srcs),
                       [](CEKSourceHandle h){ return EKSource{ h }; });
        c_free(sources);
        return srcs;
    }
    void EKEventStore::commit() {
        char err[200];
        memset(err, 0, 200);
        int res = es_cls._commit(_eventStore, err);
        if (res != 0) throw EKException(err);
        else return ;
    }
    void EKEventStore::reset() { return es_cls._reset(_eventStore); }
    void EKEventStore::refresh_sources_if_needed() { return es_cls._refresh_sources_if_needed(_eventStore); }
    std::vector<EKCalendar> EKEventStore::calendars_for_entity(CEntityType entity) {
        CCalendarHandle* cals = nullptr;
        int len = 0;
        es_cls._calendars_for_entity(_eventStore, entity, &cals, &len);
        std::vector<EKCalendar> _cals;
        _cals.reserve(len * 2); // try to avoid any copying of EKCalendar whatsoever
        std::transform(cals, cals + len, std::back_inserter(_cals),
                       [](CCalendarHandle h){ return EKCalendar { h }; });
        c_free(cals);
        return _cals;
    }
    std::optional<EKCalendar> EKEventStore::calendar_with_ident(const char *ident) {
        CCalendarHandle cal = es_cls._calendar_with_ident(_eventStore, ident);
        if (cal != nullptr) return EKCalendar(cal);
        else return std::nullopt;
    }
    void EKEventStore::save_calendar(const EKCalendar& calendar, bool commit) {
        char err[500];
        memset(err, 0, 500);
        int res = es_cls._save_calendar(_eventStore, calendar._cal, commit, err);
        if (res != 0) throw EKException(err);
        else return ;
    }
    void EKEventStore::remove_calendar(const EKCalendar& calendar, bool commit) {
        char err[500];
        memset(err, 0, 500);
        int res = es_cls._remove_calendar(_eventStore, calendar._cal, commit, err);
        if (res != 0) throw EKException(err);
        else return ;
    }
    std::optional<EKEvent> EKEventStore::event_with_identifier(const char* ident) {
        CEventHandle ev = es_cls._event_with_ident(_eventStore, ident);
        if (ev != nullptr) return EKEvent{ ev };
        else return std::nullopt;
    }
    void EKEventStore::remove(const EKEvent& ev, CSpan sp) {
        char err[500];
        memset(err, 0, 500);
        int res = es_cls._remove(_eventStore, ev._ev, sp, err);
        if (res != 0) throw EKException(err);
        else return ;
    }
    void EKEventStore::save(const EKEvent& ev, CSpan sp) {
        char err[500];
        memset(err, 0, 500);
        int res = es_cls._save(_eventStore, ev._ev, sp, err);
        if (res != 0) throw EKException(err);
        else return ;
    }
    cx::NSPredicate EKEventStore::predicate_for_events(
            const Date& start, const Date& end,
            std::vector<CCalendarHandle> _ccals) {
//        std::vector<CCalendarHandle> _ccals;
//        _ccals.reserve(calendars.size());
//        std::transform(
//                calendars.begin(), calendars.end(),
//                std::back_inserter(_ccals),
//                [](EKCalendar* a){ return a->_cal; });
        return { es_cls._predicate_for_events(_eventStore, start._date, end._date, _ccals.data(), static_cast<int>(_ccals.size())) };
    }
    std::vector<EKEvent> EKEventStore::events_matching(const cx::NSPredicate& pred) {
        CEventHandle * events = nullptr;
        int len = 0;
        es_cls._events_matching(_eventStore, pred._pred, &events, &len);
        std::vector<EKEvent> evs;
        evs.reserve(len);
        std::transform(events, events + len, std::back_inserter(evs),
                       [](CEventHandle h) { return EKEvent{ h }; }); // steal
        c_free(events);
        return evs;
    }


    ///// NSPredicate /////
    NSPredicate::~NSPredicate() {
        if (_pred) {
            es_cls._deinit_predicate(_pred);
            _pred = nullptr;
        }
    }

    ///// EKEvent /////
    EKEvent::EKEvent(const EKEventStore& es) : _ev { ev_cls._init(es._eventStore) } {}
    EKEvent::EKEvent(const EKEvent & other) {
        if (_ev != nullptr) ev_cls._deinit(_ev);
        _ev = ev_cls._copy(other._ev);
        assert(_ev != other._ev);
    }
    EKEvent::EKEvent(EKEvent&& other) {
        std::swap(_ev, other._ev);
        other._ev = nullptr;
    }
    EKEvent::~EKEvent() noexcept {
        if (_ev != nullptr) {
            ev_cls._deinit(_ev);
            _ev = nullptr;
        }
    }
    int EKEvent::compare_start_dates_with(const EKEvent &other) { return ev_cls._compare_start_dates(_ev, other._ev); }
    std::string EKEvent::event_identification() {
        char ident[200];
        memset(ident, 0, 200);
        ev_cls._event_ident(_ev, ident);
        return { ident };
    }
    Date EKEvent::get_start_date() { return Date { ev_cls._get_start_date(_ev) }; }
    void EKEvent::set_start_date(const Date& d) { ev_cls._set_start_date(_ev, d._date); }
    Date EKEvent::get_end_date() { return Date { ev_cls._get_end_date(_ev) }; }
    void EKEvent::set_end_date(const Date& d) { ev_cls._set_end_date(_ev, d._date); }
    bool EKEvent::get_is_all_day() { return ev_cls._get_is_all_day(_ev); }
    void EKEvent::set_is_all_day(bool s) { ev_cls._set_is_all_day(_ev, s); }
    std::string EKEvent::calitem_ident() {
        char ident[200];
        memset(ident, 0, 200);
        ev_cls._calitem_ident(_ev, ident);
        return { ident };
    }
    EKCalendar EKEvent::get_calendar() { return EKCalendar{ ev_cls._get_calendar(_ev) }; }
    void EKEvent::set_calendar(const EKCalendar& cal) { ev_cls._set_calendar(_ev, cal._cal); }
    std::string EKEvent::get_title() {
        char title[200];
        memset(title, 0, 200);
        ev_cls._get_title(_ev, title);
        return { title };
    }
    void EKEvent::set_title(const char *title) { ev_cls._set_title(_ev, title); }
    Date EKEvent::creation_date() { return Date { ev_cls._creation_date(_ev) }; }
    bool EKEvent::has_notes() { return ev_cls._has_notes(_ev); }
    result<std::string, int> EKEvent::get_notes() {
        if (!has_notes()) return success(std::string());
        const int LEN = 900;
        char notes[LEN];
        memset(notes, 0, LEN);
        int res = ev_cls._get_notes(_ev, notes, LEN);
        if (res != 0) return failure(res);
        else return success(std::string(notes));
    }
    void EKEvent::set_notes(const char * notes) { ev_cls._set_notes(_ev, notes); }
    result<void, int> EKEvent::append_notes(const char * notes) {
        std::string oldNotes = BOOST_OUTCOME_TRYX(get_notes());
        oldNotes += notes;
        set_notes(oldNotes.c_str());
        return success();
    }
    ////// Date ////////
    Date::Date(const Date & other) {
        if (_date != nullptr) dt_cls._deinit(_date);
        _date = dt_cls._copy(other._date);
        assert(_date != other._date);
    }
    Date::Date(Date && other) {
        std::swap(_date, other._date);
        other._date = nullptr;
    }
    Date::~Date() {
        if (_date != nullptr) {
            dt_cls._deinit(_date);
            _date = nullptr;
        }
    }
    Date& Date::operator=(Date&& other) {
        std::swap(_date, other._date);
        other._date = nullptr;
        return *this;
    }

    Date Date::init_from_time_interval_since_now(CTimeInterval itv) { return { dt_cls._init_from_time_interval_since_now(itv) }; }
    Date Date::init_from_time_interval_since_reference_date(CTimeInterval itv) { return { dt_cls._init_from_time_interval_since_reference_date(itv) }; }
    Date Date::init_from_time_interval_since_1970(CTimeInterval itv) { return { dt_cls._init_from_time_interval_since_1970(itv) }; }
    Date Date::distant_future() { return { dt_cls._distant_future() }; }
    Date Date::distant_past() { return { dt_cls._distant_past() }; }
    CTimeInterval Date::time_interval_since_reference_date() { return dt_cls._time_interval_since_reference_date(_date); }
    CTimeInterval Date::time_interval_since_1970() { return dt_cls._time_interval_since_1970(_date); }
    std::string Date::formatted() {
        char d[200];
        memset(d, 0, 200);
        dt_cls._formatted(_date, d);
        return { d };
    }

    /////// EKCalendar //////
    EKCalendar::EKCalendar(CEntityType entity, const EKEventStore &es)
        : _cal { cal_cls._init(entity, es._eventStore) } {}
    EKCalendar::EKCalendar(CCalendarHandle _cal) : _cal { _cal } {}
    EKCalendar::EKCalendar(const EKCalendar & other) : _cal { cal_cls._copy(other._cal) } {}
    EKCalendar::EKCalendar(EKCalendar && other) {
        std::swap(_cal, other._cal);
        other._cal = nullptr;
    }
    EKCalendar::~EKCalendar() noexcept {
        if (_cal != nullptr) cal_cls._deinit(_cal);
        _cal = nullptr;
    }
    bool EKCalendar::allows_content_modification() { return cal_cls._allows_content_modifications(_cal); }
    bool EKCalendar::is_immutable() { return cal_cls._is_immutable(_cal); }
    CGColor EKCalendar::get_cg_color() {
        CGColor c {0.0, 0.0, 0.0};
        cal_cls._get_cg_color(_cal, &c.red, &c.green, &c.blue);
        return c;
    }
    void EKCalendar::set_cg_color(CGColor c) { cal_cls._set_cg_color(_cal, c.red, c.green, c.blue); }
    std::string EKCalendar::get_title() {
        char title[300];
        memset(title, 0, 300);
        cal_cls._get_title(_cal, title);
        return { title };
    }
    void EKCalendar::set_title(const char* title) { cal_cls._set_title(_cal, title); }
    void EKCalendar::set_source(const EKSource & source) { cal_cls._set_source(_cal, source._source); }
    std::string EKCalendar::get_cal_ident() {
        char ident[300];
        memset(ident, 0, 300);
        cal_cls._get_cal_ident(_cal, ident);
        return { ident };
    }

    //////// EKSource /////////
    EKSource::EKSource(const EKSource & other)
        : _source { eksrc_cls._copy(other._source) } {}
    EKSource::EKSource(EKSource && other) {
        std::swap(_source, other._source);
        other._source = nullptr;
    }
    EKSource& EKSource::operator=(EKSource &&other) {
        std::swap(_source, other._source);
        other._source = nullptr;
        return *this;
    }
    EKSource::~EKSource() {
        if (_source != nullptr) eksrc_cls._deinit(_source);
        _source = nullptr;
    }
    CEKSourceType EKSource::source_type() { return eksrc_cls._sourceType(_source); }
    std::string EKSource::source_ident() {
        char ident[300];
        memset(ident, 0, 300);
        eksrc_cls._source_ident(_source, ident);
        return { ident };
    }
    std::string EKSource::title() {
        char title[300];
        memset(title, 0, 300);
        eksrc_cls._title(_source, title);
        return { title };
    }

    //////// DT Calendar ///////
    DTCalendar::DTCalendar()
        : _dtcal { dtcal_cls._init() } {}
    DTCalendar::DTCalendar(const DTCalendar & other)
        : _dtcal { dtcal_cls._copy(other._dtcal) }
    {}
    DTCalendar::DTCalendar(DTCalendar && other) {
        std::swap(_dtcal, other._dtcal);
        other._dtcal = nullptr;
    }
    DTCalendar::~DTCalendar() {
        if (_dtcal != nullptr) dtcal_cls._deinit(_dtcal);
        _dtcal = nullptr;
    }

    int DTCalendar::component(DTCalendarComponent c, const Date & dt) { return dtcal_cls._component(_dtcal, c, dt._date); }
    std::string DTCalendar::timezone_ident() {
        char tz[300];
        memset(tz, 0, 300);
        dtcal_cls._timezone_ident(_dtcal, tz);
        return { tz };
    }
    int DTCalendar::seconds_from_gmt() { return dtcal_cls._seconds_from_gmt(_dtcal); }
    Date DTCalendar::start_of_day(const Date &date) { return { dtcal_cls._start_of_day(_dtcal, date._date) }; }
    Date DTCalendar::date_from_components(DTDateComponents cp) { return { dtcal_cls._date_from_components(_dtcal, cp) }; }
}
