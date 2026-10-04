#include "common.h"
#include "bootstrap.h"


#define TAR_BLOCK_SIZE 512
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
};


// функция принимает укзатель на начало бинаного файла и его размер
// назначает заданное имя (цифра) соответсвующее названию
// передает параметры proc_born()

// функция преобразует восьмеричное число в строке в десятичное
int oct2int(char *oct, int len) {
    int dec = 0;
    for (int i = 0; i < len; i++) {
        if (oct[i] < '0' || oct[i] > '7')
            break;

        dec = dec * 8 + (oct[i] - '0');
    }
    return dec;
}

// функция принимает имя сервера
// возвращает укзатель на начало бинаного файла и его размер
struct tar_file find_binary(unsigned char *archive_start, const char *filename) {
    struct tar_header *header = (struct tar_header *)archive_start;
    struct tar_file result = {0, 0}; // По умолчанию файл не найден {0, 0}

    // В конце TAR всегда идут два пустых блока по 512 байт.
    // Если имя файла пустое (первый байт '\0'), значит архив кончился.
    while (header->name[0] != '\0') {
        // Извлекаем размер из поля size вашей структуры
        unsigned int filesize = oct2int(header->size, 12);

        // Проверяем: обычный файл ('0' или '\0') и совпадает ли имя (через вашу strcmp)
        if ((header->type == '0' || header->type == '\0') &&
            strcmp(header->name, filename) == 0) {
            // Используем ваше поле data! Оно автоматически указывает на нужный адрес
            result.data = (unsigned char *)header->data;
            result.size = filesize;
            return result;
        }

        // Выравниваем размер данных файла по границе 512 байт
        unsigned int aligned_size = ((filesize + (TAR_BLOCK_SIZE - 1)) / TAR_BLOCK_SIZE) * TAR_BLOCK_SIZE;
        // Смещаемся к следующему файлу в архиве: 
        // берем адрес текущего заголовка, прибавляем размер самого заголовка (512) и размер его данных
        header = (struct tar_header *)((unsigned char *)header + TAR_BLOCK_SIZE + aligned_size);
    }

    return result;
}

