extern void main(void);
__attribute__((section(".text.start"))) 
void start(void) {
    main();// вызов main основного си файла
    while(1);
}
