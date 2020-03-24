文档 -- 记录函数实现
====================

主要记录实现功能，调用要求，依赖的其他函数（避免循环依赖）， 提供的异常处理和undefined behaviour。

`template <typename IntT = uint16_t> class BigInt` (bigint.hpp)
---------------------------------------------------------------

IntT模板参数指明每一段都使用了什么数据类型存储，应为长度不超过最长基本类型一半 的非负整数类型。目前显式实例化的有`uint8_t, uint16_t, uint32_t`。

### 成员变量

``` {.cpp}
static constexpr size_t LIMB = sizeof(IntT) << 3;
static constexpr size_t MAX_CAP = size_t(1) << 63;
size_t cap_;  // capacity, currently must be *a power of 2*
size_t len_;  // actual used length
IntT* val_;
inline static std::random_device rand_dev_;
inline static auto rand_gen_ = static_cast<std::mt19937>(rand_dev_());
inline static std::uniform_int_distribution<IntT> rand_;
```

`LIMB`和`MAX_CAP`是常用的静态常量，分别表示每段的位长和最大的容量。

`cap_`指明现在的数组长度。为了二分方便，目前**保证永远是2的幂**。 这需要构造函数保证构造为2的幂，且每次加长或缩短数组的时候都保证乘或除2的某个幂。 `cap_`介于`[1, MAX_CAP]`，不要求最短，但是尽量不要超出太多。

`len_`指明现在正在使用的段数，段从低往高，仿照小端存储。 为了安全起见，**未被使用的段要求为0**。 同时，要求**绝大部分时候保证`len_`最短**，因为保证`len_`短的开销应该不会太大， 又能够减少其它计算的开销。

`val_`为存储数据的数组。以补码存储。`val_[len_-1]`的最高位是符号位， 意味着能表示的数据范围是`[-2^{LIMB*len_-1}, 2^{LIMB*len_-1})`

最后三个是随机数生成器，生成每段的随机数的方法是`rand_(rand_gen_)`。

#### 大多数情况下必须要处理或判断的特殊情况

用`new_cap, new_len`表示想要得到的新值。

``` {.cpp}
new_cap > MAX_CAP
new_cap == 0
new_len > MAX_CAP
new_len > cap_
new_len == 0
while (len_ > 1 && some_condition()) --len_;
```

特别注意当整个数的值为0时`len_`也不能是0，所以`len_ > 1`多数时间必须判定。

特别当心`size_t`的溢出，检验方法可以是`a+b<a`。

### 构造函数

构造函数 *要求* 构造出的数组长度必须为2的幂。

#### `explicit BigInt(int value = 0);`

默认构造函数，构建长度能够放下int的数组，初始化为`value`。 `len_`是根据实际数据大小设置的。

#### `BigInt(const BigInt& rhs);`

拷贝构造函数，自我拷贝构造不会抛出错误，而是长度为1，赋值为0。

#### `BigInt(BigInt&& rhs) noexcept;`

依赖`BigInt()`, `ShrinkLen()`。 移动构造函数，自我移动不抛出错误，置0。被移动对象置0。

#### `explicit BigInt(const char* str, size_t base = 0);`

以`\0`结尾的字符串构造。base指明进制，支持范围`[2, 36]`， 或0代表按照前缀自动检测，支持`0b, 0B, 0, 0x, 0X`。 允许大小写混用， 自动停止在非进制内字符。 指明进制后，无论是否加上前缀应当能正确处理，然而指明进制又加上错误的前缀（不区分大小写）会导致构造结果为0。 特别注意，未指明进制时，首位为0按照8进制处理；指明进制时，前导0不会产生错误，会正常处理。

#### `explicit BigInt(const std::string& str, size_t base = 0)`

直接调用`BigInt(const char*,size_t)`。

### 析构函数`virtual ~BigInt();`

删除`val_`数组。

### 赋值和类型转换运算符

#### `BigInt& operator=(const BigInt& rhs);`

拷贝赋值。仅拷贝数据，不保证数组长度相同。这是为了减少内存分配。 自我拷贝不会产生效果。

#### `BigInt& operator=(BigInt&& rhs) noexcept;`

移动赋值。被移动对象置0。自我移动置0。

#### `BigInt& operator=(const char* str);`

读入字符串的赋值情况。自动检测进制。

#### `BigInt& operator=(const std::string& str);`

读入字符串的赋值情况。自动检测进制。

#### `explicit operator std::string() const;`

转换至字符串。10进制。

#### `std::string ToString(size_t base, bool uppercase = false, int showbase = 0) const;`

输出至字符串，可选进制`[2, 36]`，可选大小写。showbase为0时不添加前后缀，为1时对2, 8, 16进制添加前缀，其余非10进制添加`_$(base)`形式的后缀（WolframAlpha支持），为2时对于任何非10进制均添加上述后缀。

#### `explicit operator bool() const;`

转换为bool，可用于条件语句。非0即为真。

### 基本操作

