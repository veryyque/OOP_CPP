#include "BitArray.h"
//BitArray:: - оператор разрешения области видимости - говорим, что это метод класса!!!
const int ULONG_BITS = sizeof(unsigned long) * 8;

//конструктор по умолчанию
BitArray::BitArray() {
    this->data_ = nullptr; 
    this->size_ = 0; //явное количество битов
    this->capacity_ = 0; //количество ячеек ulong выделенных памятью
}

//деструктор
BitArray::~BitArray() {
    delete[] this->data_;
}

//конструктор с параметрами explicit
BitArray::BitArray(int num_bits, unsigned long value) {
    if (num_bits < 0) {
        throw std::invalid_argument("Число битов не может быть отрицательным");
    }
    this->size_ = num_bits;
    this->capacity_ = calculate_capacity(num_bits);

    if (num_bits == 0) {
        this->data_ = nullptr;
        return;
    }

    this->data_ = new unsigned long[capacity_]();
    
    // Инициализируем значением value
    if (this->capacity_ > 0) {
        this->data_[0] = value;

        int last_cell_index = capacity_ - 1; //индекс последнего блока в дата
        int bits_in_last_block = size_ % ULONG_BITS; //колво бит в последнем блоке
        
        //обрезать последний неполный блок бит
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1; // 64 бита сдвигаем влево на биты в последнем блоке - 1(типо инвертирует)
            this->data_[last_cell_index] &= mask; //побитовое и присваивание
        }
    }
}

//конструктор копироавния ВЫЗЫВАЕТСЧ НЕЯВНО ПРИ передаче параметров в функцию и при ретернах
BitArray::BitArray(const BitArray& b){
    this->size_ = b.size_;
    this->capacity_ = b.capacity_;
    this->data_ = nullptr;

    if (b.data_ != nullptr && this->capacity_ > 0) {
        this->data_ = new unsigned long[this->capacity_];
        for (int i = 0; i < this->capacity_; ++i) {
            this->data_[i] = b.data_[i];
        }
    }
}

//swap
void BitArray::swap(BitArray& b) {
    std::swap(this->data_, b.data_);
    std::swap(this->size_, b.size_);
    std::swap(this->capacity_, b.capacity_);
}

//оператор присваивания
BitArray& BitArray::operator=(const BitArray& b){
    if (this == &b){
        return *this;
    }
    delete[] this->data_;
    this->size_ = b.size_;
    this->capacity_ = b.capacity_;
    this->data_ = new unsigned long[b.capacity_];

    for (int i = 0; i < b.capacity_; i++){
        this->data_[i] = b.data_[i];
    }
    return *this;
}

//Изменяет размер массива. В случае расширения, новые элементы 
//инициализируются значением value.
void BitArray::resize(int num_bits, bool value) {
    if (num_bits < 0) {
        throw std::invalid_argument("Число битов не может быть отрицательным");
    }

    if (num_bits == this->size_) {
        if (value) {
            this->set(); 
        } else {
            this->reset();
        }
        return;
    }

    if (num_bits > this->size_) {
        int old_size = this->size_;
        int new_capacity = calculate_capacity(num_bits);

        if (new_capacity > this->capacity_) {
             unsigned long* new_data = new unsigned long[new_capacity]();
            if (this->data_ != nullptr) {
                int copy_count = this->capacity_;
                for (int i = 0; i < copy_count; i++) {
                    new_data[i] = this->data_[i];
                }
                delete[] this->data_;
            }
            this->data_ = new_data;
            this->capacity_ = new_capacity;
        }
        this->size_ = num_bits;
        for (int i = old_size; i < num_bits; i++) {
            this->set(i, value);
        }
    }
    else {
        this->size_ = num_bits;
    }
    //очистка мусорных битов после сдвига
    if (this->size_ > 0) {
        int last_block = (this->size_ - 1) / ULONG_BITS;
        int bits_in_last_block = this->size_ % ULONG_BITS;
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1; //64 бита сдвигаем влево на биты в последнем блоке и - 1 для инверсии
            this->data_[last_block] &= mask; //при помощи маски и побит и убираем лишние биты
        }
    }
}
//проверка массивы одной длины
void BitArray::same_size(const BitArray& b) const {
    if (this->size_ != b.size_) {
        throw std::invalid_argument("Массивы должны быть одного размера");
    }
}
//очищаем массив, но оставляем выделенную память 
void BitArray::clear() {
    this->size_ = 0;
}

