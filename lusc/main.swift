//
//  main.swift
//  lusc
//
//  Created by mqp on 6/7/22.
//

import Foundation
import EventKit

class clsDTCalendar: NSCopying {
    var _value: Foundation.Calendar;
    
    init () { _value = Calendar.current; }
    func component(component: Calendar.Component, from date: Date) -> Int { _value.component(component, from: date) }
    func dateComponents(components: Set<Calendar.Component>, from date: Date) -> DateComponents { _value.dateComponents(components, from: date) }
    func dateComponents(components: Set<Calendar.Component>, from start: Date, to end: Date) -> DateComponents { _value.dateComponents(components, from: start, to: end)}
    
    func timeZoneIdentifier() -> String { _value.timeZone.identifier }
    func secondsFromGMT() -> Int { _value.timeZone.secondsFromGMT() }
    
    func startOfDay(for date: clsDate) -> clsDate { clsDate(_value: _value.startOfDay(for: date._value)) }
    func enumerateDates(startingAfter: clsDate, matching: DateComponents, matchingPolicy: Calendar.MatchingPolicy, repeatedTimePolicy: Calendar.RepeatedTimePolicy, direction: Calendar.SearchDirection, using: (Date?, Bool, inout Bool) -> Void ) {
        _value.enumerateDates(startingAfter: startingAfter._value, matching: matching, matchingPolicy: matchingPolicy, using: using)
    }
    func nextDate(after: Date, matching: DateComponents, matchingPolicy: Calendar.MatchingPolicy, repeatedTimePolicy: Calendar.RepeatedTimePolicy, direction: Calendar.SearchDirection) -> Date? {
            _value.nextDate(after: after, matching: matching, matchingPolicy: matchingPolicy, repeatedTimePolicy: repeatedTimePolicy, direction: direction) }
    
    func date(from dcmps: DateComponents) -> Date? { _value.date(from: dcmps) }
    func copy(with zone: NSZone? = nil) -> Any {
        var c = clsDTCalendar();
        c._value = _value;
        return c;
    }
}

