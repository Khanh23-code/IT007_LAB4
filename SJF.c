#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        P[i].iPID = i + 1;

        printf("Please input for the Process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &P[i].iArrival);
        printf("Burst Time: ");
        scanf("%d", &P[i].iBurst);
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
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            if (P[i].iStart != 0) {
                printf (" ------ %d", P[i].iStart);
            }
        }

        if (i > 0) {
            if (P[i].iStart != P[i - 1].iFinish) {
                printf (" ------ %d", P[i].iStart);
            }
        }

        printf(" ------ %d", P[i].iFinish);
    }
    printf("\n");

    printf("|");
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            if (P[i].iStart != 0) {
                printf ("   //   ");

                int t = P[i].iStart;
                while (t > 0) {
                    printf("|");
                    t = t / 10;
                }
            }
        }

        if (i > 0) {
            if (P[i].iStart != P[i - 1].iFinish) {
                printf ("   //   ");

                int t = P[i].iStart;
                while (t > 0) {
                    printf("|");
                    t = t / 10;
                }
            }
        }

        printf("   P%d   ", P[i].iPID);

        int t = P[i].iFinish;
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
                    if ((P[j].iArrival < P[minIndex].iArrival) || (P[j].iArrival == P[minIndex].iArrival && P[j].iBurst < P[minIndex].iBurst)) {
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
                    if (P[j].iBurst < P[minIndex].iBurst) {
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

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];

    // Nhập tiến trình

    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    sort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    // Đưa tiến trình đầu tiên vào ReadayQueue, cập nhật thông tin chạy cho tiến trình đó

    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);

    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);

    // Vòng lặp thuật toán chính

    while (iTerminated < iNumberOfProcess)
    {
        // Xử lý khi các tiến trình chưa vào ReadyQueue hết
        while (iRemain > 0)
        {
            // Đưa các tiến trình có iArrival trước khi tiến trình hiện tại hoàn thành iFinish
            if (Input[0].iArrival <= ReadyQueue[0].iFinish)
            {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                continue;
            }
            // Nếu còn tiến trình trong Input nhưng không còn tiến trình trong ReadyQueue => Có khoảng trống CPU
            // Tiếp tục đưa tiến trình kế tiếp từ Input vào ReadayQueue
            else if (iReady == 0 && iTerminated < iNumberOfProcess) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);

                ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
                ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
                ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
                continue;
            }
            else break;
        }

        // Thực hiện chạy tiến trình ReadyQueue[0]
        if (iReady > 0)
        {
            printf("Next Terminated Progress (t = %d): ", ReadyQueue[0].iStart);
            printProcess(1, ReadyQueue);

            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);

            // **Sắp xếp ReadyQueue theo BurstTime 
            // => Đảm bảo tiến trình chạy kế tiếp ReadyQueue[0] là tiến trình có iBurst thấp nhất
            sort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);

            // Cập nhật giá trị cho tiến trình chạy kế tiếp - ReadyQueue[0]
            if (iReady <= 0) continue;
            ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
        }
    }

    // Xuất kết quả
    
    printf("\n===== SJF Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    sort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);

    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}
