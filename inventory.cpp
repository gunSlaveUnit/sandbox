#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <vector>   // понадобится как контейнер для инвентаря и операций

const int MAXIMUM_FILE_NAME_LENGTH = 255; // максимальная длина имени файла
short AmountRequests = 0;				  // костыль для правильной работы в функции запроса, нужен из-за символа конца строки

void SetLanguageFont() // устанавливаем кодировку и необходимый шрифт
{
	HANDLE hCon = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hCon != INVALID_HANDLE_VALUE) {
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 12;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = 400;
		wcscpy_s(cfi.FaceName, L"Lucida Console");
		SetCurrentConsoleFontEx(hCon, FALSE, &cfi);
	}

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}
void ShowMenu();         // меню программы
char OperationRequest(); // запрос операции
char* NameFileRequest(); // запрос имени файла
void MakeFile();         // создание инвентарного файла или файла с операциями
void AddingOperations(); // добавление операции
void ViewInventory();    // показать инвентарный файл  -- различие из-за хранимых данных
void ViewOperations();   // показать файл с операциями -/
void UpdateInventory();  // обновить инвентарный файл на основе данных файла с операциями
// далее - две шаблонных функции, составляющих алгоритм сортировки слиянием
template<typename Type> // можно было и убрать шаблон приспособив функцию только для объекта Product
static void merge(std::vector<Type>& buf, size_t left, size_t right, size_t middle);
template<typename Type> // можно было и убрать шаблон приспособив функцию только для объекта Product
void MergeSort(std::vector<Type>& buf, size_t left, size_t right);

struct Product // изделие
{
	// на всякий случай конструктор по умолчанию
	Product() : VendorCode(""), Name(""), Unit(""), Amount(0), Operation(""), Date("") {}

	char VendorCode[20]; // артикул
	char Name[100];      // имя
	char Unit[10];       // единица измерения
	int Amount;          // количество
	char Operation[15];  // операция "поступление / отпуск"
	char Date[15];       // дата проведения операции
};

int main()
{
	SetLanguageFont(); // ставим кодировку и шрифт

	ShowMenu();        // показываем меню

	// далее основной цикл программы
	char UserSelection = 'q';
	do
	{
		ShowMenu();

		UserSelection = OperationRequest(); // запрос операции

		if (UserSelection == 'm')
			MakeFile();
		else if (UserSelection == 'a')
		{
			AddingOperations();
		}
		else if (UserSelection == 's')
		{
			system("cls");
			printf("Показать инвентарный файл или файл с операциями? (1, 0): ");
			char e = getchar();
			char Choice = '0';
			scanf_s("%c", &Choice, 1);
			if (Choice == '1')
				ViewInventory();
			else if (Choice == '0')
				ViewOperations();
		}
		else if (UserSelection == 'u')
		{
			UpdateInventory();
		}
		else if (UserSelection == 'q')
		{
			printf("Завершение работы программы.\n");
			exit(0);
		}

		printf("Главное меню, выход (1, q): ");
		scanf_s("%c", &UserSelection, 1);
		if (UserSelection == 'q')
		{
			printf("Завершение работы программы.\n");
			exit(0);
		}
	} while (UserSelection != 'q');

	system("pause");
	return 0;
}

void ShowMenu()
{
	system("cls");
	printf("Инвентарная программа.\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Доступные операции:\n\n");
	printf("m. Создать инвентарный файл или файл с операциями.\n\n");
	printf("a. Дополнение файла с операциями поступления и отпуска продукции.\n\n");
	printf("s. Просмотр инвентарного файла или файла с операциями поступления и отпуска продукции.\n\n");
	printf("u. Обновление данных инвентарного файла на основе файла с операциями поступления и отпуска продукции.\n\n");
	printf("q. Выход из программы.\n\n");
}

char OperationRequest()
{
	++AmountRequests;

	char SymbolEndline = '0';
	char OperationInputUser = '-';
	printf("Введите желаемую операцию: ");
	// если запросов больше, чем 1, во ввод попадает \n и программа не продолжает работу
	// переменная AmountRequests это предотвращает
	if (AmountRequests > 1)
		SymbolEndline = getchar();
	scanf_s("%c", &OperationInputUser, 1);

	return OperationInputUser;
}

char* NameFileRequest()
{
	fflush(stdin);
	char* NameInputFile = new char[MAXIMUM_FILE_NAME_LENGTH];
	gets_s(NameInputFile, MAXIMUM_FILE_NAME_LENGTH);

	return NameInputFile;
}

