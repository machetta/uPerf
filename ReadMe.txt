========================================================================
    CONSOLE APPLICATION : uPerf Project Overview
========================================================================

uPerf is an open-source alternative to the DELL DPACK performance monitoring utility.

Format: uPerf <log file name> [<interval in milisecs, 10 - 1000000, default: 1000>]

The data displayed below can be used to update the configuration file.
Configuration file format: \\PhysicalDisk(<Instance>)\\<Counter>
Example: \\PhysicalDisk(0 C: D:)\\Disk Reads/sec

By default the following counters will be monitored:
\PhysicalDisk(0)\Disk Reads/sec
\PhysicalDisk(0 C: D:)\Disk Writes/sec
\PhysicalDisk(0 C: D:)\Disk Read Bytes/sec
\PhysicalDisk(0 C: D:)\Disk Write Bytes/sec
\PhysicalDisk(0 C: D:)\Avg. Disk Bytes/Read
\PhysicalDisk(0 C: D:)\Avg. Disk Bytes/Write
\PhysicalDisk(0 C: D:)\Avg. Disk sec/Write
\PhysicalDisk(0 C: D:)\Avg. Disk sec/Read
\PhysicalDisk(0 C: D:)\Avg. Disk Read Queue Length
\PhysicalDisk(0 C: D:)\Avg. Disk Write Queue Length
