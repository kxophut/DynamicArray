#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <string>

#include "Exception.h"
#include "Iterator.h"
#include "toString.h"


//Класс итератора динамического массива
template <class T>
class ArrayIterator: public Iterator<T>
{
private:
    T* current;
    int max, index;
public:

    ArrayIterator(T* _curr, int _max, int _index):
            current(_curr),
            max(_max),
            index(_index)
    {}

    bool Next() override{
        ++current;
        ++index;
        if(index >= max) return false;
        return true;
    };

    bool Prev() override{
        --current;
        --index;
        if (index <= 0) return false;
        return true;
    };

    bool isValuable() override{
        return (index >= 0 && index <= max);
    };


    bool isEqual(const T& item) {
        if (this->isValuable()) return (item == *current);
        else throw Exception(1);
    };


    T GetData() override{
        if (this->isValuable()) return *(current);
        else throw Exception(1);
    }

    bool isEqual(const ArrayIterator<T>* item) {
        if(this->isValuable() && item->isValuable())return (*(item->current) == *current);
    };
};

//Описание класса динамического массива
template <typename T>
class DynamicArray :Iterable<T>{
private:

    T *head;//указатель на массив (первый элемент массива)

    int size;//размер массива

    int RealSize;//кол-во заполненных ячеек массива

    friend class ArrayIterator<T>;

public:

    //Конструкторы

    DynamicArray(){
        size = 0;
        head = nullptr;
        RealSize = 0;
    }   //Конструктор пустого динамического массива

    DynamicArray(const int &size_)
    {
        if (size_>0) {
            size = size_;
            RealSize = 0;
            head = new T[size_];
        }
        else throw Exception(3);
    }   //Конструктор массива по размеру

    DynamicArray(DynamicArray<T> const &dynamicArray){
        size=dynamicArray.size;
        RealSize=dynamicArray.RealSize;
        head = new T[dynamicArray.size];
        if (head != nullptr) {
            for (int i = 0; i < dynamicArray.RealSize; i++) {
                head[i] = dynamicArray.head[i];
            }
        }
        else Exception(4);
    }   //Копирующий конструктор

    DynamicArray(T *items, const int &count)
    {
        if (count > 0) {
            size = count;
            RealSize = count;
            head = new T[count];
            if (head != nullptr)
                for (int i = 0; i < count; i++)
                    head[i] = items[i];
            else
                throw Exception(2);
        } else Exception(3);

    };//конструктор по переданному массиву

    ~DynamicArray() {
        delete [] head;
    };//деструктор

    //методы-геттеры

    T *GetHead() {
        if (head == nullptr) throw Exception(2);
        return head;
    }   //возвращение головы списка

    int GetSize() { return size; }  //Возвращение размера массива

    int GetReal() { return RealSize; }  //Возвращение количества заполненных ячеек

    void SetReal(const int &Real) {
        RealSize = Real;
        if(Real > size)
            size = Real;
    }   //Сеттер количества заполненных ячеек

    void SetSize(int _size){ size = _size;}     //Сеттер размера массива


    /// Методы работы с массивом

    //Взятие первого элемента
    T GetFirst(){
        if(RealSize>0)
            return head[0];
        else {
//            printf("Error! This array is empty");
            throw Exception(2);
        }
    }

    T GetLast() {
        if (RealSize > 0)
            return head[RealSize-1];
        else {
//            printf("Error! This array is empty");
            throw Exception(2);
        }
    }   //взятие последнего элемента

    T Remove(int index){
        if(index<RealSize && index>=0) {
            T res = head[index];
            for (int i = index; i < RealSize - 1; i++) {
                head[i] = head[i+1];
            }
            Resize(GetReal()-1);
            return res;
        }
        else throw Exception(1);
    }

    void Resize(const int &size_)
    {
        if (size_ < 0) throw Exception(3);
        else if (size_ == 0) {
            delete [] head;
            head = nullptr;
            RealSize = 0;
            size = 0;
        }
        else {
            if(size_ >= RealSize){
                T* New = new T[size_];
                for(int i = 0; i<RealSize; i++)
                    New[i] = head[i];
                delete [] head;
                head = New;
                size = size_;
            }
            else {
                T* New = new T[size_];
                for(int i = 0; i<size_; i++)
                    New[i] = head[i];
                delete [] head;
                head = New;
                size = size_;
                RealSize = size_;
            }
        }
    }   //изменение размера массива

    DynamicArray<T>* Concat(DynamicArray<T>* arr){
        auto res = new DynamicArray<T>(RealSize+arr->RealSize);
        res->RealSize = RealSize+arr->RealSize;
        for(int i = 0; i<RealSize; i++)
            *(res->head+i) = *(head+i);
        for(int i = 0; i < arr->RealSize; i++)
            *(res->head + i + RealSize) = *(arr->head + i);
        return res;
    }   //Конкатенация (объединение) двух массивов

    void Set(int index, const T &value)
    {
        if (index>=0 && index < RealSize)
            head[index] = value;
        else
        {
            throw Exception(1);
        }
    }   //Замена элемента массива по номеру

    T Get (const int &index)
    {
        if (index>=RealSize || index<0) {
            throw Exception(1);
        } else
            return *(head+index);
    }   //Получение элемента по номеру

    //Дописывание элемента в начало
    void Append(const T& item){
        if(RealSize == size){
            Resize(size+1);
        }
        RealSize++;
        for (int i = RealSize-1; i>0; i--){
            *(head+i) = *(head+i-1);
        }
        *(head+0) = item;
    }
    //Дописывание элемента в конец
    void Prepend(const T& item){
        if(RealSize == size)
            Resize(size+1);
        RealSize++;
        head[RealSize-1] = item;
    }
    //Выводит содержимое всех ячеек массива (все те, что вводил пользователь)
    std::string String(){
        std::string res;
        if (head !=nullptr) {
            for (int i = 0; i < RealSize; i++)
                res+= toString<T>(head[i])+" ";
            res = res + "\n";
            return res;
        }
        else return res;
    }

    Iterator<T>* begin() override{ return (new ArrayIterator<T>(head, RealSize-1, 0)); }

    Iterator<T>* end() override { return (new ArrayIterator<T> (head + RealSize - 1, RealSize - 1, RealSize - 1)); }

    //Вывод динамического массива
    friend std::ostream& operator <<(std::ostream &out, const DynamicArray<T> &dynamicArray){
        for(int i = 0; i<dynamicArray.RealSize; i++){
            out<<dynamicArray.head[i]<<' ';
        }
        return out;
    }

};


#endif
