#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class BigInteger {
private:
    string number;

public:
    // Constructors
    BigInteger() {
        number = "0";
    }

    BigInteger(string num) {
        if (num.empty() || (num.size() == 1 && num[0] == '0'))
            number = "0";
        else {
            number = num;
            removeLeadingZeros();
        }
    }

    // Remove leading zeros
    void removeLeadingZeros() {
        number.erase(0, min(number.find_first_not_of('0'), number.size() - 1));
    }

    // Addition of two big integers
    BigInteger operator+(const BigInteger& other) const {
        string num1 = number;
        string num2 = other.number;
        
        int n1 = num1.size(), n2 = num2.size();
        int carry = 0;
        string result;
        
        for (int i = n1 - 1, j = n2 - 1; i >= 0 || j >= 0 || carry; i--, j--) {
            int digit1 = (i >= 0) ? num1[i] - '0' : 0;
            int digit2 = (j >= 0) ? num2[j] - '0' : 0;
            
            int sum = digit1 + digit2 + carry;
            carry = sum / 10;
            sum %= 10;
            
            result.push_back(sum + '0');
        }
        
        reverse(result.begin(), result.end());
        
        return BigInteger(result);
    }

    // Subtraction of two big integers
    BigInteger operator-(const BigInteger& other) const {
        string num1 = number;
        string num2 = other.number;

        int n1 = num1.size(), n2 = num2.size();
        string result;
        bool isNegative = false;

        // Determine the sign of the result
        if (compareAbsoluteValue(num1, num2) < 0) {
            swap(num1, num2);
            isNegative = true;
        }

        int carry = 0;

        for (int i = n1 - 1, j = n2 - 1; i >= 0 || j >= 0; i--, j--) {
            int digit1 = (i >= 0) ? num1[i] - '0' : 0;
            int digit2 = (j >= 0) ? num2[j] - '0' : 0;

            int diff = digit1 - digit2 - carry;

            if (diff < 0) {
                diff += 10;
                carry = 1;
            } else {
                carry = 0;
            }

            result.push_back(diff + '0');
        }

        reverse(result.begin(), result.end());

        // Remove leading zeros
        BigInteger res(result);
        res.removeLeadingZeros();

        if (isNegative && res.number != "0")
            res.number = "-" + res.number;

        return res;
    }

    // Multiplication of two big integers
    BigInteger operator*(const BigInteger& other) const {
        string num1 = number;
        string num2 = other.number;

        int n1 = num1.size();
        int n2 = num2.size();

        // Result can have at most n1 + n2 digits
        string result(n1 + n2, '0');

        for (int i = n1 - 1; i >= 0; i--) {
            int carry = 0;
            int digit1 = num1[i] - '0';

            for (int j = n2 - 1; j >= 0; j--) {
                int digit2 = num2[j] - '0';
                int sum = (digit1 * digit2) + (result[i + j + 1] - '0') + carry;

                carry = sum / 10;
                result[i + j + 1] = (sum % 10) + '0';
            }

            result[i] += carry;
        }
         // Create a BigInteger object with the result
    BigInteger product(result);

    // Remove leading zeros from the product
    product.removeLeadingZeros();
        return BigInteger(result);
    }

    // Division of two big integers
    BigInteger operator/(const BigInteger& other) const {
        BigInteger divisor = other;
        BigInteger dividend = *this;
        BigInteger quotient;

        if (divisor == BigInteger("0")) {
            cerr << "Error: Division by zero" << endl;
            return quotient;
        }

        bool isNegative = (dividend.number[0] == '-' && divisor.number[0] != '-') || (dividend.number[0] != '-' && divisor.number[0] == '-');

        if (dividend.number[0] == '-') dividend.number.erase(0, 1);
        if (divisor.number[0] == '-') divisor.number.erase(0, 1);

        while (dividend >= divisor) {
            BigInteger temp_divisor = divisor;
            BigInteger temp_quotient("1");

            while (temp_divisor * BigInteger("2") <= dividend) {
                temp_divisor = temp_divisor * BigInteger("2");
                temp_quotient = temp_quotient * BigInteger("2");
            }

            dividend = dividend - temp_divisor;
            quotient = quotient + temp_quotient;
        }

        quotient.removeLeadingZeros();

        if (isNegative && quotient.number != "0")
            quotient.number = "-" + quotient.number;

        return quotient;
    }

    // Factorial of a big integer
    BigInteger factorial() const {
        BigInteger result("1");
        BigInteger num = *this;

        while (num > BigInteger("1")) {
            result = result * num;
            num = num - BigInteger("1");
        }

        return result;
    }

    // Comparison operators
    bool operator==(const BigInteger& other) const {
        return number == other.number;
    }

    bool operator!=(const BigInteger& other) const {
        return !(*this == other);
    }

    bool operator<(const BigInteger& other) const {
        return compareAbsoluteValue(number, other.number) < 0;
    }

    bool operator>(const BigInteger& other) const {
        return compareAbsoluteValue(number, other.number) > 0;
    }

    bool operator<=(const BigInteger& other) const {
        return (*this < other) || (*this == other);
    }

    bool operator>=(const BigInteger& other) const {
        return (*this > other) || (*this == other);
    }

    // Overloading << operator to print Big Integer
    friend ostream& operator<<(ostream& os, const BigInteger& bigInt) {
        os << bigInt.number;
        return os;
    }

    // Overloading >> operator to read Big Integer
    friend istream& operator>>(istream& is, BigInteger& bigInt) {
        string input;
        is >> input;
        bigInt = BigInteger(input);
        return is;
    }

private:
    // Compare absolute values of two big integers
    int compareAbsoluteValue(const string& num1, const string& num2) const {
        string n1 = num1;
        string n2 = num2;

        // Remove leading zeros
        n1.erase(0, min(n1.find_first_not_of('0'), n1.size() - 1));
        n2.erase(0, min(n2.find_first_not_of('0'), n2.size() - 1));

        // Compare lengths
        if (n1.size() < n2.size())
            return -1;
        else if (n1.size() > n2.size())
            return 1;
        else
            return n1.compare(n2);
    }
};

int main() {
    char choice;
    do {
        cout << "Enter operation ( +, -, *, /, ! ): ";
        cin >> choice;

        if (choice == '+' || choice == '-' || choice == '*' || choice == '/') {
            cout << "Enter two big integers:" << endl;
            BigInteger num1, num2;
            cin >> num1 >> num2;

            switch (choice) {
                case '+':
                    cout << "Sum: " << num1 + num2 << endl;
                    break;
                case '-':
                    cout << "Difference: " << num1 - num2 << endl;
                    break;
                case '*':
                    cout << "Product: " << num1 * num2 << endl;
                    break;
                case '/':
                    if (num2 == BigInteger("0")) {
                        cerr << "Error: Division by zero" << endl;
                    } else {
                        cout << "Quotient: " << num1 / num2 << endl;
                    }
                    break;
                default:
                    break;
            }
        } else if (choice == '!') {
            cout << "Enter a big integer for factorial:" << endl;
            BigInteger num;
            cin >> num;

            cout << "Factorial: " << num.factorial() << endl;
        } else {
            cout << "Invalid operation." << endl;
        }

        cout << "Do you want to perform another operation? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}