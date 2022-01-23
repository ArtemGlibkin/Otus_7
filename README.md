 Bayan - программа для нахождения идентичных файлов 
 
 Параметры: 
 
 -D -директория для сканирования(может быть несколько)
 
 -R -директории, которые требуется исключить из сканирования(может быть несколько)
 
 -S -уровень сканирования (1 - с вложенными директориями, 0 - без вложенных директорий(значение по умолчанию))
 
 -F -маски ввиде регулярных выражений для сканируемых файлов(может быть несколько)
 
 -B -размер блока для сравнения файла(по умолчанию 1)
 
 -A,-алгоритм контрольной суммы (md5 или sha1) по умолчанию md5
 
 -M -минимальный размер файла в байтах для анализа (по умолчанию 1 байт)
 
 
 Пример запуска программы:  
   
 ./bayan -D /home/user/test -D /home/user2/test -R /home/usert2/test/rejectdir -A sha1 -S 1  