var dtcal_cls = CDTCalendarClass (
    _init: {
        () -> CDTCalendarHandle? in
        return Unmanaged<clsDTCalendar>.passRetained(clsDTCalendar()).toOpaque();
    },
    _incref: {
        (this: CDTCalendarHandle? ) -> Void in
        Unmanaged<clsDTCalendar>.fromOpaque(this!).retain();
    },
    _copy: {
        (this: CDTCalendarHandle?) -> CDTCalendarHandle? in
        let self_ = unsafeBitCast(this!, to: clsDTCalendar.self);
        return Unmanaged<clsDTCalendar>.passRetained(self_.copy() as! clsDTCalendar).toOpaque();
    },
    _deinit: {
        (this: CDTCalendarHandle?) -> CLong in
        let self_ = unsafeBitCast(this!, to: clsDTCalendar.self);
        let refs = CFGetRetainCount(self_);
        Unmanaged<clsDTCalendar>.fromOpaque(this!).takeRetainedValue();
        return refs;
    },
    _component: {
        (this: CDTCalendarHandle?, component: DTCalendarComponent, date: CDateHandle?) -> CInt in
        let dtcal = unsafeBitCast(this!, to: clsDTCalendar.self);
        let dt = unsafeBitCast(date!, to: clsDate.self);
        return CInt(dtcal.component(
            component: {
                switch component {
                case era: return Calendar.Component.era;
                case year: return Calendar.Component.year;
                case yearForWeekOfYear: return Calendar.Component.yearForWeekOfYear;
                case quarter: return Calendar.Component.quarter;
                case month: return Calendar.Component.month;
                case weekOfYear: return Calendar.Component.weekOfYear;
                case weekOfMonth: return Calendar.Component.weekOfMonth;
                case weekday: return Calendar.Component.weekday;
                case weekdayOrdinal: return Calendar.Component.weekdayOrdinal;
                case day: return Calendar.Component.day;
                case hour: return Calendar.Component.hour;
                case minute: return Calendar.Component.minute;
                case second: return Calendar.Component.second;
                case nanosecond: return Calendar.Component.nanosecond;
                case calendar: return Calendar.Component.calendar;
                case timeZone: return Calendar.Component.timeZone;
                default:
                    objc_terminate();
                }
            }(),
            from: dt._value));
    },
    _timezone_ident: {
        (dtcalc: CDTCalendarHandle?, out: UnsafeMutablePointer<CChar>?) -> Void in
        let dtcalc = unsafeBitCast(dtcalc!, to: clsDTCalendar.self);
        let tzid: NSString? = NSString(string: dtcalc.timeZoneIdentifier());
        out!.assign(from: tzid!.utf8String!, count: tzid!.length);
    },
    _seconds_from_gmt: {
        (dtcalc: CDTCalendarHandle?) -> CInt in
        let dtcalc = unsafeBitCast(dtcalc!, to: clsDTCalendar.self);
        return CInt(dtcalc.secondsFromGMT());
    },
    _start_of_day: {
        (dtcalc: CDTCalendarHandle?, date: CDateHandle?) -> CDateHandle? in
        let dtcalc = unsafeBitCast(dtcalc!, to: clsDTCalendar.self);
        let date = unsafeBitCast(date, to: clsDate.self);
        return Unmanaged<clsDate>.passRetained(dtcalc.startOfDay(for: date)).toOpaque();
    },
    _date_from_components: {
        (this: CDTCalendarHandle?, dtcmp: DTDateComponents) -> CDateHandle? in
        let dtcalc = unsafeBitCast(this!, to: clsDTCalendar.self);
        let sw_date_components = DateComponents(
            calendar: dtcalc._value,
            timeZone: dtcalc._value.timeZone,
            era: dtcmp.era < 0 ? nil : Int(dtcmp.era),
            year: dtcmp.year < 0 ? nil : Int(dtcmp.year),
            month: dtcmp.month < 0 ? nil : Int(dtcmp.month),
            day: dtcmp.day < 0 ? nil : Int(dtcmp.day),
            hour: dtcmp.hour < 0 ? nil : Int(dtcmp.hour),
            minute: dtcmp.minute < 0 ? nil : Int(dtcmp.minute),
            second: dtcmp.second < 0 ? nil : Int(dtcmp.second),
            nanosecond: dtcmp.nanosecond < 0 ? nil : Int(dtcmp.nanosecond),
            weekday: dtcmp.weekday < 0 ? nil : Int(dtcmp.weekday),
            weekdayOrdinal: dtcmp.weekdayOrdinal < 0 ? nil : Int(dtcmp.weekdayOrdinal),
            quarter: dtcmp.quarter < 0 ? nil : Int(dtcmp.quarter),
            weekOfMonth: dtcmp.weekOfMonth < 0 ? nil : Int(dtcmp.weekOfMonth),
            weekOfYear: dtcmp.weekOfYear < 0 ? nil : Int(dtcmp.weekOfYear),
            yearForWeekOfYear: dtcmp.yearForWeekOfYear < 0 ? nil : Int(dtcmp.yearForWeekOfYear));
        let ret = dtcalc.date(from: sw_date_components);
        let ret_ = Unmanaged<clsDate>.passRetained(clsDate(_value: ret!)).toOpaque();
        return ret_;
    }
);

class clsDate: NSCopying {
    var _value: Date;
    
    init () { _value = Date(); }
    init (_value: Date) { self._value = _value }
    init (timeIntervalSinceNow: TimeInterval)
    { _value = Date(timeIntervalSinceNow: timeIntervalSinceNow); }
    
    init (timeInterval: TimeInterval, since: clsDate)
    { _value = Date(timeInterval: timeInterval, since: since._value); }
    
    init (timeIntervalSinceReferenceDate: TimeInterval)
    { self._value = Date(timeIntervalSinceReferenceDate: timeIntervalSinceReferenceDate); }
    
    init (timeIntervalSince1970: TimeInterval)
    { self._value = Date( timeIntervalSince1970: timeIntervalSince1970 ); }
    
    static var now: clsDate { get { clsDate(_value: Date.now) } }
    static var distantFuture: clsDate { get { clsDate(_value: Date.distantFuture) } }
    static var distantPast: clsDate { get { clsDate(_value: Date.distantPast); } }
    
    func compare(date: clsDate) -> ComparisonResult { self._value.compare(date._value) }
    func distance(to: clsDate) -> TimeInterval { self._value.distance(to: to._value) }
    
