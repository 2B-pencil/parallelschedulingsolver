#include "parallelschedulingsolver/solution_builder.hpp"

using namespace parallelschedulingsolver;

SolutionBuilder& SolutionBuilder::set_instance(
        const Instance& instance)
{
    solution_.instance_ = &instance;
    solution_.machines_ = std::vector<Solution::MachineSchedule>(instance.number_of_machines());
    solution_.jobs_ = std::vector<Solution::JobInfo>(instance.number_of_jobs());
    return *this;
}

void SolutionBuilder::append_job_to_machine(
        MachineId machine_id,
        JobId job_id,
        Time start)
{
    Solution::MachineSchedule& machine_schedule = this->solution_.machines_[machine_id];
    Solution::ScheduledJob scheduled_job;
    scheduled_job.job_id = job_id;
    scheduled_job.start = start;
    scheduled_job.completion = 0; // computed in build()
    machine_schedule.jobs.push_back(scheduled_job);
}

Solution SolutionBuilder::build()
{
    const Instance& instance = this->solution_.instance();
    this->solution_.jobs_ = std::vector<Solution::JobInfo>(instance.number_of_jobs());

    for (MachineId machine_id = 0;
            machine_id < instance.number_of_machines();
            ++machine_id) {
        Solution::MachineSchedule& machine_schedule = this->solution_.machines_[machine_id];

        for (JobId job_pos = 0;
                job_pos < (JobId)machine_schedule.jobs.size();
                ++job_pos) {
            Solution::ScheduledJob& scheduled_job = machine_schedule.jobs[job_pos];
            JobId job_id = scheduled_job.job_id;
            const Job& job = instance.job(job_id);

            // Check for duplicate jobs.
            if (this->solution_.contains(job_id)) {
                throw std::invalid_argument(
                        FUNC_SIGNATURE + ": "
                        "duplicate job in solution; "
                        "job_id: " + std::to_string(job_id) + ".");
            }

            // Compute completion time.
            scheduled_job.completion = scheduled_job.start + job.processing_times[machine_id];

            // Update job info.
            Solution::JobInfo& job_info = this->solution_.jobs_[job_id];
            job_info.machine_id = machine_id;
            job_info.job_pos = job_pos;

            // Update machine metrics.
            machine_schedule.makespan = (std::max)(
                    machine_schedule.makespan,
                    scheduled_job.completion);
            machine_schedule.total_flow_time
                += job.weight * (scheduled_job.completion - job.release_date);
            if (job.due_date >= 0) {
                if (scheduled_job.completion <= job.due_date) {
                    this->solution_.throughput_ += job.weight;
                }
                if (scheduled_job.completion > job.due_date) {
                    machine_schedule.total_tardiness
                        += job.weight * (scheduled_job.completion - job.due_date);
                }
                machine_schedule.maximum_lateness = (std::max)(
                        machine_schedule.maximum_lateness,
                        scheduled_job.completion - job.due_date);
            }

            this->solution_.number_of_jobs_++;
        }

        // Update solution-level metrics.
        this->solution_.makespan_ = (std::max)(
                this->solution_.makespan_,
                machine_schedule.makespan);
        this->solution_.total_flow_time_ += machine_schedule.total_flow_time;
        this->solution_.total_tardiness_ += machine_schedule.total_tardiness;
        if (machine_schedule.maximum_lateness > std::numeric_limits<Time>::min()) {
            this->solution_.maximum_lateness_ = (std::max)(
                    this->solution_.maximum_lateness_,
                    machine_schedule.maximum_lateness);
        }
    }
    return std::move(solution_);
}

void SolutionBuilder::read(
        const std::string& certificate_path)
{
    std::ifstream file(certificate_path);
    if (!file.good()) {
        throw std::runtime_error(
                FUNC_SIGNATURE + ": "
                "unable to open file \"" + certificate_path + "\".");
    }

    nlohmann::json j;
    file >> j;
    const Instance& instance = this->solution_.instance();

    for (MachineId machine_id = 0;
            machine_id < instance.number_of_machines();
            ++machine_id) {
        for (const auto& json_job: j["machines"][machine_id]["jobs"]) {
            this->append_job_to_machine(
                    machine_id,
                    json_job["job_id"],
                    json_job["start"]);
        }
    }
}
