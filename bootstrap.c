// извлекает исполнеямый код серверов из tar диска
// отвечает за запуск сервреов при старте

struct tar_header {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char type;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
    char data[]; 
}__attribute__((packed));

struct tar_file {
    unsigned char *data;
    unsigned int size;
}


// функция принимает имя сервера
// возвращает укзатель на начало бинаного файла и его размер


// функция принимает укзатель на начало бинаного файла и его размер
// назначает заданное имя (цифра) соответсвующее названию
// передает параметры proc_born()
