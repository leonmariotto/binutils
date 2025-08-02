#!/usr/bin/env python3

import logging
import subprocess

unit_list = ["elftool_parse", "elftool_getopt"]

def unit_test_run():
    for unit in unit_list:
        cmd = ["make", "-f", "unit_test.mk", f"UNIT_NAME={unit}"]
        logging.debug("Running cmd=[%s]", " ".join(cmd))
        subprocess.run(
                cmd,
                text=True,
                check=True,
        )
    # Create output dir
    subprocess.run(
            ["mkdir", "unit_test_report"],
            text=True,
    )
    base_cmd = ["gcovr"]
    for unit in unit_list:
        base_cmd += ["--add-tracefile", f"unit_test_{unit}/report/coverage.json"]
    cmd = base_cmd + ["--cobertura-pretty", "-o", f"unit_test_report/coverage.xml"]
    logging.debug("Running cmd=[%s]", " ".join(cmd))
    subprocess.run(
            cmd,
            text=True,
            check=True,
    )
    cmd = base_cmd + ["--html", "--html-details", "-o", f"unit_test_report/coverage.html"]
    logging.debug("Running cmd=[%s]", " ".join(cmd))
    subprocess.run(
            cmd,
            text=True,
            check=True,
    )


if __name__ == "__main__":
    unit_test_run()
