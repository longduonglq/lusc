// Project AIDA
// Created by Long Duong on 6/18/22.
// Purpose: 
//

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include "wrapper.h"
#include <fmt/format.h>
#include <boost/filesystem.hpp>

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(pyek, m) {
    py::class_<cx::Date>(m, "Date")
            .def("init_from_time_interval_since",
                 [](double itv, const char* since){
                if (strcmp(since, "now") == 0) return cx::Date::init_from_time_interval_since_now(itv);
                if (strcmp(since, "reference_date") == 0) return cx::Date::init_from_time_interval_since_reference_date(itv);
                if (strcmp(since, "1970") == 0) return cx::Date::init_from_time_interval_since_1970(itv);
                throw cx::EKException("Unknown `since`={}", since);
                },
                 py::arg("since"), py::return_value_policy::move)
            .def("time_interval_since_reference_date", &cx::Date::time_interval_since_reference_date, py::return_value_policy::move)
            .def("time_interval_since_1970", &cx::Date::time_interval_since_1970)
            .def("distant_future", &cx::Date::distant_future, py::return_value_policy::move)
            .def("distant_past", &cx::Date::distant_past, py::return_value_policy::move)
            .def("__repr__", &cx::Date::formatted);

    py::class_<cx::DTCalendar>(m, "DTCalendar")
            .def(py::init<>())
            .def("component", &cx::DTCalendar::component)
            .def_property_readonly("timezone_ident", &cx::DTCalendar::timezone_ident, py::return_value_policy::move)
            .def_property_readonly("seconds_from_gmt", &cx::DTCalendar::seconds_from_gmt)
            .def_property_readonly("start_of_day", &cx::DTCalendar::start_of_day, py::return_value_policy::move)
            .def("date_from_components", &cx::DTCalendar::date_from_components, py::return_value_policy::move)
            .def("__repr__", [](cx::DTCalendar const& self)-> std::string { return fmt::format("<DTCalendar: _dtcal={}>", self._dtcal); } );

    py::enum_<DTCalendarComponent>(m, "DTCalendarComponent")
            .value("era", DTCalendarComponent::era)
            .value("year", DTCalendarComponent::year)
            .value("yearForWeekOfYear", DTCalendarComponent::yearForWeekOfYear)
            .value("quarter", DTCalendarComponent::quarter)
            .value("month", DTCalendarComponent::month)
            .value("weekOfYear", DTCalendarComponent::weekOfYear)
            .value("weekOfMonth", DTCalendarComponent::weekOfMonth)
            .value("weekday", DTCalendarComponent::weekday)
            .value("weekdayOrdinal", DTCalendarComponent::weekdayOrdinal)
            .value("day", DTCalendarComponent::day)
            .value("hour", DTCalendarComponent::hour)
            .value("minute", DTCalendarComponent::minute)
            .value("second", DTCalendarComponent::second)
            .value("nanosecond", DTCalendarComponent::nanosecond)
            .value("calendar", DTCalendarComponent::calendar)
            .value("timeZone", DTCalendarComponent::timeZone)
            .export_values();

    py::class_<DTDateComponents>(m, "DTDateComponents")
            .def(py::init<>([](){ return DTDateDefault; }))
            .def_readwrite("era", &DTDateComponents::era)
            .def_readwrite("year", &DTDateComponents::year)
            .def_readwrite("yearForWeekOfYear", &DTDateComponents::yearForWeekOfYear)
            .def_readwrite("quarter", &DTDateComponents::quarter)
            .def_readwrite("month", &DTDateComponents::month)
            .def_readwrite("isLeapMonth", &DTDateComponents::isLeapMonth)
            .def_readwrite("weekOfMonth", &DTDateComponents::weekOfMonth)
            .def_readwrite("weekOfYear", &DTDateComponents::weekOfYear)
            .def_readwrite("weekday", &DTDateComponents::weekday)
            .def_readwrite("weekdayOrdinal", &DTDateComponents::weekdayOrdinal)
            .def_readwrite("day", &DTDateComponents::day)
            .def_readwrite("hour", &DTDateComponents::hour)
            .def_readwrite("minute", &DTDateComponents::minute)
            .def_readwrite("second", &DTDateComponents::second)
            .def_readwrite("nanosecond", &DTDateComponents::nanosecond)
            .def("__repr__", [](DTDateComponents const& self){ return fmt::format("DTDateComponent[ era={}, year={}, yearForWeekOfYear={}, quarter={}, month={}, weekOfMonth={}, weekOfYear={}, weekday={}, weekdayOrdinal={}, day={}, hour={}, minute={}, second={}, nanosecond={} ]", self.era, self.year, self.yearForWeekOfYear, self.quarter, self.month, self.weekOfMonth, self.weekOfYear, self.weekday, self.weekdayOrdinal, self.day, self.hour, self.minute, self.second, self.nanosecond);});

    py::enum_<CEKSourceType>(m, "CEKSourceType")
            .value("local", CEKSourceType::local)
            .value("exchange", CEKSourceType::exchange)
            .value("calDAV", CEKSourceType::calDAV)
            .value("mobileMe", CEKSourceType::mobileMe)
            .value("subscribed", CEKSourceType::subscribed)
            .value("birthdays", CEKSourceType::birthdays)
            .export_values();

    py::class_<cx::EKSource>(m, "EKSource")
            .def_property_readonly("source_type", &cx::EKSource::source_type)
            .def_property_readonly("source_ident", &cx::EKSource::source_ident, py::return_value_policy::move)
            .def_property_readonly("title", &cx::EKSource::title, py::return_value_policy::move)
            .def("__repr__", [](cx::EKSource const& self) -> std::string { return fmt::format("<EKSource: _source={}>", self._source); });

    py::enum_<CEntityType>(m, "CEntityType")
            .value("event", CEntityType::event)
            .value("reminder", CEntityType::reminder)
            .export_values();

    py::class_<cx::CGColor>(m, "CGColor")
            .def(py::init<double, double, double>())
            .def_readwrite("red", &cx::CGColor::red)
            .def_readwrite("green", &cx::CGColor::green)
            .def_readwrite("blue", &cx::CGColor::blue);

    py::class_<cx::EKCalendar>(m, "EKCalendar")
            .def(py::init<CEntityType, const cx::EKEventStore&>())
            .def_property_readonly("allows_content_modification", &cx::EKCalendar::allows_content_modification)
            .def_property_readonly("is_immutable", &cx::EKCalendar::is_immutable)
            .def_property("cg_color", &cx::EKCalendar::get_cg_color, &cx::EKCalendar::set_cg_color)
            .def_property("title", &cx::EKCalendar::get_title, &cx::EKCalendar::set_title, py::return_value_policy::move)
            .def_property("source", nullptr, &cx::EKCalendar::set_source)
            .def_property("cal_ident", &cx::EKCalendar::get_cal_ident, nullptr, py::return_value_policy::move)
            .def("__repr__", [](cx::EKCalendar const& self) -> std::string { return fmt::format("<EKCalendar: _cal={}>", self._cal); })
            .def_property_readonly("inner_cpp_object_address", [](cx::EKCalendar& ekcal) { return ekcal._cal; }, py::return_value_policy::copy );

    py::class_<cx::EKEvent>(m, "EKEvent")
            .def(py::init<const cx::EKEventStore&>())
            .def("compare_start_dates_with", &cx::EKEvent::compare_start_dates_with)
            .def_property_readonly("event_identification", &cx::EKEvent::event_identification)
            .def_property("start_date", &cx::EKEvent::get_start_date, &cx::EKEvent::set_start_date, py::return_value_policy::move)
            .def_property("end_date", &cx::EKEvent::get_end_date, &cx::EKEvent::set_end_date, py::return_value_policy::move)
            .def_property("is_all_day", &cx::EKEvent::get_is_all_day, &cx::EKEvent::set_is_all_day)
            .def_property_readonly("calitem_ident", &cx::EKEvent::calitem_ident, py::return_value_policy::move)
            .def_property("calendar", &cx::EKEvent::get_calendar, &cx::EKEvent::set_calendar, py::return_value_policy::move, py::return_value_policy::reference)
            .def_property("title", &cx::EKEvent::get_title, &cx::EKEvent::set_title, py::return_value_policy::move)
            .def_property_readonly("creation_date", &cx::EKEvent::creation_date, py::return_value_policy::move)
            .def_property_readonly("has_notes", &cx::EKEvent::has_notes)
            .def_property("notes", &cx::EKEvent::get_notes, &cx::EKEvent::set_notes, py::return_value_policy::move)
            .def("append_notes", &cx::EKEvent::append_notes)
            .def("__repr__", [](cx::EKEvent const& ev) -> std::string { return fmt::format("<EKEvent: _ev={}>", ev._ev); });

    py::class_<cx::NSPredicate>(m, "NSPredicate")
            .def("__repr__", [](cx::NSPredicate const& p){ return fmt::format("<NSPredicate: _pred={}>", p._pred);});

    py::class_<cx::EKEventStore>(m, "EKEventStore")
            .def(py::init<>())
            .def_property_readonly("identification", &cx::EKEventStore::identification, py::return_value_policy::move)
            .def("request_access_to_entity", &cx::EKEventStore::request_access_to_entity)
            .def_property_readonly("sources", &cx::EKEventStore::get_sources, py::return_value_policy::move)
            .def("commit", &cx::EKEventStore::commit)
            .def("reset", &cx::EKEventStore::reset)
            .def("refresh_sources_if_needed", &cx::EKEventStore::refresh_sources_if_needed)
            .def("calendars_for_entity", &cx::EKEventStore::calendars_for_entity, py::return_value_policy::move)
            .def("calendar_with_ident", &cx::EKEventStore::calendar_with_ident,  py::return_value_policy::move)
            .def("save_calendar", &cx::EKEventStore::save_calendar)
            .def("remove_calendar", &cx::EKEventStore::remove_calendar)
            .def("event_with_identifier", &cx::EKEventStore::event_with_identifier, py::return_value_policy::move)
            .def("remove", &cx::EKEventStore::remove)
            .def("save", &cx::EKEventStore::save)
            .def("predicate_for_events", &cx::EKEventStore::predicate_for_events, py::return_value_policy::move)
            .def("events_matching", &cx::EKEventStore::events_matching, py::return_value_policy::move)
            .def("__repr__", [](cx::EKEventStore const& es) { return fmt::format("<EKEventStore: _eventStore={}>", es._eventStore); });

    py::enum_<CSpan>(m, "CSpan")
            .value("this_event", CSpan::this_event)
            .value("future_events", CSpan::future_events)
            .export_values();

//    py::class_<cx::EKException>(m, "EKException")
//            .def_property_readonly("what", &cx::EKException::what, py::return_value_policy::reference);
    py::register_exception<cx::EKException>(m, "EKException");
}

