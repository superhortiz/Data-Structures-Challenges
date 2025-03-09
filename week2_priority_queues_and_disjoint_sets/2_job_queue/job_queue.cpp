#include <iostream>
#include <algorithm>
#include <queue>      // For priority_queue
#include <vector>
#include <functional> // For std::greater (if creating a min-heap)

using std::vector;
using std::cin;
using std::cout;

class JobQueue {
private:
    int num_workers_;
    vector<int> jobs_;

    vector<int> assigned_workers_;
    vector<long long> start_times_;

    void WriteResponse() const {
        for (int i = 0; i < jobs_.size(); ++i) {
            cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
        }
    }

    void ReadData() {
        int m;
        cin >> num_workers_ >> m;
        jobs_.resize(m);
        for(int i = 0; i < m; ++i)
            cin >> jobs_[i];
    }

    void AssignJobs() {
        assigned_workers_.resize(jobs_.size());
        start_times_.resize(jobs_.size());

        // Create a min-heap priority queue of pairs using a lambda function
        auto cmp = [](const std::pair<long long, int>& left, const std::pair<long long, int>& right) {
            return left > right;  // Use greater-than to create a min-heap
        };
        std::priority_queue<std::pair<long long, int>, vector<std::pair<long long, int>>, decltype(cmp)> pq(cmp);
        int i, next_worker;
        long long current_time = 0;

        // For each worker, add a task in the pq
        for (i = 0; i < num_workers_; ++i) {
            if (i < jobs_.size()) {
                pq.push({current_time + static_cast<long long>(jobs_[i]), i});
                assigned_workers_[i] = i;
                start_times_[i] = 0;
            }
        }

        while (!pq.empty() && i < jobs_.size()) {
            // Get the top element
            std::pair<long long, int> top = pq.top();
            pq.pop();

            if (i < jobs_.size()) {
                // Unpack the top element
                current_time = top.first;
                next_worker = top.second;

                // Put in the priority queue the next task to process
                pq.push({current_time + jobs_[i], next_worker});
                assigned_workers_[i] = next_worker;
                start_times_[i++] = current_time;
            }
        }
    }

public:
    void Solve() {
        ReadData();
        AssignJobs();
        WriteResponse();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    JobQueue job_queue;
    job_queue.Solve();
    return 0;
}
