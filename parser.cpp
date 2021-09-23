/*
программа для парсинга сайта с расписанием и подготовки даных для дальнейшего использования
*/
#includes <fstream>
#includes <string>
void startCurl(string url){
	
}

int main(){
	string url;
	ifstream fd;
	fd.open("destination_site.txt",ios_base::in);//открытие файла с сайтом парсинга
	if (fd == is_open){
	url = scanf(&fd);
	fd.close();	
	}
	else{
	printf("ошибка открытия");
	return 1;
	}
	raw_data = startCurl(url);
	
}