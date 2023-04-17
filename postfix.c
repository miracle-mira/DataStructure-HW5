/* postfix.c
 *
 * Data Structures, Homework #5
 * School of Computer Science at Chungbuk National University
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20


/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
    lparen = 0, /* ( 왼쪽 괄호 */
    rparen = 9, /* ) 오른쪽 괄호*/
    times = 7, /* * 곱셈 */
    divide = 6, /* / 나눗셈 */
    plus = 5, /* + 덧셈 */
    minus = 4, /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;


char infixExp[MAX_EXPRESSION_SIZE]; /* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE]; /* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE]; /* postfix로 변환을 위해 필요한 스택*/
int evalStack[MAX_STACK_SIZE]; /* 계산을 위해 필요한 스택 */
int postfixStackTop = -1; /* postfixStack용 top */
int evalStackTop = -1; /* evalStack용 top */
int evalResult = 0; /* 계산 결과를 저장 */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
    char command;

    printf("----- [박미라] [2020069006] -----\n");

    do{
        printf("----------------------------------------------------------------\n");
        printf(" Infix to Postfix, then Evaluation\n");
        printf("----------------------------------------------------------------\n");
        printf(" Infix=i, Postfix=p, Eval=e, Debug=d,Reset=r, Quit=q \n");
        printf("----------------------------------------------------------------\n");
        
        printf("Command = ");
        scanf(" %c", &command);

        switch(command) {
            case 'i': case 'I':
                getInfix();
                break;

            case 'p': case 'P':
                toPostfix();
                break;

            case 'e': case 'E':
                evaluation();
                break;

            case 'd': case 'D':
                debug();
                break;

            case 'r': case 'R':
                reset();
                break;

            case 'q': case 'Q':
                break;

            default:
                printf("\n >>>>> Concentration!! <<<<<\n");
                break;
        }
    }while(command != 'q' && command != 'Q');

    return 1;
}


void postfixPush(char x)
{
 postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
    return '\0';
    else {
    x = postfixStack[postfixStackTop--];
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)
    return -1;
    else
    return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}
precedence getToken(char symbol)
{
    switch(symbol) {
    case '(' : return lparen;
    case ')' : return rparen;
    case '+' : return plus;
    case '-' : return minus;
    case '/' : return divide;
    case '*' : return times;
    default : return operand;
    }
}

precedence getPriority(char x)
{
    return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
    if (postfixExp == '\0')
        strncpy(postfixExp, c, 1);
    else
        strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
    /* infixExp의 문자 하나씩을 읽기위한 포인터 */
    char*exp= infixExp;
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */
    precedence token; // 연산자 우선순위 저장하는 변수 token 선언

    /* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
    while(*exp != '\0'){
        x= *exp; // 변환 전 식의 연산자/피연산자 하나를 저장하는 변수 선언 및 대입 
        token= getToken(x); // token에 x의 우선순위를 구하여 대입
        if(token== operand) // x가 피연산자일 경우
            charCat(&x); // charCat 함수를 통해 postfix 변환식에 그대로 붙임 
        else if(token== rparen) // x가 우괄호일 경우
        {
            precedence stacktoken= getToken(postfixStack[postfixStackTop]); // 현재 postfix 변환 위한 stack의 요소의 연산 우선순위 구하여 
            while(stacktoken!= lparen) // stack 요소가 좌괄호가 아닐 때까지
            {
                char c= postfixPop(); // char형 변수 c를 선언하여 stack에서 pop한 값 저장 
                charCat(&c); // pop한 값을 charCat 함수를 통해 postfix 변환식에 그대로 붙임 
                stacktoken= getToken(postfixStack[postfixStackTop]); // 좌괄호 찾을 때까지 반복
            }
            postfixPop(); // 왼쪽 괄호 버림
        }
        else{
            if(getPriority(postfixStack[postfixStackTop]) >= (getPriority(x) ? getPriority(x) : 10)) // stack의 top 요소가 incoming 요소보다 연 산 우선순위가 높거나 같을 경우
            {  
                while(getPriority(postfixStack[postfixStackTop]) >= (getPriority(x) ? getPriority(x) : 10))  // stack top 요소 우선순위가 더 높은 동안
                {
                    char c= postfixPop(); // stack에서 요소 pop해 char형 변수 c에 저장
                    charCat(&c); // pop한 값을 charCat 함수를 통해 postfix 변환식에 그대로 붙임
                }
                postfixPush(x); // incoming 요소 연산 우선순위가 더 높아지면 incoming 요소 x를 stack에 push함
            }
            
            else if(getPriority(postfixStack[postfixStackTop]) < (getPriority(x) ? getPriority(x) : 10)) //stack의 top 요소가 incoming보다 연산 우선순위가 낮은 경우
                postfixPush(x);
        }

        exp++; // 기존 식의 다음 연산자/피연산자를 처리하기 위해 exp++
    }
        while(1) // postfix 변환 위한 stack에 남아있는 요소가 있을 때까지
        {
            char temp= postfixPop(); // char형 변수 temp에 stack에서 pop한 값 저장
            if(temp=='\0')//stack이 비어있을 경우
                break;
            else
                charCat(&temp); // 아닐 경우 pop한 값 charCat 함수 통해 postfix 변환식에 그대로 붙임
        }

    printf("%s\n",postfixExp); // 변환이 완료된 postfix 식을 출력 printf("\n");
/* 필요한 로직 완성 */
}

void debug()
{
    printf("\n---DEBUG\n");
    printf("infixExp = %s\n", infixExp);
    printf("postExp = %s\n", postfixExp);
    printf("eval result = %d\n", evalResult);
    printf("postfixStack : ");
    for(int i = 0; i < MAX_STACK_SIZE; i++)
    printf("%c ", postfixStack[i]);
    printf("\n");
}

void reset()
{
    infixExp[0] = '\0';
    postfixExp[0] = '\0';
    for(int i = 0; i < MAX_STACK_SIZE; i++)
        postfixStack[i] = '\0';

    postfixStackTop = -1;
    evalStackTop = -1;
    evalResult = 0;
}

void evaluation()
{
    /* postfixExp, evalStack을 이용한 계산 */
    char*exp= postfixExp; // 변환된 후위표기식의 연산자/피연산자 하나를 순서대로 저장
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */
    precedence token; // 연산자 우선순위 저장하는 변수 token 선언
    int op1, op2; // 피연산자 2개를 각각 저장할 int형 변수 선언

    while(*exp!= '\0') // 후위표기식의 모든 연산자/피연산자에 대해 계산을 끝낼 때까지
    {
        x= *exp; // 임시 저장변수 x에 exp의 값 저장 
        token= getToken(x); // x의 연산 우선순위 구함 
        if(token== operand) // x가 피연산자일 경우
            evalPush(x-'0'); // int형 변환을 위해 '0'을 뺀 값을 연산 위한 stack에 push

        else //x가 연산자일 경우
        {
            op2= evalPop(); // 두번째 피연산자값을 pop하여 저장 
            op1= evalPop(); // 첫번째 피연산자값을 pop하여 저장
            switch(token) // 연산자 종류에 따라
            {
                case plus: evalPush(op1+op2); break; // 알맞는 연산을 수행하여 결과값을 연산 위한 stack에 push
                case minus: evalPush(op1-op2); break;
                case times: evalPush(op1*op2); break;
                case divide: evalPush(op1/op2); break;
                default: break;
            }
        }
        exp++;
    }
     evalResult= evalPop(); // 연산이 완료되어 최종적으로 stack에 남은 값을 pop하여 결과 저장 
     printf("%d\n", evalResult); // 연산 결과값 출력 
}