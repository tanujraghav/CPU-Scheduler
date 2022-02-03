#include <bits/stdc++.h>
using namespace std;

#define PROCESS_COUNT 1e4 + 1

struct prntf_r{
    char *s;
};

prntf_r prntf(const char *s, ...){
    va_list l;

    va_start(l, s);

    prntf_r i;
    vasprintf(&i.s, s, l);

    va_end(l);

    return i;
}

ostream& operator << (ostream& o, prntf_r i){
    o << i.s;
    free(i.s);

    return o;
}

int randint(int l, int r){
    return l + rand() % max(r-l+1, 1);
}

class Process{

    int ProcessId, ArrivalTime, BurstTime, CompletionTime = 0, TurnAroundTime = 0, WaitingTime = 0, ResponseTime = 0;

    public:
        Process(){}

        Process(int i){
            ProcessId = i;

            ArrivalTime = randint(1, i);
            BurstTime = randint(1, 9);
        }

        int getProcessId() const{
            return ProcessId;
        }

        int getArrivalTime() const{
            return ArrivalTime;
        }

        int getBurstTime() const{
            return BurstTime;
        }

        bool isReady() const{
            return CompletionTime == 0;
        }

        void update(int ct, int rt){
            CompletionTime = ct;

            TurnAroundTime = CompletionTime - ArrivalTime;
            WaitingTime = TurnAroundTime - BurstTime;

            ResponseTime = rt - ArrivalTime;
        }

        void printData(){
            ofstream file;
            file.open("processes.txt", ios_base::app);

            file << prntf("|%11d |%13d |%11d |%16d |%17d |%13d |%14d |\n", ProcessId, ArrivalTime, BurstTime, CompletionTime, TurnAroundTime, WaitingTime, ResponseTime);
        
            file.close();
        }
};

class Process_Creator{

    vector<Process> P;

    public:
        Process_Creator(){
            P.resize(PROCESS_COUNT);

            for(int i=1; i<PROCESS_COUNT; i++){
                Process tmp(i);
                P[i] = tmp;
            }
        }

        vector<Process> getQueue() const{
            return P;
        }

        void update(vector<Process> p){
            P = p;
        }

        void printData(){
            ofstream file;
            file.open("processes.txt");

            file << "| Process Id | Arrival Time | Burst Time | Completion Time | Turn Around Time | Waiting Time | Response Time |\n";
            file << "+------------+--------------+------------+-----------------+------------------+--------------+---------------+\n";
            
            file.close();

            for(int i=1; i<PROCESS_COUNT; i++){
                if(not P[i].isReady())
                    P[i].printData();
            }
        }
};

class minHeap_FCFS{

    vector<Process> heap;

    public:
        minHeap_FCFS(vector<Process> Q){
            heap = {Q.begin()+1, Q.end()};
            heapify();
        }

        int parent(int idx){
            return (idx-1)>>1;
        }

        vector<Process> getHeap() const{
            return heap;
        }

        void heapify(){
            for(int i=heap.size(); i>0; i--){
                for(int idx = i-1; idx != 0 && (heap[parent(idx)].getArrivalTime() > heap[idx].getArrivalTime() || (heap[parent(idx)].getArrivalTime() == heap[idx].getArrivalTime() && heap[parent(idx)].getProcessId() > heap[idx].getProcessId())); idx = parent(idx)){
                    swap(heap[parent(idx)], heap[idx]);
                }
            }
        }

        int deleteMin(vector<Process> &h){
            int idx = heap[0].getProcessId();

            heap = {heap.begin()+1, heap.end()};
            heapify();

            h = getHeap();

            return idx;
        }
};

class Scheduler{
    public:
        void FCFS(vector<Process> &Q, int n){
            minHeap_FCFS heap(Q);
            int flag=1;
            vector<Process> h = heap.getHeap();

            for(int t=1; t<=n; t++){
                ofstream file;
                file.open("status.txt", ios_base::app);

                for(int i=0; i<h.size(); i++){
                    if(h[i].getArrivalTime() == t){
                        file << prntf("|%4d |%11d |  Arrived |\n", t, h[i].getProcessId());
                    }
                }

                if(t == flag){
                    if(t > 1){
                        file << prntf("|%4d |%11d |   Exited |\n", t, heap.deleteMin(h));
                    }

                    flag += h[0].getBurstTime();
                    Q[h[0].getProcessId()].update(flag, flag - Q[h[0].getProcessId()].getBurstTime());

                    file << prntf("|%4d |%11d |  Running |\n", t, h[0].getProcessId());
                }

                file.close();
            }
        }

        void SRTF(vector<Process> &Q, int n){
            cout<<"shortest remaining time first!";
        }

        void RR(vector<Process> &Q, int n, int k){
            cout<<"round robin!";
        }
};

class Simulator : public Scheduler{
    public:
        Simulator(int n, int k, int q){
            ofstream file;
            file.open("status.txt");

            file << "|  #  | Process Id |  Status  |\n";
            file << "+-----+------------+----------+\n";

            file.close();

            Process_Creator P;
            vector<Process> readyQ = P.getQueue();

            switch (k){
                case 0:
                    Scheduler::FCFS(readyQ, n);
                    break;
                case 1:
                    Scheduler::SRTF(readyQ, n);
                    break;
                case 2:
                    Scheduler::RR(readyQ, n, q);
                    break;
            }

            P.update(readyQ);
            P.printData();
        }
};

int main(){
    int SimulationTime, SchedulingAlgorithm, TimeQuantum = 0;

    cout<<"Please enter Simulation Time: ";
    cin>>SimulationTime;

    cout<<"Please select a Scheduling Algorithm:\n0. First Come First Serve (FCFS)\n1. Shortest Remaining Time First (SRTF)\n2. Round Robin (RR)\n";
    cin>>SchedulingAlgorithm;

    if(SchedulingAlgorithm == 3){
        cout<<"Please enter Time Quantum for Round Robin (RR) Algoritm: ";
        cin>>TimeQuantum;
    }

    Simulator simulator(SimulationTime * 1000, SchedulingAlgorithm % 3, TimeQuantum);
}