void MakeFile()
{
	system("cls");
	printf("Введите имя инвентарного файла или файла с операциями: ");
	char SymbolEndline = getchar();
	char* InventoryName = NameFileRequest();

	FILE* INVENTORY_FILE;
	errno_t InventoryFile;

	InventoryFile = fopen_s(&INVENTORY_FILE, InventoryName, "wb");

	delete[]InventoryName;
	if (INVENTORY_FILE)
		fclose(INVENTORY_FILE);
}

void AddingOperations()
{
	system("cls");
	printf("Введите имя файла, в который будет добавляться информация: ");
	char SymbolEndline = getchar();
	char* NameOperationsFile = NameFileRequest();

	FILE* PRODUCTFILE;
	errno_t ProductFile;

	ProductFile = fopen_s(&PRODUCTFILE, NameOperationsFile, "ab");

	printf("\nВведите информацию о продукте.\n\n");
	Product ProductToAdd;

	printf("Введите артикул изделия: ");
	gets_s(ProductToAdd.VendorCode, 20);

	printf("Введите имя изделия: ");
	gets_s(ProductToAdd.Name, 100);

	printf("Введите единицу измерения изделия: ");
	gets_s(ProductToAdd.Unit, 10);

	printf("Введите количество изделия: ");
	scanf_s("%u", &ProductToAdd.Amount);

	SymbolEndline = getchar();
	fflush(stdin);
	printf("Введите вид операции над изделием (поступление / отпуск): ");
	gets_s(ProductToAdd.Operation, 15);

	printf("Введите дату проведения операции в формате день.месяц.год: ");
	gets_s(ProductToAdd.Date, 15);

	if (PRODUCTFILE)
	{
		fwrite(ProductToAdd.VendorCode, sizeof(char), 20, PRODUCTFILE);
		fwrite(ProductToAdd.Name, sizeof(char), 100, PRODUCTFILE);
		fwrite(ProductToAdd.Unit, sizeof(char), 10, PRODUCTFILE);
		fwrite(&ProductToAdd.Amount, sizeof(int), 1, PRODUCTFILE);
		fwrite(ProductToAdd.Operation, sizeof(char), 15, PRODUCTFILE);
		fwrite(ProductToAdd.Date, sizeof(char), 15, PRODUCTFILE);
	}
	if (PRODUCTFILE)
		fclose(PRODUCTFILE);
}

void ViewOperations()
{
	system("cls");
	printf("Введите имя файла для вывода: ");
	char SymbolEndline = getchar();
	char* NameFileRequired = NameFileRequest();

	FILE* REQUIRED_FILE;
	errno_t FileToView;

	FileToView = fopen_s(&REQUIRED_FILE, NameFileRequired, "rb");

	Product ProductToView;
	printf("\n");
	if (REQUIRED_FILE)
	{
		while (!feof(REQUIRED_FILE))
		{
			strcpy_s(ProductToView.VendorCode, 20, "");
			strcpy_s(ProductToView.Name, 100, "");
			strcpy_s(ProductToView.Unit, 10, "");
			ProductToView.Amount = 0;
			strcpy_s(ProductToView.Operation, 15, "");
			strcpy_s(ProductToView.Date, 15, "");

			fread(ProductToView.VendorCode, sizeof(char), 20, REQUIRED_FILE);
			fread(ProductToView.Name, sizeof(char), 100, REQUIRED_FILE);
			fread(ProductToView.Unit, sizeof(char), 10, REQUIRED_FILE);
			fread(&ProductToView.Amount, sizeof(int), 1, REQUIRED_FILE);
			fread(ProductToView.Operation, sizeof(char), 15, REQUIRED_FILE);
			fread(ProductToView.Date, sizeof(char), 15, REQUIRED_FILE);

			if (ProductToView.Amount != 0)
			{
				printf("%s\t", ProductToView.VendorCode);
				printf("%s\t", ProductToView.Name);
				printf("%s\t", ProductToView.Unit);
				printf("%d\t", ProductToView.Amount);
				printf("%s\t", ProductToView.Operation);
				printf("%s\t", ProductToView.Date);
				printf("\n");
			}
		}
	}

	printf("\n");
	if (REQUIRED_FILE)
		fclose(REQUIRED_FILE);
}

