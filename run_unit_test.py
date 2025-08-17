#!/usr/bin/env python3

import logging
import subprocess
import os

logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s - %(levelname)s - %(name)s - %(message)s",
)

unit_list = ["elftool_parse", "elftool_getopt", "elftool_dump", "elftool_write", "elftool_transform", "elftool_query"]

def unit_test_run():
    for unit in unit_list:
        subprocess.run(
            ["make", "-f", "unit_test.mk", f"UNIT_NAME={unit}", "clean"],
            text=True,
            check=True,
        )

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
            ["rm", "-rf", "unit_test_report"],
            text=True,
            check=True
    )
    subprocess.run(
            ["mkdir", "unit_test_report"],
            text=True,
    )
    base_cmd = ["gcovr"]
    for unit in unit_list:
        report_file = f"unit_test_{unit}/report/coverage.json"
        if not os.path.exists(report_file) or os.path.getsize(report_file) == 0:
            raise FileNotFoundError(f"Individual report not found or empty: {report_file}")
        base_cmd += ["--add-tracefile", report_file]
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
