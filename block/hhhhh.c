/*
//수학과 2022006744 박현빈
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#define size 10
int check(int a, int* b, int c);

int main() {
   int ary[size] = { NULL }; // 배열 ary를 0으로 초기화
   int num; // 입력하는 변수
   int *pary = ary;//포인터 설정
   //배열 출력 부분
   for (int k = 0;; k++) {
      printf("\n현재 집합 {");
         for (int i = 0; i < 10; i++) {
            if (i == 0 && ary[0] == 0) {
               break;
            }

            if (i == 9) {
               printf("%d", ary[i]);         //10번째 까지만 출력하고 ,가 출력 안나오게끔 하는 구간            
               break;
            }
            if (ary[i + 1] != 0) {            //입력 받은 값이 0이 아닐 경우 배열에 추가해서 출력하는 구간      
               printf("%d, ", ary[i]);
            }
            else {
               printf("%d", ary[i]);            // 배열을 하나씩 출력하는 구간
               break;
            }
         }
         printf("}\n");
   

      printf("추가할 원소를 입력하세요(종료는 0) : ");
      scanf("%d", &num);
      if (num == 0) { //0입력 시 종료
         return 0;
      }
      k = check(num, pary, k); //////
      // if (num < 0) {
      //    k -= 2;
      // }
      
      //if (k < 10) {
      //   if (*(ary + k) == 0) {
      //      k -= 1; //배열에 입력이 안되면 초기화
      //   }
      //}
      
   }
}

int check(int a, int *b, int c) {
   int x = 0;
   if (c > 9 && a>0) {
      printf("더 입력할 수 없습니다.\n");                     //10개 이상 입력했을경우 멘트 출력
      c -= 1;
   }
   else {
      if (a < 0) {
         for (int i = 0; i < size; i++) {
            if (a == -1 * *(b + i)) {                        //음수를 받았을 때 겹치면 없애는 부분
               for (int j = i; j < c - 1; j++) {
                  *(b + j) = *(b + j + 1);
               }
               *(b + c - 1) = NULL; //////
               c -= 2; //////
               x =1;
               break;
            }
         }
         if (x == 0) {                     //음수를 받았을 떄 겹치는 숫자가 없을 경우
              printf("%d은 집합에 존재하지 않습니다.", a);
              
            }

      }
      else if (c < 11) { /////
         x =2;
         for (int i = 0; i < size; i++) {
            if (a == *(b + i)) {                           // 같은 원소가 있을 경우 알려주고 초기화
               printf("이미 같은 원소가 [%d]번째에 있습니다.\n", i + 1);
               //a = 0;
               c -= 1; /////
               x = 3;
               break;
            }
         }
         if (x == 2){
            *(b + c) = a;                                    //배열에 값 대입
         }
      }
   }
   
   return c;
}*/



#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// bool 정의
typedef enum { false, true } bool;

// 입력 값의 최대 사이즈 지정
#define ARRAY_SIZE 100

// 입력 받은 수식이 저장되는 배열
//char input[ARRAY_SIZE] = { 0 };

// 입력 받은 수식의 길이를 보관하는 변수
int inputLength = 0;

void printInputArray();// 입력받은 수식 배열의 값을 출력하는 함수
int fundamentalOperation(char localValue[ARRAY_SIZE], int localCount);// 사칙 연산의 결과를 int형으로 반환하는 함수이다. 파라미터로 사칙연산 수식과, 그 수식의 길이를 받는다.
void bracketOperation();// 전역변수 input에 있는 수식의 가장 먼저 만나고 가장 깊은 괄호 하나를 풀어내고 input에 수식을 재정리하는 함수이다.

int main() {
    char input[ARRAY_SIZE] = { 0 };
    while (1) {
        printf("수식을 입력하세요. (종료는0) : ");
        scanf("%s", input);
        //gets(input, ARRAY_SIZE);


        if (input[0] == '0') {
            // 0을 입력받았으면 종료
            break;
        }
        else {
            // 입력한 수식의 길이를 알아낸다.
            for (int i = 0; i < ARRAY_SIZE; i++) {
                if (input[i] == 0) {
                    inputLength = i;
                    break;
                }
            }
            // 배열 입력 값 검사.
            // 0~9와 *-+/()만 입력 가능하도록 필터링한다.
                int result = fundamentalOperation(input, inputLength);
                printf("= % d\n", result); // 결과 출력
        }
  
        memset(&input[0], 0, sizeof(input));
        inputLength = 0;
    }
    printf("종료합니다.\n");


    return 0;
}

void printInputArray(char input[ARRAY_SIZE]) {
    for (int i = 0; i < inputLength; i++) {
        printf("%c", input[i]);
    }
    printf("\n");
}

