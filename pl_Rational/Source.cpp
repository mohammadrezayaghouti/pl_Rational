#include <iomanip>
#include <iostream>
#include <string>

#include "BigNumber.h"
#include "Rational.h"
using namespace std;

#define TEST(e) std::cout << std::setw(40) << std::left << std::boolalpha << #e": " << (e) << std::endl;

int main() {
    std::string num1, den1, num2, den2;

     cout << "Enter the numerator of the first rational number: ";
     cin >> num1;
     cout << "Enter the denominator of the first rational number: ";
     cin >> den1;

     cout << "Enter the numerator of the second rational number: ";
     cin >> num2;
     cout << "Enter the denominator of the second rational number: ";
     cin >> den2;

    BigNumber bnum(num1);
    BigNumber bdnum(den1);
    BigNumber bnum2(num2);
    BigNumber bdnum2(den2);

    // bnum.add(bdnum).print();
   /* TEST(BigNumber("0").trim())

        TEST(BigNumber("444").add("666"))
        TEST(BigNumber("444").add("666"))
        TEST(BigNumber("0").add("2010"))

        TEST(BigNumber("700") < BigNumber("666"))
        TEST(BigNumber("200") < BigNumber("666"))
        TEST(BigNumber("666") < BigNumber("666"))

        TEST(BigNumber("666") == BigNumber("666"))
        TEST(BigNumber("666") == BigNumber("222"))
        TEST(BigNumber("123") == BigNumber("2"))

        TEST(BigNumber("700").subtract("666"))
        TEST(BigNumber("1005").subtract("999"))

        TEST(BigNumber("1005").multiply("2"))
        TEST(BigNumber("2").multiply("123"))
        TEST(BigNumber("0").multiply("123"))
        TEST(BigNumber("11").multiply("111"))

        TEST(BigNumber("1234").divideby("21"))
        TEST(BigNumber("1234").divideby("2"))
        TEST(BigNumber("1234").divideby("1234"))
        TEST(BigNumber("20").divideby("1"))

        TEST(BigNumber("1234").modulo("1000"))
        TEST(BigNumber("5").modulo("3"))
        TEST(BigNumber("15").modulo("3"))
        TEST(BigNumber("1234").modulo("1234"))

        TEST(Rational("20", "1"))
        TEST(Rational("20", "10"))
        TEST(Rational("2", "12"))
        TEST(Rational("200", "3"))
        TEST(Rational("201", "3"))
        TEST(Rational("0", "3"))*/

       
         Rational rational1(bnum, bdnum);
         Rational rational2(bnum2, bdnum2);

         Rational sum = rational1.add(rational2);
         Rational difference = rational1.subtract(rational2);
         Rational product = rational1.multiply(rational2);
         Rational quotient = rational1.divide(rational2);

         cout << "First Rational Number: ";
         rational1.print();
         cout << endl;

         cout << "Second Rational Number: ";
         rational2.print();
         cout << endl;

         cout << "Sum: ";
         sum.print();

         cout << endl;

         cout << "Difference: ";
         difference.print();
         cout << endl;

         cout << "Product: ";
         product.print();
         cout << endl;

         cout << "Quotient: ";
         quotient.print();
         cout << endl;

        return 0;
}