    func timeIntervalSince(date: clsDate) -> TimeInterval { self._value.timeIntervalSince(date._value) }
    var timeIntervalSinceNow: TimeInterval { self._value.timeIntervalSinceNow }
    var timeIntervalSinceReferenceDate: TimeInterval { self._value.timeIntervalSinceReferenceDate }
    var timeIntervalSince1970: TimeInterval { self._value.timeIntervalSince1970 }
    static var timeIntervalSinceReferenceDate: TimeInterval { Date.timeIntervalSinceReferenceDate }
    typealias Stride = Date.Stride;
    
    func formatted() -> String { return self._value.formatted(); }
    
    func copy(with zone: NSZone? = nil) -> Any {
        return clsDate(_value: _value);
    }
}

var dt_cls = CDateClass (
    _init: {
        () -> CDateHandle? in
        return Unmanaged<clsDate>.passRetained(clsDate()).toOpaque();
    },
    
    _incref: {
        (this: CDateHandle?) -> Void in
        Unmanaged<clsDate>.fromOpaque(this!).retain();
    },
    
    _copy: {
        (this: CDateHandle?) -> CDateHandle? in
        let d = unsafeBitCast(this!, to: clsDate.self);
        return Unmanaged<clsDate>.passRetained(d.copy() as! clsDate).toOpaque();
    },
    
    _deinit: {
        (this: CDateHandle?) -> CLong in
        let d = Unmanaged<clsDate>.fromOpaque(this!).takeUnretainedValue();
        let refs = CFGetRetainCount(d);
        Unmanaged<clsDate>.fromOpaque(this!).takeRetainedValue();
        return CLong(refs);
    },
    
    _init_from_time_interval_since_now: {
        (interval: CTimeInterval) -> CDateHandle? in
        return Unmanaged<clsDate>.passRetained(clsDate(timeIntervalSinceNow: interval)).toOpaque();
    },
    
    _init_from_time_interval_since_reference_date: {
        (time_interval: CTimeInterval) -> CDateHandle? in
        return Unmanaged<clsDate>.passRetained(clsDate(timeIntervalSinceReferenceDate: time_interval)).toOpaque();
    },
    
    _init_from_time_interval_since_1970: {
        (time_interval: CTimeInterval) -> CDateHandle? in
        return Unmanaged<clsDate>.passRetained(clsDate(timeIntervalSince1970: time_interval)).toOpaque();
    },
    
    _distant_future: {
        () -> CDateHandle? in
        return Unmanaged<clsDate>.passRetained(clsDate.distantFuture).toOpaque();
    },
    
    _distant_past: {
        () -> CDateHandle? in
        return Unmanaged<clsDate>.passRetained(clsDate.distantPast).toOpaque();
    },
    
    _time_interval_since_reference_date: {
        (this: CDateHandle?) -> CTimeInterval in
        let d = unsafeBitCast(this, to: clsDate.self);
        return d.timeIntervalSinceReferenceDate;
    },
    
    _time_interval_since_1970: {
        (this: CDateHandle?) -> CTimeInterval in
        let d = unsafeBitCast(this, to: clsDate.self);
        return d.timeIntervalSince1970;
    },
    
    _formatted: {
        (this: CDateHandle?, out: UnsafeMutablePointer<CChar>?) in
        let d = unsafeBitCast(this, to: clsDate.self);
        let st = d._value.formatted();
        let dstr: NSString? = NSString(utf8String: d.formatted());
        out!.assign(from: dstr!.utf8String!, count: dstr!.length);
    }
);

