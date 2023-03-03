# ==========================================================================
# Copyright (C) 2023 Intel Corporation
#
# SPDX-License-Identifier: MIT
# ==========================================================================

import os
import shutil
import subprocess
import argparse
import csv
import platform
import datetime
from bench_utils import *
import accel_conf

has_qpl = True
try: 
    import run_qpl_benchmarks
    from run_qpl_benchmarks import FIELDS_BASE
    from run_qpl_benchmarks import FIELDS
except:
    has_qpl = False

has_dml = True
try: 
    import run_dml_benchmarks
except:
    has_dml = False

has_micros = True
try: 
    import run_micros
except:
    has_micros = False

has_psutil=True
try:
    import psutil
except:
    has_psutil=False


def hrSize(size):
    for order in ["", "K", "M", "G", "T"]:
        if size < 1024:
            return f"{size:.2f}{order}B"
        size /= 1024

def getMachineInfo(path):
    info_file = open(path + "/machine_info.csv", "w+")
    
    print("System info:")
    try:
        print("  BIOS: ", end='')
        version = subprocess.check_output("dmidecode --string bios-version", universal_newlines=True, shell=True).split("\n")[0]
        date    = subprocess.check_output("dmidecode --string bios-release-date", universal_newlines=True, shell=True).split("\n")[0]
        print("")
        print("    Version: " + version)
        print("    Date:    " + date)
    except:
        print("cannot get BIOS info")

    print("  Host:")
    uname = platform.uname()
    print("    Name: " + uname.node)
    print("    OS:   " + uname.system + ", " + uname.release)

    if has_psutil:
        print("  CPU:")
        print("    Cores:     " + str(psutil.cpu_count(logical=False)) + ", " + str(psutil.cpu_count(logical=True)))
        freq = psutil.cpu_freq()
        print("    Frequency: " + str(freq.min) + "-" + str(freq.max) + ", " + str(freq.current))
        print("    Cache:     n/a")
    else:
        print("  CPU: required psutil module")
    
    if has_psutil:
        print("  Memory:")
        mem = psutil.virtual_memory()
        print("    Total:     " + hrSize(mem.total))
        print("    Avaliable: " + hrSize(mem.available))
        #subprocess.check_output("dmidecode -t memory | grep Locator: | grep CPU0", universal_newlines=True, shell=True)
        print("    Channels:  n/a")
    else:
        print("  Memory: required psutil module")
   
    info_file.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Multiconf benchmark runnner')
    if has_qpl:
        parser.add_argument('--qpl-path', default='./qpl_benchmarks', metavar='QPL_PATH', help='Path to QPL benchmark')
    if has_dml:
        parser.add_argument('--dml-path', default='./qpl_benchmarks', metavar='QPL_PATH', help='Path to DML benchmark')
    if has_micros:
        parser.add_argument('--micros-path', default='./qpl_benchmarks', metavar='QPL_PATH', help='Path to Micros benchmark')
    parser.add_argument('--repetitions', default=1, type=int, metavar='REPETITIONS', help='Number of tests repetitions')
    parser.add_argument('--res-path', default='./results/', metavar='RES_PATH', help='Path to results')
    parser.add_argument('--configs', default="", metavar='CONFIGS_PATH', nargs='+', help='Set of config files for iteration')
    parser.add_argument('--summary', default=False, action='store_true', help='Generate summary report across all measurements')
    parser.add_argument('--clean', default=False, action='store_true', help='Clean results folder before measurements')
    parser.add_argument('--no-info', default=False, action='store_true', help='Do not check system information')
    parser.add_argument('--verbose', default=False, action='store_true', help='Verbose output')
    parser.add_argument('--test', default=False, action='store_true', help='Test run with fastest options')

    if has_qpl:
        parser.add_argument('--qpl-data-path', default="", metavar='QPL_DATA_PATH', help='Benchmark filter')
        parser.add_argument('--qpl-filter', default="", metavar='QPL_FILTER', help='QPL Benchmark filter')
        
    if has_dml or has_micros:
        parser.add_argument('--path', default="all", metavar='PATH', help='Execution path: hw, sw, auto, all. Default: all. May be not applicable to some APIs')
        parser.add_argument('--exec', default="all", metavar='EXECUTION', help='Test execution mode: sync, async, all. Default: all')
        parser.add_argument('--api', default="", metavar='API', help='API to run: micros, dml-hl, dml-ml, dml-job')
        parser.add_argument('--case', default="", metavar='CASE', help='Case name to run')
        parser.add_argument('--threads', default=0, type=int, metavar='THREADS', help='Number if threads to use for async submission')
        parser.add_argument('--qsize', default=0, type=int, metavar='QSIZE', help='Specific queue size to test')
        parser.add_argument('--batch', default=-1, type=int, metavar='BATCH_SIZE', help='Specific batch size to test: 0 - do not test; 1 - test batch only;')
        parser.add_argument('--size', default=0, type=int, metavar='SIZE', help='Specific size to test')
    args = parser.parse_args()

    if has_qpl:
        print("QPL path:     ", args.qpl_path)
    if has_dml:
        print("DML path:     ", args.dml_path)
    if has_micros:
        print("Micros path:  ", args.micros_path)
    
    start_time = datetime.datetime.combine(datetime.date.min, datetime.datetime.now().time())
    if args.configs:
        configs = args.configs
    else:
        configs = ["current"]

    if args.clean and os.path.exists(args.res_path):
        shutil.rmtree(args.res_path)

    if not os.path.exists(args.res_path):
        os.makedirs(args.res_path)

    if not args.no_info:
        getMachineInfo(args.res_path)

    for config in configs:
        print("")
        print("Config: " + config)
        if not config == "current":
            accel_conf.config_device(config, "")

        if has_qpl:
            qpl_args = { "bin_path":args.qpl_path,
                         "res_path":args.res_path,
                         "repetitions":args.repetitions,
                         "data_path":args.qpl_data_path,
                         "filter":args.qpl_filter,
                         "test":args.test,
                         "clean":False }

            print("")
            print("Running QPL benchmark")
            print("Time: ", datetime.datetime.now().strftime("%H:%M:%S"))
            run_qpl_benchmarks.run_bench(args=DictAttrib(qpl_args))

        if has_dml:
            if not args.api or "dml" in args.api or "hl" in args.api or "ml" in args.api or "job" in args.api or "high" in args.api or "mid" in args.api or "job" in args.api:
                dml_args = { "bin_path":args.dml_path,
                             "res_path":args.res_path,
                             "repetitions":args.repetitions,
                             "path":args.path,
                             "api":args.api,
                             "case":args.case,
                             "threads":args.threads,
                             "exec":args.exec,
                             "size":args.size,
                             "batch":args.batch,
                             "qsize":args.qsize,
                             "filter":"",
                             "clean":False }
    
                print("")
                print("Running DML benchmark")
                print("Time: ", datetime.datetime.now().strftime("%H:%M:%S"))
                run_dml_benchmarks.run_bench(args=DictAttrib(dml_args))
            else:
                print("Skipping DML benchmark")

        if has_micros:
            if (not args.api or "micros" in args.api) and (args.path == "all" or args.path == "hw"):
                micros_args = { "bin_path":args.micros_path,
                                "res_path":args.res_path,
                                "repetitions":args.repetitions,
                                "case":args.case,
                                "threads":args.threads,
                                "exec":args.exec,
                                "size":args.size,
                                "batch":args.batch,
                                "qsize":args.qsize,
                                "clean":False,
                                "verbose":args.verbose }
    
                print("")
                print("Running dsa micros")
                print("Time: ", datetime.datetime.now().strftime("%H:%M:%S"))
                run_dsa_micros.run_bench(args=DictAttrib(micros_args))
            else:
                print("Skipping dsa micros")

    print("End Time: ", datetime.datetime.now().strftime("%H:%M:%S"))
    print("Elapsed:  ", datetime.datetime.combine(datetime.date.min, datetime.datetime.now().time()) - start_time)
    
    # Build summary
    if args.summary:
        print("")
        print("Building summary report")
        # Build merged report
        merged_name = args.res_path + "/merged.csv"

        merged_old = []
        if os.path.exists(merged_name):
            merged_file = open(merged_name, "r")
            merged_old = list(csv.DictReader(merged_file, delimiter=';'))
            merged_file.close()

        merged_file = open(merged_name, "w+")
        merged = csv.DictWriter(merged_file, fieldnames=FIELDS.keys(), delimiter=';')
        merged.writeheader()

        print("Processing reports:")
        report_paths = [f for f in os.listdir(args.res_path) if os.path.isfile(os.path.join(args.res_path, f))]
        reports = []
        for report_path in report_paths:
            if ("dml" in report_path or "micros" in report_path or "qpl" in report_path) and not "_run_" in report_path :
                print("  " + report_path)
                report_file = open(args.res_path + "/" + report_path, "r")
                reports.append(list(csv.DictReader(report_file, delimiter=';')));
                report_file.close()

        # If old file is present update/merge content
        if merged_old:
            print("Found old merged file, updating")
            for merged_old_line in merged_old:
                merged_descr = ""
                for key in FIELDS_BASE:
                    merged_descr += merged_old_line[key] + "_"
                
                found = False
                for report in reports:
                    for report_line in report:
                        report_descr = ""
                        for key in FIELDS_BASE:
                            report_descr += report_line[key] + "_"
                        
                        if merged_descr == report_descr:
                            try:
                                if float(merged_old_line[LATENCY]) < float(report_line[LATENCY]):
                                    report_line[LATENCY]    = merged_old_line[LATENCY]
                                    report_line[THROUGHPUT] = merged_old_line[THROUGHPUT]
                            except:
                                if not isinstance(report_line[LATENCY], float):
                                    report_line[LATENCY]    = merged_old_line[LATENCY]
                                    report_line[THROUGHPUT] = merged_old_line[THROUGHPUT]
    
                            merged.writerow(report_line)
                            report.remove(report_line)
                            found = True
                            break
                    if found:
                        break
                if not found:
                    merged.writerow(merged_old_line)

            # Write remaining entries
            for report_line in report:
                merged.writerow(report_line)
        else:
            for report in reports:
                for report_line in report:
                    merged.writerow(report_line)

        merged_file.close()
    
        merged_file = open(merged_name, "r")
        merged = list(csv.DictReader(merged_file, delimiter=';'));
        merged_file.close()
    
        apis = []
        for line in merged:
            if line[API] not in apis:
                apis.append(line[API])
    
        SUM_FIELDS_BASE = dict(FIELDS_BASE)
        SUM_FIELDS_BASE.pop(API)
        SUM_FIELDS      = dict(SUM_FIELDS_BASE)
        for api in apis:
            SUM_FIELDS[api + " " + THROUGHPUT] = ""
            SUM_FIELDS[api + " " + LATENCY]    = ""
            
        print("Generating summary file")
        summary_list = []
        for merged_line in merged:
            merged_descr = ""
            for key in SUM_FIELDS_BASE:
                merged_descr += merged_line[key] + "_"
    
            found = False
            for summary_line in summary_list:
                summary_descr = ""
                for key in SUM_FIELDS_BASE:
                    summary_descr += summary_line[key] + "_"
                
                if summary_descr == merged_descr:
                    if summary_line[merged_line[API] + " " + THROUGHPUT] != "":
                        print("Warning: unexpected data overwrite")
                        print(summary_line)
                        print(merged_line)
                    summary_line[merged_line[API] + " " + THROUGHPUT] = merged_line[THROUGHPUT]
                    summary_line[merged_line[API] + " " + LATENCY]    = merged_line[LATENCY]
                    found = True
                    
            if not found:
                field = dict(SUM_FIELDS)
                for key in merged_line:
                    if key in field:
                        field[key] = merged_line[key]
                field[merged_line[API] + " " + THROUGHPUT] = merged_line[THROUGHPUT]
                field[merged_line[API] + " " + LATENCY]    = merged_line[LATENCY]
                summary_list.append(field)
    
        summary_file = open(args.res_path + "/summary.csv", "w+")
        summary = csv.DictWriter(summary_file, fieldnames=SUM_FIELDS.keys(), delimiter=';')
        summary.writeheader()
        for line in summary_list:
            summary.writerow(line)
        summary_file.close()
