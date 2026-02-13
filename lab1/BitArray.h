#include <string>

class BitArray {
private:
    unsigned long* data_;
    int size_;
    int capacity_; //количество ячеек ulong выделенных памятью
    static int calculate_capacity(int num_bits) {
        const int ULONG_BITS = sizeof(unsigned long) * 8;
        return (num_bits + ULONG_BITS - 1) / ULONG_BITS;
    }
    void same_size(const BitArray& other) const;
    void validate_index(int index) const;
public:
    BitArray();
    explicit BitArray(int num_bits, unsigned long value = 0);
    ~BitArray();

    void swap(BitArray& b);

    BitArray(const BitArray& b);
    BitArray& operator=(const BitArray& b);
    
    void resize(int num_bits, bool value = false);
    void clear();
    void push_back(bool bit);
    
    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);
    
    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;
    
    BitArray& set(int n, bool val = true);
    BitArray& set();
    BitArray& reset(int n);
    BitArray& reset();
    
    bool any() const;
    bool none() const;
    BitArray operator~() const;
    int count() const;
    
    bool operator[](int i) const;
    int size() const;
    bool empty() const;
    std::string to_string() const;
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);
BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);
