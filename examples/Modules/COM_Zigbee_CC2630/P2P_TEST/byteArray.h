#ifndef _BYTEARRAY_H_
#define _BYTEARRAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <initializer_list>

typedef unsigned char uint8_t;

class byteArray {
   private:
    size_t _length;
    uint8_t *_ptr;

   public:
    // size_t size;
   public:
    byteArray(size_t size = 0);
    byteArray(const uint8_t *data, size_t size);
    byteArray(const byteArray &array);
    explicit byteArray(const std::initializer_list<uint8_t> args) {
        this->initializer(args);
    }
    ~byteArray();

    byteArray &initializer(const std::initializer_list<uint8_t> args) {
        this->_ptr =
            (uint8_t *)realloc(this->_ptr, sizeof(uint8_t) * args.size());
        this->_length = 0;
        for (const uint8_t &item : args) this->_ptr[this->_length++] = item;
        return *this;
    }

    byteArray &clean() {
        _length = 0;
        if (_ptr != nullptr) delete _ptr;
        return *this;
    }

    int indexof(char data, int pos = 0);
    int indexof(const byteArray &array, int pos = 0);

    byteArray mid(size_t pos, size_t len = -1);
    byteArray &append(const uint8_t data);
    byteArray &append(const char *s);
    byteArray &append(const byteArray &array);
    uint8_t at(size_t i);
    uint8_t *dataptr() {
        return _ptr;
    };

    bool isEmpty() {
        return (_length <= 0) ? true : false;
    }
    bool isNull() {
        return (_ptr == nullptr) ? true : false;
    }

    size_t length() {
        return _length;
    }
    size_t size() {
        return _length;
    }

    byteArray &operator=(const byteArray &);
    byteArray &operator=(const char *str);

    uint8_t &operator[](size_t i);
    const uint8_t &operator[](size_t i) const;

    byteArray &operator+=(char c);
    byteArray &operator+=(const char *s);
    byteArray &operator+=(const byteArray &a);

    template <class Stream>
    void printfArray(Stream *uartPtr) {
        uartPtr->printf("[SIZE]%d [DATA]", this->_length);
        for (int i = 0; i < this->_length; i++) {
            uartPtr->printf("%02x ", this->at(i));
        }
        uartPtr->printf(" \r\n");
    }
};

#endif