#define F_CPU 16000000UL //CPU클럭 시간
#include <avr/io.h>
#include <util/delay.h>



 //64분주(prescaler) 0c0핀을 1000hz 구형파 생성   (첫번째 실습)
 int main(void)
 {
 	DDRB= 0x10;	 //0b00010000 , PB4를 출력으로 설정
 	
 	
 	TCCR0 = 0x1C;// 0b00011100;  //밑이랑 같은 의미임
 	//TCCR0 |=(1<<COM00) | (1<<WGM01) | (1<<CS02);
 	
 	OCR0 = 124;//값 계산해서 적은거임
 	
     while (1)	
     {  
 		while((TIFR & 0x02) == 0);
 		
 		TIFR = 0x02; //위에 와일문 탈출하면 재설정.
 		OCR0 = 124;	
     }
 }


 
 