void ViewInventory()
{
	system("cls");
	printf("Введите имя файла для вывода: ");
	char SymbolEndline = getchar();
	char* NameFileRequired = NameFileRequest();

	FILE* REQUIRED_FILE;
	errno_t FileToView;

	FileToView = fopen_s(&REQUIRED_FILE, NameFileRequired, "rb+");

	Product ProductToView;
	printf("\n");
	if (REQUIRED_FILE)
	{
		while (!feof(REQUIRED_FILE))
		{
			strcpy_s(ProductToView.VendorCode, 20, "");
			strcpy_s(ProductToView.Name, 100, "");
			strcpy_s(ProductToView.Unit, 10, "");
			ProductToView.Amount = 0;

			fread(ProductToView.VendorCode, sizeof(char), 20, REQUIRED_FILE);
			fread(ProductToView.Name, sizeof(char), 100, REQUIRED_FILE);
			fread(ProductToView.Unit, sizeof(char), 10, REQUIRED_FILE);
			fread(&ProductToView.Amount, sizeof(int), 1, REQUIRED_FILE);

			if (ProductToView.Amount != 0)
			{
				printf("%s\t", ProductToView.VendorCode);
				printf("%s\t", ProductToView.Name);
				printf("%s\t", ProductToView.Unit);
				printf("%d\t", ProductToView.Amount);
				printf("\n");
			}
		}
	}

	printf("\n");
	if (REQUIRED_FILE)
		fclose(REQUIRED_FILE);
}

// ниже два перегруженных оператора для сравнения имён двух объектов структуры Product в лексиграфическом порядке
// используются при бинарном поиске в UpdateInventory
bool operator<=(const Product& Prod1, const Product& Prod2)
{
	return strcmp(Prod1.Name, Prod2.Name) <= 0;
}

bool operator==(const Product& Prod1, const Product& Prod2)
{
	if (strcmp(Prod1.VendorCode, Prod2.VendorCode) == 0)
	{
		if (strcmp(Prod1.Name, Prod2.Name) == 0)
			return true;
	}
	else
		return false;
}

