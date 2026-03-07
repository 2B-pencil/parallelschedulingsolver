#pragma once

#include "parallelschedulingsolver/solution.hpp"

namespace parallelschedulingsolver
{

class SolutionBuilder
{

public:

    /** Constructor. */
    SolutionBuilder() { }

    /** Set the instance of the solution. */
    SolutionBuilder& set_instance(const Instance& instance);

    /** Read a solution from a file. */
    void read(const std::string& certificate_path);

    /** Append a job to the end of a machine's schedule with a given start time. */
    void append_job_to_machine(
            MachineId machine_id,
            JobId job_id,
            Time start);

    /*
     * Build
     */

    /** Build. */
    Solution build();

private:

    /*
     * Private attributes
     */

    /** Solution. */
    Solution solution_;

};

}
