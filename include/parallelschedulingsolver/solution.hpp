#pragma once

#include "parallelschedulingsolver/instance.hpp"

namespace parallelschedulingsolver
{

/**
 * Solution class.
 */
class Solution
{

public:

    /**
     * Structure for the schedule of a job in the solution.
     */
    struct ScheduledJob
    {
        /** Job id. */
        JobId job_id = -1;

        /** Start time. */
        Time start = 0;

        /** Completion time. */
        Time completion = 0;
    };

    /**
     * Structure for a machine schedule in the solution.
     */
    struct MachineSchedule
    {
        /** Jobs scheduled on the machine, in order. */
        std::vector<ScheduledJob> jobs;

        /** Makespan of this machine. */
        Time makespan = 0;

        /** Total (weighted) flow time contributed by this machine. */
        Time total_flow_time = 0;

        /** Total (weighted) tardiness contributed by this machine. */
        Time total_tardiness = 0;

        /** Maximum lateness contributed by this machine. */
        Time maximum_lateness = std::numeric_limits<Time>::min();
    };

    struct JobInfo
    {
        MachineId machine_id = -1;

        JobId job_pos = -1;
    };

    /*
     * Getters
     */

    /** Get the instance of the solution. */
    const Instance& instance() const { return *this->instance_; }

    /** Get a machine schedule. */
    const MachineSchedule& machine(MachineId machine_id) const { return this->machines_[machine_id]; }

    /** Get the number of jobs in the solution. */
    JobId number_of_jobs() const { return this->number_of_jobs_; }

    /** Get the info of a job in the solution. */
    const JobInfo& job(JobId job_id) const { return this->jobs_[job_id]; }

    /** Check if the solution contains a given job. */
    bool contains(JobId job_id) const { return this->jobs_[job_id].machine_id != -1; }

    /** Get the makespan of the solution. */
    Time makespan() const { return makespan_; }

    /** Get the total (weighted) flow time of the solution. */
    Time total_flow_time() const { return total_flow_time_; }

    /** Get the throughput (weighted) of the solution. */
    Time throughput() const { return throughput_; }

    /** Get the total tardiness of the solution. */
    Time total_tardiness() const { return total_tardiness_; }

    /** Get the maximum lateness of the solution. */
    Time maximum_lateness() const { return maximum_lateness_; }

    /** Check if the solution is feasible. */
    bool feasible() const;

    /** Check if the solution is strictly better than another solution. */
    bool strictly_better(const Solution& solution) const;

    /*
     * Export
     */

    /** Write the solution to a file. */
    void write(
            const std::string& certificate_path,
            const std::string& format = "json") const;

    /** Export solution characteristics to a JSON structure. */
    nlohmann::json to_json() const;

    /** Write a formatted output of the solution to a stream. */
    void format(
            std::ostream& os,
            int verbosity_level = 1) const;

private:

    /*
     * Private methods
     */

    /** Create a solution manually. */
    Solution() { }

    /*
     * Private attributes
     */

    /** Instance. */
    const Instance* instance_;

    /** Machine schedules. */
    std::vector<MachineSchedule> machines_;

    /** Job infos. */
    std::vector<JobInfo> jobs_;

    /** Number of jobs assigned. */
    JobId number_of_jobs_ = 0;

    /** Makespan. */
    Time makespan_ = 0;

    /** Total (weighted) flow time. */
    Time total_flow_time_ = 0;

    /** Weighted throughput. */
    Time throughput_ = 0;

    /** Total (weighted) tardiness. */
    Time total_tardiness_ = 0;

    /** Maximum lateness. */
    Time maximum_lateness_ = std::numeric_limits<Time>::min();

    friend class SolutionBuilder;

};

}