#### `bool Sign() const;`

返回当前数的符号。1为负数，0为非负数。

#### `size_t Shrink();`

依赖`ShrinkLen()`, `Resize(size_t)`。 尽量缩短数组长度，但仍然保证长度是2的幂。返回缩短后的数组长度。

#### `BigInt& CutLen(size_t seg_len, size_t bit_len = 0);`

依赖`Sign()`, `SetLen(size_t,bool)`。 剪切长度。剪切至`seg_len`段，最高位`bit % LIMB`（范围`[1, LIMB]`）个二进制位。 保留符号，因此若`bit % LIMB == 0`则意味着 假如最高位不等于符号，那么会得到`seg_len + 1`段。

#### `BigInt& CutBit(size_t bitlen);`

依赖`CutLen(size_t,size_t)`。与CutLen同一作用，但指派的是总的二进制位数。 保留符号。

#### `BigInt& GenRandom(size_t length = 0, size_t fixed = 0);`

依赖`SetLen(size_t, bool)`。 生成随机数。`length`指明随机数占用的段数。`fixed`指明最高段的占用位数。

若`fixed == 0`，则最高段占用任意位，但保证非负。 此时的随机数范围是`[0, 2^{LIMB * length - 1})`

其他情况下，若`fixed % LIMB == 0`，则最高段保证最高位一定为1，这意味着生成的是负数。 此时的随机数范围是`[-2^{LIMB * length - 1}, 0)`

其他情况下，取`fixed %= LIMB`，最高段保证从低往高第`fixed`位为1，更高位均为0。 此时的随机数范围是`[2^{LIMB * (length - 1) + fixed}, 2^{LIMB * (length-1) + fixed + 1})`

#### `const IntT* Data() const;`

返回存储数据的数组。

#### `size_t Length() const;`

返回当前有效数据的长度。

### 位运算

注意位运算的结果不会保留所有位，仍会按照有符号数进行缩短，因此`a ^ ~a`永远只得到一个段长度的`0xff`。

#### `BigInt& ToBitInv();`

对自己取位反。是`operator~`的自我修改版本。

#### `BigInt operator~() const;`

依赖`ToBitInv()`。位反操作符。

#### `BigInt& operator&=(const BigInt& rhs);`

``` {.cpp}
BigInt& operator|=(const BigInt& rhs);
BigInt& operator^=(const BigInt& rhs);
```

调用了`Sign()`, `SetLen()`, `ShrinkLen()`。 位运算+赋值运算符。

#### `BigInt& operator<<=(size_t rhs);`

调用了`AutoShrinkSize()`, `AutoExpandSize()`。 结果段数手动进行了调整（因为最多调1）。 负数的结果不总能保障，目前是直接移位，不单独处理符号位。

#### `BigInt& operator>>=(size_t rhs);`

调用了`AutoShrinkSize()`。 结果段数手动进行了调整（因为最多调1）。 负数的结果不总能保障，目前是直接移位，不单独处理符号位。

#### `std::weak_ordering operator<=>(const BigInt& rhs) const;`

C++20功能。仅在通过宏测试到三路比较运算符可用时调用。

#### `int Compare(const BigInt& rhs) const;`

比较函数。若`operator<=>`能够使用则这个函数不会被声明和定义。

#### `BigInt& operator+=(IntT& rhs);`

基本的加法赋值，为了成员函数中的一些使用方便。

#### `BigInt& operator+=(const BigInt& rhs);`

加法赋值。处理`*this.len_ != rhs.len_`。

#### `BigInt& operator-=(const BigInt& rhs);`

减法赋值。采用`a - b == ~(~a + b)`，减少出错概率。

#### `BigInt& operator++();`

并不是`+= 1`，单独写了一遍，理论上会稍微快一些。

#### `BigInt& operator--();`

采用`--a == ~(++~a)`，减少出错概率。

#### `BigInt operator++(int);`

返回旧值。

#### `BigInt operator--(int);`

返回旧值。

#### `BigInt& ToOpposite();`

相反数的自我修改版本。

#### `BigInt operator-() const;`

相反数。`-a == ++~a`。

#### `BigInt& operator*=(IntT rhs);`

基础乘法，右乘小的非负数，可以被其他乘法调用。 对于2的幂进行了位移优化。

#### `BigInt& operator*=(const BigInt& rhs);`

#### `BigInt& operator/=(IntT rhs);`

基础除法。除数为0不进行任何操作。除1不会浪费时间。注意这里除数非负。

#### `BigInt& operator/=(const BigInt& rhs);`

#### `BigInt& operator%=(IntT rhs);`

基础取模。并没有做除法，而是按照每一位的`2^LIMB % rhs`进行求和。 模0不进行任何操作。

#### `BigInt& operator%=(const BigInt& rhs);`

#### `BigInt& BasicDivEq(IntT rhs, IntT* mod = nullptr);`

#### `BigInt& PlainMulEq(const BigInt& rhs);`

### 输入输出

