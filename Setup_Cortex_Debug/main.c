#define RCC_AHB1ENR   (*(volatile unsigned int*)0x40023830)
#define GPIOA_MODER   (*(volatile unsigned int*)0x40020000)
#define GPIOA_ODR     (*(volatile unsigned int*)0x40020014)
void delay(){
    for (volatile int i = 0; i < 1000000; i++); // delay
}
int main(void)
{
    RCC_AHB1ENR |= (1 << 0);        // Enable GPIOA clock
    GPIOA_MODER |= (1 << 10);       // PA5 as output

    while (1) {
        GPIOA_ODR ^= (1 << 5);      // Toggle LED
        delay(); // delay
    }
}
