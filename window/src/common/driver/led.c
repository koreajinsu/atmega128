// #include "led.h" // LED 제어에 필요한 정의를 포함하는 헤더 파일
// void ledInit(LED *led)
// {
//     *(led->port -1) |= (1<<led->pin);       // LED가 연결된 포트의 DDR 레지스터를 수정하여 해당 핀을 출력 모드로 설정
// }
// void ledOn(LED *led)
// {
//     *(led->port) |= (1<<led->pin);      // LED가 연결된 포트의 비트를 설정하여 LED를 켬        
// }
// void ledOff(LED *led)                       
// {
//     *(led->port) &= ~(1<<led->pin);          // LED가 연결된 포트의 비트를 클리어하여 LED를 끔   
// }
// void ledTog(LED *led)
// {
//      *(led->port) |= (1<<led->pin);      // LED가 연결된 포트의 비트를 설정하여 LED를 켬      
// }