//Добавляет новый бит в конец массива. В случае необходимости 
//происходит перераспределение памяти.
void BitArray::push_back(bool bit) {
    resize(this->size_ + 1, bit);
}
//Битовые операции над массивами.
//Работают только на массивах одинакового размера.
//Обоснование реакции на параметр неверного размера входит в задачу.

//Оператор побитового И
BitArray& BitArray::operator&=(const BitArray& b){
    same_size(b);

    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] &= b.data_[i];
    }
    
    return *this;
}

//Оператор побитового или
BitArray& BitArray::operator|=(const BitArray& b){
    same_size(b);

    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] |= b.data_[i];
    }
    
    return *this;
}

//Оператор побитового исключающего или
BitArray& BitArray::operator^=(const BitArray& b){
    same_size(b);

    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] ^= b.data_[i];
    }
    
    return *this;
}

//Битовый сдвиг влево с заполнением нулями с присваиванием
BitArray& BitArray::operator<<=(int n){
    if (n < 0){
        throw std::invalid_argument("Число сдвига не может быть отрицательным");
    }
    if (n >= this->size_) {
        for (int i = 0; i < this->capacity_; i++) {
            this->data_[i] = 0;
        }
        return *this;
    }
    if (n > 0) {
        int block_shift = n / ULONG_BITS;
        int bit_shift = n % ULONG_BITS;

        if (block_shift > 0) {
            for (int i = 0; i < this->capacity_ - block_shift; i++) {
                this->data_[i] = this->data_[i + block_shift];
            } //сдвиг целого блока(64 бита)
            for (int i = this->capacity_ - block_shift; i < this->capacity_; i++) {
                this->data_[i] = 0;
            }//иниц последних блоков нулями
        }

        if (bit_shift > 0) { //если сдвиг кратный 8 достаточно байтового сдвига и битовый не делаем
            for (int i = 0; i < this->capacity_ - 1; i++) {
                this->data_[i] = (this->data_[i] << bit_shift) | (this->data_[i + 1] >> (ULONG_BITS - bit_shift));
            } //перенос битов между блоками
            this->data_[this->capacity_ - 1] <<= bit_shift; //отдельно обработка последнего блока
        }
        
        //очистка мусорных битов после сдвига
        if (this->size_ > 0) {
            int last_block = (this->size_ - 1) / ULONG_BITS;
            int bits_in_last_block = this->size_ % ULONG_BITS;
            if (bits_in_last_block > 0) {
                unsigned long mask = (1UL << bits_in_last_block) - 1; //64 бита сдвигаем влево на биты в последнем блоке и - 1 для инверсии
                this->data_[last_block] &= mask; //при помощи маски и побит и убираем лишние биты
            }
        }
    }
    
    return *this;
}

//Битовый сдвиг вправо с заполнением нулями с присваиванием
BitArray& BitArray::operator>>=(int n){
    if (n < 0){
        throw std::invalid_argument("Число сдвига не может быть отрицательным");
    }
    if (n >= this->size_) {
        for (int i = 0; i < this->capacity_; i++) {
            this->data_[i] = 0;
        }
        return *this;
    }
    if (n > 0) {
        int block_shift = n / ULONG_BITS;
        int bit_shift = n % ULONG_BITS;

        if (block_shift > 0) {
            for (int i = this->capacity_ - 1; i >= block_shift; i--) {
                this->data_[i] = this->data_[i - block_shift];
            } //сдвиг целого блока(64 бита)
            for (int i = 0; i < block_shift; i++) {
                this->data_[i] = 0;
            }//инициализация первых блоков нулями
        }

        if (bit_shift > 0) { //если сдвиг кратный 8 достаточно байтового сдвига и битовый не делаем
            for (int i = this->capacity_ - 1; i > 0; i--) {
                this->data_[i] = (this->data_[i] >> bit_shift) | (this->data_[i - 1] >> (ULONG_BITS - bit_shift));
            } //перенос битов между блоками
            this->data_[0] >>= bit_shift; //отдельно обработка первого блока
        }
        
        //очистка мусорных битов после сдвига
        if (this->size_ > 0) {
            int last_block = (this->size_ - 1) / ULONG_BITS;
            int bits_in_last_block = this->size_ % ULONG_BITS;
            if (bits_in_last_block > 0) {
                unsigned long mask = (1UL << bits_in_last_block) - 1; //64 бита сдвигаем влево на биты в последнем блоке и - 1 для инверсии
                this->data_[last_block] &= mask; //при помощи маски и побит и убираем лишние биты
            }
        }
    }
    
    return *this;
}

