class Date:
	@staticmethod
	def init_from_time_interval_since(interval: double, since='now'/'reference_date'/'1970') -> Date:

	def time_interval_since_reference_date(self) -> Date:

	def time_interval_since_1970(self) -> Date:

	@staticmethod
	def distant_future() -> Date:

	@staticmethod
	def distant_past() -> Date:

class DTCalendar:
	def __init__(self):

	def component(self, componenet: DTCalendarComponent, date: Date) -> int:

	@property def timezon_ident(self) -> string:
	@property def seconds_from_gmt(self) -> int:
	@property def start_of_day(self, date: Date) -> Date:

	def date_from_components(self, dateComponents: DTDateComponents) -> Date:

class DTCalendarComponent(Enum):
	era, year, yearForWeekOfYear, quarter, month, weekofYear, weekOfMonth, weekday, weekdayOrdinal,
	day, hour, minute, second, nanosecond, calendar, timeZone

class DTDateComponents:
	pass

class CEKSourceType(Enum):
	local
	exchange
	calDAV
	mobileMe
	subscribed
	birthdays	

class CEntityType(Enum):
	event
	reminder

class EKSource:
	@property
	def source_type(self) -> CEKSourceType:

	@property
	def source_ident(self) -> string:

	@property
	def title(self) -> string:
	
class CGColor:
	def __init__(self):
		self.red, self.green, self.blue = 0, 0, 0

class EKCalendar:
	def __init__(self, entityType: CEntityType, es: EKEventStore):

	@property def allows_content_modification(self) -> bool:
	@property def is_immutable(self) -> bool:

	@property def cg_color(self) -> CGColor:

	@property def title(self) -> string:

	@property(write_only)
	def source(self, source: EKSource):

	@property def cal_ident(self) -> string:

	@property
	def inner_cpp_object_address(self) -> CCalendarHandle:

class EKEvent:
	def __init__(self, es: EKEventStore):

	def compare_start_dates_with(self, other: EKEvent) -> bool:

	@property def event_identification(self) -> string

	@property def start_date(self) 
	@property def end_date(self)

	@property def is_all_day(self):

	@property def calitem_ident(self):

	@property
	def calendar(self):

	@property def title(self):

	@property
	def creation_date(self):

	@property def has_notes(self)
	@property def notes(self):
	@property def append_notes(self):

class NSPredicate:
	pass	

class EKEventStore:
	@property
	def identification(self)

	def request_access_to_entity(self)

	@property(readonly)
	def sources(self)

	def commit(self)
	def reset(self)
	def refresh_sources_if_needed(self)

	def calendars_for_entity(self) -> [EKCalendar]
	def calendar_with_ident(self, ident: string) -> EKCalendar

	def save_calendar(self, cal: EKCalendar, commit: bool)
	def remove_calendar(self, cal: EKCalendar, commit: bool)

	def event_with_identifier(self, ident: string) -> EKEvent
	def remove(self, ev: EKEvent, span: CSpan)
	def save(self, ev: EKEvent, span: CSpan)
	def predicate_for_events(self, start: Date, end: Date, calendars: [CCalendarHandle])
	def events_matching(self, pred: NSPredicate) -> [EKEvent]