#ifndef LUSC_TESTING
extern "C" int wrapper_handoff(int argc, char** argv) {
    py::scoped_interpreter guard{};

    if (argc < 2) {
        fmt::print("Please supply path of .py!");
        return 1;
    }
    auto path = std::filesystem::path(argv[1]);
    if (path.filename().generic_string() != "api.py") {
        fmt::print("Please supply path of api.py!\n");
        return 1;
    }
    std::ifstream api(path.c_str());
    std::stringstream buffer;
    buffer << api.rdbuf();

    py::exec(buffer.str());

    return 0;
}
#endif

#ifdef LUSC_TESTING
extern "C" int wrapper_handoff() {
    py::scoped_interpreter guard {};

    auto pyek_module = py::module_::import("pyek");
    py::exec(R"(
import pyek as ek
d = ek.Date.init_from_time_interval_since(1999, since='1970')
print(d)
#print(d.time_interval_since_reference_date())

es = ek.EKEventStore()
print(f'Event Store Ident: {es.identification}')
es.refresh_sources_if_needed()

calendars = es.calendars_for_entity(ek.CEntityType.event)
for i, calendar in enumerate(calendars):
    print(f"""
        Calendar #{i + 1}
        Calendar color: {calendar.cg_color.red}, {calendar.cg_color.green}, {calendar.cg_color.blue}
        Allow content mod: {calendar.allows_content_modification}
        Is immutable: {calendar.is_immutable}
        Cal's title: {calendar.title}
        Cal's ident: {calendar.cal_ident}
    """)

assert(es.calendar_with_ident("Home") == None)
assert(es.calendar_with_ident("5E46E96A-99E3-4178-AF2C-F22782C3CA81").title == "Home")

sources = es.sources
icloudSource = None
for src in sources:
    print(f"""
        Source title: {src.title}
        Source ident: {src.source_ident}
    """)
    if src.title == "iCloud":
        icloudSource = src
        print(f"Before move: {src}")
print(f"After moved: {icloudSource}")


newcal = ek.EKCalendar(ek.CEntityType.event, es)
newcal.title = "TEST_CALC_C++"
newcal.cg_color = ek.CGColor(0.1176, 0.5647, 1)
newcal.source = icloudSource
print(f"Newcal's ident: {newcal.cal_ident}")


dtcal = ek.DTCalendar()
startComps = ek.DTDateComponents()
startComps.year = 2022
startComps.month = 5
startComps.day = 2
startComps.hour = 11
print(startComps)

endComps = ek.DTDateComponents()
endComps.year = 2022
endComps.month = 5
endComps.day = 11
endComps.hour = 16
print(endComps)

startDate = dtcal.date_from_components(startComps)
endDate = dtcal.date_from_components(endComps)

stat134Ident = 'A3CBB882-0D33-4283-8480-8E545AC0E623'
stat134Cal = es.calendar_with_ident(stat134Ident)
print(f'startDate={startDate}  endDate={endDate}')
print(stat134Cal)

pred = es.predicate_for_events(startDate, endDate, [stat134Cal.inner_cpp_object_address])

print(pred)
evs = es.events_matching(pred)
for i, ev in enumerate(evs):
    print(f"""
        Event #{i + 1}
        Event's ident: {ev.event_identification}
        Event starts on: {ev.start_date}
        Event ends on: {ev.end_date}
        Event is all day?: {ev.is_all_day}
        Event's calitem ident: {ev.calitem_ident}
        Event belongs to cal at: {ev.calendar}
        Event's title is: {ev.title}
    """)
    if ev.has_notes:
        print(f"Event's notes: {ev.notes}")

try:
    es.save_calendar(newcal, True)
except Exception as e:
    print(f"Err when try to save and commit calendar: {e}")

print(f"Tz ident: {dtcal.timezone_ident}")
print(f"secs from GMI: {dtcal.seconds_from_gmt}")

try:
    es.commit()
except Exception as e:
    print(f"Commit error: {e}")

for i in range(0, 10):
    dtcStart_ = ek.DTDateComponents()
    dtcStart_.year = 2022
    dtcStart_.month = 6
    dtcStart_.day = 15 + i
    dtcStart_.hour = 7
    dtcStart_.minute = 30

    dtcEnd_ = ek.DTDateComponents()
    dtcEnd_.year = 2022
    dtcEnd_.month = 6
    dtcEnd_.day = 15 + i
    dtcEnd_.hour = 8
    dtcEnd_.minute = 45

    start = dtcal.date_from_components(dtcStart_)
    end = dtcal.date_from_components(dtcEnd_)

    ev = ek.EKEvent(es)
    ev.start_date = start
    ev.end_date = end
    ev.is_all_day = (i == 0)
    ev.title = f"Eventpy #{i}"
    ev.notes = f"Note for eventpy #{i}"
    ev.calendar = newcal

    print(f"""
        Event #{i}
        Event starts on: {ev.start_date}
        Event ends on: {ev.end_date}
    """)

    try:
        es.save(ev, ek.CSpan.this_event)
    except Exception as e:
        print(f"Error when saving event: {e}")

try:
    es.commit()
except Exception as e:
    print(f"Commit error: {e}")
    )");
    return 0;
}
#endif
