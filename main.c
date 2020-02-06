// /home/user/Projects/LABAOS/a.txt
/*/*
 * Poluyaktov
 * Lab 1, variant 13
 * Task:
 * Написать программу подсчета числа строк, слов и символов указан-ного по запросу текстового файла.
 * Полученную информацию пере-дать через межпроцессный канал в параллельный процесс и в нем вывести
 * только информацию о количестве слов, записав ее при этом в файл. Предусмотреть возможность
 * неоднократного прерывания по сигналу <CTRL>+<C>. При поступлении каждого нечетного преры-вания
 * выводить информацию обо всех текстовых файлах текущего каталога.
 */


#include <signal.h>//для обработки прерываний
#include <stdio.h>//для работы с потоками ввода вывода
#include <setjmp.h>//sigjmp
#include<string.h>// memset
#define CANT_OPEN_ERROR 1
sigjmp_buf a;
int i = 0; //счетчик прерываний

void func()
{
    i++;
    if (i % 2)
    {


        if (fork() == 0)
        {
             execl("/home/user/Projects/build-LABAOS-Desktop-Default/replac", NULL);
        }
        else
        {
            wait(NULL);
            siglongjmp(a,1);//функция восстанавливает окружение сохраненное последним вызовом  sigsetjmp
        }

    }
}
void get_correct_file_name(char* filename){
    while (1){
        memset(filename, 0, 256);
        printf("\nВведите имя файла: ");
        scanf("%s", filename);
        if (access(filename, 0)) // проверка на существование ,функция проверки доступа
        {
            printf("\nFile \"%s\" не найден\n", filename);
        } else {
            return;
        }
    }
}




int main(int argc,char *argv[])
{
    struct sigaction prer;//обработка сигнала-структура
    char filename[252];

    //printf("%c",q);
    printf("%d",argc);
    int p[2];// для канала
    void func();//функция обработчик сигнала
    memset(filename,0,252);

    prer.sa_handler=func;
    prer.sa_flags=0;
    sigaction(SIGINT,&prer,NULL);
    get_correct_file_name(&filename);
    pipe(p);
    if (fork()==0){
        close(1);// закрыть dev/tty на запись
        dup(p[1]);
        execl("/usr/bin/wc",  "wc",  filename, NULL);

    }
else
    {

        wait(NULL);
        dup2(0,5);
        close(0);
        dup(p[0]);
        int rows,words,symbols;
         char name[256];
        scanf("%d%d%d%s",&rows,&words,&symbols,&name);
        printf("words=%d\n",words);


        FILE* file;    // указатель на объект
        if ((file=fopen("info.txt", "w"))==NULL)                 // открыть файл для записи,если  не существует создать
        {
            fprintf(stderr, "Can't open file info.txt");         // файл не доступен
            exit(CANT_OPEN_ERROR);
        }
        else
        {
            fprintf(file, "Count WORDS in FILE %s: %d\n", name, words); // если файл доступен -записать информацию туда
            fclose(file);
        }
    }

    dup2(5,0);

    while(getchar()!='q') //
            {
            sigsetjmp(a,1);//сохранили  состояние процесса
            }

            printf("FINISHED\n");
    return 0;
}

