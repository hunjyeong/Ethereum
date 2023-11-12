//    а  2022006744       

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int mak();

//         >>>      Լ                
int score[5] = { 0 };												// 迭 ũ       
int a, b, c, e=0, n;												//a =     , b =               , c = score ù  °   , e =     , n =      ū      ε         ϴ      
char d,q;															//d = ǥ   ϴ      , q =  ٽ  Ҷ           

int main() {
	do {
		for (int i = 0; i < 5; i++) {								//      Է  ϴ      
			printf("%d          ? ", i + 1);
			scanf("%d", &a);
			score[i] = a;
			e += a;													//        ϴ   κ 
		}
		printf("         ϳ       ڸ  ǥ   ұ  ? ");
		scanf("%d", &b);								
		mak();														//    ׷     Լ      
		printf("\n----------------------------------\n");
		printf("    %4d   ", e / 5);								//       
		for (int x = 0; x < (e / 25); x++) {						//     ׷       
			printf("%c", d);
		}
		e = 0;
		printf("\n\n     Ͻðڽ  ϱ ? (Y:   )");					
		scanf("%s", &q);											//   ڿ     Է¹   
		printf("\n");
	} while (q == 'Y');												//   ڿ     Է              
	return 0;
}

int mak() {
	for (int j = 0; j < 5; j++) {									//5    ݺ 
		printf("\n%d   %5d   ", j + 1, score[j]);
		for (int k = 0; k < score[j] / b; k++) {					//       ǥ        ׷          
			d = '#';												// ʱ  ǥ       
			c = score[0];											//   ϱ      c   score 迭   ù  °          
			n = 0;													// ִ밪    ִ   迭    ε   
			for (int z = 0; z < 5; z++) {							// ִ밪 ã  	
				if (c < score[z + 1]) {								//c      ٸ      ũ   c   ū    ֱ 
					c = score[z + 1];
					n = z + 1;										//     ū      ִ   ּ ?
				}
			}
			if (j == n) {											// ִ밪   ã     ǥ        ڸ  *    ٲ   
				d = '*';
			}
			printf("%c", d);										//        
		}
	}
	
}