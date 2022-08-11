#include "byteArray.h"

byteArray::byteArray(size_t size) {
    if (size == 0) {
        _length = 0;
        _ptr    = nullptr;
        return;
    }
    _length = size;
    _ptr    = (uint8_t *)malloc(sizeof(uint8_t) * size);
}

byteArray::byteArray(const uint8_t *data, size_t size) {
    if (size == 0) {
        _length = 0;
        _ptr    = nullptr;
        return;
    }
    _length = size;
    _ptr    = (uint8_t *)malloc(sizeof(uint8_t) * size);
    memcpy(_ptr, data, sizeof(uint8_t) * size);
}

byteArray::byteArray(const byteArray &array) {
    if ((array._length == 0) || (array._ptr == nullptr)) {
        delete this->_ptr;
        this->_length = 0;
    }
    this->_length = array._length;
    this->_ptr =
        (uint8_t *)realloc(this->_ptr, sizeof(uint8_t) * array._length);
    memcpy(this->_ptr, array._ptr, sizeof(uint8_t) * array._length);
}

byteArray::~byteArray() {
    if (_ptr == nullptr) return;
    delete _ptr;
}

byteArray &byteArray::append(uint8_t data) {
    _ptr            = (uint8_t *)realloc(_ptr, sizeof(uint8_t) * (_length + 1));
    _ptr[_length++] = data;
    return *this;
}

byteArray &byteArray::append(const char *s) {
    _ptr = (uint8_t *)realloc(_ptr, sizeof(uint8_t) * (_length + strlen(s)));
    memcpy(&_ptr[_length], s, strlen(s));
    _length += strlen(s);
    return *this;
}

byteArray &byteArray::append(const byteArray &array) {
    _ptr =
        (uint8_t *)realloc(_ptr, sizeof(uint8_t) * (_length + array._length));
    memcpy(&_ptr[_length], array._ptr, array._length);
    _length += array._length;
    return *this;
}

uint8_t byteArray::at(size_t i) {
    return _ptr[i];
}

int byteArray::indexof(char data, int pos) {
    if ((this->_length == 0) || (this->_ptr == nullptr) ||
        (pos > this->_length))
        return -1;
    for (int i = pos; i < _length; i++) {
        if (this->_ptr[pos + i] == data) return i;
    }
    return -1;
}

int byteArray::indexof(const byteArray &array, int pos) {
    bool flag = false;
    if ((this->_length == 0) || (this->_ptr == nullptr) ||
        (pos > this->_length))
        return -1;
    for (int i = pos; i < (_length - array._length); i++) {
        flag = true;
        for (int n = 0; n < array._length; n++) {
            if (this->_ptr[pos + i + n] != array._ptr[n]) {
                flag = false;
                break;
            };
        }
        if (flag) return i;
    }
    return -1;
}

byteArray byteArray::mid(size_t pos, size_t len) {
    if ((pos > this->_length) || (pos < 0)) return byteArray();
    if ((pos == 0) && (len == this->_length)) return *this;
    if ((pos + len) > this->_length) len = -1;
    if (len == -1) {
        return byteArray(&this->_ptr[pos], this->_length - pos);
    }
    return byteArray();
}

byteArray &byteArray::operator=(const byteArray &array) {
    if (this == &array) return *this;
    if ((array._length == 0) || (array._ptr == nullptr)) {
        delete this->_ptr;
        this->_length = 0;
        return *this;
    }
    this->_length = array._length;
    this->_ptr =
        (uint8_t *)realloc(this->_ptr, sizeof(uint8_t) * array._length);
    memcpy(this->_ptr, array._ptr, sizeof(uint8_t) * array._length);

    return *this;
}
byteArray &byteArray::operator=(const char *str) {
    if (strlen(str) == 0) {
        delete this->_ptr;
        this->_length = 0;
        return *this;
    }
    this->_ptr = (uint8_t *)realloc(this->_ptr, sizeof(uint8_t) * strlen(str));
    this->_length = strlen(str);
    strcpy((char *)this->_ptr, str);
    return *this;
}

uint8_t &byteArray::operator[](size_t i) {
    return _ptr[i];
}

const uint8_t &byteArray::operator[](size_t i) const {
    return _ptr[i];
}

byteArray &byteArray::operator+=(char c) {
    return append(c);
}
byteArray &byteArray::operator+=(const char *s) {
    return append(s);
}
byteArray &byteArray::operator+=(const byteArray &a) {
    return append(a);
}
