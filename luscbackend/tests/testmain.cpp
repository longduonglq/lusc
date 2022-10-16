// Project AIDA
// Created by Long Duong on 6/12/22.
// Purpose:
//

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <iostream>
#include <fmt/format.h>
#include "../backend.h"
#include "../wrapper.h"

extern "C" int test_handoff() {
    return Catch::Session().run();
}

//TEST_CASE("event store class") {
//    auto es_handle = es_cls._init();
//
//    SECTION("_event_store_ident") {
//        char es_ident[100];
//        memset(es_ident, 0, 100);
//        auto res = es_cls._event_store_ident(es_handle, es_ident, 100);
//        REQUIRE(res == 0);
//        fmt::print("Event Store Identifier: {}\n", es_ident);
//    }
//
//    SECTION("_request_access_to") {
//        char err_msg[100];
//        memset(err_msg, 0, 100);
//        auto res = es_cls._request_access_to(es_handle, CEntityType::event, err_msg);
//        REQUIRE(res == 0);
//        fmt::print("Requesting access to event handle's event. The error message is (empty string indicates success): {}\n", err_msg);
//    }
//
//    SECTION("_refresh_sources_if_needed") {
//         es_cls._refresh_sources_if_needed(es_handle);
//    }
//
//    SECTION("_calendars_for_entity") {
//        CCalendarHandle* calendars = nullptr;
//        int len = 0;
//        es_cls._calendars_for_entity(es_handle, CEntityType::event, &calendars, &len);
//        SECTION("CCalendarClass::") {
//            for (int i = 0; i < len; i++) {
//                CCalendarHandle cal_handle = calendars[i];
//                fmt::print("Calendar #{}\n", i);
//                fmt::print("Calendar handle: {}\n", cal_handle);
//                //// Color /////
//                CCGFloat r = 0.0, g = 0.0, b = 0.0;
//                cal_cls._get_cg_color(cal_handle, &r, &g, &b);
//
//                fmt::print("Calc color (rgb): {}, {}, {}\n", r, g, b);
//                //// bool stuff /////
//                fmt::print("Allow content mod: {}\n", cal_cls._allows_content_modifications(cal_handle));
//                fmt::print("Is immutable: {} \n", cal_cls._is_immutable(cal_handle));
//                /// Title ////
//                char title[100];
//                memset(title, 0, 100);
//                cal_cls._get_title(cal_handle, title);
//                fmt::print("Cal's title: {}\n", title);
//                ///// Ident /////
//                char ident[100];
//                memset(ident, 0, 100);
//                cal_cls._get_cal_ident(cal_handle, ident);
//                fmt::print("Cal's ident: {}\n", ident);
//                fmt::print("\n\n");
//                cal_cls._deinit(cal_handle);
//            }
//        }
//        if (calendars)  c_free(calendars);
//    }
//
//    SECTION("_calendar_with_ident") {
//        CCalendarHandle cal_handle = es_cls._calendar_with_ident(es_handle, "Home");
//        REQUIRE(cal_handle == nullptr);
//        cal_handle = es_cls._calendar_with_ident(es_handle, "5E46E96A-99E3-4178-AF2C-F22782C3CA81");
//        char title[100];
//        memset(title, 0, 100);
//        cal_cls._get_title(cal_handle, title);
//        fmt::print("Title of Calendar with ident `5E46E96A-99E3-4178-AF2C-F22782C3CA81` (should be Home): {}\n", title);
//        REQUIRE(strcmp(title, "Home") == 0);
//        cal_cls._deinit(cal_handle);
//    }
//
//    SECTION("CCalendar create") {
//        CEKSourceHandle * calsources = nullptr;
//        int sourcesLen = 0;
//        CEKSourceHandle icloud_source = nullptr;
//        /* CEventStoreClass::_get_sources */ {
//            es_cls._get_sources(es_handle, &calsources, &sourcesLen);
//            fmt::print("Number of sources: {}\n", sourcesLen);
//            for (int i = 0; i < sourcesLen; i++) {
//                CEKSourceHandle srcHandle = calsources[i];
//
//                fmt::print("Source #{}\n", i);
//                char title[100];
//                memset(title, 0, 100);
//                eksrc_cls._title(srcHandle, title);
//                char ident[100];
//                memset(ident, 0, 100);
//                eksrc_cls._source_ident(srcHandle, ident);
//                fmt::print("Source title: {}\n", title);
//                fmt::print("Source ident: {}\n", ident);
//
//                if (strcmp(title, "iCloud") == 0)
//                    icloud_source = srcHandle;
//            }
//        }
//        CCalendarHandle newcal = cal_cls._init(CEntityType::event, es_handle);
//        const char* title = "TEST_CALC_DISCARD";
//        cal_cls._set_title(newcal, title);
//        cal_cls._set_cg_color(newcal, 0.1176, 0.5647, 1);
//        assert(icloud_source);
//        cal_cls._set_source(newcal, icloud_source);
//
//        char newcal_ident[100];
//        memset(newcal_ident, 0, 100);
//        cal_cls._get_cal_ident(newcal, newcal_ident);
//        fmt::print("New calendar's ident: {}\n", newcal_ident);
//
//        /* CEventStoreClass::_events_matching */ {
//            /* CDTCalendarClass:: testing */
//            CDTCalendarHandle dtCal = dtcal_cls._init();
//            DTDateComponents startComponents = DTDateDefault;
//            startComponents.year = 2022;
//            startComponents.month = 5;
//            startComponents.day = 2;
//            startComponents.hour = 11;
//
//            DTDateComponents endCmps = DTDateDefault;
//            endCmps.year = 2022;
//            endCmps.month = 5;
//            endCmps.day = 10;
//            endCmps.hour = 16;
//
//            CDateHandle startDate = dtcal_cls._date_from_components(
//                    dtCal,
//                    startComponents);
//            CDateHandle endDate = dtcal_cls._date_from_components(
//                    dtCal,
//                    endCmps);
//            /* _get_cal_by_ident */
//            const char* stat134Ident = "A3CBB882-0D33-4283-8480-8E545AC0E623";
//            CCalendarHandle stat134Cal = es_cls._calendar_with_ident(es_handle, stat134Ident);
//            CNSPredicate pred = es_cls._predicate_for_events(
//                    es_handle,
//                    startDate,
//                    endDate,
//                    static_cast<CCalendarHandle const *>(&stat134Cal), // array of 1
//                    1);
//            dt_cls._deinit(startDate);
//            dt_cls._deinit(endDate);
//            cal_cls._deinit(stat134Cal);
//            CEventHandle* eventsMatched = nullptr;
//            int eventsNum = 0;
//            es_cls._events_matching(es_handle, pred, &eventsMatched, &eventsNum);
//            /* CEventClass */
//            for (int i = 0; i < eventsNum; i++) {
//                CEventHandle event = eventsMatched[i];
//                if (i != eventsNum - 1)
//                    REQUIRE(ev_cls._compare_start_dates(eventsMatched[eventsNum - 1], event) == -1);
//                if (i != 0)
//                    REQUIRE(ev_cls._compare_start_dates(eventsMatched[0], event) == +1);
//
//                /* CEventClass::_event_ident */
//                char eventIdent[100];
//                memset(eventIdent, 0, 100);
//                ev_cls._event_ident(event, eventIdent);
//                fmt::print("Event #{} -------------------\n", i);
//                fmt::print("Event's ident: {}\n", eventIdent);
//
//                /* CEventClass::{_get_start_date, _get_end_date} */
//                CDateHandle eventStartDate = ev_cls._get_start_date(event);
//                CDateHandle eventEndDate = ev_cls._get_end_date(event);
//                char eventStartDateStr[100];
//                char eventEndDateStr[100];
//                memset(eventStartDateStr, 0, 100);
//                memset(eventEndDateStr, 0, 100);
//                dt_cls._formatted(eventStartDate, eventStartDateStr);
//                dt_cls._formatted(eventEndDate, eventEndDateStr);
//                fmt::print("Event's start on: {}\n", eventStartDateStr);
//                fmt::print("Event's end on: {}\n", eventEndDateStr);
//                ev_cls._deinit(eventEndDate);
//                ev_cls._deinit(eventStartDate);
//
//                /* CEventClass::_get_is_all_day */
//                fmt::print("Event is all-day?: {}\n", ev_cls._get_is_all_day(event));
//
////                /* CEventClass::_occurrence_date */
////                CDateHandle occurrenceDate = ev_cls._occurrence_date(event);
////                char occurrenceDateStr[100];
////                memset(occurrenceDateStr, 0, 100);
////                dt_cls._formatted(occurrenceDate, occurrenceDateStr);
////                fmt::print("Event's occurrence date: {}\n", occurrenceDateStr);
////                dt_cls._deinit(occurrenceDate);
//
//                /* CEventClass::_calitem_ident */
//                char calitemIdent[100];
//                memset(calitemIdent, 0, 100);
//                ev_cls._calitem_ident(event, calitemIdent);
//                fmt::print("Event's calitem ident: {}\n", calitemIdent);
//
//                /* CEventClass::_calendar */
//                CCalendarHandle ch = ev_cls._get_calendar(event);
//                fmt::print("Event belongs to calendar at addr: {}\n", ch);
//                cal_cls._deinit(ch);
//
//                /* CEventClass::_get_title */
//                char eventTitle[100];
//                memset(eventTitle, 0, 100);
//                ev_cls._get_title(event, eventTitle);
//                fmt::print("Event's title is: {}\n", eventTitle);
//
//                /* CEventClass::_get_notes */
//                char eventNotes[300];
//                memset(eventNotes, 0, 300);
//                if (ev_cls._has_notes(event)) {
//                    int res = ev_cls._get_notes(event, eventNotes, 300);
//                    fmt::print("Event's notes: {}\n", eventNotes);
//                    REQUIRE(res == 0);
//                }
//
//                fmt::print("\n");
//                ev_cls._deinit(event);
//            }
//            c_free(eventsMatched);
//
//            es_cls._deinit_predicate(pred);
//            dtcal_cls._deinit(dtCal);
//        }
//
//        /* CEventStoreClass::_save_calendar */ {
//            char err_msg[100];
//            memset(err_msg, 0, 100);
//            int res = es_cls._save_calendar(es_handle, newcal, false, err_msg);
//            REQUIRE(res == 0);
//            fmt::print("Error when attempted to save calendar: {}\n", err_msg);
//
//            /* CEventStoreClass::_commit */ {
//                char commit_err[100];
//                memset(commit_err, 0, 100);
//                es_cls._commit(es_handle, commit_err);
//                fmt::print("Error when commit changes: {}\n", commit_err);
//            }
//
//            /* Now we put events on this newly created calendar */ {
//                CDTCalendarHandle dtCal_ = dtcal_cls._init();
//                /* CDTCalendarClass::{_timezone_ident, _seconds_from_gmt} */ {
//                    char tzIdent[100];
//                    memset(tzIdent, 0, 100);
//                    dtcal_cls._timezone_ident(dtCal_, tzIdent);
//                    fmt::print("Time Zone identification: {}\n", tzIdent);
//                    fmt::print("Seconds from GMT: {}\n", dtcal_cls._seconds_from_gmt(dtCal_));
//                }
//                CEventHandle evs[10];
//                for (int i = 0; i < 10; i++) {
//                    evs[i] = ev_cls._init(es_handle);
//                    DTDateComponents dtcStart_ = DTDateDefault;
//                    dtcStart_.year = 2022;
//                    dtcStart_.month = 6;
//                    dtcStart_.day = 15 + i;
//                    dtcStart_.hour = 7;
//                    dtcStart_.minute = 30;
//
//                    DTDateComponents dtcEnd_ = DTDateDefault;
//                    dtcEnd_.year = 2022;
//                    dtcEnd_.month = 6;
//                    dtcEnd_.day = 15 + i;
//                    dtcEnd_.hour = 8;
//                    dtcEnd_.minute = 45;
//
//                    CDateHandle dtStart = dtcal_cls._date_from_components(dtCal_, dtcStart_);
//                    CDateHandle dtEnd = dtcal_cls._date_from_components(dtCal_, dtcEnd_);
//                    ev_cls._set_start_date(evs[i], dtStart);
//                    ev_cls._set_end_date(evs[i], dtEnd);
//                    dt_cls._deinit(dtStart);
//                    dt_cls._deinit(dtEnd);
//
//                    ev_cls._set_is_all_day(evs[i], i == 0);
//
//                    ev_cls._set_title(evs[i], fmt::format("Event #{}\n", i + 1).c_str());
//                    ev_cls._set_notes(evs[i], fmt::format("This is note for event #{}\n", i + 1).c_str());
//
//                    ev_cls._set_calendar(evs[i], newcal);
//
//                    char dtStartStr[100];
//                    char dtEndStr[100];
//                    memset(dtStartStr, 0, 100);
//                    memset(dtEndStr, 0, 100);
//                    CDateHandle dts = ev_cls._get_start_date(evs[i]);
//                    CDateHandle dte = ev_cls._get_end_date(evs[i]);
//                    dt_cls._formatted(dts, dtStartStr);
//                    dt_cls._formatted(dte, dtEndStr);
//                    fmt::print("Event's start: {}\n", dtStartStr);
//                    fmt::print("Event's end at: {}\n", dtEndStr);
//                    ev_cls._deinit(dts);
//                    ev_cls._deinit(dte);
//
//                    char err[100];
//                    memset(err, 0, 100);
//                    es_cls._save(es_handle, evs[i], CSpan::this_event, err);
//                    fmt::print("Error when saving event: {} \n", err);
//                }
//                for (int i = 0; i < 10; i++) {
//                    ev_cls._deinit(evs[i]);
//                }
//                dtcal_cls._deinit(dtCal_);
//
//                char saveEventsErr[100];
//                memset(saveEventsErr, 0, 100);
//                es_cls._commit(es_handle, saveEventsErr);
//                fmt::print("Error when committing saving changes: {}\n", saveEventsErr);
//            }
//
//            /* CEventStoreClass::{ _remove_calendar, _get_calendar_with_ident } */ {
//                char newcal_title[100];
//                memset(newcal_title, 0, 100);
//                cal_cls._get_title(newcal, newcal_title);
//                REQUIRE(strcmp(newcal_title, title) == 0);
//
//                char commit_err[100];
//                memset(commit_err, 0, 100);
//                // es_cls._remove_calendar(es_handle, newcal, true, commit_err);
//                fmt::print("Error when removing calendar is: {}\n", commit_err);
//            }
//        }
//
//        /// deinit the EKSource object earlier
//        for (int i = 0; i < sourcesLen; i++) {
//            eksrc_cls._deinit(calsources[i]);
//        }
//        c_free(calsources); /// free the array itself
//
//        cal_cls._deinit(newcal);
//    }
//
//    es_cls._deinit(es_handle);
//}

