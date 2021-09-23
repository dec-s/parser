/*
программа для парсинга сайта с расписанием и подготовки даных для дальнейшего использования
*/
#includes <stdio.h>
#includes <fstream>
#includes <string>

#define WINDOWS = 1

#if WINDOWS == 1
void startCurl(string url){ //запуск curl в виндовс
		
	FILE   *curl;
	string request = "\curl-7.79.1\bin\curl.exe -o \..\..\site.html ";
	request = recuest + url;
    if( (curl = _popen( request, "wt" )) == NULL )
      exit( 1 );
}
#endif

#if WINDOWS == 0
void startCurl(string url){ // код под Linux
	
}
#endif

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