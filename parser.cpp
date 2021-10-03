/*
программа для парсинга сайта с расписанием и подготовки даных для дальнейшего использования
*/
#include <fstream> //библиотека работы с файами
#include <string> //строковая библиотека
#include <ctime> //библиотека времени
#include <vector>
using namespace std;

#define WINDOWS 1

#if WINDOWS == 1
void startCurl(string url, vector<char>* raw_data) { //запуск curl в виндовс

	long size = 0;
	char buffer[128];
	bool freeday = 0;
	time_t now = time(0);
	char* dt = ctime(&now);//достаем время системы 
	for (int i = 0; i <= 2; i++) { //болк проверки пустого дня
		if ("Sat"[i] != dt[i]) freeday = 1;
	}
	if (!freeday) {
		printf("on Saturday, no need to worry about studying on Sunday");
		exit(1);
	}//конец блока

	FILE* curl;
	string request = R"(curl-7.79.1\bin\curl.exe )";
	request = request + url;
	const char* c = request.c_str();// подготовка символьной пременной для _popen
	if ((curl = _popen(c, "rt")) == NULL) {
		exit(1);
	}

	while (fgets(buffer, 128, curl))//вывод данных в основной цикл программы в виде вектора символов
	{
		for (int i = 0; i < 128; i++) {
		raw_data->push_back(buffer[i]);
		}
	}

}
#endif

#if WINDOWS == 0
void startCurl(string url){ // код под Linux
	
}
#endif

int main(){

	unsigned int itterator = 0;
	bool flag = 0;
	string url;
	char chars;
	char find_string[] = R"(class="mpei-galaktika-lessons-grid-date")";//="mpei-galaktika-lessons-grid-date" class="mpei-galaktika-lessons-grid-date"
	vector<char> raw_data;
	ifstream fd("destination_site.txt", ios::in);//открытие файла с сайтом парсинга;;
	if (!fd.is_open()){
		printf("ошибка открытия");
		return 1;
	}
	else{
		
		while (!fd.eof()) {  // прочитали его и заполнили им строку
			fd.get(chars);
			url.push_back(chars);
		}
		
	}
	startCurl(url, &raw_data);
	unsigned int size_vector = raw_data.size()-1;
	unsigned int size_vector_find = size(find_string)-1;
	for (unsigned int i = 0; i < size_vector; i++) {//самодельный перебор элементов вектора со сравнением с эталоном
		short int acceptable_error = 2;
		for (unsigned int j = 0; j < size_vector_find; j++) {
			if (raw_data[i + j] != find_string[j]) {
				acceptable_error--;
				if (!acceptable_error)break;				
			}
			if (j == (size_vector_find-1)) {
				itterator = i;
				flag = 1;
				break;
			}
		}

		if (flag == 1)break;
	}

	printf("%d \n",itterator);
	for (unsigned int i = 0; i < size_vector_find + 30; i++) {
		printf("%c", raw_data[itterator + i]);
	}

	scanf(&chars);
	return 0 ;
}