var ev_cls = CEventClass (
    _init: {
        (es_handle: CEventStoreHandle?) -> CEventHandle? in
        let es = unsafeBitCast(es_handle!, to: EKEventStore.self);
        return Unmanaged<EKEvent>.passRetained(EKEvent(eventStore: es)).toOpaque();
    },
    
    _incref: {
        (this: CEventHandle?) -> Void in
        Unmanaged<EKEvent>.fromOpaque(this!).retain();
    },
    
    _copy: {
        (this: CEventHandle?) -> CEventHandle? in
        let ev = unsafeBitCast(this!, to: EKEvent.self);
        return Unmanaged<EKEvent>.passRetained(ev.copy() as! EKEvent).toOpaque();
    },
    
    _deinit: {
        (this: CEventHandle?) -> CLong in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let refs = CFGetRetainCount(_self);
        Unmanaged<EKEvent>.fromOpaque(this!).takeRetainedValue();
        return CLong(refs);
    },
    
    _compare_start_dates: {
        (this: CEventHandle?, other: CEventHandle?) -> CInt in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let _other = unsafeBitCast(other!, to: EKEvent.self);
        let res = _self.compareStartDate(with: _other);
        switch res {
        case .orderedAscending:
            return 1;
        case .orderedSame:
            return 0;
        case .orderedDescending:
            return -1;
        }
    },
    
    _event_ident: {
        (this: CEventHandle?, out: UnsafeMutablePointer<CChar>?) -> Void in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let ev_ident: NSString? = NSString(utf8String: _self.eventIdentifier);
        out?.assign(from: ev_ident!.utf8String!, count: ev_ident!.length);
    },
    
    _get_start_date: {
        (this: CEventHandle?) -> CDateHandle? in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        return Unmanaged<clsDate>.passRetained(clsDate(_value: _self.startDate!)).toOpaque();
    },
    
    _set_start_date: {
        (this: CEventHandle?, date: CDateHandle?) -> Void in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let _date = unsafeBitCast(date!, to: clsDate.self);
        _self.startDate = _date._value;
    },
    
    _get_end_date: {
        (this: CEventHandle?) -> CDateHandle? in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        return Unmanaged<clsDate>.passRetained(clsDate(_value: _self.endDate!) ).toOpaque();
    },
    
    _set_end_date: {
        (this: CEventHandle?, date: CDateHandle?) -> Void in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let _date = unsafeBitCast(date!, to: clsDate.self);
        _self.endDate = _date._value;
    },
    
    _get_is_all_day: {
        (this: CEventHandle?) -> Bool in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        return _self.isAllDay;
    },
    
    _set_is_all_day: {
        (this: CEventHandle?, all_day: CBool) -> Void in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        _self.isAllDay = all_day;
    },
    
    _occurrence_date: {
        (this: CEventHandle?) -> CDateHandle? in
        assert(false, "this API is problematic and not necessary so is disabled for now");
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        print(_self);
        return Unmanaged<clsDate>.passRetained(clsDate(_value: _self.occurrenceDate)).toOpaque();
    },
    
    _is_detached: {
        (this: CEventHandle?) -> Bool in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        return _self.isDetached;
    },
    
    _calitem_ident: {
        (this: CEventHandle?, out: UnsafeMutablePointer<CChar>?) -> Void in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let calitem_ident: NSString? = NSString(utf8String: _self.calendarItemIdentifier);
        out!.assign(from: calitem_ident!.utf8String!, count: calitem_ident!.length);
    },
    
    _get_calendar: {
        (this: CEventHandle?) -> CCalendarHandle? in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        return Unmanaged<EKCalendar>.passRetained(_self.calendar).toOpaque();
    },
    
    _set_calendar: {
        (this: CEventHandle?, cal: CCalendarHandle?) in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let _cal = unsafeBitCast(cal!, to: EKCalendar.self);
        _self.calendar = _cal;
    },
    
    _get_title: {
        (this: CEventHandle?, out: UnsafeMutablePointer<CChar>?) -> Void in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let str: NSString? = NSString(string: _self.title);
        out!.assign(from: str!.utf8String!, count: str!.length);
    },
    
    _set_title: {
        (this: CEventHandle?, in_title: UnsafePointer<CChar>?) -> Void in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let title: String? = String(cString: in_title!);
        _self.title = title;
    },
    
    _creation_date: {
        (this: CEventHandle?) -> CDateHandle? in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        return Unmanaged<clsDate>.passRetained(clsDate(_value: _self.creationDate!)).toOpaque();
    },
    
    _has_notes: {
        (this: CEventHandle?) -> Bool in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        return _self.hasNotes;
    },
    
    _get_notes: {
        (this: CEventHandle?, out: UnsafeMutablePointer<CChar>?, len: CInt) -> CInt in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let notes: NSString? = NSString(utf8String: _self.notes!);
        if notes!.length > len - 1 {return -1;}
        out!.assign(from: notes!.utf8String!, count: notes!.length);
        return 0;
    },
    
    _set_notes: {
        (this: CEventHandle?, in_notes: UnsafePointer<CChar>?) -> Void in
        let _self = unsafeBitCast(this!, to: EKEvent.self);
        let notes: String? = String(cString: in_notes!);
        _self.notes = notes;
    }
);

