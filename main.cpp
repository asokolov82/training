
#include "IntArray.h"



int main()
{
    system("chcp 1251");
    int requiredValue = 0;
    try {
        IntArray array(7), array_1(1);
       //IntArray wrongarray(0); //проверка неправильной длины создания
        for (int i = 0; i < 7; ++i)
            array[i] = i;
       //cout << array[10];  //проверка обращения к неправильному адресу
        cout << "исходный массив: ";
        print(array);
        array.resize(5);
        cout << "массив с измененным размером:  ";
        print(array);
        array.remove(3);
        cout << "массив с удаленным элементом позиции 3: ";
        print(array);
        //array.insert(33, 10); //проверка обращения к неправильному адресу
        array.insert(33, 3);
        cout << "массив с добавленным элементом на позицию 3: ";
        print(array);
        array.insertAtBeginning(44);
        cout << "массив с добавленным элементом в начало: ";
        print(array);
        array.insertAtEnd(55);
        cout << "массив с добавленным элементом в конец: ";
        print(array);
        array_1 = array;
        cout << "скопированный массив: ";
        print(array_1);
        if (array.search(requiredValue) >= 0)
            cout << endl << "искомое значение " << requiredValue <<  " на " << array.search(requiredValue) << " позиции" << endl;
        else cout << "искомое значение не найдено" << endl;
    }
    catch (exception& e){
        cout << e.what();
    }
    
    
    
    

    return 0;
}