void UpdateInventory()
{
	system("cls");

	printf("Введите имя инвентарного файла: ");
	char SymbolEndline = getchar();
	char* InventoryFile = NameFileRequest();
	FILE* INVENTORY_FILE;
	errno_t Inventory;
	Inventory = fopen_s(&INVENTORY_FILE, InventoryFile, "rb+");

	printf("Введите имя файла с операциями: ");
	char* OperationsFile = NameFileRequest();
	FILE* OPERATIONS_FILE;
	errno_t FileWithOperations;
	FileWithOperations = fopen_s(&OPERATIONS_FILE, OperationsFile, "rb");

	std::vector<Product> AllInventory;  // храним всю продукцию из инвентарного файла
	std::vector<Product> AllOperations; // храним все операции

	Product ProductInventory;
	if (INVENTORY_FILE)
	{
		// читаем всю продукцию
		while (!feof(INVENTORY_FILE))
		{
			fread(ProductInventory.VendorCode, sizeof(char), 20, INVENTORY_FILE);
			fread(ProductInventory.Name, sizeof(char), 100, INVENTORY_FILE);
			fread(ProductInventory.Unit, sizeof(char), 10, INVENTORY_FILE);
			fread(&ProductInventory.Amount, sizeof(int), 1, INVENTORY_FILE);

			// и заводим её в вектор
			AllInventory.push_back(ProductInventory);
		}
	}

	// во время операции чтения в конце появляется дубликат, его убираем ниже
	if (!AllInventory.empty())
		AllInventory.pop_back();

	// сортируем вектор, так как бинарный поиск работает только с отсортированными данными
	if (!AllInventory.empty())
		MergeSort(AllInventory, 0, AllInventory.size() - 1);

	Product ProductFromFile;
	if (OPERATIONS_FILE)
	{
		// читаем все операции
		while (!feof(OPERATIONS_FILE))
		{
			fread(ProductFromFile.VendorCode, sizeof(char), 20, OPERATIONS_FILE);
			fread(ProductFromFile.Name, sizeof(char), 100, OPERATIONS_FILE);
			fread(ProductFromFile.Unit, sizeof(char), 10, OPERATIONS_FILE);
			fread(&ProductFromFile.Amount, sizeof(int), 1, OPERATIONS_FILE);
			fread(ProductFromFile.Operation, sizeof(char), 15, OPERATIONS_FILE);
			fread(ProductFromFile.Date, sizeof(char), 15, OPERATIONS_FILE);

			// и заводим их в вектор
			AllOperations.push_back(ProductFromFile);
		}
	}

	// удаляем дубликат
	if (!AllOperations.empty() && AllOperations.size() != 1)
		AllOperations.pop_back();

	// берём операцию и ищем для неё подходящий продукт
	for (auto& Operation : AllOperations)
	{
		// основной алгоритм бинарного поиска
		int mid_index = 0;
		int first_index = 0;
		int last_index = 0;
		AllInventory.size() == 0 ? last_index = 0 : last_index = AllInventory.size() - 1;

		while (first_index < last_index)
		{
			mid_index = first_index + (last_index - first_index) / 2;
			// в зависимости от значения сравнения движемся либо влево, либо вправо
			Operation <= AllInventory[mid_index] ? last_index = mid_index : first_index = mid_index + 1;
		}
		if (AllInventory.size() != 0)
		{
			// если нашли в инвентаре продукцию, над которой производится операция
			// изменяем количество продукции в зависимости от операции
			if (AllInventory[last_index] == Operation)
			{
				if (!strcmp(Operation.Operation, "поступление"))
					AllInventory[last_index].Amount += Operation.Amount;
				else if (!strcmp(Operation.Operation, "отпуск"))
					AllInventory[last_index].Amount -= Operation.Amount;
			}
		}
		// а если не нашли, просто добавляем
		else
		{
			AllInventory.push_back(Operation);
		}

		mid_index = 0;
		first_index = 0;
		last_index = AllInventory.size() - 1;
	}

	// следующие три строки для перезаписи файла
	fclose(INVENTORY_FILE);
	remove(InventoryFile);
	Inventory = fopen_s(&INVENTORY_FILE, InventoryFile, "wb+");

	// вновь отсортируем продукцию
	if (!AllInventory.empty())
		MergeSort(AllInventory, 0, AllInventory.size() - 1);

	// выведем продукцию в файл
	if (INVENTORY_FILE)
	{
		for (auto Item : AllInventory)
		{
			fwrite(Item.VendorCode, sizeof(char), 20, INVENTORY_FILE);
			fwrite(Item.Name, sizeof(char), 100, INVENTORY_FILE);
			fwrite(Item.Unit, sizeof(char), 10, INVENTORY_FILE);
			fwrite(&Item.Amount, sizeof(int), 1, INVENTORY_FILE);
		}

		fclose(INVENTORY_FILE);
	}

	// переименуем файл с операциями в архивный
	char* FullArchiveName = new char[MAXIMUM_FILE_NAME_LENGTH];
	strcpy_s(FullArchiveName, MAXIMUM_FILE_NAME_LENGTH, "Архивный - ");
	strcat_s(FullArchiveName, MAXIMUM_FILE_NAME_LENGTH, OperationsFile);
	fclose(OPERATIONS_FILE);
	int IsRenamed = rename(OperationsFile, FullArchiveName);
}

// ниже два перегруженных оператора для сравнения имён двух объектов структуры Product в лексиграфическом порядке
// используются в сортировке слиянием
bool operator>(const Product& Prod1, const Product& Prod2)
{
	return strcmp(Prod1.Name, Prod2.Name) > 0;
}

bool operator<(const Product& Prod1, const Product& Prod2)
{
	return strcmp(Prod1.Name, Prod2.Name) < 0;
}

template<typename Type>
void merge(std::vector<Type>& buf, size_t left, size_t right, size_t middle) //слияние
{
	if (left >= right || middle < left || middle > right) return;
	if (right == left + 1 && buf[left] > buf[right])
	{
		Product Temp = buf[left];
		buf[left] = buf[right];
		buf[right] = Temp;
		return;
	}

	std::vector<Type> tmp(&buf[left], &buf[right] + 1);

	for (size_t i = left, j = 0, k = middle - left + 1; i <= right; ++i) {
		if (j > middle - left) {
			buf[i] = tmp[k++];
		}
		else if (k > right - left) {
			buf[i] = tmp[j++];
		}
		else {
			buf[i] = (tmp[j] < tmp[k]) ? tmp[j++] : tmp[k++];
		}
	}
}

template<typename Type>
void MergeSort(std::vector<Type>& buf, size_t left, size_t right) //рекурсивная часть сортировки слиянием
{
	if (left >= right) return;

	size_t middle = left + (right - left) / 2;

	MergeSort(buf, left, middle);      // отсортировали левую часть
	MergeSort(buf, middle + 1, right); // отсортировали правую часть
	merge(buf, left, right, middle);   // слили две части
}


