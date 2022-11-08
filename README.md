# KapProfiler

KapProfiler is a ready-to-go profiler viewer for **[KapEngine's integrated profiler](https://github.com/benji-35/KapEngine/)**. It uses generated **.kprof** files in a given directory and reads it continuously.

## Features

- list all class and method calls for each thread
- retrieves call counts and average
- retrieves time of method execution and total time
- displays time percentage usage for all methods in a specific thread

## Getting Started

Get **[KapEngine](https://github.com/benji-35/KapEngine/)**, download the last release
of **[KapProfiler](https://github.com/aureliancnx/KapProfiler/releases/)**, extract this in the **Packages** folder of your
KapProfier game and let's go!

Build the executable with **[CMake](https://github.com/Kitware/CMake)** and run it!

  ``$ ./KapProfiler <directory>``
  
Don't forget to put the correct folder where the .kprof files are generated as the first argument (usually the folder from which your application using KapEngine is running)

