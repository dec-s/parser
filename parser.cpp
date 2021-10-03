/*
программа для парсинга сайта с расписанием и подготовки даных для дальнейшего использования
*/
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;

#define WINDOWS 1

#if WINDOWS == 1
void startCurl(string url, string* raw_data){ //запуск curl в виндовс
		
	FILE   *curl;
	char buffer[128];
	string request = R"(curl-7.79.1\bin\curl.exe )"; //..\..\curl-7.79.1\bin\curl.exe -o \..\..\site.html "C:\Users\User\Documents\parser_schedule\curl-7.79.1\bin\curl.exe;
	request = request + url;
	const char* c = request.c_str();
    if( (curl = _popen(c, "rt" )) == NULL )
      exit( 1 );
	while (fgets(buffer, 128, curl))
	{
		puts(buffer);
	}
	if (feof(curl))
	{
		printf("\nProcess returned %d\n", _pclose(curl));
	}
	else
	{
		printf("Error: Failed to read the pipe to the end.\n");
	}
	
}
#endif

#if WINDOWS == 0
void startCurl(string url){ // код под Linux
	
}
#endif

int main(){
	setlocale(LC_ALL, "rus"); // корректное отображение Кириллицы
	string url, raw_data;
	char chars;
	ifstream fd("destination_site.txt", ios::in);//открытие файла с сайтом парсинга;;
	if (!fd.is_open()){
		printf("ошибка открытия");
		system("pause");
		return 1;
	}
	else{
		
		while (!fd.eof()) {  // прочитали его и заполнили им строку
			fd.get(chars);
			url.push_back(chars);
		}
		
	}
	startCurl(url, &raw_data);
	system("pause");
	return 0 ;
}