TEST_CASE("C++") {
    auto es = cx::EKEventStore();

    auto es_ident = es.identification();
    fmt::print("Event Store Ident: {}\n", es_ident);

    {
        try { es.request_access_to_entity(CEntityType::event); }
        catch (std::exception& e) {
            fmt::print("Request access fail with msg: {}\n", e.what());
            throw e;
        }
        fmt::print("Request access successful!\n");
    }
    es.refresh_sources_if_needed();

    {
        auto calendars = es.calendars_for_entity(CEntityType::event);
        int i = 0;
        for (auto& cal: calendars) {
            fmt::print("Calendar #{}\n", i++);
            fmt::print("Calendar handle: {}\n", cal._cal);
            auto color = cal.get_cg_color();
            fmt::print("Calendar color: {}, {}, {}\n", color.red, color.green, color.blue);
            fmt::print("Allow content mod: {}\n", cal.allows_content_modification());
            fmt::print("Is immutable: {}\n", cal.is_immutable());
            fmt::print("Cal's title: {}\n", cal.get_title());
            fmt::print("Cal's ident: {}\n", cal.get_cal_ident());
        }
    }

    {
        REQUIRE_THROWS( es.calendar_with_ident("Home") );
        auto cal = es.calendar_with_ident("5E46E96A-99E3-4178-AF2C-F22782C3CA81");
        REQUIRE((cal.has_value() && cal.value().get_title() == "Home"));
    }
    {
        auto srcs = es.get_sources();
        cx::EKSource icloudSrc { nullptr };
        for (auto& src: srcs) {
            fmt::print("Source title: {}\n", src.title());
            fmt::print("Source ident: {}\n", src.source_ident());
            if (src.title() == "iCloud") {
                icloudSrc = std::move(src);
            }
        }
        auto newcal = cx::EKCalendar(CEntityType::event, es);
        newcal.set_title("TEST_CALC_C++");
        newcal.set_cg_color({0.1176, 0.5647, 1});
        newcal.set_source(icloudSrc);
        fmt::print("New cal's ident: {}\n", newcal.get_cal_ident());

        auto dtcal = cx::DTCalendar();
        auto startComps = DTDateDefault;
        startComps.year = 2022;
        startComps.month = 5;
        startComps.day = 2;
        startComps.hour = 11;
        auto endComps = DTDateDefault;
        endComps.year = 2022;
        endComps.month = 5;
        endComps.day = 10;
        endComps.hour = 16;
        auto startDate = dtcal.date_from_components(startComps);
        auto endDate = dtcal.date_from_components(endComps);
        const char* stat134Ident = "A3CBB882-0D33-4283-8480-8E545AC0E623";
        auto stat134Cal = es.calendar_with_ident(stat134Ident).value();
        auto pred = es.predicate_for_events(startDate, endDate, { std::addressof(stat134Cal ) });
        auto evs = es.events_matching(pred);
        for (auto it = evs.begin(); it != evs.end(); it++) {
            if (it != --evs.end())
                REQUIRE(std::prev(evs.end())->compare_start_dates_with(*it) == -1);
            if (it != evs.begin())
                REQUIRE(evs[0].compare_start_dates_with(*it) == +1);

            fmt::print("Event #{} --------------- \n", std::distance(evs.begin(), it));
            fmt::print("Event's ident: {}\n", it->event_identification());

            fmt::print("Event starts on: {}\n", it->get_start_date().formatted());
            fmt::print("Event ends on: {}\n", it->get_end_date().formatted());
            fmt::print("Event is all day?: {}\n", it->get_is_all_day());
            fmt::print("Event's calitem ident: {}\n", it->calitem_ident());
            fmt::print("Event belongs to cal at: {}\n", it->get_calendar()._cal);
            fmt::print("Event's title is: {}\n", it->get_title());
            auto nt = it->get_notes();
            if (nt.has_value()) fmt::print("Event's notes: {}\n", it->get_notes().assume_value());
            else fmt::print("Event doesn't have note\n");
            fmt::print("\n");
        }

        try{ es.save_calendar(newcal, true); }
        catch (std::exception& e) {
            fmt::print("Error when attempted to save and commit calendar: {}\n", e.what());
            throw e;
        }

        try{ es.commit(); }
        catch(std::exception& e) { fmt::print("Commit error: {}\n", e.what());}

        fmt::print("Tz ident: {}\n", dtcal.timezone_ident());
        fmt::print("Secs from GMT: {}\n", dtcal.seconds_from_gmt());
        for (int i = 0; i < 10; i ++) {
            DTDateComponents dtcStart_ = DTDateDefault;
            dtcStart_.year = 2022;
            dtcStart_.month = 6;
            dtcStart_.day = 15 + i;
            dtcStart_.hour = 7;
            dtcStart_.minute = 30;

            DTDateComponents dtcEnd_ = DTDateDefault;
            dtcEnd_.year = 2022;
            dtcEnd_.month = 6;
            dtcEnd_.day = 15 + i;
            dtcEnd_.hour = 8;
            dtcEnd_.minute = 45;

            auto start = dtcal.date_from_components(dtcStart_);
            auto end = dtcal.date_from_components(dtcEnd_);

            auto ev = cx::EKEvent(es);
            ev.set_start_date(start);
            ev.set_end_date(end);
            ev.set_is_all_day(i == 0);
            ev.set_title(fmt::format("Event++ #{}", i + 1).c_str());
            ev.set_notes(fmt::format("Note for event++ #{}", i + 1).c_str());
            ev.set_calendar(newcal);

            fmt::print("Event starts on: {}\n", ev.get_start_date().formatted());
            fmt::print("Event ends on: {}\n", ev.get_end_date().formatted());

            try{ es.save(ev, CSpan::this_event); }
            catch (std::exception& e) {
                fmt::print("Error when saving event: {}\n", e.what());
            }
        }
        try{ es.commit(); }
        catch(std::exception& e) { fmt::print("Commit error: {}\n", e.what());}
    }
}