/* Небольшой тест
* Был создан пустой инвентарный файл.
* Был создан файл с операциями, в который записали А-1 Ноутбук шт 15 поступление 12.04.2020
* Обновили инвентарный файл: появилась запись А-1 Ноутбук шт 15
* Был создан другой файл с операциями, в который записали А-1 Ноутбук шт 4 отпуск 12.04.2020
* Обновили инвентарный файл: появилась запись А-1 Ноутбук шт 11, как и должно быть
*/

/* Бонус: код программы для генерации инвентарных файлов и файлов с операциями, иногда им пользовался

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));

	char** VendorCodes = new char*[10];
	for (int i = 0; i < 10; ++i)
		VendorCodes[i] = new char[20];
	strcpy_s(VendorCodes[0], 20, "1382700");
	strcpy_s(VendorCodes[1], 20, "1370384");
	strcpy_s(VendorCodes[2], 20, "1625927");
	strcpy_s(VendorCodes[3], 20, "1625883");
	strcpy_s(VendorCodes[4], 20, "1075314");
	strcpy_s(VendorCodes[5], 20, "1627095");
	strcpy_s(VendorCodes[6], 20, "1611371");
	strcpy_s(VendorCodes[7], 20, "1603457");
	strcpy_s(VendorCodes[8], 20, "1052877");
	strcpy_s(VendorCodes[9], 20, "1287838");


	char** Names = new char* [10];
	for (int i = 0; i < 10; ++i)
		Names[i] = new char[100];
	strcpy_s(Names[0], 100, "Ноутбук ASUS ROG Mothership GZ700GX-AD028T чёрный");
	strcpy_s(Names[1], 100, "Оперативная память Kingston HyperX FURY RGB 64 ГБ");
	strcpy_s(Names[2], 100, "Видеокарта PNY Quadro M5000");
	strcpy_s(Names[3], 100, "Ноутбук Apple MacBook Pro Retina TB серебристый");
	strcpy_s(Names[4], 100, "Ноутбук MSI GT63 9SG-054RU черный");
	strcpy_s(Names[5], 100, "Процессор AMD Ryzen Threadripper 3990X BOX");
	strcpy_s(Names[6], 100, "Процессор Intel Xeon E5-1660 v4 OEM");
	strcpy_s(Names[7], 100, "Видеокарта EVGA GeForce RTX 2080 Ti");
	strcpy_s(Names[8], 100, "Оперативная память Corsair Vengeance LPX 128 ГБ");
	strcpy_s(Names[9], 100, "Оперативная память Kingston HyperX FURY Black 128 ГБ");

	const char* Unit = "шт";



	char** Operations = new char* [1];
	for (int i = 0; i < 1; ++i)
		Operations[i] = new char[15];
	strcpy_s(Operations[0], 15, "поступление");
	//strcpy_s(Operations[1], 15, "отпуск");
											   // включить блок для генерации
											   // файла с операциями
	char** Dates = new char* [3];
	for (int i = 0; i < 3; ++i)
		Dates[i] = new char[15];
	strcpy_s(Dates[0], 15, "10.04.2020");
	strcpy_s(Dates[1], 15, "11.04.2020");
	strcpy_s(Dates[2], 15, "12.04.2020");


	FILE* Out;
	errno_t OutFile;
	const char* Name = "operations.bin"; // поменять для файла с операциями
	fopen_s(&Out, Name, "ab");

	for (int i = 0; i < 5; ++i)
	{
		int RandomCode = rand() % 10;
		int RandomName = rand() % 10;
		int RandomAmount = 1 + rand() % 100;
		int RandomOperation = rand() % 1; // включить для файла с операциями
		int RandomDate = rand() % 3;      // включить для файла с операциями

		if (Out)
		{
			fwrite(VendorCodes[RandomCode], sizeof(char), 20, Out);
			fwrite(Names[RandomName], sizeof(char), 100, Out);
			fwrite(Unit, sizeof(char), 10, Out);
			fwrite(&RandomAmount, sizeof(int), 1, Out);
			fwrite(Operations[RandomOperation], sizeof(char), 15, Out); // включить для файла с операциями
			fwrite(Dates[RandomDate], sizeof(char), 15, Out);           // включить для файла с операциями
		}
	}

	if(Out)
		fclose(Out);

	return 0;
};

*/
