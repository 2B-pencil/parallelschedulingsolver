import matplotlib.pyplot as plt
import json
import argparse
from matplotlib.patches import Patch
import plotly.express as px


def plot_schedule(data, save_path=None):
    _, ax = plt.subplots(figsize=(14, 6))

    machines = data["machines"]
    number_of_machines = len(machines)

    machine_height = 1.0
    machine_spacing = 0.3

    # Job colors by job_id
    color_list = px.colors.qualitative.Plotly
    job_colors = {}

    yticks = []
    yticklabels = []

    for machine_id, machine in enumerate(machines):
        machine_y = machine_id * (machine_height + machine_spacing)
        machine_center_y = machine_y + machine_height / 2

        yticks.append(machine_center_y)
        yticklabels.append(f"Machine {machine_id}")

        for job_entry in machine["jobs"]:
            job_id = job_entry["job_id"]
            start = job_entry["start"]
            completion = job_entry["completion"]
            processing_time = job_entry["processing_time"]
            due_date = job_entry.get("due_date", -1)

            if job_id not in job_colors:
                job_colors[job_id] = color_list[len(job_colors) % len(color_list)]
            job_color = job_colors[job_id]

            # Draw job bar
            ax.barh(
                y=machine_center_y,
                width=processing_time,
                left=start,
                height=machine_height,
                align='center',
                alpha=0.85,
                edgecolor='black',
                linewidth=1.0,
                color=job_color
            )

            # Label inside bar
            rotation = 90 if processing_time < 3 else 0
            fontsize = min(8, max(5, processing_time))
            ax.text(
                x=start + processing_time / 2,
                y=machine_center_y,
                s=str(job_id),
                va='center',
                ha='center',
                fontsize=fontsize,
                rotation=rotation,
                color='black',
                fontweight='bold'
            )

            # Due date marker
            if due_date >= 0:
                y_bottom = machine_y
                y_top = machine_y + machine_height
                ax.plot(
                    [due_date, due_date],
                    [y_bottom, y_top],
                    color=job_color,
                    linestyle='--',
                    linewidth=1.5,
                    alpha=0.9
                )
                ax.text(
                    x=due_date,
                    y=y_top,
                    s=str(job_id),
                    va='bottom',
                    ha='center',
                    fontsize=7,
                    color=job_color,
                    fontweight='bold'
                )

    # Y-axis
    ax.set_yticks(yticks)
    ax.set_yticklabels(yticklabels)

    # X-axis
    ax.set_xlabel("Time")
    ax.xaxis.grid(True, linestyle='--', which='major', color='lightgray', alpha=0.7)

    # Legend
    handles = []
    for job_id in sorted(job_colors.keys()):
        handles.append(Patch(facecolor=job_colors[job_id], edgecolor='black', label=f"Job {job_id}"))

    ax.legend(handles=handles, title="Legend", bbox_to_anchor=(1.05, 1), loc='upper left')

    plt.tight_layout()

    if save_path:
        plt.savefig(save_path)
        print(f"Figure saved as '{save_path}'")
    else:
        plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Plot parallel scheduling Gantt chart')
    parser.add_argument('json_file', type=str, help='Path to input JSON schedule file')
    parser.add_argument('--save', type=str, default=None, help='Optional: path to save the figure (PNG format)')
    args = parser.parse_args()

    with open(args.json_file, 'r', encoding='utf-8') as f:
        data = json.load(f)
        plot_schedule(data, save_path=args.save)
