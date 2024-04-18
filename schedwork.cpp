#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool schedHelper(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, vector<int>& shiftsWorking, size_t row, size_t col);

bool isValid(const AvailabilityMatrix& avail, size_t maxShifts, const DailySchedule& sched, const vector<int>& shiftsWorking, size_t row, size_t col);

void printSchedule(const DailySchedule& sched);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if (avail.empty()) {
        return false;
    }
    sched.clear();
    
    // Initialize the schedule matrix
    for (size_t i = 0; i < avail.size(); ++i) {
        vector<Worker_T> day(dailyNeed, INVALID_ID);
        sched.push_back(day);
    }

    vector<int> shiftsWorking(avail[0].size(), 0);
    return schedHelper(avail, dailyNeed, maxShifts, sched, shiftsWorking, 0, 0);
}

bool schedHelper(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, vector<int>& shiftsWorking, size_t row, size_t col) {
    // Base case: If all rows are processed, return true
    if (row == avail.size()) {
        return true;
    }

    // Determine the indices for the next row and column
    size_t nextRow = row;
    size_t nextCol = col + 1;
    if (nextCol == dailyNeed) {
        nextRow = row + 1;
        nextCol = 0;
    }

    // Try assigning each worker to the current shift
    for (Worker_T worker = 0; worker < avail[0].size(); ++worker) {
        // Assign the worker to the current shift
        sched[row][col] = worker;
        ++shiftsWorking[worker];

        // Check if the assignment is valid
        if (isValid(avail, maxShifts, sched, shiftsWorking, row, col)) {
            // Recursively call the helper function for the next shift
            if (schedHelper(avail, dailyNeed, maxShifts, sched, shiftsWorking, nextRow, nextCol)) {
                return true; // Return true if a valid schedule is found
            } else {
                // Undo the assignment if it leads to an invalid schedule
                sched[row][col] = INVALID_ID;
                --shiftsWorking[worker];
            }
        } else {
            // Undo the assignment if it violates constraints
            sched[row][col] = INVALID_ID;
            --shiftsWorking[worker];
        }
    }

    // Return false if no valid schedule is found for the current configuration
    return false;
}

bool isValid(const AvailabilityMatrix& avail, size_t maxShifts, const DailySchedule& sched, const vector<int>& shiftsWorking, size_t row, size_t col) {
    Worker_T worker = sched[row][col];
    vector<Worker_T> daySchedule = sched[row];
    daySchedule.erase(daySchedule.begin() + col);

    // Check if the worker is assigned to multiple shifts in a day
    if (find(daySchedule.begin(), daySchedule.end(), worker) != daySchedule.end()) {
        return false;
    }

    // Check if the worker is available for the current shift
    if (avail[row][worker] == 0) {
        return false;
    }

    // Check if the worker has exceeded the maximum number of shifts
    if (shiftsWorking[worker] > static_cast<int>(maxShifts)) {
        return false;
    }

    // Return true if all constraints are satisfied
    return true;
}