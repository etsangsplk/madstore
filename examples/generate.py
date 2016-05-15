#!/usr/bin/env python2.7

from faker import Faker
from faker.providers import internet, date_time, misc, address, person
from time import time
from datetime import datetime, timedelta

fake = Faker()
fake.seed(time())
fake.add_provider(internet.Provider)
fake.add_provider(date_time.Provider)
fake.add_provider(misc.Provider)
fake.add_provider(address.Provider)

# Number of events to generate:
num_entries = 2000000
# Time period for the generated data:
period_hrs = 8670 # 1 year
hr_inc = period_hrs / float(num_entries)
hr_offset = 0
start_time = datetime(2016,05,15) - timedelta(hours=period_hrs)

for i in range(0, num_entries):
    hr_offset += hr_inc
    print "\t".join([
        fake.date_time_between(start_time, start_time + timedelta(hours=int(hr_offset))).strftime("%y%m%d%H"),
        fake.name(),
        fake.uri() + fake.uri_path(),
        fake.sentence(),
        fake.language_code(),
        fake.user_agent(),
        fake.country_code(),
        fake.city(),
        "1",
        str(fake.random_int(0, 100)),
        str(fake.random_int())
    ]);