//Битовый сдвиг влево без присваивания
BitArray BitArray::operator<<(int n) const {
    BitArray result(*this); //копирую массив чтобы не менять исходный массив
    result <<= n;
    return result; //возврат НОВОГО массива
}

//Битовый сдвиг вправо без присваивания
BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

void BitArray::validate_index(int index) const {
    if (index < 0 || index >= this->size_){
        throw std::invalid_argument("Индекс не может быть меньше 0 или быть больше размера массива");
    }
}
////Устанавливает бит с индексом n в значение val.
BitArray& BitArray::set(int n, bool val) {
    validate_index(n);

    int block = n / ULONG_BITS;
    int position = n % ULONG_BITS;
    
    if (val) {
        this->data_[block] |= (1UL << position); //установка 1 на место position в блоке data
    } else {
        this->data_[block] &= ~(1UL << position);//установка 0
    }
    
    return *this;
}

//заполняет массив истиной
BitArray& BitArray::set() {
    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] = ~0UL;  //побитовая инверсия на все 1111
    }
    
    //очистка последних битов
    if (this->size_ > 0) {
        int last_block = (this->size_ - 1) / ULONG_BITS;
        int bits_in_last_block = this->size_ % ULONG_BITS;

        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            this->data_[last_block] &= mask;
        }
    }
    
    return *this;
}

//Устанавливает бит с индексом n в значение false.
BitArray& BitArray::reset(int n){
    return this->set(n,false);
}

//Заполняет массив ложью.
BitArray& BitArray::reset(){
    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] = 0UL;  //все биты блока в 0
    }
    
    if (this->size_ > 0) {
        int last_block = (this->size_ - 1) / ULONG_BITS;
        int bits_in_last_block = this->size_ % ULONG_BITS;
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            this->data_[last_block] &= mask;
        }
    }
    return *this;
}

//true, если массив содержит истинный бит.
bool BitArray::any() const{
    for (int i = 0; i < this->capacity_; i++) {
        if (this->data_[i] != 0UL) {
            return true;
        }
    }
    return false;  // все биты false
}

//true, если все биты массива ложны.
bool BitArray::none() const {
    return !this->any();
}

//оператор побитовой инверсии
BitArray BitArray::operator~() const{
    BitArray result(*this); //создаю копию массива
    
    for (int i = 0; i < this->capacity_; i++) {
        result.data_[i] = ~this->data_[i];
    }
    
    //очистка последних битов
    if (this->size_ > 0) {
        int last_block = (this->size_ - 1) / ULONG_BITS;
        int bits_in_last_block = this->size_ % ULONG_BITS;
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            result.data_[last_block] &= mask;
        }
    }
    
    return result;
}

//Подсчитывает количество единичных бит.
int BitArray::count() const {
    int count = 0;
    
    for (int i = 0; i < this->size_; i++) {
        if ((*this)[i]) {
            count++;
        }
    }
    
    return count;
}

//Возвращает значение бита по индексу i.
bool BitArray::operator[](int i) const{
    validate_index(i); //в блоке сдвиг вправо - нужный бит получится самый младший 
                        //через побитовое и с единицей получим только младший бит!
    return (this->data_[i / ULONG_BITS] >> (i % ULONG_BITS)) & 1;
}

//значение поля size_
int BitArray::size() const{
    return this->size_;
}

//проверка на пустой массив
bool BitArray::empty() const{
    if (size()== 0){
        return true;
    }
    else{
        return false;
    }
}

//вывод массива в строковом виде от старшего к младшему
std::string BitArray:: to_string() const{
    std::string result;
    
    if (this->size_ == 0) {
        return result;
    }

    for (int i = this->size_ - 1; i >= 0; i--) {
        if ((*this)[i]) {
            result += '1';
        } else {
            result += '0';
        }
    }
    
    return result;
}

//оператор сравнения
bool operator==(const BitArray& a, const BitArray& b){
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    
    return true;
}

//оператор не равно
bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

//оператор побитового И без присваивания
BitArray operator&(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Массивы не могут быть разного размера для &");
    }

    BitArray result(a);
    result &= b;
    return result;
}

//оператор побитового или без присваивания
BitArray operator|(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Массивы не могут быть разного размера для |");
    }

    BitArray result(a);
    result |= b;
    return result;
}

//Оператор побитового исключающего или без присваивания
BitArray operator^(const BitArray& a, const BitArray& b) {
    // Проверяем одинаковый размер
    if (a.size() != b.size()) {
        throw std::invalid_argument("Массивы не могут быть разного размера для ^");
    }

    BitArray result(a);
    result ^= b;
    return result;
}