`src/bigint_io.cpp`包含广义的输入输出，即字符串转化的实现也在里面。

#### `void Print(size_t base = 10, bool uppercase = false,int showbase = 0, std::FILE* f = stdout);`

C风格的输出。与`ToString(size_t, bool, int)`相同，但是进制是2的幂时额外占用常数空间，不会先转化为字符串。

### 私有构造函数

#### `explicit BigInt(const IntT* data, size_t length);`

通过数组直接构造。`length == 0`则不复制，创建数组长度为1，并置0。 最多复制`MAX_CAP`长度的数据。

### 私有方法

#### `void SetLen(size_t new_len, bool preserve_sign);`

依赖`Sign()`, `AutoShrinkSize()`, `AutoExpandSize()`。 设置段数为`new_len`，范围`[1, MAX_CAP]`，接受缩短和延长。 延长的时候非负数则高位补`0`，负数补`-1`，使得补码下数值不变。 如果`new_len == len_`则无影响。 如果是缩短且`preserve_sign == true`则会把最高段的最高位也切掉，设置为符号位。 延长的时候`preserve_sign`没有影响。 *非常建议* 全用这个方法改变`len_`。

#### `void ShrinkLen();`

依赖`Sign()`。尽量缩短`len_`。 *非常建议* 可能改变段数的基本操作结束后都调用这个。

#### `void Resize(size_t new_cap);`

改变`cap_`。**不判断**`new_cap`是否合法。 调用`Resize(size_t)`之前 *必须* 判断`new_cap`介于`[1, MAX_CAP]`。

#### `void AutoExpandSize(size_t target_len);`

依赖`Resize(size_t)`。自动根据`target_len`来增大`cap_`至某个最小的2的幂。 此函数永远不减少`cap_`。减少`cap_`请看`AutoShrinkSize()`。

#### `void AutoShrinkSize();`

依赖`Resize(size_t)`。只有在`len_ <= (cap_ >> 3)`时缩短`cap_`至最小。 如果愿意的话可以调用，但要注意到重新分配内存的开销并不太小。 此函数永远不增加`cap_`。增加`cap_`请看`AutoExpandSize()`。

#### `std::istream& operator>>(std::istream& in, BigInt<IntT>& rhs);`

输入操作符，能够识别`std::oct, std::hex`，否则按照自动进制处理。

#### `std::ostream& operator<<(std::ostream& out, const BigInt<IntT>& rhs);`

输出操作符，能够识别fmtflags。

### 不修改的双目运算符

``` {.cpp}
template <typename IntT>
BigInt<IntT> operator&(BigInt<IntT> lhs, const BigInt<IntT>& rhs);
template <typename IntT>
BigInt<IntT> operator|(BigInt<IntT> lhs, const BigInt<IntT>& rhs);
template <typename IntT>
BigInt<IntT> operator^(BigInt<IntT> lhs, const BigInt<IntT>& rhs);
template <typename IntT>
BigInt<IntT> operator+(BigInt<IntT> lhs, const BigInt<IntT>& rhs);
template <typename IntT>
BigInt<IntT> operator-(BigInt<IntT> lhs, const BigInt<IntT>& rhs);
template <typename IntT>
BigInt<IntT> operator<<(BigInt<IntT> lhs, size_t rhs);
template <typename IntT>
BigInt<IntT> operator>>(BigInt<IntT> lhs, size_t rhs);
template <typename IntT>
BigInt<IntT> operator*(BigInt<IntT> lhs, IntT rhs);
template <typename IntT>
BigInt<IntT> operator*(BigInt<IntT> lhs, const BigInt<IntT>& rhs);
template <typename IntT>
BigInt<IntT> operator/(BigInt<IntT> lhs, IntT rhs);
template <typename IntT>
BigInt<IntT> operator/(BigInt<IntT> lhs, const BigInt<IntT>& rhs);
template <typename IntT>
BigInt<IntT> operator%(BigInt<IntT> lhs, IntT rhs);
template <typename IntT>
BigInt<IntT> operator%(BigInt<IntT> lhs, const BigInt<IntT>& rhs);
```

均为非成员函数。 第一个参数定义成非引用是为了优化一连串的式子能够优化（网上说法）。 都是使用的对应的运算+赋值。

### 6种比较运算符

``` {.cpp}
#ifndef __cpp_impl_three_way_comparison
template <typename IntT>
bool operator<(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs);
template <typename IntT>
bool operator>(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs);
template <typename IntT>
bool operator<=(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs);
template <typename IntT>
bool operator>=(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs);
template <typename IntT>
bool operator==(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs);
template <typename IntT>
bool operator!=(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs);
#endif
```

C++20中`operator<=>`会自动生成这些比较， 所以只需要在不支持三路比较的时候定义这些。

### 显式实例化

头文件有显式实例化的声明，`bigint.cpp`有显式实例化。 包括了`uint8_t, uint16_t, uint32_t`，及其对应的运算符。
