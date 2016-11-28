#!/usr/bin/env python2.7

import jinja2, sys

with open("table_factory.cc", "w") as out:
    out.write(jinja2.Environment(
        loader=jinja2.FileSystemLoader("./")
    ).get_template("table_factory.cc.j2").render(tables=sys.argv[1]))

