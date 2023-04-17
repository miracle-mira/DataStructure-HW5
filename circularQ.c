/*
* circularQ.c
*
* Data Structures, Homework #5
* Department of Computer Science at Chungbuk National University
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 4 // QUEUE의 최대 사이즈를 4로 지정, 즉 elements는 3개까지 들어갈 수 있음 

typedef char element; //char를 별칭 element로 정의
typedef struct{
    element queue[MAX_QUEUE_SIZE];
    int front, rear; // QueueType 구조체 선언 통해 circularQueue의 기본 구조 구성 
 }QueueType;

QueueType *createQueue(); // circularQueue 구조를 생성하는 함수 선언
int freeQueue(QueueType*cQ); // 생성한 circularQueue에 할당한 공간 해제하는 함수 선언
int isEmpty(QueueType*cQ); // Queue가 empty한 상태인지 확인하는 함수 선언
int isFull(QueueType*cQ); // Queue가 full한 상태인지 확인하는 함수 선언
void enQueue(QueueType*cQ, element item); // Queue에 새로운 값 enqueue 하기 위한 함수 선언 
void deQueue(QueueType*cQ, element* item); // Queue에서 값을 dequeue 하기 위한 함수 선언 
void printQ(QueueType*cQ); // Queue의 요소를 출력하는 함수 선언
void debugQ(QueueType*cQ); // 디버그를 위한 Queue의 정보 출력 함수 선언
element getElement();

int main(void)
{
    QueueType*cQ= createQueue(); // QueueType 포인터 cQ에 Queue 기본구조를 만들어 동적할당 
    element data; // 입력할 요소값 저장하는 data 변수 선언
    char command; // 메뉴 선택값 저장하는 변수 선언

    printf("----- [박미라] [2020069006] -----\n");

    do{ 
        printf("\n-----------------------------------------------------\n"); 
        printf(" Circular Q \n"); printf("------------------------------------------------------\n"); 
        printf(" Insert=i, Delete=d, PrintQ=p, Debug=b, Quit=q \n"); 
        printf("------------------------------------------------------\n");


        printf("Command = ");
        scanf(" %c", &command);

        switch(command) { // 선택한 메뉴값에 따라 알맞은 함수 호출

            case 'i': case 'I':
                data= getElement();
                enQueue(cQ, data); 
                break;

            case 'd': case 'D': 
                deQueue(cQ, &data);
                break;

            case 'p': case 'P': 
                printQ(cQ);
                break;

            case 'b': case 'B':
                debugQ(cQ);
                break;
            
            case 'q': case 'Q':
                freeQueue(cQ);
                break; 
            
            default:
                printf("\n>>>>>    Concentration!    <<<<<    \n"); //잘못된 메뉴값 입력할 경우 오류 메시지 출력
        }
    }while(command != 'q' && command != 'Q'); //q[Q] 입력시까지 반복 실행

    return 1;
}

QueueType*createQueue() // Queue 기본 구조 생성함수 
{
    QueueType*cQ;
    cQ= (QueueType*)malloc(sizeof(QueueType)); // 동적할당을 통해 Queue의 저장 위한 메모리 공간 생성 후 cQ->front= 0;
    cQ->rear= 0; // 초기화된 상태이므로 front와 rear은 각각 0으로 지정

    return cQ; // 생성한 cQ의 포인터값 반환
}

int freeQueue(QueueType*cQ) // Queue 할당 해제 함수
{
    if(cQ== NULL) 
        return 1; // 할당된 공간이 없을 경우 종료 
    free(cQ); // 할당된 공간이 있을 경우 free 함수를 통해 할당 해제 

    return 1;
}

element getElement() // enQueue할 요소값을 입력받는 함수
{
    element item;
    printf("Input element = "); // enQueue할 함수의 값을 입력받음 
    scanf(" %c", &item);
    return item; // 입력받은 값을 item에 저장하여 반환
}

/* complete the function */
int isEmpty(QueueType*cQ) // Queue가 empty한 상태인지 확인하는 함수 
{
    if(cQ->front== cQ->rear) // Queue가 empty할 때, 즉 front == rear이면 
        return 1; // 1 반환
    return 0; // empty한 상태가 아닐 경우 0 반환 
}
   
 /* complete the function */
int isFull(QueueType*cQ) // Queue가 full한 상태인지 확인하는 함수 
{
    if(cQ->front== cQ->rear) // Queue가 full할 때, 즉 front == rear이면 
        return 1; // 1 반환
    return 0; // full한 상태가 아닐 경우 0 반환 
}

/* complete the function */
void enQueue(QueueType*cQ, element item) {
    int temp= cQ->rear; // circularQueue의 현재 rear 값 임시 저장할 변수 선언 및 대입
    cQ->rear= ((cQ->rear) + 1) % MAX_QUEUE_SIZE; // 새로운 rear 지정 통한 원형 구조 실현 위한 식 대입 후 
                                                 // 원형큐에서 full상태는 rear의 값에서 1을 더했을 때, 그 위치가 front와 일치할 때 full이다.  
    
    if(isFull(cQ) == 0) // Queue가 full한 상태가 아닐 경우
        cQ->queue[cQ->rear] = item; // 새로 지정한 rear 위치에 item 대입

    else // Queue가 full한 상태일 경우
    {
        printf("Queue is full!\n"); // 메시지 출력 및 
        cQ->rear= temp; // rear 값을 기존 위치로 재설정
    }                                
}

/* complete the function */
void deQueue(QueueType*cQ, element*item) 
{
    if(isEmpty(cQ) == 0) // Queue가 empty한 상태가 아닐 경우
        cQ->front= ((cQ->front)+1) % MAX_QUEUE_SIZE; // 새로운 front 지정 통한 원형구조 실현 위한 식 대입
    else printf("Queue is empty!\n"); // Queue가 empty한 상태일 경우 메시지 출력 
}

void printQ(QueueType*cQ) 
{
    int i, first, last;
    first= (cQ->front+ 1) % MAX_QUEUE_SIZE;
    last= (cQ->rear+ 1) % MAX_QUEUE_SIZE; // Queue 출력 위해 원형구조 기준 first, last 위치 대입 printf("Circular Queue : [");
    i= first; // i에 first 값 대입

    printf("[");

    while(i!= last){ // 마지막 요소까지 출력할 때까지
        printf("%3c", cQ->queue[i]); // Queue의 각 i번째 위치에 대해 출력 
        i= (i+1)%MAX_QUEUE_SIZE;
    }
    printf("   ]\n");
}

void debugQ(QueueType*cQ) // 디버그를 위한 Queue의 정보 출력 함수 선언 
{
    printf("\n---DEBUG\n");
    for(int i= 0; i < MAX_QUEUE_SIZE; i++) {
        if(i== cQ->front) {
            printf(" [%d] = front\n", i); // front 위치 출력 
            continue;
        }
    
        printf(" [%d] = %c\n", i, cQ->queue[i]); // Queue의 각 요소값 출력
    }

    printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // Queue의 front값, rear값 출력
}
    
    