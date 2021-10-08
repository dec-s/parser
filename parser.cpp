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
void startCurl(string url, vector<char>* raw_data) {//запуск curl и выдача данных запроса

	long size = 0;
	char buffer[128];
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
			if ((int)buffer[i] > 10) {//исключение управляющих символов из общего потока( при текущей кодировке символы юникода так же исключаются, возможно будут правки)
				raw_data->push_back(buffer[i]);

			}
		}
	}

}
#endif

#if WINDOWS == 0
void startCurl(string url){ // код под Linux
	
}
#endif

unsigned int  FindString(unsigned int size_vector_find, unsigned int size_vector,
	char* find_string, vector<char> *raw_data, unsigned int start_itterator = 0) {//поиск строки симвоволв в векторе символов 
	//с последующей выдачей номера начало найденой строки 

	unsigned int itterator = 0;
	bool flag = 0;
	for (unsigned int i = start_itterator; i < size_vector; i++) {//самодельный перебор элементов вектора со сравнением с эталоном
		for (unsigned int j = 0; j < size_vector_find; j++) {
			if ((*raw_data)[i + j] != find_string[j]) {
				break;
			}
			if (j == (size_vector_find - 1)) {
				itterator = i;
				flag = 1;
				break;
			}

		}

		if (flag)break;
	}// конец поисковика
	if (flag) { 
		return itterator;
	}
	else {
		return 0xFFFFFFFF;// переполненная переменная не имеет смысла к контексте программы так чте ее возврат сообщает об ошибке
	}
}

int  FindNumber(unsigned int start_itterator,  vector<char>* raw_data) {//поиск чисел 

	int output_number = 0;
	short int k = 0;
	bool flag = 0;
	for (unsigned int i = start_itterator; i < start_itterator + 100; i++) {
		int symbol_number = (int)(*raw_data)[i];
		if (47 < symbol_number && symbol_number < 59) {
			flag = 1;
			if (symbol_number != 58) {
				if (k == 0 && symbol_number == 48) {}
				else{				
					output_number *= 10 ;	
					output_number += (symbol_number - 48);
					k++;
				}

			}
			
		}
		else {
			if (flag) break;
		}
	}
	return output_number;
}

int  FindNumber(unsigned int start_itterator, char* raw_data) {//поиск чисел 

	int output_number = 0;
	short int k = 0;
	bool flag = 0;
	for (unsigned int i = start_itterator; i < start_itterator + 100; i++) {
		if (47 < (int)raw_data[i] && (int)raw_data[i] < 58) {
			flag = 1;
			output_number *= 10 * k;
			output_number += ((int)raw_data[i] - 48);
			k++;
		}
		else {
			if (flag) break;
		}
	}
	return output_number;
}

int main(){
	
	int table_data = 0;//дата полученная из расписания
	int data = 0;//дата сегодняшнего дня
	int time_up = 0;// время первого занятия
	int start_itterator = 0;//смещение начала вектора в поиске даты
	unsigned int itterator = 0;//индекс найденой строки в векторе
	string url;
	char chars;
	char find_string[] = R"(class="mpei-galaktika-lessons-grid-date")";// хорошо бы это в конфиги вытащить но с этим будем разбираться многопоже
	vector<char> raw_data;
	bool freeday = 0;

	time_t now = time(0);//достаем время системы 
	char* dt = ctime(&now);//достаем время системы 

//болк проверки пустого дня(нужно перенести в функцию со всеми вытекающими)
	for (int i = 0; i <= 2; i++) { 
		if ("Sat"[i] != dt[i]) freeday = 1;
	}
	if (!freeday) {
		printf("ERROR on Saturday, no need to worry about studying on Sunday");
		exit(0);
	}
//конец блока

	ifstream fd("destination_site.txt", ios::in);//открытие файла с сайтом парсинга
	if (!fd.is_open()){
		printf("ERROR opening error");
		exit(1);
	}
	else{
		
		while (!fd.eof()) {  // прочитали его и заполнили им строку
			fd.get(chars);
			url.push_back(chars);
		}
		
	}
	startCurl(url, &raw_data);//вызов функции запроса
	unsigned int size_vector_find = size(find_string)-1;//уменьшение размера вектора связано с наличием команды конца сторки в строке эталон
	unsigned int size_vector = raw_data.size() - size_vector_find;// особого смысла не имеет но в крайнем случае исключит ненужный поиск в конечных элементах
	
	data = FindNumber(0, dt);
	data++;//100% вероятна ошибка переполнения даты месяца нужнен обработчик
	
	for (int i = 0; i < 7;i++) {//цикл нахождения даты следующего дня в таблице расписания
	// ограничение колличества иттераций выбрано исходя наихудшего случая 6 дневки
	// для оптимизации возможна замена, так же в теории это значение можно подтягивать из конфигурационого файла(даже не в разработке)
		
		if (i) {
			start_itterator = itterator + size_vector_find;
		}
		
		itterator = FindString(size_vector_find, size_vector, 
			find_string, &raw_data, start_itterator);
		if (itterator == 0xFFFFFFFF) {//проверка на полное прохождение вектора(детальнее см функцию FindString)
			printf("ERROR the schedule for tomorrow was not found");
			exit(1);
		}

		table_data = FindNumber(itterator + size_vector_find, &raw_data);
		if (table_data == data) {
			break;
		}
	}

	time_up = FindNumber(itterator + size_vector_find + 20, &raw_data);// подготовка данных к выдаче в формате int 


	//отладочные сообщения
	printf("%u \n",itterator);
	printf("%u \n", start_itterator-size_vector_find);
	printf("%d \n", table_data);
	printf("%d \n", data);
	printf("%d \n", time_up);

	for (unsigned int i = 0; i < size_vector_find + 1000; i++) {
		printf("%c", raw_data[itterator + i]);
	}

	scanf(&chars);
	//конец отладочных сообщений 
	return 0 ;
}