var es_cls = CEventStoreClass (
    _init: { () -> CEventStoreHandle? in
        return Unmanaged<EKEventStore>.passRetained(EKEventStore()).toOpaque();
    },
    
    _incref: {
        (this: CEventStoreHandle?) -> Void in
        Unmanaged<EKEventStore>.fromOpaque(this!).retain();
    },
    
    _copy: {
        (this: CEventStoreHandle?) -> CEventStoreHandle? in
        let es = unsafeBitCast(this!, to: EKEventStore.self);
        return Unmanaged<EKEventStore>.passRetained(es.copy() as! EKEventStore).toOpaque();
    },
    
    _deinit: { (this: CEventStoreHandle?) -> CLong in
        let refs = CFGetRetainCount(unsafeBitCast(this, to: EKEventStore.self));
        Unmanaged<EKEventStore>.fromOpaque(this!).takeRetainedValue();
        return CLong(refs);
    },
    
    _event_store_ident: { (this: CEventStoreHandle?, out: UnsafeMutablePointer<CChar>?, max_length: CInt) -> CInt in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        var ident = es.eventStoreIdentifier.utf8CString;
        
        if ident.count > max_length {
            return 1;
        }
        else {
            ident.withUnsafeBytes({
                (buf: UnsafeRawBufferPointer) in
                let b = unsafeBitCast(buf.baseAddress!, to: UnsafePointer<CChar>.self);
                out!.assign(from: b, count: ident.count)
            })
            return 0;
        }
    },
    
    _request_access_to: { (this: CEventStoreHandle?, entity: CEntityType, out_err_buffer: UnsafeMutablePointer<CChar>? ) -> CInt in
        
        let es = unsafeBitCast(this, to: EKEventStore.self);
        var authorized: CInt = 0;
        var err_msg: NSString? = nil;
        es.requestAccess(
            to: {
                switch entity {
                case event: return EKEntityType.event;
                case reminder: return EKEntityType.reminder;
                default: return EKEntityType.event;
                }
            }(),
            completion: {
                (granted, error) in
                authorized = granted ? 0 : 1;
                if !granted && error != nil {
                    err_msg = error!.localizedDescription as NSString;
                }
            })
        if err_msg != nil {
            out_err_buffer!.assign(from: err_msg!.utf8String!, count: err_msg!.length)
        }
        return authorized;
    },
    
    _get_sources: {
        (this: CEventStoreHandle?,
         out_sources: UnsafeMutablePointer<UnsafeMutablePointer<CEKSourceHandle?>?>?,
         len: UnsafeMutablePointer<CInt>?) -> Void in
        let es = unsafeBitCast(this!, to: EKEventStore.self);
        let sources = es.sources;
        
        let mem_start = c_malloc(sources.count * MemoryLayout<CEKSourceHandle>.stride)!;
        memset(mem_start, 0, sources.count * MemoryLayout<CEKSourceHandle>.stride);
        let mem_buffer_ptr = UnsafeMutableBufferPointer<CEKSourceHandle>(
            start: unsafeBitCast(mem_start, to: UnsafeMutablePointer<CEKSourceHandle>.self),
            count: sources.count
        );
        for i in mem_buffer_ptr.indices {
            mem_buffer_ptr[i] = Unmanaged<EKSource>.passRetained(sources[i]).toOpaque();
        }
        out_sources!.pointee = unsafeBitCast(mem_start, to: UnsafeMutablePointer<CEKSourceHandle?>.self);
        len!.pointee = CInt(sources.count);
    },
    
    _commit: { (this: CEventStoreHandle?, out_err_msg: UnsafeMutablePointer<CChar>?) -> CInt in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        do {
            try es.commit();
        } catch {
            let err_str: NSString? = NSString(utf8String: error.localizedDescription) ;
            out_err_msg!.assign(from: err_str!.utf8String!, count: err_str!.length)
            return 1;
        }
        return 0;
    },
    
    _reset: {(this: CEventStoreHandle?) -> Void in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        es.reset();
    },
    
    _refresh_sources_if_needed: {(this: CEventStoreHandle?) -> Void in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        es.refreshSourcesIfNecessary();
    },
    
    _calendars_for_entity: {
        (this: CEventStoreHandle?,
         entity: CEntityType,
         out_array: UnsafeMutablePointer<UnsafeMutablePointer<CCalendarHandle?>?>?,
         len: UnsafeMutablePointer<CInt>?
        ) -> Void in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        let cals = es.calendars(for: {
            switch entity {
            case event: return EKEntityType.event;
            case reminder: return EKEntityType.reminder;
            default: return EKEntityType.event;
            }}());
        
        let mem_start = c_malloc(cals.count * MemoryLayout<CCalendarHandle>.stride)!;
        memset(mem_start, 0, cals.count * MemoryLayout<CCalendarHandle>.stride);
        let mem_buffer_ptr = UnsafeMutableBufferPointer<CCalendarHandle>(
            start: unsafeBitCast(mem_start, to: UnsafeMutablePointer<CCalendarHandle>.self),
            count: cals.count);
        
        for i in mem_buffer_ptr.indices {
            mem_buffer_ptr[i] = Unmanaged<EKCalendar>.passRetained(cals[i]).toOpaque();
        }
        
        out_array!.pointee = unsafeBitCast(mem_start, to: UnsafeMutablePointer<CCalendarHandle?>.self);
        len!.pointee = CInt(cals.count);
    },
    
    _calendar_with_ident: {
        (this: CEventStoreHandle?, ident: UnsafePointer<CChar>?) -> CCalendarHandle? in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        let ident_str: String = String(cString: ident!);
        let cal = es.calendar(withIdentifier: ident_str);
        if cal != nil {
            return Unmanaged<EKCalendar>.passRetained(cal!).toOpaque();
        }
        return nil;
    },
    
    _save_calendar: {
        (this: CEventStoreHandle?, cal: CCalendarHandle?, commit: Bool, out_err: UnsafeMutablePointer<CChar>?) -> CInt in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        let cal = unsafeBitCast(cal, to: EKCalendar.self);
        do {
            try es.saveCalendar(cal, commit: commit)
        } catch {
            let err_str: NSString? = NSString(utf8String: error.localizedDescription) ;
            out_err!.assign(from: err_str!.utf8String!, count: err_str!.length)
            return 1;
        }
        return 0;
    },
    
    _remove_calendar: {
        (this: CEventStoreHandle?, cal: CCalendarHandle?, commit: Bool, out_err: UnsafeMutablePointer<CChar>?) -> CInt in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        let cal = unsafeBitCast(cal, to: EKCalendar.self);
        do {
            try es.removeCalendar(cal, commit: commit)
        } catch {
            let err_str: NSString? = NSString(utf8String: error.localizedDescription) ;
            out_err!.assign(from: err_str!.utf8String!, count: err_str!.length)
            return 1;
        }
        return 0;
    },
    
    _event_with_ident: {
        (this: CEventStoreHandle?, ident: UnsafePointer<CChar>?) in
        let es = unsafeBitCast(this, to: EKEventStore.self);
        let ident_str = String(cString: ident!);
        let event = es.event(withIdentifier: ident_str);
        if event != nil {
            return Unmanaged<EKEvent>.passRetained(event!).toOpaque();
        }
        return nil;
    },
    
    _remove: {
        (this: CEventStoreHandle?,
         event: CEventHandle?,
         c_span: CSpan,
         err_out: UnsafeMutablePointer<CChar>?
        ) -> CInt in
        let es = unsafeBitCast(this!, to: EKEventStore.self);
        let event = unsafeBitCast(event!, to: EKEvent.self);
        do {
            try es.remove(event, span: {
                switch c_span {
                case this_event:
                    return EKSpan.thisEvent;
                case future_events:
                    return EKSpan.futureEvents;
                default:
                    return EKSpan.thisEvent;
                }
            }());
        } catch {
            let err_str = NSString(string: error.localizedDescription);
            err_out!.assign(from: err_str.utf8String!, count: err_str.length);
            return -1;
        }
        return 0;
    },
    
    _save: {
        (this: CEventStoreHandle?,
         event: CEventHandle?,
         c_span: CSpan,
         err_out: UnsafeMutablePointer<CChar>?
        ) -> CInt in
        let es = unsafeBitCast(this!, to: EKEventStore.self);
        let event = unsafeBitCast(event!, to: EKEvent.self);
        do {
            try es.save(
                event,
                span: {
                    switch c_span {
                    case this_event:
                        return EKSpan.thisEvent;
                    case future_events:
                        return EKSpan.futureEvents;
                    default:
                        return EKSpan.thisEvent;
                    }}(),
                commit: false
                );
        } catch {
            let err_str = NSString(string: error.localizedDescription);
            err_out!.assign(from: err_str.utf8String!, count: err_str.length);
            return -1;
        }
        return 0;
    },
    
    _predicate_for_events: {
        (this: CEventStoreHandle?,
         start: CDateHandle?,
         end: CDateHandle?,
         cals: UnsafePointer<CCalendarHandle?>?,
         len: CInt) -> CNSPredicate? in
        let es = unsafeBitCast(this!, to: EKEventStore.self);
        let start_date = unsafeBitCast(start!, to: clsDate.self);
        let end_date = unsafeBitCast(end!, to: clsDate.self);
        if cals != nil {
            let cals_buf = UnsafeBufferPointer<CCalendarHandle?>(start: cals!, count: Int(len));
            
            let cals_array: [EKCalendar] = cals_buf.map { (raw_handle: CCalendarHandle?) -> EKCalendar in
                return unsafeBitCast(raw_handle!, to: EKCalendar.self)
            }
            let pred = es.predicateForEvents(withStart: start_date._value, end: end_date._value, calendars: cals_array);
            return Unmanaged<NSPredicate>.passRetained(pred).toOpaque();
        }
        let pred = es.predicateForEvents(withStart: start_date._value, end: end_date._value, calendars: nil);
        return Unmanaged<NSPredicate>.passRetained(pred).toOpaque();
    },
    
    _deinit_predicate: {
        (_pred: CNSPredicate?) -> Void in
        Unmanaged<NSPredicate>.fromOpaque(_pred!).takeRetainedValue();
    },
    
    _events_matching: {
        (this: CEventStoreHandle?,
         c_pred: CNSPredicate?,
         out_evts: UnsafeMutablePointer<UnsafeMutablePointer<CEventHandle?>?>?,
         len: UnsafeMutablePointer<CInt>?) -> Void in
        
        let estore = Unmanaged<EKEventStore>.fromOpaque(this!).takeUnretainedValue();
        let pred = Unmanaged<NSPredicate>.fromOpaque(c_pred!).takeUnretainedValue();
        let events = estore.events(matching: pred);
        
        let mem_start = c_malloc(events.count * MemoryLayout<CEventHandle>.stride)!;
        memset(mem_start, 0, events.count * MemoryLayout<CEventHandle>.stride);
        let mem_buffer_ptr = UnsafeMutableBufferPointer<CEventHandle>(
            start: unsafeBitCast(mem_start, to: UnsafeMutablePointer<CEventHandle>.self),
            count: events.count);
        
        for i in mem_buffer_ptr.indices {
            mem_buffer_ptr[i] = Unmanaged<EKEvent>.passRetained(events[i]).toOpaque();
        }
        
        out_evts!.pointee = unsafeBitCast(mem_start, to: UnsafeMutablePointer<CEventHandle?>.self);
        len!.pointee = CInt(events.count);
    }
);

