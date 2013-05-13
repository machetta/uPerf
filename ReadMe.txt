uPerf is an open-source alternative to the DELL DPACK performance monitoring utility.

Format: uPerf <log file name> [<interval in milisecs, 10 - 600000, default: 1000>]

Run the program without arguments to find available disk counters. Copy/paste them into the configuration file.
Configuration file format: \PhysicalDisk(<Instance>)\<Counter>
Example: \PhysicalDisk(0 C: D:)\Disk Reads/sec

By default, the following counters are monitored:
\PhysicalDisk(0 C:)\Disk Reads/sec
\PhysicalDisk(0 C:)\Disk Writes/sec
\PhysicalDisk(0 C:)\Disk Read Bytes/sec
\PhysicalDisk(0 C:)\Disk Write Bytes/sec
\PhysicalDisk(0 C:)\Avg. Disk Bytes/Read
\PhysicalDisk(0 C:)\Avg. Disk Bytes/Write
\PhysicalDisk(0 C:)\Avg. Disk sec/Write
\PhysicalDisk(0 C:)\Avg. Disk sec/Read
\PhysicalDisk(0 C:)\Avg. Disk Read Queue Length
\PhysicalDisk(0 C:)\Avg. Disk Write Queue Length
