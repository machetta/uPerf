uPerf is an open-source alternative to the DELL DPACK performance monitoring utility.

Format: uPerf <log file name> [<interval in milisecs, 10 - 1000000, default: 1000>]

Run the program without arguments to find available disk counters. You can use them in the configuration file.
Configuration file format: \PhysicalDisk(<Instance>)\<Counter>
Example: \PhysicalDisk(0 C: D:)\Disk Reads/sec

By default, the following counters are monitored:
\PhysicalDisk(0)\Disk Reads/sec
\PhysicalDisk(0)\Disk Writes/sec
\PhysicalDisk(0)\Disk Read Bytes/sec
\PhysicalDisk(0)\Disk Write Bytes/sec
\PhysicalDisk(0)\Avg. Disk Bytes/Read
\PhysicalDisk(0)\Avg. Disk Bytes/Write
\PhysicalDisk(0)\Avg. Disk sec/Write
\PhysicalDisk(0)\Avg. Disk sec/Read
\PhysicalDisk(0)\Avg. Disk Read Queue Length
\PhysicalDisk(0)\Avg. Disk Write Queue Length