let eksrc_cls = CEKSourceClass (
    _incref: {
        (this: CEKSourceHandle?) -> Void in
        Unmanaged<EKSource>.fromOpaque(this!).retain();
    },
    
    _copy: {
        (this: CEKSourceHandle?) -> CEKSourceHandle? in
        let s = unsafeBitCast(this!, to: EKSource.self);
        return Unmanaged<EKSource>.passRetained(s.copy() as! EKSource).toOpaque();
    },
    
    _deinit: {
        (this: CEKSourceHandle?) -> CLong in
        let self_ = unsafeBitCast(this!, to: EKSource.self);
        let refs = CFGetRetainCount(self_);
        Unmanaged<EKSource>.fromOpaque(this!).takeRetainedValue();
        return CLong(refs);
    },
    _sourceType: {
        (this: CEKSourceHandle?) -> CEKSourceType in
        let self_ = unsafeBitCast(this!, to: EKSource.self);
        switch self_.sourceType {
        case .local: return local;
        case .exchange: return exchange;
        case .calDAV: return calDAV;
        case .mobileMe: return mobileMe;
        case .subscribed: return subscribed;
        case .birthdays: return birthdays;
        default: objc_terminate();
        }
    },
    _source_ident: {
        (this: CEKSourceHandle?, out_ident: UnsafeMutablePointer<CChar>?) in
        let self_ = unsafeBitCast(this!, to: EKSource.self);
        let id: NSString? = NSString(string: self_.sourceIdentifier);
        out_ident!.assign(from: id!.utf8String!, count: id!.length);
    },
    _title: {
        (this: CEKSourceHandle?, out_title: UnsafeMutablePointer<CChar>?) in
        let self_ = unsafeBitCast(this!, to: EKSource.self);
        let title: NSString? = NSString(string: self_.title);
        out_title!.assign(from: title!.utf8String!, count: title!.length);
    }
);

