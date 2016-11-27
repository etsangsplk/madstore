#!/usr/bin/env python2.7

import jinja2, sys

context = { "dims": int(sys.argv[1]), "metrics": int(sys.argv[2]) }

with open("store_factory.cc", "w") as out:
    out.write(jinja2.Environment(
        loader=jinja2.FileSystemLoader("./")
    ).get_template("store_factory.cc.j2").render(context))

