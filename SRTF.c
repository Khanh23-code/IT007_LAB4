#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

int t = 0;
int order[100];
int current = 0;

typedef struct{
    int iPID;
    int iArrival, iBurst, iRemain;
    int iWaiting, iResponse, iTaT, iExcuteNumber;
    int iStart[10], iStop[10];
} PCB;

void inputProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        P[i].iPID = i + 1;

        printf("Please input for the Process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &P[i].iArrival);
        printf("Burst Time: ");
        scanf("%d", &P[i].iBurst);

        P[i].iRemain = P[i].iBurst;
        P[i].iExcuteNumber = 0;
    }
}

void printProcess(int n, PCB P[]) { 
    for (int i = 0; i < n; i++) {
        // printf("PID: %d - iA: %d - iB: %d - iS: %d - iF: %d - iR: %d - iW: %d - iTaT: %d \n", 
        //     P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, P[i].iFinish, P[i].iResponse, P[i].iWaiting, P[i].iTaT);

        printf("PID: %d - Arrival Time: %d - Burst Time: %d \n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}

void exportGanttChart (int n, PCB P[]) {
    printf("Gantt Chart:\n");
    
    printf("0");
    for (int i = 0; i < current; i += 2) {
        int pid = order[i];
        int time = order[i+1];

        if (i == 0) {
            if (P[pid].iStart[time] != 0) {
                printf (" ------ %d", P[pid].iStart[time]);
            }
        }

        if (i > 0) {
            if (P[pid].iStart[time] != P[order[i - 2]].iStop[order[i - 1]]) {
                printf (" ------ %d", P[pid].iStart[time]);
            }
        }

        if (i + 2 < current && order[i] == order[i + 2]) continue;

        printf(" ------ %d", P[pid].iStop[time]);
    }
    printf("\n");

    printf("|");
    for (int i = 0; i < current; i += 2) {
        int pid = order[i];
        int time = order[i+1];

        if (i == 0) {
            if (P[pid].iStart[time] != 0) {
                printf ("   //   ");

                int t = P[pid].iStart[time];
                while (t > 0) {
                    printf("|");
                    t = t / 10;
                }
            }
        }

        if (i > 0) {
            if (P[pid].iStart[time] != P[order[i - 2]].iStop[order[i - 1]]) {
                printf ("   //   ");

                int t = P[pid].iStart[time];
                while (t > 0) {
                    printf("|");
                    t = t / 10;
                }
            }
        }

        if (i + 2 < current && order[i] == order[i + 2]) continue;

        printf("   P%d   ", pid + 1);

        int t = P[pid].iStop[time];
        while (t > 0) {
            printf("|");
            t = t / 10;
        }
    }

    printf("\n\n");
}

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]) {
    for (int i = index; i < (*n)-1; i++) {
        P[i] = P[i+1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q; 
    *Q = temp; 
}

// int partition (PCB P[], int low, int high, int iCriteria);

void sort(PCB P[], int low, int high, int iCriteria){
    switch(iCriteria) {
        case SORT_BY_ARRIVAL:
            for (int i = low; i < high; i++) {
                int minIndex = i;

                for (int j = i + 1; j <= high; j++) {
                    if (P[j].iArrival < P[minIndex].iArrival) {
                        minIndex = j;
                    }
                }

                if (minIndex != i) {
                    swapProcess(&P[i], &P[minIndex]);
                }
            }
            return;
        case SORT_BY_PID:
            for (int i = low; i < high; i++) {
                int minIndex = i;

                for (int j = i + 1; j <= high; j++) {
                    if (P[j].iPID < P[minIndex].iPID) {
                        minIndex = j;
                    }
                }

                if (minIndex != i) {
                    swapProcess(&P[i], &P[minIndex]);
                }
            }
            return;
        case SORT_BY_BURST:
            for (int i = low; i < high; i++) {
                int minIndex = i;

                for (int j = i + 1; j <= high; j++) {
                    if (P[j].iRemain < P[minIndex].iRemain || (P[j].iRemain == P[minIndex].iRemain && P[j].iArrival < P[minIndex].iArrival)) {
                        minIndex = j;
                    }
                }

                if (minIndex != i) {
                    swapProcess(&P[i], &P[minIndex]);
                }
            }
            return;
        case SORT_BY_START:
            for (int i = low; i < high; i++) {
                int minIndex = i;

                for (int j = i + 1; j <= high; j++) {
                    if (P[j].iStart < P[minIndex].iStart) {
                        minIndex = j;
                    }
                }

                if (minIndex != i) {
                    swapProcess(&P[i], &P[minIndex]);
                }
            }
    }
}

void calculateAWT(int n, PCB P[]) {
    printf("Waiting Time:\n");

    float averW = 0;
    for (int i = 0; i < n; i++) {
        printf("PID: %d - Waiting Time: %d\n", P[i].iPID, P[i].iWaiting);
        averW += P[i].iWaiting;
    }
    averW = averW / (float)n;

    printf("Average Waiting Time: %.2f\n\n", averW);
}

void calculateATaT(int n, PCB P[]) {
    printf("Turnaround Time:\n");

    float averT = 0;
    for (int i = 0; i < n; i++) {
        printf("PID: %d - Turnaround Time: %d\n", P[i].iPID, P[i].iTaT);
        averT += P[i].iTaT;
    }
    averT = averT / (float)n;

    printf("Average Turnaround Time: %.2f\n\n", averT);
}

void excuteProgress(PCB *Q, int runTime) {
    order[current++] = Q->iPID - 1;
    order[current++] = Q->iExcuteNumber;

    Q->iStart[Q->iExcuteNumber] = t;
    Q->iStop[Q->iExcuteNumber] = t + runTime;
    Q->iExcuteNumber++;
    Q->iRemain -= runTime;    
}

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];

    int arrivalTime[10];
    int arIndex;

    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    sort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    for (int i = 0; i < iRemain; i++) {
        arrivalTime[i] = Input[i].iArrival;
    }
    arIndex = 1;

    // printProcess(iNumberOfProcess, Input);

    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);

    if (ReadyQueue[0].iArrival > 0) t = ReadyQueue[0].iArrival;

    // ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    // ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    // ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    // ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    // ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    // printProcess(iRemain, Input);

    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);

    while (iTerminated < iNumberOfProcess)
    {
        while (arIndex < iNumberOfProcess) {
            int next = arrivalTime[arIndex] - arrivalTime[arIndex - 1];
            arIndex++;

            if (iReady == 0) {
                t += next; 
            }

            if (ReadyQueue[0].iRemain > next && next != 0) {
                // printf("Next Terminated Progress (t = %d) - Next %d: ", t, next);
                printf("Next Terminated Progress (t = %d): ", t);
                printProcess(1, ReadyQueue);

                excuteProgress(&ReadyQueue[0], next);

                // pushProcess(&iReady, ReadyQueue, Input[0]);
                // removeProcess(&iRemain, 0, Input);

                t += next;
            }
            else {
                int runTime = next;

                while (iReady > 0 && runTime > 0) {
                    if (runTime >= ReadyQueue[0].iRemain) {
                        int temp = ReadyQueue[0].iRemain;

                        // printf("Next Terminated Progress (t = %d) - Temp %d: ", t, temp);
                        printf("Next Terminated Progress (t = %d): ", t);
                        printProcess(1, ReadyQueue);

                        excuteProgress(&ReadyQueue[0], temp);
                        runTime -= temp;

                        t += temp;

                        ReadyQueue[0].iResponse = ReadyQueue[0].iStart[0] - ReadyQueue[0].iArrival;
                        ReadyQueue[0].iTaT = ReadyQueue[0].iStop[ReadyQueue[0].iExcuteNumber - 1] - ReadyQueue[0].iArrival;
                        ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst;

                        pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
                        removeProcess(&iReady, 0, ReadyQueue);
                    }
                    else {
                        // printf("Next Terminated Progress (t = %d) - RunTime %d: ", t, runTime);
                        printf("Next Terminated Progress (t = %d): ", t);
                        printProcess(1, ReadyQueue);

                        excuteProgress(&ReadyQueue[0], runTime);

                        t += runTime;
                        runTime = 0;
                    }
                }

                if (iReady == 0 && runTime > 0) {
                    t = arrivalTime[arIndex - 1];
                    runTime = 0;
                }

                // printf("Next Terminated Progress (t = %d): ", t);
                // printProcess(1, ReadyQueue);

                // order[current++] = ReadyQueue[0].iPID - 1;
                // order[current++] = ReadyQueue[0].iExcuteNumber;
                // excuteProgress(&ReadyQueue[0], ReadyQueue[0].iBurst);

                // printf("Next Terminated Progress (t = %d): ", t);
                // printProcess(1, ReadyQueue);

                // order[current++] = ReadyQueue[0].iPID - 1;
                // order[current++] = ReadyQueue[0].iExcuteNumber;
                // excuteProgress(&ReadyQueue[1], next - ReadyQueue[0].iBurst);

                // ReadyQueue[0].iResponse = ReadyQueue[0].iStart[0] - ReadyQueue[0].iArrival;
                // ReadyQueue[0].iTaT = ReadyQueue[0].iStop[ReadyQueue[0].iExcuteNumber - 1] - ReadyQueue[0].iArrival;
                // ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst;

                // pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
                // removeProcess(&iReady, 0, ReadyQueue);

                // pushProcess(&iReady, ReadyQueue, Input[0]);
                // removeProcess(&iRemain, 0, Input);
            }

            if (t == arrivalTime[arIndex - 1]) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
            }

            sort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);
        }

        if (iReady > 0) {
            int finishTime = ReadyQueue[0].iRemain;

            // printf("Next Terminated Progress (t = %d) - FinishTime %d: ", t, finishTime);
            printf("Next Terminated Progress (t = %d): ", t);
            printProcess(1, ReadyQueue);        
            excuteProgress(&ReadyQueue[0], finishTime);

            t += finishTime;

            ReadyQueue[0].iResponse = ReadyQueue[0].iStart[0] - ReadyQueue[0].iArrival;
            ReadyQueue[0].iTaT = t - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst;

            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);
        }
    }

    printf("\n===== SRTF Scheduling =====\n");
    sort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    exportGanttChart(iTerminated, TerminatedArray);

    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}