let cal_cls = CCalendarClass (
    _init: {(entity: CEntityType, es_handle: CEventStoreHandle?) -> CEventStoreHandle? in
        return Unmanaged<EKCalendar>.passRetained(
            EKCalendar(
                for: {
                    switch entity {
                    case event: return EKEntityType.event;
                    case reminder: return EKEntityType.reminder;
                    default: return EKEntityType.event;
                    }
                }(),
                eventStore: unsafeBitCast(es_handle, to: EKEventStore.self))
        ).toOpaque();
    },
    
    _incref: {
        (this: CCalendarHandle?) -> Void in
        Unmanaged<EKCalendar>.fromOpaque(this!).retain();
    },
    
    _copy: {
        (this: CCalendarHandle?) -> CCalendarHandle? in
        let cal = unsafeBitCast(this!, to: EKCalendar.self);
        return Unmanaged<EKCalendar>.passRetained(cal.copy() as! EKCalendar).toOpaque();
    },
    
    _deinit: {
        (this: CCalendarHandle?) -> CLong in
        let cal = unsafeBitCast(this!, to: EKCalendar.self);
        let refs = CFGetRetainCount(cal);
        Unmanaged<EKCalendar>.fromOpaque(this!).takeRetainedValue();
        return CLong(refs);
    },
    
    _allows_content_modifications: {(this: CCalendarHandle?) -> Bool in
        let cal = unsafeBitCast(this, to: EKCalendar.self);
        return cal.allowsContentModifications;
    },
    
    _is_immutable: { (this: CCalendarHandle?) -> Bool in
        let cal = unsafeBitCast(this, to: EKCalendar.self);
        return cal.isImmutable;
    },
    
    _get_cg_color: { (this: CCalendarHandle?,
                  red: UnsafeMutablePointer<CCGFloat>?,
                  green: UnsafeMutablePointer<CCGFloat>?,
                  blue: UnsafeMutablePointer<CCGFloat>?) -> Void in
        let cal = unsafeBitCast(this, to: EKCalendar.self);
        var color = cal.cgColor;
        color!.converted(to: CGColorSpace(name: CGColorSpace.sRGB)!, intent: .defaultIntent, options: nil);
        red!.pointee = color!.components![0];
        green!.pointee = color!.components![1];
        blue!.pointee = color!.components![2];
    },
    
    _get_title: { (this: CCalendarHandle?, out_title: UnsafeMutablePointer<CChar>?) -> Void in
        let cal = unsafeBitCast(this, to: EKCalendar.self);
        let title: NSString? = NSString(utf8String: cal.title);
        out_title!.assign(from: title!.utf8String!, count: title!.length)
    },
    
    _get_cal_ident: { (this: CCalendarHandle?, out_ident: UnsafeMutablePointer<CChar>?) -> Void in
        
        let cal = unsafeBitCast(this, to: EKCalendar.self);
        let ident: NSString? = NSString(utf8String: cal.calendarIdentifier);
        out_ident!.assign(from: ident!.utf8String!, count: ident!.length)
    },
    
    _set_cg_color: { (this: CCalendarHandle?,
                      red: CCGFloat,
                      green: CCGFloat,
                      blue: CCGFloat) -> Void in
        let cal = unsafeBitCast(this, to: EKCalendar.self);
        let color = CGColor.init(red: red, green: green, blue: blue, alpha: 1);
        cal.cgColor = color;
    },
    
    _set_title: { (this: CCalendarHandle?, in_title: UnsafePointer<CChar>?) -> Void in
        let cal = unsafeBitCast(this, to: EKCalendar.self);
        let title = String(cString: in_title!);
        cal.title = title;
    },
    
    _set_source: {
        (this: CCalendarHandle?, source: CEKSourceHandle?) in
        let cal = unsafeBitCast(this!, to: EKCalendar.self);
        let source = unsafeBitCast(source!, to: EKSource.self);
        cal.source = source;
    }
);

receive_classes(
    es_cls,
    cal_cls,
    ev_cls,
    dt_cls,
    dtcal_cls,
    eksrc_cls);
c_handoff(CommandLine.argc, CommandLine.unsafeArgv)