int fundamentalOperation(char localValue[ARRAY_SIZE], int localCount) {
    int  v[ARRAY_SIZE] = { 0 };   // 수를 저장하는 공간
    int  vIdx = 0;              // v배열에 들어가는 갯수를 계산하기 위한 변수
    char con[ARRAY_SIZE] = { 0 }; // 수식기호를 저장하는 공간
    int  conIdx = 0;            // con배열에 들어가는 갯수를 계산하기 위한 변수
    int  sqr = 0;               // 문자열 배열로 깨져있는 수를 조합하는 공간(이후 저장하는 공간으로 이동)
    bool hasDivMultip = false;  // 곱하기나 나누기를 가지고 있는지 확인하기 위한 변수
    for (int j = 0; j < localCount; j++) {
        if ('0' <= localValue[j] && localValue[j] <= '9') { // 숫자일 경우 sqr에 저장한다.
            sqr *= 10;  // 기존에 들어있던 sqr 값은 10을 곱하여 한칸 밀어주고
            sqr += localValue[j] - '0'; // 새로운 숫자를 더해준다. 캐릭터형이기 때문에 '0'을 빼서 원래 값을 구한다.
        }
        else if (localValue[j] == '-' || localValue[j] == '+') { // - + 연산자일 경우
            v[vIdx] = sqr;      // 기존에 구한 숫자를 v배열에 추가
            sqr = 0;            // sqr을 초기화
            vIdx++;             // 배열의 크기를 계산하기 위한 vIdx는 1을 더해준다.

            if (hasDivMultip) { // 곱하기를 가지고 있음을 확인한 상태라면
                int tmp = 0;
                if (con[conIdx - 1] == '*') {//곱셈계산
                    tmp = v[vIdx - 2] * v[vIdx - 1];
                }
                hasDivMultip = false;
                // 계산한 값들을 배열에서 제외해준다.
                con[conIdx - 1] = 0;
                conIdx--;

                v[vIdx - 1] = 0;
                v[vIdx - 2] = 0;
                vIdx -= 2;

                v[vIdx] = tmp;
                vIdx++;
            }

            con[conIdx] = localValue[j]; // 수식도 배열에 추가한다.
            conIdx++;

        }
        else if (localValue[j] == '*') { // * 연산자일 경우,
            v[vIdx] = sqr;      // 기존에 구한 숫자를 v배열에 추가하고,
            sqr = 0;            // sqr을 초기화한다.
            vIdx++;             // 배열의 크기를 계산하기 위한 vIdx는 1을 더해준다.

            if (hasDivMultip) { // 기존에 곱셈을 받아둔게 있다면
                int tmp = 0;
                if (con[conIdx - 1] == '*') {//곱셈계산
                    tmp = v[vIdx - 2] * v[vIdx - 1];
                }
                con[conIdx - 1] = 0;
                conIdx--;

                v[vIdx - 1] = 0;
                v[vIdx - 2] = 0;
                vIdx -= 2;

                v[vIdx] = tmp;
                vIdx++;
            }

            con[conIdx] = localValue[j]; // 수식도 배열에 추가.
            conIdx++;

            hasDivMultip = true; // 곱하기를 입력받았음을 표현해둔다.
        }
        if (j == localCount - 1) { // 수식을 다 입력받은 상태로, 곱하기를 입력 받은 상태라면
            v[vIdx] = sqr;      // 기존에 구한 숫자를 v배열에 추가하고,
            sqr = 0;            // sqr을 초기화한다.
            vIdx++;             // 배열의 크기를 계산하기 위한 vIdx는 1을 더해준다.

            if (hasDivMultip) {
                int tmp = 0;
                if (con[conIdx - 1] == '*') {//곱셈계산
                    tmp = v[vIdx - 2] * v[vIdx - 1];
                }
                hasDivMultip = false;
                // 계산한 값들을 배열에서 제외해준다.
                con[conIdx - 1] = 0;
                conIdx--;

                v[vIdx - 1] = 0;
                v[vIdx - 2] = 0;
                vIdx -= 2;

                v[vIdx] = tmp;
                vIdx++;
            }
        }
    }

    int result = v[0];
    for (int i = 0; i < conIdx; i++) {
        if (con[i] == '+') {
            result += v[i + 1];
        }
        else if (con[i] == '-') {
            result -= v[i + 1];
        }
    }
    return result;
}
void bracketOperation(char input[ARRAY_SIZE]) {
    // 1순위 -> 곱셈
    // 2순위 -> 덧셈/뺄셈
    int newForm[ARRAY_SIZE] = { 0 };
    int newFormIdx = 0;
    char localValue[ARRAY_SIZE] = { 0 };
    int localCount = 0;
    int localStart = 0;
    int localEnd = 0;
    int localResult = 0;
    for (int i = 0; i < inputLength; i++) {
        for (int j = 0; j < ARRAY_SIZE; j++) {
            if (j < localStart) {
                newForm[newFormIdx] = input[j];
                newFormIdx++;
            }
            else if (j == localStart) {
                char localResultArr[11] = { 0 };
                sprintf(localResultArr, "%d", localResult);
                for (int k = 0; k < 11; k++) {
                    if (localResultArr[k] != 0) {
                        newForm[newFormIdx] = localResultArr[k];
                        newFormIdx++;
                    }
                    else {
                        break;
                    }
                }
            }
            else if (j > localEnd && input[j] != 0) {
                newForm[newFormIdx] = input[j];
                newFormIdx++;
            }
        }
        break;
    }
    memset(&input[0], 0, sizeof(input));
    inputLength = 0;

    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (newForm[i] == 0) {
            break;
        }
        else {
            input[i] = newForm[i];
            inputLength++;
        }
    }
}