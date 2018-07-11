#include "disk_log.h"

DiskLog::DiskLog() : i(0) {

}

void DiskLog::addLogEntry(LogEntry logE) {
    logB.entries[i++] = logE;
    if(i==200) {
        write();
        i=0;
    }
}

void DiskLog::write() {

}
