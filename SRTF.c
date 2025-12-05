#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

// ** Các biến toàn cục hỗ trợ
int t = 0;              // Biến lưu thời gian hiện tại suốt chương trình
int order[100];         // Mảng lưu trình tự thực thi của các tiến trình, giúp vẽ GanttChart nhanh hơn
int current = 0;        // Biến chỉ mục cho mảng order[]

// ** Thêm thuộc tính iRemain để lưu thời gian thực thi còn lại (ban đầu = iBurst và giảm dần mỗi lần thực thi)
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

// ** Hàm mới để chạy tiến trình: giảm iRemain và lưu thứ tự thực thi
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

    // Nhập tiến trình

    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    sort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    // ** Mảng dùng để lưu các mốc thời gian tới của các tiến trình
    int arrivalTime[10];
    int arIndex;
    for (int i = 0; i < iRemain; i++) {
        arrivalTime[i] = Input[i].iArrival;
    }
    arIndex = 1;

    // Đưa tiến trình đầu tiên vào ReadyQueue, cập nhật thông tin chạy cho tiến trình đó

    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);

    // ** Cập nhật t trong trường hợp tiến trình đầu tiên đến có iArrival > 0
    if (ReadyQueue[0].iArrival > 0) t = ReadyQueue[0].iArrival;

    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);

    // Vòng lặp thuật toán chính

    while (iTerminated < iNumberOfProcess)
    {
        // ** Xử lý khi các tiến trình chưa tới hết
        while (arIndex < iNumberOfProcess) {
            // ** Thiết lập biến next là thời gian tới thời điểm tiến trình kế tiếp tới
            int next = arrivalTime[arIndex] - arrivalTime[arIndex - 1];
            arIndex++;

            // ** Trường hợp không có tiến trình nào trong ReadyQueue => CPU "nhàn rỗi" => Cập nhật t đến thời điểm tiến trình kế tiếp tới
            if (iReady == 0) {
                t += next; 
            }

            // ** Nếu tiến trình có iRemain > next thì chỉ chạy next để đợi tiến trình kế tới để thực hiện sắp xếp
            if (ReadyQueue[0].iRemain > next && next != 0) {
                printf("Next Terminated Progress (t = %d): ", t);
                printProcess(1, ReadyQueue);

                excuteProgress(&ReadyQueue[0], next);

                t += next;
            }
            // ** Trường hợp tiến trình chạy xong vẫn còn thời gian next (vẫn chưa tới thời điểm tiến trình kế tiếp tới)
            else {
                // ** Tạo biến runTime tiến hành tính toán
                int runTime = next;

                // ** Nếu còn dư runTime và có tiến trình trong ReadyQueue thì cứ tiếp tục chạy
                while (iReady > 0 && runTime > 0) {
                    // ** Tương tự nếu tiến trình chạy vẫn dư runTime thì cập nhật lại runTime và tiếp tục cho tiến trình sau chạy
                    if (runTime >= ReadyQueue[0].iRemain) {
                        int temp = ReadyQueue[0].iRemain;

                        printf("Next Terminated Progress (t = %d): ", t);
                        printProcess(1, ReadyQueue);

                        // Chạy tiến trình và cập nhật runTime
                        excuteProgress(&ReadyQueue[0], temp);
                        runTime -= temp;

                        t += temp;

                        ReadyQueue[0].iResponse = ReadyQueue[0].iStart[0] - ReadyQueue[0].iArrival;
                        ReadyQueue[0].iTaT = ReadyQueue[0].iStop[ReadyQueue[0].iExcuteNumber - 1] - ReadyQueue[0].iArrival;
                        ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst;

                        pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
                        removeProcess(&iReady, 0, ReadyQueue);
                    }
                    // ** Chạy hết runTime
                    else {
                        printf("Next Terminated Progress (t = %d): ", t);
                        printProcess(1, ReadyQueue);

                        excuteProgress(&ReadyQueue[0], runTime);

                        t += runTime;
                        runTime = 0;
                    }
                }

                // ** Trường hợp dư runTime nhưng đã hết tiến trình trong ReadyQueue => Cập nhật t đến thời điểm tiến trình kế tiếp tới
                if (iReady == 0 && runTime > 0) {
                    t = arrivalTime[arIndex - 1];
                    runTime = 0;
                }
            }

            // ** Tiến trình mới tới
            if (t == arrivalTime[arIndex - 1]) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
            }

            // ** Sắp xếp mỗi khi có tiến trình mới
            sort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);
        }

        // ** Giai đoạn tất cả tiến trình đã tới hết => Trở thành SJF
        if (iReady > 0) {
            int finishTime = ReadyQueue[0].iRemain;

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

    // Xuất kết quả

    printf("\n===== SRTF Scheduling =====\n");
    sort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    exportGanttChart(iTerminated, TerminatedArray);

    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}
